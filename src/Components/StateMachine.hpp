#pragma once

#include <KPFoundation.hpp>
#include <KPStateMachine.hpp>

class StateMachine : public KPStateMachine {
public:
	const char * entryStateName	   = nullptr;
	const char * stopStateName	   = nullptr;
	const char * idleStateName	   = nullptr;
	const char * finishedStateName = nullptr;

	StateMachine(const char * name, const char * entryStateName, const char * stopStateName,
		const char * idleStateName, const char * finishedStateName)
		: KPStateMachine(name),
		  entryStateName(entryStateName),
		  stopStateName(stopStateName),
		  idleStateName(idleStateName),
		  finishedStateName(finishedStateName) {}

	void begin() {
		transitionTo(entryStateName);
	}
	void stop() {
		transitionTo(stopStateName);
	}
	void idle() {
		transitionTo(idleStateName);
	}

	void setup() override {
		Serial.print("StateMachine setup");
	}

	/**
	 * Return whether the state machine is being run (not finished or nullptr)
	 */

	bool isBusy() const {
		return (getCurrentState() == nullptr
			|| 0 == strcmp(getCurrentState()->getName(), finishedStateName));
	}
};