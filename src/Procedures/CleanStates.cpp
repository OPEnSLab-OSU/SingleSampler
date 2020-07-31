#include <Procedures/CleanStates.hpp>

// Idle
void CleanStateIdle::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	if (app.current_valve < app.last_valve)
		setTimeCondition(time, [&]() { sm.transitionTo(CleanStateNames::SAMPLE); });
	else
		sm.transitionTo(CleanStateNames::FINISHED);
}

// Stop
void CleanStateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	sm.transitionTo(CleanStateNames::IDLE);
}

// Flush
void CleanStateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide
	app.pump.on();

	setTimeCondition(time, [&]() { sm.transitionTo(CleanStateNames::SAMPLE); });
}

void CleanStateSample::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on();
	setTimeCondition(time, [&]() { sm.transitionTo(CleanStateNames::STOP); });
}

void CleanStateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.iterateValve();
}

void CleanStateFinished::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.current_valve = 0;
}