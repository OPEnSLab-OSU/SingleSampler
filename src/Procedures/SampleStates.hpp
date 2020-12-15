#pragma once
#include <KPState.hpp>
#include <Application/Constants.hpp>

namespace SampleStateNames {
	constexpr const char * IDLE			 = "sample-state-idle";
	constexpr const char * FLUSH		 = "sample-state-flush";
	constexpr const char * SAMPLE		 = "sample-state-sample";
	constexpr const char * STOP			 = "sample-state-stop";
	constexpr const char * FINISHED		 = "sample-state-finished";
	constexpr const char * PURGE		 = "sample-state-purge";
	constexpr const char * SETUP		 = "sample-state-setup";
	constexpr const char * ONRAMP		 = "sample-state-onramp";
	constexpr const char * BETWEEN_PUMP	 = "sample-state-between-pump";
	constexpr const char * BETWEEN_VALVE = "sample-state-between-valve";
};	// namespace SampleStateNames

class SampleStateIdle : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = DefaultTimes::IDLE_TIME;
};

class SampleStateFlush : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = DefaultTimes::FLUSH_TIME;
};

class SampleStateSample : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	void leave(KPStateMachine & sm) override;
	bool badPressure();
	int time   = DefaultTimes::SAMPLE_TIME;
	int volume = 100;
};

class SampleStateStop : public KPState {
public:
	void enter(KPStateMachine & sm) override;
};

class SampleStateFinished : public KPState {
public:
	void enter(KPStateMachine & sm) override;
};

// Obsolete
class SampleStatePurge : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	// void leave(KPStateMachine & sm) override;
	unsigned long time = DefaultTimes::PURGE_TIME;
};

class SampleStateSetup : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = DefaultTimes::SETUP_TIME;
	int tod_enabled;
	int tod;
};

class SampleStateOnramp : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = 5;
};

class SampleStateBetweenPump : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = 5;
};

class SampleStateBetweenValve : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = 5;
};