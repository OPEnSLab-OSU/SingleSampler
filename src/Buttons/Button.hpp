#include <KPFoundation.hpp>
#include <Components/StateMachine.hpp>
class Button : public KPComponent {
public:
	unsigned short pin;
	bool last;
	Button(unsigned short n_pin, const char * name) : pin(n_pin), KPComponent(name) {
		pinMode(pin, INPUT);
		last = digitalRead(pin);
	}
	virtual void listen(StateMachine & sm) {
		if (last != digitalRead(pin) && LOW == last) {
			act(sm);
		}
		last = digitalRead(pin);
	}
	virtual void act(StateMachine & sm) {
		sm.begin();
	};
};