#pragma once
#include <KPFoundation.hpp>
#include <Adafruit_ADS1015.h>
#ifdef MEDIAN
	#include <array>
	#include <algorithm>
	#include <iterator>
#endif

class LoadCell : public KPComponent {
public:
	Adafruit_ADS1115 ads;
	float tare;

	float factor = 7378.478054;
	float offset = -73328.85841;


	LoadCell(const char * name, KPController * controller)
		: KPComponent(name, controller), ads(0x48) {}
	void setup() override {
		ads.setGain(GAIN_ONE);
		ads.begin();
		tare = 0;
		print("Initial load; ");
		println(reTare(200));
	}
	float read(int qty) {
		#ifdef MEDIAN
			std::array<float, qty> load_arr;
			for (int i = 0; i < qty; ++i) {
				load_arr.at(i) = (float)ads.readADC_SingleEnded(0);
			}
				std::sort(load_arr.begin(), load_arr.end());
			return load_arr.at(load_arr.size() / 2);
		#endif
		#ifndef MEDIAN
				float sum = 0;
				int count = 0;
				for (int i = 0; i < qty; ++i) {
					//sum += (float)ads.readADC_SingleEnded(0);
					int16_t reading = ads.readADC_SingleEnded(0);
					#ifdef LOAD_CAL
						println();
						print("Load reading ");
						print(i);
						print(";; ");
						println(reading);
					#endif
					if (qty<70){
						sum += reading;
						count += 1;
						//print("Averaging all values over less than 70 readings, current sum: ");
						//println(sum);
					}				
					else{
						if (i>70){
							sum += reading;
							count += 1;
							//print("Averaging starting with value 71, current sum: ");
							//print(sum);
							//print(", count ");
							//println(count);
						}
					}

				}
				return sum / count;
		#endif
	}

	float getLoad(int qty) {
		// gets factor and offset from this file during setup, gets factor and offset from SD after
		//return read(qty) * factor - offset;
		//factor = 7378.478054;
		//offset = -73328.85841;
		return factor * log(read(qty)) + offset;
	}

	float getLoadPrint(int qty) {
		float load = getLoad(qty);
		Serial.print("FLAGGED LOAD; ");
		Serial.println(load);
		return load;
	}

	float reTare(int qty) {
		tare = getLoad(qty);
		return tare;
	}

	float getTaredLoad(int qty) {
		return getLoad(qty) - tare;
	}

	float getVoltage() {
		return ads.readADC_SingleEnded(0);
	}

	float readGrams() {
		//return ((float)ads.readADC_SingleEnded(0)) * factor - offset;
		return factor * log(((float)ads.readADC_SingleEnded(0))) + offset;
	}
};