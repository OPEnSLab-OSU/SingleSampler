#pragma once
#include <KPState.hpp>
#include <Application/Constants.hpp>

namespace EmptyStateNames {
	constexpr const char * IDLE		= "state-idle-empty";
	constexpr const char * STOP		= "state-stop-empty";
	constexpr const char * FINISHED = "state-finished-empty";
	constexpr const char * PURGE	= "state-purge-empty";
}  // namespace EmptyStateNames

class StateIdleEmpty : public KPState {
	void enter(KPStateMachine & sm) override;
	unsigned long time = DefaultTimesEmpty::IDLE_TIME;
};
class StatePurgeEmpty : public KPState {
	void enter(KPStateMachine & sm) override;
	void leave(KPStateMachine & sm) override;
	unsigned long time = DefaultTimesEmpty::PURGE_TIME;
};
class StateFinishedEmpty : public KPState {
	void enter(KPStateMachine & sm) override;
};

class StateStopEmpty : public KPState {
	void enter(KPStateMachine & sm) override;
};