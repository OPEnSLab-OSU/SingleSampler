#include <FileIO/Logger.hpp>
#include <Application/Application.hpp>
#include <time.h>
#include <sstream>

// Write out to log.csv the time, currentStateName, and pressure.
void Logger::log() {
	// Only every buffer_time millis
	if (millis() - last_time > buffer_time) {
		Serial.print("Writing...");
		Application & app  = *static_cast<Application *>(controller);
		const auto timenow = app.power.getTime();
		std::stringstream ss;
		ss << timenow;
		std::string time_string = ss.str();

		std::string strings[3] = {time_string, app.sm.getCurrentStateName(), "0"};
		writer.writeStrings(strings, 3);
		last_time = millis();
	}
}