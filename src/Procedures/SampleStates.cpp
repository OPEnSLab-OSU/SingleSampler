#include <Procedures/SampleStates.hpp>
#include <Application/Application.hpp>

void writeLatch(bool controlPin, ShiftRegister & shift) {
	shift.setPin(controlPin, HIGH);
	shift.write();
	delay(80);
	shift.setPin(controlPin, LOW);
	shift.write();
}

// Idle
void SampleStateIdle::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	if (app.current_valve < app.last_valve)
		setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::PURGE); });
	else
		sm.transitionTo(SampleStateNames::FINISHED);
}

// Stop
void SampleStateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();

	sm.transitionTo(SampleStateNames::IDLE);
}

// Flush
void SampleStateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide
	app.pump.on();

	setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::SAMPLE); });
}

void SampleStateSample::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on();
	setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::STOP); });
}

void SampleStateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.iterateValve();
}

void SampleStateFinished::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.current_valve = 0;
}

void SampleStatePurge::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(app.current_valve + Shift::FIRST_SAMPLE_VALVE, HIGH);
	app.shift.write();
	app.pump.on(Direction::reverse);
	setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::FLUSH); });
}

void SampleStateSetup::enter(KPStateMachine & sm) {
	setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::PURGE); });
}