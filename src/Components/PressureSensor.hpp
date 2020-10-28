#pragma once
#include <MS5803_02.h>
#include <KPFoundation.hpp>
#include <Components/ErrorAble.hpp>
#define PRESSURE_ADDR 0x77

inline bool checkForConnection(unsigned char addr) {
	Wire.begin();
	Wire.requestFrom(addr, 1);
	return Wire.read() != -1;
}

class PressureSensor : public KPComponent, public ErrorAble {
	bool connected;
	MS_5803 sensor;

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

	int error_code() { return 0;}
};