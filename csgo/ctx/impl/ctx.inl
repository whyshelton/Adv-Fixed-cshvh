#pragma once

#include "../ctx.hpp"

ALWAYS_INLINE auto& c_ctx::offsets( ) { return m_offsets; }

ALWAYS_INLINE const auto& c_ctx::cvars( ) const { return m_cvars; }

ALWAYS_INLINE bool& c_ctx::allow_update_anim( ) { return m_allow_update_anim; };

ALWAYS_INLINE bool& c_ctx::allow_setup_bones( ) { return m_allow_setup_bones; };
