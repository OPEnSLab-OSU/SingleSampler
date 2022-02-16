#pragma once
#include <KPFoundation.hpp>
#include <ADS1232.h>
#include <FileIO/SerialSD.hpp>
#include <time.h>

#define _dout A3
#define _sclk 0
#define _pdwn 1

class LoadCell : public KPComponent {
public:

	ADS1232 weight = ADS1232(_pdwn, _sclk, _dout);
	float tare;
	float factor = 0.002377;
	float offset = -19954.570;
	long reading = 0;
	long sum;

	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller) {}
	void setup() override {
		weight.power_up();
		tare = 0;
		SSD.println("Start Time");
		SSD.println(now());
		SSD.print("Initial load; ");
		SSD.println(reTare(50));
	}
	long read(int qty) {
		//println("in read");
		#ifdef LOAD_CAL
			println();
			sum = 0;
			//display every reading and print to SD
			for (int i = 0; i < qty; ++i) {
				reading = weight.raw_read(1);
				SSD.print("Load reading ");
				SSD.print(i);
				SSD.print(";; ");
				SSD.println(reading);
				sum += reading;
			}
			reading = sum/qty;
		#endif
		#ifndef LOAD_CAL
			// get average value for qty reads
			reading = weight.raw_read(qty);
		#endif
		return reading;
	}

	float getLoad(int qty) {
		// gets factor and offset from this file during setup, gets factor and offset from SD after
		//println("in getLoad");
		return factor * read(qty) + offset;
	}

	float getLoadPrint(int qty) {
		float load = getLoad(qty);
		SSD.print("FLAGGED LOAD; ");
		SSD.println(load);
		return load;
	}

	float reTare(int qty) {
		//println("in reTare");
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
		//println("in readGrams");
		return factor * (long)weight.raw_read(1) + offset;
	}
};