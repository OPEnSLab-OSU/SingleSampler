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

#include <Application/Shell.hpp>

#include <string>

#include <FileIO/Logger.hpp>

#include <SD.h>
#include <ArduinoJson.h>

#include <Application/Power.hpp>

// subclassing?

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
		if (sm.isBusy()) {
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
		File file = SD.open("state.js", FILE_READ);
		if (file) {
			StaticJsonDocument<512> doc;
			std::string contents = readEntireFile(file);
			deserializeJson(doc, contents);
			sm.getState<SampleStateFlush>(SampleStateNames::FLUSH).time
				= doc["sample"]["flush_time"];
			sm.getState<SampleStateSample>(SampleStateNames::SAMPLE).time
				= doc["sample"]["sample_time"];
			sm.getState<SampleStateIdle>(SampleStateNames::IDLE).time = doc["sample"]["idle_time"];
			sm.getState<SampleStateSetup>(SampleStateNames::SETUP).time
				= doc["sample"]["setup_time"];
			sm.last_cycle = doc["sample"]["last_cycle"];
			csm.getState<CleanStateSample>(CleanStateNames::SAMPLE).time
				= doc["clean"]["sample_time"];
			csm.getState<CleanStateIdle>(CleanStateNames::IDLE).time   = doc["clean"]["idle_time"];
			csm.getState<CleanStateFlush>(CleanStateNames::FLUSH).time = doc["clean"]["flush_time"];
			csm.last_cycle = doc["sample"]["last_cycle"];
		}
		file.close();
	}
};

//
/*
		// null is eof
		char str[80];
		strcpy(str, line);
		const char delim[2] = " ";
		const char * tok	= strtok(str, delim);
		char cmd[30];
		int arg1 = -1;

		serial.print("recieved: ");
		serial.print(str);
		serial.print("\n");

		strcpy(cmd, tok);
		tok = strtok(null, delim);
		if (tok != null) {
			arg1 = atoi(tok);
			tok	 = strtok(null, delim);
		}
		if (0 == strcmp(cmd, "run")) {
			sm.begin();
		} else if (0 == strcmp(cmd, "sampletime")) {
			sm.getstate<samplestatesample>(samplestatenames::sample).time = arg1;
		} else if (0 == strcmp(cmd, "flushtime")) {
			sm.getstate<samplestateflush>(samplestatenames::flush).time = arg1;
		} else if (0 == strcmp(cmd, "idletime")) {
			sm.getstate<samplestateidle>(samplestatenames::idle).time = arg1;
		} else if (0 == strcmp(cmd, "lastat")) {
			last_sample = arg1;
		} else if (0 == strcmp(cmd, "stop")) {
			current_sample = 24;
			sm.stop();
			current_sample = 24;
			csm.stop();
		} else if (0 == strcmp(cmd, "setuptime")) {
			sm.getstate<samplestatesetup>(samplestatenames::setup).time = arg1;
		} else if (0 == strcmp(cmd, "whereat")) {
			serial.print(current_sample);
		} else if (0 == strcmp(cmd, "purgetime")) {
			sm.getstate<samplestatepurge>(samplestatenames::purge).time = arg1;
		} else if (0 == strcmp(cmd, "runat")) {
			current_sample = arg1;
			last_sample	   = arg1;
			sm.begin();
		} else if (0 == strcmp(cmd, "startat")) {
			current_sample = arg1;
		} else if (0 == strcmp(cmd, "crun")) {
			csm.begin();
		} else if (0 == strcmp(cmd, "cstop")) {
			current_sample = 24;
			csm.stop();
		} else if (0 == strcmp(cmd, "sstop")) {
			current_sample = 24;
			sm.stop();
		}
		*/