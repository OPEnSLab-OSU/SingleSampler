#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPStateMachine.hpp>

#include <KPSerialInputObserver.hpp>
#include <KPSerialInput.hpp>

#include <Procedures/SampleStateMachine.hpp>
#include <Procedures/CleanStateMachine.hpp>
#include <Procedures/EmptyStateMachine.hpp>

#include <Application/Constants.hpp>

#include <Components/ShiftRegister.hpp>
#include <Components/Pump.hpp>

#include <Buttons/Button.hpp>

// subclassing?

class Application : public KPController, public KPSerialInputObserver {
public:
	unsigned short current_valve = 0;
	unsigned short last_valve	 = 24;
	SampleStateMachine sm;
	CleanStateMachine csm;
	EmptyStateMachine esm;
	Button run_button{13, "run-button"};
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
		addComponent(run_button);
		addComponent(KPSerialInput::sharedInstance());
		// sm.setup();
		KPSerialInput::sharedInstance().addObserver(this);
		Serial.print("Hello");
	}
	void update() override {
		run_button.listen(sm);
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
		int arg1 = -1;

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
		} else if (0 == strcmp(cmd, "sampletime")) {
			sm.getState<SampleStateSample>(SampleStateNames::SAMPLE).time = arg1;
		} else if (0 == strcmp(cmd, "flushtime")) {
			sm.getState<SampleStateFlush>(SampleStateNames::FLUSH).time = arg1;
		} else if (0 == strcmp(cmd, "idletime")) {
			sm.getState<SampleStateIdle>(SampleStateNames::IDLE).time = arg1;
		} else if (0 == strcmp(cmd, "lastat")) {
			last_valve = arg1;
		} else if (0 == strcmp(cmd, "stop")) {
			sm.stop();
		} else if (0 == strcmp(cmd, "setuptime")) {
			sm.getState<SampleStateSetup>(SampleStateNames::SETUP).time = arg1;
		} else if (0 == strcmp(cmd, "whereat")) {
			Serial.print(current_valve);
		} else if (0 == strcmp(cmd, "purgetime")) {
			sm.getState<SampleStatePurge>(SampleStateNames::PURGE).time = arg1;
		} else if (0 == strcmp(cmd, "runat")) {
			current_valve = arg1;
			last_valve	  = arg1;
			sm.begin();
		} else if (0 == strcmp(cmd, "startat")) {
			current_valve = arg1;
		}
	}
};

//