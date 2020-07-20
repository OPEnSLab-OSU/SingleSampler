#include <Procedures/States.hpp>
#include <Application/Application.hpp>

// Idle
void StateIdle::enter(KPStateMachine & sm) {
	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::FLUSH); });
	
}

// Stop
void StateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::IDLE); });
}

// Flush
void StateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.on();

	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::STOP); });
}