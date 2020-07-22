#pragma once
#include <KPState.hpp>
#include <Application/Constants.hpp>

namespace StateNames {
	constexpr const char * IDLE		= "state-idle";
	constexpr const char * FLUSH	= "state-flush";
	constexpr const char * SAMPLE	= "state-sample";
	constexpr const char * STOP		= "state-stop";
	constexpr const char * FINISHED = "state-finsihed";
}  // namespace StateNames

class StateIdle : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	unsigned long time = DefaultTimes::IDLE_TIME;
};

class StateFlush : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	unsigned long time = DefaultTimes::FLUSH_TIME;
};

class StateSample : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	void leave(KPStateMachine & sm) override;
	unsigned long time = DefaultTimes::SAMPLE_TIME;
};

class StateStop : public KPState {
public:
	void enter(KPStateMachine & sm) override;
};

class StateFinished : public KPState {
public:
	void enter(KPStateMachine & sm) override;
};