#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPStateMachine.hpp>

#include <KPSerialInputObserver.hpp>
#include <KPSerialInput.hpp>

#include <Procedures/SampleStateMachine.hpp>
#include <Procedures/CleanStateMachine.hpp>

#include <Application/Constants.hpp>

#include <Components/ShiftRegister.hpp>
#include <Components/Pump.hpp>

#include <Buttons/Button.hpp>

#include <Components/Shell.hpp>

#include <string>

#include <FileIO/Logger.hpp>

#include <SD.h>
#include <ArduinoJson.h>

#include <Application/Power.hpp>
#include <Components/LED.hpp>
#include <Components/PressureSensor.hpp>

class Application : public KPController, public KPSerialInputObserver {
public:
	//	unsigned short current_sample = 0;	// todo move to the state machines
	//	unsigned short last_sample	  = 24;
	Power power{"power"};
	Logger logger{"logger", this};
	SampleStateMachine sm;
	CleanStateMachine csm;
	Button run_button{HardwarePins::RUN_BUTTON};
	Button clean_button{HardwarePins::CLEAN_BUTTON};
	Pump pump{"pump", HardwarePins::MOTOR_FORWARDS, HardwarePins::MOTOR_REVERSE};
	ShiftRegister shift{"shift-register",
		32,
		HardwarePins::SHFT_REG_DATA,
		HardwarePins::SHFT_REG_CLOCK,
		HardwarePins::SHFT_REG_LATCH};
	Shell shell{"shell", this};
	LED led{"led", this};
	PressureSensor pressure_sensor{"pressure-sensor", this};
	StaticJsonDocument<512> doc;

	void setup() override {
		Serial.begin(115200);
		addComponent(sm);
		addComponent(csm);
		addComponent(pump);
		addComponent(shift);
		addComponent(KPSerialInput::sharedInstance());
		addComponent(shell);
		addComponent(logger);
		addComponent(power);
		addComponent(led);
		addComponent(pressure_sensor);
		KPSerialInput::sharedInstance().addObserver(this);
		SD.begin(HardwarePins::SD);
		loadInfo();
	}

	bool isBusy() {
		return sm.isBusy() || csm.isBusy();
	}

	void update() override {
		if (!csm.isBusy()) {
			run_button.listen(sm);
		}
		if (!sm.isBusy()) {
			clean_button.listen(csm);
		}
		if (sm.isRunning()) {
			logger.log();
		}
		KPController::update();
	}
	// Serial Monitor
	void commandReceived(const char * line, size_t size) override {
		std::string * args = new std::string[5];
		char str[80];
		strcpy(str, line);
		println("Recieved: ", str);
		const char delim[2] = " ";
		const char * tok	= strtok(str, delim);
		int i				= 0;
		while (tok != NULL) {
			args[i] = tok;
			tok		= strtok(NULL, delim);
			++i;
		}
		shell.runFunction(args, i);
	}

	// this should be in utility but the scope isnt working todo solve
	std::string readEntireFile(File & file) {
		std::string contents;
		while (-1 != file.peek()) {
			contents.push_back(file.read());
		}
		return contents;
	}
	void loadInfo() {
		if (SD.exists("state.js")) {
			File file = SD.open("state.js", FILE_READ);
			if (file) {
				std::string contents = readEntireFile(file);
				deserializeJson(doc, contents);
				sm.getState<SampleStateFlush>(SampleStateNames::FLUSH).time
					= doc["sample"]["flush_time"];
				sm.getState<SampleStateSample>(SampleStateNames::SAMPLE).time
					= doc["sample"]["sample_time"];
				sm.getState<SampleStateIdle>(SampleStateNames::IDLE).time
					= doc["sample"]["idle_time"];
				sm.getState<SampleStateSetup>(SampleStateNames::SETUP).time
					= doc["sample"]["setup_time"];
				sm.last_cycle = doc["sample"]["last_cycle"];
				csm.getState<CleanStateSample>(CleanStateNames::SAMPLE).time
					= doc["clean"]["sample_time"];
				csm.getState<CleanStateIdle>(CleanStateNames::IDLE).time
					= doc["clean"]["idle_time"];
				csm.getState<CleanStateFlush>(CleanStateNames::FLUSH).time
					= doc["clean"]["flush_time"];
				csm.last_cycle = doc["clean"]["last_cycle"];
			} else {
				Serial.println("Error file read");
			}
			file.close();
		} else {
			// createStateFile();
			Serial.println("Error file read");
		}
	}
	// Future: more than two "levels" in loc, new value not int?
	// Rewrites into the JSON and file
	void reWrite(const char ** loc, int & value, int new_value) {
		value				= new_value;
		doc[loc[0]][loc[1]] = value;
		std::string contents;
		serializeJson(doc, contents);
		const char * contents_const = contents.c_str();
		if (SD.exists("state.js"))
			SD.remove("state.js");
		File file = SD.open("state.js", FILE_WRITE);
		file.write(contents_const);
		file.close();
	}

	// Broken
	void createStateFile() {
		if (SD.exists("state.js"))
			SD.remove("state.js");
		const char * contents = "{\n	\"sample\" : {\n		\"flush_time\" : 50,\n		"
								"\"sample_time\" : 60,\n	"
								"	\"idle_time\" : 3490,\n		\"setup_time\" : 0,\n		"
								"\"last_cycle\" : 24\n	"
								"},\n	\"clean\" : {\n		\"sample_time\" : 5,\n		"
								"\"idle_time\" : 0,\n		"
								"\"flush_time\" : 20,\n		\"last_cycle\" : 24\n	}\n}";
		File file = SD.open("state.js", FILE_WRITE);
		file.write(contents);
	}
};