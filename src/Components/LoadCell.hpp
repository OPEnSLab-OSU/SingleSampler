#pragma once
#include <KPFoundation.hpp>
#include <Adafruit_ADS1015.h>
#ifdef MEDIAN
	#include <array>
	#include <algorithm>
	#include <iterator>
#endif
#define AVG 41

class LoadCell : public KPComponent {
public:
	Adafruit_ADS1115 ads;
	int32_t tare;

	float factor = 0.0832;
	float offset = 279.56;
	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller), ads(0x48) {}
	void setup() override {
		ads.setGain(GAIN_ONE);
		ads.begin();
		tare = 0;
		reTare();
	}
	float read() {
#ifdef MEDIAN
		std::array<float, AVG> load_arr;
		for (int i = 0; i < AVG; ++i) {
			load_arr.at(i) = (float)ads.readADC_SingleEnded(0);
		}
		std::sort(load_arr.begin(), load_arr.end());
		return load_arr.at(load_arr.size() / 2);
#endif
#ifndef MEDIAN
		float sum = 0;
		for (int i = 0; i < AVG; ++i) {
			sum += (float)ads.readADC_SingleEnded(0);
		}
		return sum / AVG;
#endif
	}

	float getLoad() {
		return read() * factor - offset;
	}

#ifdef INFO_SPAM
	float getLoadPrint() {
		float load = getLoad();
		Serial.println("FLAGGED LOAD: ", load);
		return load;
	}
#endif

	void reTare() {
#ifndef INFO_SPAM
		tare = getLoad();
#endif
#ifdef INFO_SPAM
		tare = getLoadPrint();
#endif
	}

	int getTaredLoad() {
		return getLoad() - tare;
	}

	int getVoltage() {
		return ads.readADC_SingleEnded(0);
	}

	float readGrams() {
		return ((float)ads.readADC_SingleEnded(0)) * factor - offset;
	}
};