#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPStateMachine.hpp>

#include "Constants.hpp"

#include <Components/ShiftRegister.hpp>
#include <Components/Pump.hpp>

// subclassing?

class Application : public KPController {
public:
	// Components
	KPStateMachine sm{"state-machine"};
	Pump pump{"pump", HardwarePins::MOTOR_FORWARDS, HardwarePins::MOTOR_BACKWARDS};

	void setup() override {
		addComponent(sm);
		addComponent(pump);

		Serial.print("Hi");
	}
	void update() override {
		KPController::update();
	}

	ShiftRegister shift{"shift-register",
		32,
		HardwarePins::SHFT_REG_DATA,
		HardwarePins::SHFT_REG_CLOCK,
		HardwarePins::SHFT_REG_LATCH};
};