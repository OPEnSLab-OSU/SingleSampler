#include <Application/Constants.hpp>
namespace Conversions {
	int tubeLengthToPurgeTime(float len) {}
	int tubeLengthToFlushTime(float len) {}
	int valveNumberToPin(int num) {
		return num + Shift::FIRST_SAMPLE_VALVE;
	}
}  // namespace Conversions
