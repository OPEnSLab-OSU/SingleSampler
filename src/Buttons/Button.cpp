#include <Buttons/Button.hpp>
//#include <Application/Application.hpp>

void act(StateMachine & sm) {
	// Application & app = *static_cast<Application *>(sm.controller);
	if (sm.isBusy()) {
		sm.halt();
	} else {
		sm.begin();
	}
}