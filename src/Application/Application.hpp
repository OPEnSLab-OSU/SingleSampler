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
	// Components
	MainStateMachine sm;
	Pump pump{"pump", HardwarePins::MOTOR_FORWARDS, HardwarePins::MOTOR_REVERSE};
	// Shift Register
	ShiftRegister shift{"shift-register",
		32,
		HardwarePins::SHFT_REG_DATA,
		HardwarePins::SHFT_REG_CLOCK,
		HardwarePins::SHFT_REG_LATCH};

	void setup() override {
		Serial.begin(115200);
		addComponent(sm);
		addComponent(pump);
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
		Serial.print("Command recieved\n");
		if (strcmp("start", line) == 0) {
			Serial.print("Running pump\n");
			sm.begin();
		}
	}
};

//