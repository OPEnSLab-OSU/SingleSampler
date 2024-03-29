#pragma once
#include <KPState.hpp>
#include <Application/Constants.hpp>

namespace SampleStateNames {
	constexpr const char * IDLE				= "sample-state-idle";
	constexpr const char * FLUSH			= "sample-state-flush";
	constexpr const char * SAMPLE			= "sample-state-sample";
	constexpr const char * STOP				= "sample-state-stop";
	constexpr const char * FINISHED			= "sample-state-finished";
	constexpr const char * PURGE			= "sample-state-purge";
	constexpr const char * SETUP			= "sample-state-setup";
	constexpr const char * ONRAMP			= "sample-state-onramp";
	constexpr const char * BETWEEN_PUMP		= "sample-state-between-pump";
	constexpr const char * BETWEEN_VALVE	= "sample-state-between-valve";
	constexpr const char * FILL_TUBE_ONRAMP = "sample-state-fill-tube-onramp";
	constexpr const char * FILL_TUBE		= "sample-state-fill-tube";
	constexpr const char * PRESSURE_TARE	= "sample-state-pressure-tare";
	constexpr const char * LOG_BUFFER		= "sample-state-log-buffer";
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

class SampleStateFillTubeOnramp : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = 5;
};

class SampleStateFillTube : public KPState {
public:
	void enter(KPStateMachine & sm) override;
	int time = 5;
};

// This sets the normal pressure range on the first cycle
class SampleStatePressureTare : public KPState {
	void enter(KPStateMachine & sm) override;
	void update(KPStateMachine & sm) override;
	void leave(KPStateMachine & sm) override;
	int time = 5;
	long sum;
	int count;
	int range_size = 200;
};

class SampleStateLogBuffer : public KPState {
	void enter(KPStateMachine & sm) override;
	void leave(KPStateMachine & sm) override;
	int time = 1;
};