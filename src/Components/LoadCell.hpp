#pragma once
#include <KPFoundation.hpp>
#include <Adafruit_ADS1015.h>
#include <array>
#include <algorithm>
#include <iterator>
#define AVG 10

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
		std::array<float, AVG> load_arr;
		for (int i = 0; i < AVG; ++i) {
			load_arr.at(i) = (float)ads.readADC_SingleEnded(0);
		}
		std::sort(load_arr.begin(), load_arr.end());
		return load_arr.at(load_arr.size() / 2);
	}
	float getLoad() {
		return read() * factor - offset;
	}
	void reTare() {
		tare = getLoad();
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