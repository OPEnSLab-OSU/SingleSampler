#include <Procedures/SampleStates.hpp>
#include <Application/Application.hpp>
#include <FileIO/SerialSD.hpp>

SerialSD SSD;

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
		setTimeCondition(time, [&]() { sm.transitionTo(SampleStateNames::ONRAMP); });
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
	app.shift.write();								   // write shifts wide*/

	setTimeCondition(time, [&]() { sm.next(); });
}

// Flush
void SampleStateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide*/

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

	auto const condition = [&]() {
		bool t		  = timeSinceLastTransition() >= secsToMillis(time);
		bool pressure = !app.pressure_sensor.checkPressure();
		bool load	  = app.load_cell.getTaredLoad() >= volume;
		if (t)
			SSD.println("Sample state ended due to: time");
		if (pressure)
			SSD.println("Sample state ended due to: pressure");
		if (load)
			SSD.println("Sample state ended due to: load");
		return t || load || pressure;
	};
	setCondition(condition, [&]() { sm.next(); });
}

// Sample leave
void SampleStateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	SSD.print("Temp: ");
	SSD.println((double)app.pressure_sensor.getTemp());

	app.sm.current_cycle += 1;
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
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateBetweenPump::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateBetweenValve::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
	app.shift.write();

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateFillTubeOnramp::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide*/

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateFillTube::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide*/

	app.pump.on();

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStatePressureTare::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.shift.setAllRegistersLow();
	app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
	app.shift.write();								   // write shifts wide*/

	app.pump.on();

	sum	  = 0;
	count = 0;

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStatePressureTare::update(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	sum += app.pressure_sensor.getPressure();
	++count;
}

void SampleStatePressureTare::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
#ifndef DISABLE_PRESSURE_TARE
	int avg = sum / count;
	SSD.print("Normal pressure set to value: ");
	SSD.println(avg);

	app.pressure_sensor.max_pressure = avg + range_size;
	app.pressure_sensor.min_pressure = avg - range_size;
	SSD.print("Max pressure: ");
	SSD.println(app.pressure_sensor.max_pressure);
	SSD.print("Min pressure: ");
	SSD.println(app.pressure_sensor.min_pressure);
#endif
#ifdef DISABLE_PRESSURE_TARE
	SSD.println("Pressure tare state is disabled.");
	SSD.println("If this is a mistake, please remove DISABLE_PRESSURE_TARE from the buildflags.");
	SSD.print("Max pressure (set manually): ");
	SSD.println(app.pressure_sensor.max_pressure);
	SSD.print("Min pressure (set manually): ");
	SSD.println(app.pressure_sensor.min_pressure);
#endif
}

void SampleStateLogBuffer::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateLogBuffer::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	SSD.print("Load @ end of cycle ");
	SSD.print(app.sm.current_cycle);
	SSD.print(": ");
	SSD.println((double)app.load_cell.getLoad());
}

void SampleStateLoadBuffer::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.load_cell.reTare();
	setTimeCondition(time, [&]() { sm.next(); });
}