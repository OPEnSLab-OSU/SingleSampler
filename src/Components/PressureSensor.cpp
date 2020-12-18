#include <Components/PressureSensor.hpp>
#include <Application/Application.hpp>
#include <string>
bool PressureSensor::checkPressure() {
	bool within_pressure = isWithinPressure();
	if (!kill_clock && !within_pressure) {
		kill_clock = true;
		kill_time  = millis() + kill_time_offset;
	} else if (kill_clock && !within_pressure) {
		if (millis() >= kill_time) {
			Application & app  = *static_cast<Application *>(controller);
			std::string str[1] = {"Pressure out of bounds, aborting!"};
			app.logger.writeStrings(str, 1);
			kill_clock = false;
			return false;
		}
	} else if (kill_clock && within_pressure) {
		kill_clock = false;
	}
	return true;
}