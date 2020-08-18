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

// subclassing?

class Application : public KPController, public KPSerialInputObserver {
public:
	unsigned short current_sample = 0;
	unsigned short last_sample	  = 24;
	SampleStateMachine sm;
	CleanStateMachine csm;
	Button run_button{HardwarePins::RUN_BUTTON};
	Button empty_button{HardwarePins::PURGE_BUTTON};
	Button clean_button{HardwarePins::CLEAN_BUTTON};
	Pump pump{"pump", HardwarePins::MOTOR_FORWARDS, HardwarePins::MOTOR_REVERSE};
	ShiftRegister shift{"shift-register",
		32,
		HardwarePins::SHFT_REG_DATA,
		HardwarePins::SHFT_REG_CLOCK,
		HardwarePins::SHFT_REG_LATCH};

	void iterateValve() {
		++current_sample;
	}

	void setup() override {
		Serial.begin(115200);
		addComponent(sm);
		addComponent(csm);
		addComponent(pump);
		addComponent(shift);
		addComponent(KPSerialInput::sharedInstance());
		// sm.setup();
		KPSerialInput::sharedInstance().addObserver(this);
		Serial.print("Hello");
	}

	bool isBusy() {
		return sm.isBusy() || csm.isBusy();
	}

	void update() override {
		if (!isBusy()) {
			run_button.listen(sm);
			clean_button.listen(csm);
		}
		KPController::update();
	}
	// Serial Monitor
	void commandReceived(const char * line, size_t size) override {}
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