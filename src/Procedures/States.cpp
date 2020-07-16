#include <Procedures/States.hpp>
#include <Application/Application.hpp>

// Idle
void StateIdle::enter(KPStateMachine & sm) {}

// Stop
void StateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	setTimeCondition(5, [&]() { sm.transitionTo(StateNames::FLUSH); });
}

// Flush
void StateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.on();

	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::STOP); });
}