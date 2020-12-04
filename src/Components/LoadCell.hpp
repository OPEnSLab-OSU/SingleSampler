#pragma once
#include <KPFoundation.hpp>
#include <Adafruit_ADS1015.h>

class LoadCell : public KPComponent {
public:
	Adafruit_ADS1115 ads;
	int32_t tare;

	const float factor = 0.5353;
	const int offset   = 10402;
	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller), ads(0x48) {}
	void setup() override {
		ads.setGain(GAIN_ONE);
		ads.begin();
		tare = 0;
		reTare();
	}
	int getLoad() {
		int32_t adcTotal = 0;
		for (int i = 0; i < 10; ++i) {
			adcTotal += readGrams();
		}
		return adcTotal / 10;
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

	int32_t readGrams() {
		return ads.readADC_SingleEnded(0) * factor - offset;
	}
};