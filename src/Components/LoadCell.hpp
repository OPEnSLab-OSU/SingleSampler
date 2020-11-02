#pragma once
#include <KPFoundation.hpp>
#include <Adafruit_ADS1015.h>

class LoadCell : public KPComponent {
	Adafruit_ADS1115 ads;
	int32_t tare;
	LoadCell(const char * name, KPController * controller) : KPComponent(name, controller) {}
	void setup() override {
		ads.setGain(GAIN_ONE);
		ads.begin();
		tare = 0;
	}
	int getLoad() {
		int32_t adcTotal = 0;
		for (int i = 0; i < 10; ++i) {
			adcTotal += ads.readADC_SingleEnded(0);
		}
		return adcTotal / 10;
	}
	void reTare() {
		tare = getLoad();
	}
	int getTaredLoad() {
		return getLoad() - tare;
	}
};