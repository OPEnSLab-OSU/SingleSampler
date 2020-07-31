#pragma once

#include <Components/StateMachine.hpp>
#include <Procedures/EmptyStates.hpp>

class EmptyStateMachine : public StateMachine {
public:
	EmptyStateMachine()
		: StateMachine("empty-state-machine", EmptyStateNames::PURGE, EmptyStateNames::STOP,
			EmptyStateNames::IDLE) {}

	void setup() override {
		registerState(StatePurgeEmpty(), EmptyStateNames::PURGE);
		registerState(StateStopEmpty(), EmptyStateNames::STOP);
		registerState(StateFinishedEmpty(), EmptyStateNames::FINISHED);
		registerState(StateIdleEmpty(), EmptyStateNames::IDLE);
	}
};