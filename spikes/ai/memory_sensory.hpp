#pragma once

#include "../math.hpp"
#include "memory_record.hpp"

#include <unordered_map>

class TheBot;

class MemorySensory {
public:
	MemorySensory(TheBot* owner, float memory_span = 10.f /*secs*/);

	void     update_all_bots_wihtin_fov(float dt);
	void     update_with_sound_source(TheBot* noise_marker, float dt);
	void     remove_bot_from_memory(TheBot* bot);

	bool     is_opponent_shootable(TheBot* opponent)const;
	bool     is_opponent_within_fov(TheBot* opponent)const;
	v2       get_last_recorded_pos_of_opponent(TheBot* opponent)const;
	float    get_time_opponent_has_been_visible(TheBot* opponent)const;
	float    get_time_since_last_sensed(TheBot* opponent)const;
	float    get_time_opponent_has_been_out_of_view(TheBot* opponent)const;
	std::list<TheBot*>& get_recently_sensed_opponents()const;

private:
	void       make_new_record_if_not_present(TheBot* bot);

private:
	TheBot*    m_owner;
	float      m_memory_time_span; // time span until the bot is removed from memory
	std::unordered_map<TheBot*, MemoryRecord>  m_memory_map;
};