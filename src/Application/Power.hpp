#pragma once

#include <KPFoundation.hpp>
#include <DS3232RTC.h>
#include <Application/Constants.hpp>
#include <Wire.h>
#include <SPI.h>

#define RTC_ADDR 0x68

class Power : public KPComponent {
public:
	DS3232RTC rtc;
	Power(const char * name) : KPComponent(name), rtc(false) {}
	void setup() {
		// waitForConnection();
		rtc.begin();
		rtc.squareWave(SQWAVE_NONE);
	}
	void set(unsigned long long seconds) {
		rtc.set(seconds);
	}
	unsigned long getTime() {
		return rtc.get();
	}

	// don't need
	void waitForConnection() {
		Wire.begin();
		for (;; delay(5000)) {
			Wire.requestFrom(RTC_ADDR, 1, false);
			if (Wire.read() == -1) {
				Serial.println("RTC not connected.");
			} else {
				Serial.println("RTC connected");
				break;
			}
		}

		Wire.end();
	}
};