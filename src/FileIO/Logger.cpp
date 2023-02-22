#include <FileIO/Logger.hpp>
#include <Application/Application.hpp>
#include <time.h>
#include <sstream>

// Write out to log.csv the time, currentStateName, and pressure.
void Logger::log() {
	// Only every buffer_time millis
	if (millis() - last_time > buffer_time) {
#ifdef WRITERDEBUG
		Serial.print("Writing...");
#endif
		Application & app = *static_cast<Application *>(sm.controller);
		const auto timenow = now();
		std::stringstream ss;
		ss << timenow;
		std::string time_string = ss.str();
        float current_load = app.load_cell.getLoad(1);
        char load_string[50];
	    sprintf(load_string, "%d.%02u", (int)current_load, (int)((current_load - (int)current_load) * 100));
		std::string strings[2] = {time_string, load_string};
		writer.writeStrings(strings, 2);
		last_time = millis();
	}
}

void Logger::writeStrings(const std::string * strs, int len) {
	writer.writeStrings(strs, len);
}

void Logger::update() {
	Application & app = *static_cast<Application *>(controller);
	if (app.sm.isRunning())
		log();
}