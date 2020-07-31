#include <KPFoundation.hpp>
#include <Components/StateMachine.hpp>
class Button : public KPComponent {
public:
	unsigned short pin;
	Button(unsigned short n_pin, const char * name) : pin(n_pin), KPComponent(name) {}
	virtual void listen(StateMachine & sm) {
		if (digitalRead(pin)) {
			act(sm);
		}
	}
	virtual void act(StateMachine & sm) {
		sm.begin();
	};
};