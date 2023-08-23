#pragma once

namespace hacks {
	struct pen_data_t {
		valve::cs_player_t* m_hit_player{};
		int					m_dmg{}, m_hitbox{}, m_hitgroup{}, m_remaining_pen{};
	};

	class c_auto_wall {
	private:
		bool trace_to_exit(
			const sdk::vec3_t& src, const sdk::vec3_t& dir,
			const valve::trace_t& enter_trace, valve::trace_t& exit_trace
		) const;

		void clip_trace_to_player(
			const sdk::vec3_t& src, const sdk::vec3_t& dst, valve::trace_t& trace,
			valve::cs_player_t* const player, const valve::should_hit_fn_t& should_hit_fn
		) const;

		bool handle_bullet_penetration(
			valve::cs_player_t* const shooter, const valve::weapon_info_t* const wpn_data,
			const valve::trace_t& enter_trace, sdk::vec3_t& src, const sdk::vec3_t& dir, int& pen_count,
			float& cur_dmg, const float pen_modifier
		) const;
	public:
		void scale_dmg(
			valve::cs_player_t* const player, float& dmg,
			const float armor_ratio, const float headshot_mult, const int hitgroup
		) const;

		pen_data_t fire_bullet(
			valve::cs_player_t* const shooter, valve::cs_player_t* const target,
			const valve::weapon_info_t* const wpn_data, const bool is_taser, sdk::vec3_t src, const sdk::vec3_t& dst
		) const;

		pen_data_t fire_emulated(
			valve::cs_player_t* const shooter, valve::cs_player_t* const target, sdk::vec3_t src, const sdk::vec3_t& dst
		) const;

		ALWAYS_INLINE bool breakable( valve::base_entity_t* const entity );

	};

	inline const auto g_auto_wall = std::make_unique< c_auto_wall >( );
}

#include "impl/auto_wall.inl"