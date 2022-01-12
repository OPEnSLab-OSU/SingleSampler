#pragma once
#include <KPFoundation.hpp>
#include <SPI.h>
#include <ADS1232.h>
#include <FileIO/SerialSD.hpp>
#include <time.h>

#define SPI_SCK     13
#define SPI_MOSI    11

//SPI_MISO same as DOUT
//SPI_CS same as SCLK
#define SPI_MISO     5
#define SCALE_CS     0
#define SCALE_PDWN   A3



class LoadCell : public KPComponent {
public:

	float tare;
	float factor = 0.002377;
	float offset = -19954.570;
  	int32_t reading;
	int32_t sum;

	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller) {}
	void setup() override {
 		/* Initialize SPI bus */
  		SPI.setSCK(SPI_SCK);
  		SPI.setMOSI(SPI_MOSI);
  		SPI.setMISO(SPI_MISO);
  		SPI.begin();

		scale_adc.init(ADS1232::GAIN1);
		tare = 0;
		SSD.println("Start Time");
		SSD.println(now());
		SSD.print("Initial load; ");
		SSD.println(reTare(50));
	}
	long read(int qty) {

			println();
			sum = 0;
			//display every reading and print to SD
			for (int i = 0; i < qty; ++i) {
				reading = scale_adc.read();
				#ifdef LOAD_CAL
				SSD.print("Load reading ");
				SSD.print(i);
				SSD.print(";; ");
				SSD.println(reading);
				#endif
				sum += reading;
			}
			reading = sum/qty;
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
		return reading = scale_adc.read();
	}

	float readGrams() {
		return factor * reading = scale_adc.read() + offset;
	}
};