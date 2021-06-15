#pragma once
#include <FileIO/CSVWriter.hpp>
#include <String>
class SerialSD {
public:
	CSVWriter csvw{"serial.csv"};

	void print(const char * str) {
		std::string temp[1] = {str};
		csvw.writeStrings(temp, 1);
		Serial.print(str);
	}
	void print(double d) {
		char buf[50];
		sprintf(buf, "%d.%02u", (int)d, (int)((d - (int)d) * 100));
		this->print(buf);
	}
	void println(const char * str) {
		std::string temp[1] = {str};
		csvw.writeStrings(temp, 1);
		Serial.println(str);
	}
	void println(double d) {
		char buf[50];
		sprintf(buf, "%d.%02u", (int)d, (int)((d - (int)d) * 100));
		this->println(buf);
	}
};
extern SerialSD SSD;