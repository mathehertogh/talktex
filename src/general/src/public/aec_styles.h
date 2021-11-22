/**
 * A bunch of global AEC++ styles we use throughout the code.
 */

#pragma once

#include "aecpp.h"

namespace aec_style {
	inline const auto error    = aec::bold + aec::red;
	inline const auto filename = aec::bold + aec::yellow;
	inline const auto input    = aec::bold + aec::green;
}
