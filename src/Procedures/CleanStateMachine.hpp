#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/CleanStates.hpp>

class CleanStateMachine : public StateMachine {
public:
	CleanStateMachine()
		: StateMachine("clean-state-machine", CleanStateNames::FLUSH, CleanStateNames::STOP,
			CleanStateNames::IDLE, CleanStateNames::FINISHED) {}

	void setup() override {
		registerState(CleanStateIdle(), CleanStateNames::IDLE);
		registerState(CleanStateStop(), CleanStateNames::STOP);
		registerState(CleanStateFlush(), CleanStateNames::FLUSH);
		registerState(CleanStateSample(), CleanStateNames::SAMPLE);
		registerState(CleanStateFinished(), CleanStateNames::FINISHED);
	}
};