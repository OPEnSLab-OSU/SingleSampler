#pragma once
#include <KPState.hpp>

namespace StateNames {
	constexpr const char * IDLE	  = "state-idle";
	constexpr const char * FLUSH  = "state-flush";
	constexpr const char * SAMPLE = "state-sample";
	constexpr const char * STOP	  = "state-stop";
}  // namespace StateNames

class StateIdle : public KPState {
	void enter(KPStateMachine & sm) override;
};

class StateFlush : public KPState {
	void enter(KPStateMachine & sm) override;
};

class StateSample : public KPState {
	void enter(KPStateMachine & sm) override;
};

class StateStop : public KPState {
	void enter(KPStateMachine & sm) override;
};