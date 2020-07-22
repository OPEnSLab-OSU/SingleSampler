#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPStateMachine.hpp>

#include <KPSerialInputObserver.hpp>
#include <KPSerialInput.hpp>

#include <Procedures/States.hpp>
#include <Procedures/MainStateMachine.hpp>
#include "Constants.hpp"

#include <Components/ShiftRegister.hpp>
#include <Components/Pump.hpp>

// subclassing?

class Application : public KPController, public KPSerialInputObserver {
public:
	unsigned short current_valve = 0;
	unsigned short no_runs		 = 24;
	MainStateMachine sm;
	Pump pump{"pump", HardwarePins::MOTOR_FORWARDS, HardwarePins::MOTOR_REVERSE};
	ShiftRegister shift{"shift-register",
		32,
		HardwarePins::SHFT_REG_DATA,
		HardwarePins::SHFT_REG_CLOCK,
		HardwarePins::SHFT_REG_LATCH};

	void iterateValve() {
		++current_valve;
	}

	void setup() override {
		Serial.begin(115200);
		addComponent(sm);
		addComponent(pump);
		addComponent(shift);
		addComponent(KPSerialInput::sharedInstance());
		sm.setup();
		KPSerialInput::sharedInstance().addObserver(this);
		Serial.print("Hello");
	}
	void update() override {
		KPController::update();
	}

	// Serial Monitor
	void commandReceived(const char * line) override {
		// NULL is EOF
		char str[80];
		strcpy(str, line);
		const char delim[2] = " ";
		const char * tok	= strtok(str, delim);
		char cmd[30];
		int arg1;

		Serial.print("Recieved: ");
		Serial.print(str);
		Serial.print("\n");

		strcpy(cmd, tok);
		tok = strtok(NULL, delim);
		if (tok != NULL) {
			arg1 = atoi(tok);
			tok	 = strtok(NULL, delim);
		}
		if (0 == strcmp(cmd, "run")) {
			sm.begin();
		} else if (0 == strcmp(cmd, "sample")) {
			StateSample * sample = sm.getState<StateSample>(StateNames::SAMPLE);
			sample->time		 = arg1;
		} else if (0 == strcmp(cmd, "flush")) {
			StateFlush * flush = sm.getState<StateFlush>(StateNames::FLUSH);
			flush->time		   = arg1;
		} else if (0 == strcmp(cmd, "idle")) {
			StateIdle * idle = sm.getState<StateIdle>(StateNames::IDLE);
			idle->time		 = arg1;
		} else if (0 == strcmp(cmd, "runs")) {
			no_runs = arg1;
		} else if (0 == strcmp(cmd, "stop")) {
			sm.stop();
		}
	}
};

//