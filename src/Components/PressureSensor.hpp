#pragma once
#include <MS5803_02.h>
#include <KPFoundation.hpp>
#include <Components/ErrorAble.hpp>
#include <application/Constants.hpp>
#define PRESSURE_ADDR 0x77

inline bool checkForConnection(unsigned char addr) {
	Wire.begin();
	Wire.requestFrom(addr, 1);
	return Wire.read() != -1;
}

class PressureSensor : public KPComponent, public ErrorAble {
	bool connected;
	MS_5803 sensor;
	int min_pressure			  = DefaultPressures::MIN_PRESSURE;
	int max_pressure			  = DefaultPressures::MAX_PRESSURE;
	bool kill_clock				  = false;
	unsigned int kill_time_offset = 5;
	unsigned int kill_time;

public:
	PressureSensor(const char * name, KPController * controller)
		: KPComponent(name, controller), sensor(PRESSURE_ADDR) {}

	void setup() override {
		if (sensor.initializeMS_5803()) {
			Serial.println("OK: MS5803 pressure sensor online");
		} else {
			Serial.println("ERR: MS5803 pressure sensor offline");
		}
	}

	void update() override {}

	float getPressure() {
		sensor.readSensor();
		return sensor.pressure();
	}

	float getTemp() {
		sensor.readSensor();
		return sensor.temperature();
	}

	int error_code() {
		return 0;
	}

	bool isWithinPressure() {
		int p = getPressure();
		if (p >= min_pressure && p <= max_pressure) {
			return true;
		} else {
			Serial.println("Not within pressure");
			return false;
		}
	}

	bool checkPressure() {
		bool within_pressure = isWithinPressure();
		if (!kill_clock && !within_pressure) {
			kill_clock = true;
			kill_time  = millis() + kill_time_offset;
		} else if (kill_clock && !within_pressure) {
			if (millis() >= kill_time) {
				kill_clock = false;
				return false;
			}
		} else if (kill_clock && within_pressure) {
			kill_clock = false;
		}
		return true;
	}
};