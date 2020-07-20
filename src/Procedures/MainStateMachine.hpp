#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/States.hpp>

class MainStateMachine : public StateMachine {
public:
	MainStateMachine()
		: StateMachine(
			"main-state-machine", StateNames::FLUSH, StateNames::STOP, StateNames::IDLE) {}

	void setup() override {
		registerState(StateIdle(), StateNames::IDLE);
		registerState(StateStop(), StateNames::STOP);
		registerState(StateFlush(), StateNames::FLUSH);
		// registerState(StateSample(), StateNames::SAMPLE);
	}
};