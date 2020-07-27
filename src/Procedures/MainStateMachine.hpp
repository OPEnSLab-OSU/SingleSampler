#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/States.hpp>

class MainStateMachine : public StateMachine {
public:
	MainStateMachine()
		: StateMachine(
			"main-state-machine", StateNames::SETUP, StateNames::STOP, StateNames::IDLE) {}

	void setup() override {
		registerState(StateIdle(), StateNames::IDLE);
		registerState(StateStop(), StateNames::STOP);
		registerState(StateFlush(), StateNames::FLUSH);
		registerState(StateSample(), StateNames::SAMPLE);
		registerState(StateFinished(), StateNames::FINISHED);
		registerState(StatePurge(), StateNames::PURGE);
		registerState(StateSetup(), StateNames::SETUP);
	}
};