#pragma once
#include "entity.hpp"
#include "../render/sprite_animation_state.hpp"
#include "../render/image_holder.hpp"
#include <SFML/System.hpp>

class GraveMarker : public Entity
{
public:
	GraveMarker() : Entity(),
					m_timer(0),
					m_timeout(5),
					m_timedout(false),
					m_anim()
	{
		set_type(EntityType::StaticObject);
		m_anim.add_texture_id_to_row(0, ImageHolder::instance().lookup("rip_01"));
		m_anim.add_texture_id_to_row(1, ImageHolder::instance().lookup("anim_expl_00_00"));
		m_anim.add_texture_id_to_row(1, ImageHolder::instance().lookup("anim_expl_00_01"));
		m_anim.add_texture_id_to_row(1, ImageHolder::instance().lookup("anim_expl_00_02"));
		m_anim.add_texture_id_to_row(1, ImageHolder::instance().lookup("anim_expl_00_03"));
		m_anim.add_texture_id_to_row(1, ImageHolder::instance().lookup("anim_expl_00_04"));
		m_anim.set_anim_speed(0.15f);
		m_anim.play(0);
	}

	auto update(float dt) -> void override
	{
		m_timer += dt;
		m_anim.update(dt);

		if ((m_timer > m_timeout) && !m_timedout)
		{
			m_timedout = true;
			m_anim.play(1);
		}

		if (m_anim.get_current_row() != 0 && m_anim.has_finished())
			destroy();
	}

	auto get_texture_id() const -> int override final
	{
		return m_anim.get_texture_id();
	}

private:
	float m_timer;
	float m_timeout;
	bool m_timedout;
	SpriteAnimationState m_anim;
};
