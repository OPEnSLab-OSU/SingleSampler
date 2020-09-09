#include <KPFoundation.hpp>
class LED : public KPComponent {
public:
	unsigned short r;
	unsigned short g;
	unsigned short b;
	LED(const char * name, KPController * controller) : KPComponent(name, controller) {}
	void setup() {}

	void setColor(unsigned short nr, unsigned short ng, unsigned short nb) {
		r = nr;
		g = ng;
		b = nb;
		// set color rgb
	}

	// void setRun() {}			// green
	// void setSoftwareError() {}	//
	// void set
	// void setFinished() {}
};