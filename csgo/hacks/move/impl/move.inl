#pragma once

#include "../move.hpp"

namespace hacks {
	ALWAYS_INLINE float& c_move::prev_view_yaw( ) {
		return m_prev_view_yaw;
	}

	ALWAYS_INLINE bool& c_move::should_fake_duck( ) {
		return m_should_fake_duck;
	}

	ALWAYS_INLINE bool& c_move::allow_early_stop( ) { return m_allow_early_stop; }

	ALWAYS_INLINE c_move::auto_peek_t& c_move::auto_peek_data( ) { return m_auto_peek; }
}