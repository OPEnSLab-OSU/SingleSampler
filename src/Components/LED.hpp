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
		: KPComponent(name, controller), pixel(1, TPICDevices::PIXEL, NEO_RGB + NEO_KHZ400) {}
	void setup() {
		pixel.begin();
	}

	void setColor(unsigned short nr, unsigned short ng, unsigned short nb) {
		r = nr;
		g = ng;
		b = nb;
		pixel.setPixelColor(0, r, g, b);
	}

	void setRun() {
		setColor(0, 255, 0);
	}
	void setIdle() {
		setColor(0, 0, 255);
	}
	void setSoftwareError() {}	//
	void setBlockage() {}
	void setFinished() {}
};