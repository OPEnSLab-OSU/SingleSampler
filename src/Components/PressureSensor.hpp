#include <KPFoundation.hpp>

class PressureSensor : public KPComponent {
	PressureSensor(const char * name, KPController * controller) : KPComponent(name, controller) {}

	void setup() override {
		// Attatch the pressure sensor?
	}

	int getPressure() {
		// return pressure ig?
	}
};