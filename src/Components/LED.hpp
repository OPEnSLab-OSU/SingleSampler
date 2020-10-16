#pragma once

#include <KPFoundation.hpp>
#include <Application/Constants.hpp>
#include <Adafruit_NeoPixel.h>
class LED : public KPComponent {
public:
	unsigned short r;
	unsigned short g;
	unsigned short b;
	Adafruit_NeoPixel pixel;
	LED(const char * name, KPController * controller)
		: KPComponent(name, controller), pixel(1, TPICDevices::PIXEL, NEO_RGB + NEO_KHZ800) {}
	void setup() {
		pixel.begin();
		setIdle();
	}

	void setColor(unsigned short nr, unsigned short ng, unsigned short nb) {
		pixel.clear();
		r = nr;
		g = ng;
		b = nb;
		pixel.setPixelColor(0, g, r, b);
		pixel.show();
	}

	void setRun() {
		setColor(0, 30, 0);
	}
	void setIdle() {
		setColor(0, 0, 30);
	}
	void setSoftwareError() {}	//
	void setBlockage() {
		setColor(30, 0, 0);
	}
	void setFinished() {
		setIdle();
	}
	void setClean() {
		setColor(10, 10, 10);
	}
};