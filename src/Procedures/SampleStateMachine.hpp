#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/SampleStates.hpp>

class SampleStateMachine : public StateMachine {
public:
	SampleStateMachine()
		: StateMachine("sample-state-machine", SampleStateNames::SETUP, SampleStateNames::STOP,
			SampleStateNames::IDLE, SampleStateNames::FINISHED) {}

	void setup() override {
		registerState(SampleStateIdle(), SampleStateNames::IDLE);
		registerState(SampleStateStop(), SampleStateNames::STOP, SampleStateNames::IDLE);
		registerState(SampleStateFinished(), SampleStateNames::FINISHED);

		registerState(SampleStateSetup(), SampleStateNames::SETUP, SampleStateNames::ONRAMP);
		registerState(SampleStateOnramp(), SampleStateNames::ONRAMP, SampleStateNames::FLUSH);
		registerState(SampleStatePurge(), SampleStateNames::PURGE, SampleStateNames::FLUSH);
		registerState(SampleStateFlush(), SampleStateNames::FLUSH, SampleStateNames::SAMPLE);
<<<<<<< HEAD
		registerState(SampleStateSample(), SampleStateNames::SAMPLE, SampleStateNames::PAUSE);
		registerState(SampleStatePause(), SampleStateNames::PAUSE, SampleStateNames::SAMPLE);

		registerState(SampleStatePurge(), SampleStateNames::PURGE, SampleStateNames::FLUSH);
=======
		registerState(SampleStateSample(), SampleStateNames::SAMPLE, SampleStateNames::STOP);
>>>>>>> parent of 426b838... add sample-pause cycling
	}
};