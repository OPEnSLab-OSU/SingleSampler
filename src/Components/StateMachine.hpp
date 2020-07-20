#pragma once

#include <KPFoundation.hpp>
#include <KPStateMachine.hpp>

class StateMachine : public KPStateMachine {
public:
	const char * entryStateName = nullptr;
	const char * stopStateName	= nullptr;
	const char * idleStateName	= nullptr;

	StateMachine(const char * name, const char * entryStateName, const char * stopStateName,
		const char * idleStateName)
		: KPStateMachine(name),
		  entryStateName(entryStateName),
		  stopStateName(stopStateName),
		  idleStateName(idleStateName) {}

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
};