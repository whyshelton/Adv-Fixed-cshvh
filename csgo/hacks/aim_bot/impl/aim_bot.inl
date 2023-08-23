#pragma once

#include "../aim_bot.h"

namespace hacks {
	ALWAYS_INLINE int& c_aim_bot::stop_type( ) { return m_stop_type; }

	ALWAYS_INLINE sdk::vec3_t& c_aim_bot::shoot_pos( ) { return m_shoot_pos; }

	ALWAYS_INLINE bool& c_aim_bot::shooting( ) { return m_shooting; }
}