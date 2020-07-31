#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/SampleStates.hpp>

class SampleStateMachine : public StateMachine {
public:
	SampleStateMachine()
		: StateMachine("sample-state-machine", SampleStateNames::SETUP, SampleStateNames::STOP,
			SampleStateNames::IDLE) {}

	void setup() override {
		registerState(SampleStateIdle(), SampleStateNames::IDLE);
		registerState(SampleStateStop(), SampleStateNames::STOP);
		registerState(SampleStateFlush(), SampleStateNames::FLUSH);
		registerState(SampleStateSample(), SampleStateNames::SAMPLE);
		registerState(SampleStateFinished(), SampleStateNames::FINISHED);
		registerState(SampleStatePurge(), SampleStateNames::PURGE);
		registerState(SampleStateSetup(), SampleStateNames::SETUP);
	}
};