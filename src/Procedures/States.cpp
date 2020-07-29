#include <Procedures/States.hpp>
#include <Application/Application.hpp>

void writeLatch(bool controlPin, ShiftRegister & shift) {
	shift.setPin(controlPin, HIGH);
	shift.write();
	delay(80);
	shift.setPin(controlPin, LOW);
	shift.write();
}

// Idle
void StateIdle::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	if (app.current_valve < app.last_valve)
		setTimeCondition(time, [&]() { sm.transitionTo(StateNames::PURGE); });
	else
		sm.transitionTo(StateNames::FINISHED);
}

// Stop
void StateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	sm.transitionTo(StateNames::IDLE);
}

// Flush
void StateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide
	app.pump.on();

	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::SAMPLE); });
}

void StateSample::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on();
	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::STOP); });
}

void StateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.iterateValve();
}

void StateFinished::enter(KPStateMachine & sm) {}

void StatePurge::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on(Direction::reverse);
	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::FLUSH); });
}

void StateSetup::enter(KPStateMachine & sm) {
	setTimeCondition(time, [&]() { sm.transitionTo(StateNames::PURGE); });
}