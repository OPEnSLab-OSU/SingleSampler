#include <FileIO/Logger.hpp>
#include <Application/Application.hpp>
#include <time.h>
#include <sstream>

// Write out to debug.csv the time, currentStateName, and pressure.
void Logger::log() {
	// Only every buffer_time millis
	if (millis() - last_time > buffer_time) {
#ifdef WRITERDEBUG
		Serial.print("Writing...");
#endif
		Application & app  = *static_cast<Application *>(controller);
		const auto timenow = app.clock.getTime();
		std::stringstream ss;
		ss << timenow;
		std::string time_string = ss.str();

		std::string strings[3] = {time_string, app.sm.getCurrentStateName(), "0"};
		writer.writeStrings(strings, 3);
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