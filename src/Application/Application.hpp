#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPStateMachine.hpp>

// subclassing?

class Application : public KPController {
public:
	KPStateMachine sm{"state-machine"};
	void setup() override {
		addComponent(sm);
		Serial.print("Hi");
	}
	void update() override {
		KPController::update();
	}
};