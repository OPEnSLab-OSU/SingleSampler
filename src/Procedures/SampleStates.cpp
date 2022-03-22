#include <Procedures/SampleStates.hpp>
#include <Application/Application.hpp>
#include <FileIO/SerialSD.hpp>
#include <time.h>

bool pumpOff = 1;
bool flushVOff = 1;
bool sampleVOff = 1;
short load_count = 0;
float prior_load = 0;
float new_load = 0;
unsigned long prior_time;
unsigned long new_time;
float prior_rate = 0;
float new_rate;
float weight_remaining;
float new_time_est;
float prior_time_est;
float code_time_est;
unsigned long sample_start_time;
unsigned long sample_end_time;
bool pressureEnded = 0;

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
	sample_end_time = millis();
	SSD.println("Sample Stop Time");
	SSD.println(app.power.getTime());
	pumpOff = 1;
	SSD.println("Pump off");

	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();
	flushVOff = 1;
	sampleVOff = 1;

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateOnramp::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	if (flushVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
		app.shift.write();								   // write shifts wide*/
		flushVOff = 0;
		SSD.println("Flush valve turning on");
		sampleVOff = 1;
	}

	setTimeCondition(time, [&]() { sm.next(); });
}

// Flush
void SampleStateFlush::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	if (flushVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
		app.shift.write();								   // write shifts wide*/
		flushVOff = 0;
		SSD.println("Flush valve turning on");
		sampleVOff = 1;
	}

	if (pumpOff){
		app.pump.on();
		pumpOff = 0;
		SSD.println("Pump on");
	}

	setTimeCondition(time, [&]() { sm.next(); });
}

// Sample
void SampleStateSample::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	SSD.print("Input total sampling volume in ml;;;");
	SSD.println(volume);
	SSD.print("Input total sampling time in ms;;;");
	SSD.println(time);
	SSD.println("Sample Start Time");
	SSD.println(app.power.getTime());

	if (sampleVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
		app.shift.write();
		sampleVOff = 0;
		SSD.println("Sample valve turning on");
		flushVOff = 1;
	}

	if (pumpOff){
		app.pump.on();
		pumpOff = 0;
		SSD.println("Pump on");
		sample_start_time = millis();
	}

	auto const condition = [&]() {
		bool load = 0;
		// check load, exit if matching volume
			new_load = app.load_cell.getLoad(1);
			new_time = millis();
			print("New load reading;");
			println(new_load);
			print("New time;;;");
			println(new_time);
			float current_tare = app.sm.getState<SampleStateLoadBuffer>(SampleStateNames::LOAD_BUFFER).current_tare;
			//print("Current_tare in sample state;;;; ");
			//println(current_tare);
			//print("volume var in sample state;;;; ");
			//println(volume);
			load = new_load - current_tare >= (volume - 5);
		if (load){
			SSD.println("Sample state ended due to: load ");
			pressureEnded = 0;
			return load;
		}
			//if not exiting due to load, check time and exit if over time
		else{
			bool t = timeSinceLastTransition() >= time;
			if (t){
				SSD.println("Sample state ended due to: time");
				pressureEnded = 0;
				return t;
			}
			//if not exiting due to load and time, check pressure
			else{
				bool pressure = !app.pressure_sensor.checkPressure();
				if (pressure){
					SSD.println("Sample state ended due to: pressure");
					pressureEnded = 1;
					return pressure;
				}
				//if not exiting due to load, time, and pressure, check pumping rate
				else{
					if (load_count > 0){
						//print("prior_load in sample states;;;;");
						//println(prior_load);
						//print("prior_time in sample states;;;;");
						//println(prior_time);
						new_rate = (new_load - prior_load) / ((new_time - prior_time));
						print("new_load - prior_load;;;");
						println(new_load - prior_load);
						print("(new_time - prior_time);;;");
						println((new_time - prior_time));
						print("New rate grams/ms;;;");
						println(new_rate);
						print("average rate: new_load - current_tare / new_time - sample_start_time;;;");
						println((new_load - current_tare)/(new_time - sample_start_time));
						//check to see if sampling time is appropriate
						weight_remaining = volume - (new_load - current_tare);
						print("Weight remaining (new_load - current_tare);");
						println(weight_remaining);
						// calculate new time based upon average rate
						new_time_est = weight_remaining/((new_load - current_tare)/(new_time - sample_start_time));
						print("Estimated time remaining in ms: weight_remaining/average rate;;;");
						println(new_time_est);
						print("Coded time remaining in millis;;;");
						code_time_est = time - timeSinceLastTransition();
						println(code_time_est);
						// update time if more than 10% off and new load similar to last load
						if (load_count > 4){
							if ((new_load - prior_load > 1) & (abs(new_load - prior_load) < 6)){
								if (abs((code_time_est - new_time_est)/code_time_est) > 0.1){
									time = new_time_est + timeSinceLastTransition();
									print("Code time outside 10 percent of estimated time. Updated sampling time in millis;;;");
									println(time);
								}
							}
						}
					}
					prior_load = new_load;
					prior_time = new_time;
					prior_rate = new_rate;
					prior_time_est = new_time_est;
					load_count += 1;
					return load || t || pressure;
				}
				}				
			}
	};
	setCondition(condition, [&]() { sm.next(); });
}

// Sample leave
void SampleStateSample::leave(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	load_count = 0;
	prior_load = 0;
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
	
	if (sampleVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
		app.shift.write();
		sampleVOff = 0;
		SSD.println("Sample valve turning on");
		flushVOff = 1;
	}
	app.pump.on(Direction::reverse);
	pumpOff = 0;
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
	pumpOff = 1;
	SSD.println("Pump off");
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateBetweenValve::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	
	if (sampleVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::WATER_VALVE, HIGH);
		app.shift.write();
		sampleVOff = 0;
		SSD.println("Sample valve turning on");
		flushVOff = 1;
	}
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateFillTubeOnramp::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	if (flushVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
		app.shift.write();								   // write shifts wide*/
		flushVOff = 0;
		SSD.println("Flush valve turning on");
		sampleVOff = 1;
	}
	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStateFillTube::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);

	if (flushVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
		app.shift.write();								   // write shifts wide*/
		flushVOff = 0;
		SSD.println("Flush valve turning on");
		sampleVOff = 1;
	}
	if (pumpOff){
		app.pump.on();
		pumpOff = 0;
		SSD.println("Pump on");
	}

	setTimeCondition(time, [&]() { sm.next(); });
}

void SampleStatePressureTare::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	if (flushVOff){
		app.shift.setAllRegistersLow();
		app.shift.setPin(TPICDevices::FLUSH_VALVE, HIGH);  // write in skinny
		app.shift.write();								   // write shifts wide*/
		flushVOff = 0;
		SSD.println("Flush valve turning on");
		sampleVOff = 1;		
	}
	if (pumpOff){
		app.pump.on();
		pumpOff = 0;
		SSD.println("Pump on");
	}

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
	SSD.print("Load at end of cycle ");//+ app.sm.current_cycle + ": " + (double)app.load_cell.getLoad());
	SSD.print(app.sm.current_cycle);
	SSD.print("; ");
	float final_load = app.load_cell.getLoad(25);
	SSD.println((float)final_load);

	//evaluate load and sampling time
	float current_tare = app.sm.getState<SampleStateLoadBuffer>(SampleStateNames::LOAD_BUFFER).current_tare;
	//print("current_tare in log buffer;;;;");
	//println(current_tare);
	float sampledLoad = final_load - current_tare;
	print("sampledLoad: final_load - current_tare;");
	println(sampledLoad);
	print("sample_start_time ms in log buffer;;;");
	println(sample_start_time);
	print("sample_end_time ms in log buffer;;;");
	println(sample_end_time);
	int volume = app.sm.getState<SampleStateSample>(SampleStateNames::SAMPLE).volume;
	int sampledTime = (sample_end_time - sample_start_time);
	print("sampledTime period in ms;;");
	println(sampledTime);
	//calculate average pumping rate
	float average_pump_rate = sampledLoad / sampledTime;
	print("Average pumping rate grams/ms: sampledLoad/sampledTime;;;");
	println(average_pump_rate);
	//change time opposite sign of load diff (increase for negative, decrease for positive)
	print("load_diff: volume - sampledLoad;;;");
	float load_diff = volume - sampledLoad;
	println(load_diff);

	//update time if sample didn't end due to pressure
	if (!pressureEnded){	
		// change sampling time if load was +- 5% off from set weight
		if (abs(volume - sampledLoad)/volume > 0.05){
			println("Sample volume outside of 5 percent tolerance");
			sampledTime += (load_diff)/average_pump_rate;
			print("new sampling time period in ms: load diff/avg rate;;");
		}
		else {
			print("Sampling time period set to match last sample time;;");
		}
		println(sampledTime);
		//set new sample time
		app.sm.getState<SampleStateSample>(SampleStateNames::SAMPLE).time = sampledTime;
	}
	sm.next();
}

void SampleStateLoadBuffer::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	SSD.print("Temp: ");
	SSD.println((float)app.pressure_sensor.getTemp());
	//SSD.println("Get load");
	//println(app.load_cell.getLoad(1));
	SSD.print("Tare load; ");
	current_tare = app.load_cell.reTare(25);
	SSD.println(current_tare);
	sm.next();
}