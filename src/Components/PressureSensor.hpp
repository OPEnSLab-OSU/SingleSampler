#pragma once
#include <ssc.h>
#include <KPFoundation.hpp>
#include <Components/ErrorAble.hpp>
#define PRESSURE_ADDR 0x76

inline bool checkForConnection(unsigned char addr) {
	Wire.begin();
	Wire.requestFrom(addr, 1);
	return Wire.read() != -1;
}

class PressureSensor : public KPComponent, public ErrorAble {
	SSC sensor;

public:
	PressureSensor(const char * name, KPController * controller)
		: KPComponent(name, controller), sensor(PRESSURE_ADDR) {}

	void setup() override {
		bool enabled = checkForConnection(PRESSURE_ADDR);
		if (enabled) {
			sensor.setMinRaw(1638);
			sensor.setMaxRaw(14745);
			sensor.setMinPressure(0);
			sensor.setMaxPressure(30);
			sensor.start();
		}
	}

	void update() override {
		sensor.update();
	}

	int getPressure() {
		return sensor.pressure();
	}

	int error_code() {
		return sensor.error();
	}
};