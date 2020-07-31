#include <Procedures/EmptyStates.hpp>
#include <Application/Application.hpp>

void StateIdleEmpty::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	if (app.current_valve < app.last_valve)
		setTimeCondition(time, [&]() { sm.transitionTo(EmptyStateNames::PURGE); });
	else
		sm.transitionTo(EmptyStateNames::FINISHED);
}
void StatePurgeEmpty::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on(Direction::reverse);
	setTimeCondition(time, [&]() { sm.transitionTo(EmptyStateNames::STOP); });
}

void StatePurgeEmpty::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.iterateValve();
}

void StateFinishedEmpty::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.current_valve = 0;
}

void StateStopEmpty::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	sm.transitionTo(EmptyStateNames::IDLE);
}