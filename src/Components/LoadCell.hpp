#pragma once
#include <KPFoundation.hpp>
#include <ADS123X.h>
#include <FileIO/SerialSD.hpp>
#include <time.h>

#define SCALE_DOUT   5
#define SCALE_SCLK   0
#define SCALE_PDWN   A3

class LoadCell : public KPComponent {
public:

	ADS123X weigh;

	float tare;
	float factor = 0.002377;
	float offset = -19954.570;
	long reading = 0;
	long sum;

	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller) {}
	void setup() override {
		weigh.begin(SCALE_DOUT, SCALE_SCLK, SCALE_PDWN);
		tare = 0;
		SSD.println("Start Time");
		SSD.println(now());
		SSD.print("Initial load; ");
		SSD.println(reTare(50));
	}
	long read(int qty) {
		#ifdef LOAD_CAL
			println();
			sum = 0;
			//display every reading and print to SD
			for (int i = 0; i < qty; ++i) {
  				weigh.read(AIN1, reading);
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
			weigh.read(AIN1, reading);
		#endif
		return reading;
	}

	float getLoad(int qty) {
		// gets factor and offset from this file during setup, gets factor and offset from SD after
		return factor * read(qty) + offset;
	}

	float getLoadPrint(int qty) {
		float load = getLoad(qty);
		SSD.print("FLAGGED LOAD; ");
		SSD.println(load);
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
		return weigh.read(AIN1, reading);
	}

	float readGrams() {
		return factor * weigh.read(AIN1, reading) + offset;
	}
};