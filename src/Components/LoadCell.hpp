#pragma once
#include <KPFoundation.hpp>

class LoadCell : public KPComponent {
	LoadCell(const char * name, KPController * controller) : KPComponent(name, controller) {}
	void setup() override {}
	int getLoad() {}
};