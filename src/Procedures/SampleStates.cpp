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
	if (app.sm.current_cycle < app.sm.last_cycle)
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

	sm.next();
}

void SampleStateOnramp::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide

	setTimeCondition(time, [&]() { sm.next(); });
}

// Flush
void SampleStateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.pump.on();

	setTimeCondition(time, [&]() { sm.next(); });
}

// Sample
void SampleStateSample::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
	app.shift.write();
	app.pump.on();

	// testing
	Serial.print("Load @ beginning of cycle ");
	Serial.print(app.sm.current_cycle);
	Serial.print(": ");
	Serial.println(app.load_cell.getLoad());

	auto const condition = [&]() {
		Serial.println(app.load_cell.getTaredLoad());
		return timeSinceLastTransition() >= secsToMillis(time)
			|| !app.pressure_sensor.checkPressure() || app.load_cell.getTaredLoad() >= volume;
	};
	setCondition(condition, [&]() { sm.next(); });
}

// Sample leave
void SampleStateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	// testing
	Serial.print("Load @ end of cycle ");
	Serial.print(app.sm.current_cycle);
	Serial.print(": ");
	Serial.println(app.load_cell.getLoad());

	app.sm.current_cycle += 1;
	app.load_cell.reTare();
}

// Finished
void SampleStateFinished::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.led.setFinished();
	app.sm.reset();
}
// Purge (obsolete)
void SampleStatePurge::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
	app.shift.write();
	app.pump.on(Direction::reverse);
	setTimeCondition(time, [&]() { sm.next(); });
}

// Setup
void SampleStateSetup::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.led.setRun();
	app.load_cell.reTare();
	setTimeCondition(time, [&]() { sm.next(); });
}