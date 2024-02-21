#pragma once
#include "../math.hpp"

struct MemoryRecord {
	float time_last_sensed;
	float time_became_visible;
	float time_last_visible;
	v2    last_sensed_pos;
	bool  within_fov;
	bool  shootable;

	MemoryRecord() : 
		time_last_sensed{-999}
		, time_became_visible{ -999 }
		, time_last_visible{ -999 }
		, last_sensed_pos{}
		, within_fov{ false }
		, shootable{ false }{}
};
