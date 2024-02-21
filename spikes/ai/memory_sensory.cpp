#include "memory_sensory.hpp"
#include "../the_bot.hpp"
#include "../game_world.hpp"

MemorySensory::MemorySensory(TheBot* owner, float memory_span) :
	m_owner{ owner }
	, m_memory_time_span{memory_span}
	, m_memory_map{}
{
}

void MemorySensory::update_all_bots_wihtin_fov(float dt)
{
	const auto& bots = m_owner->get_world().get_bots();
	for (auto& b : bots) {
		if (b.get() == m_owner) continue;
		make_new_record_if_not_present(b.get());
		MemoryRecord& record = m_memory_map[b.get()];
	}
}

void MemorySensory::update_with_sound_source(TheBot* noise_marker, float dt)
{
	// if we are self just return
	if (m_owner == noise_marker) {
		return;
	}

	make_new_record_if_not_present(noise_marker);

	MemoryRecord& record = m_memory_map[noise_marker];

	//if (/*line of sight is ok*/) {
	//	record.shootable = true;
	//	record.last_sensed_pos = noise_marker->get_pos();
	//}
	//else {
	//	record.shootable = false;
	//}

	//record.time_last_sensed = m_timer;
}

void MemorySensory::remove_bot_from_memory(TheBot* bot)
{
	if (auto found = m_memory_map.find(bot); found != std::end(m_memory_map))
	{
		m_memory_map.erase(found);
	}
}

bool MemorySensory::is_opponent_shootable(TheBot* opponent) const
{
	return false;
}

bool MemorySensory::is_opponent_within_fov(TheBot* opponent) const
{
	return false;
}

v2 MemorySensory::get_last_recorded_pos_of_opponent(TheBot* opponent) const
{
	return v2();
}

float MemorySensory::get_time_opponent_has_been_visible(TheBot* opponent) const
{
	return 0.0f;
}

float MemorySensory::get_time_since_last_sensed(TheBot* opponent) const
{
	return 0.0f;
}

float MemorySensory::get_time_opponent_has_been_out_of_view(TheBot* opponent) const
{
	return 0.0f;
}

std::list<TheBot*>& MemorySensory::get_recently_sensed_opponents() const
{
	// TODO: insert return statement here
}

void MemorySensory::make_new_record_if_not_present(TheBot* bot)
{
	if (m_memory_map.find(bot) == std::end(m_memory_map)) {
		m_memory_map[bot] = MemoryRecord();
	}
}
