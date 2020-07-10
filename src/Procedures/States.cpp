#include <Procedures/States.hpp>
#include <Application/Application.hpp>

void StateIdle::enter(KPStateMachine & sm) {}

void StateStop::enter(KPStateMachine & sm) {
	Application & app = *static_cast<Application *>(sm.controller);
	app.pump.off();
	app.shift.writeAllRegistersLow();
	app.shift.writeLatchOut();
}
