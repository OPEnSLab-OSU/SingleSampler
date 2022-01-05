#pragma once
#include <KPFoundation.hpp>
#include <ADS1232.h>

#define _dout 0
#define _sclk 1
#define _pdwn 5

class LoadCell : public KPComponent {
public:
	ADS1232 weight = ADS1232(_pdwn, _sclk, _dout);
	float tare;

	float factor = 7378.478054;
	float offset = -73328.85841;

	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller) {}
	void setup() override {
		weight.power_up();
		tare = 0;
		print("Initial load; ");
		println(reTare(200));
	}
	long read(int qty) {
		#ifdef LOAD_CAL
			//display every reading
			for (int i = 0; i < qty; ++i) {
				long reading = weight.raw_read(1);
				println();
				print("Load reading ");
				print(i);
				print(";; ");
				println(reading);
			}
		#endif
		#ifndef LOAD_CAL
			// get average value for qty reads
			long reading = weight.raw_read(qty);
		#endif
		return reading;
	}

	float getLoad(int qty) {
		// gets factor and offset from this file during setup, gets factor and offset from SD after
		return factor * log(read(qty)) + offset;
	}

	float getLoadPrint(int qty) {
		float load = getLoad(qty);
		Serial.print("FLAGGED LOAD; ");
		Serial.println(load);
		return load;
	}

	float reTare(int qty) {
		tare = getLoad(qty);
		return tare;
	}

	float getTaredLoad(int qty) {
		return getLoad(qty) - tare;
	}

	long getVoltage() {
		return weight.raw_read(1);
	}

	float readGrams() {
		return factor * log(((float)weight.raw_read(1))) + offset;
	}
};