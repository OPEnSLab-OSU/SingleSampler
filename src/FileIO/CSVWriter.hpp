#pragma once
#include <KPFoundation.hpp>
#include <SdFat.h>
#include <string>
#include <OPEnS_RTC.h>


class CSVWriter {
public:
	const char * dir;
	SdFat sd;
	File myFile;
	CSVWriter(const char * dir) : dir(dir) {
	}
	void writeStrings(const std::string * strs, int len, DateTime currentTime) {
		myFile = sd.open(dir, FILE_WRITE);
		// Set the date created timestamp of the File
		myFile.timestamp(T_CREATE, currentTime.year(), currentTime.month(), currentTime.day(), currentTime.hour(), currentTime.minute(), currentTime.second());
		for (int i = 0; i < len; ++i) {
			myFile.print(strs[i].c_str());
			//if (i != len - 1)
			//	file.print(" ");
		}
		myFile.write("\n");
		
#ifdef WRITERDEBUG
		if (file) {
			Serial.print("Written to ");
			Serial.print(dir);
			Serial.print("\n");
		}
#endif
		// Set the last modified date
            myFile.timestamp(T_WRITE , currentTime.year(), currentTime.month(), currentTime.day(), currentTime.hour(), currentTime.minute(), currentTime.second());

		myFile.close();
	}


};