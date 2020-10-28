#pragma once
//#include <Application/Application.hpp>
#include <SD.h>
#include <string>
#include <Application/Constants.hpp>

namespace SensorUtility {
	bool isWithinPressure(int p) {
		return (p >= DefaultPressures::MIN_PRESSURE && p <= DefaultPressures::MAX_PRESSURE);
	}

	bool voltageToGrams() {}
}  // namespace SensorUtility

namespace Utility {
	// returns true if str is a pos int
	// assumes string correctness
	bool is_posint(const char * str) {
		for (int i = 0; str[i] != '\0'; ++i) {
			if (str[i] > '9' || str[i] < '0') {
				return false;
			}
		}
		return true;
	}

	// returns true if str is a pos int
	// displays a msg if it isn't
	bool msg_posint(const char * str, int arg) {
		bool posint = is_posint(str);
		if (!posint) {
			Serial.print("Error: argument ");
			Serial.print(arg);
			Serial.print(" needs to be a pos int.");
		}
		return posint;
	}

	std::string readEntireFile(File & file) {
		std::string contents;
		while (-1 != file.peek()) {
			contents.push_back(file.read());
		}
		return contents;
	}

};	// namespace Utility