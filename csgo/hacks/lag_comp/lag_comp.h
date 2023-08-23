#pragma once

enum e_r_mode : int
{
	r_anim_logic,
	r_auto_wall_logic
};

namespace hacks {
	struct lag_backup_t {
		ALWAYS_INLINE constexpr lag_backup_t( ) = default;

		ALWAYS_INLINE lag_backup_t( valve::cs_player_t* const player );

		ALWAYS_INLINE void restore( valve::cs_player_t* const player ) const;

		float			m_foot_yaw{}, m_sim_time{}, m_obb_change_time{}, m_obb_change_height{};
		sdk::vec3_t			m_origin{}, m_abs_origin{},
			m_obb_min{}, m_obb_max{};

		valve::e_ent_flags m_flags{};

		valve::bones_t	m_bones{};
		int				m_bones_count{},
			m_readable_bones{}, m_writable_bones{};

		unsigned long	m_mdl_bone_counter{};

	};

	struct lag_anim_data_t;
	
	struct player_entry_t;

	struct lag_record_t {
		struct anim_side_t {
			valve::bones_t m_bones{};
			std::size_t m_bones_count{};

			float m_foot_yaw{}, m_prev_foot_yaw{}, m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
				m_move_yaw{}, m_move_weight_smoothed{};

			valve::anim_layers_t m_anim_layers{};
			valve::pose_params_t m_pose_params{};
		};

		ALWAYS_INLINE lag_record_t( ) = default;

		ALWAYS_INLINE lag_record_t( valve::cs_player_t* player ) : m_flags{ player->flags( ) },
			m_dormant{ player->networkable( )->dormant( ) },
			m_sim_ticks{ valve::to_ticks( player->sim_time( ) - player->old_sim_time( ) ) },
			m_choked_cmds{ m_sim_ticks }, m_receive_tick{ valve::g_client_state->m_server_tick },
			m_sim_time{ player->sim_time( ) }, m_old_sim_time{ player->old_sim_time( ) },
			m_lby{ player->lby( ) }, m_duck_amt{ player->duck_amt( ) },
			m_third_person_recoil{ player->third_person_recoil( ) }, m_move_type{player->move_type( )},
			m_walking{ player->walking( ) },
			m_eye_angles{player->eye_angles( )}, m_velocity{player->velocity( )}, m_origin{player->origin( )}, m_anim_layers{player->anim_layers( )},
			m_obb_min{ player->obb_min( ) }, m_obb_max{ player->obb_max( ) } {
			m_weapon = player->weapon( );

			m_last_shot_time = m_weapon ? m_weapon->last_shot_time( ) : std::numeric_limits< float >::max( );
		};

		ALWAYS_INLINE bool valid( ) const;

		ALWAYS_INLINE void setup( valve::cs_player_t* const player, const std::ptrdiff_t anim_side ) const;

		ALWAYS_INLINE void simulate( lag_anim_data_t* previous, player_entry_t& entry );

		bool m_broke_lc{}, m_dormant{}, m_shot{}, m_invalid_sim{};

		valve::e_ent_flags m_flags{}, m_interp_flags{};

		std::ptrdiff_t m_sim_ticks{}, m_choked_cmds{}, m_anim_side{}, m_receive_tick{}, m_velocity_sync_stage{};

		float m_sim_time{}, m_old_sim_time{}, m_lby{}, m_duck_amt{}, m_third_person_recoil{}, m_last_shot_time{};

		bool m_force_safe_points{}, m_anim_resolver{}, m_jitter_correction_applied{};

		valve::e_move_type m_move_type{};

		bool m_walking{}, m_jittering{};

		e_r_mode m_r_mode{};

		std::optional< bool > m_on_ground{};
		float m_act_time{}, m_time_diff{};

		sdk::qang_t m_eye_angles_diff{};

		sdk::vec3_t m_velocity_diff{};

		float m_duck_amt_diff{}, m_lby_diff{}, m_anim_time{};

		valve::weapon_cs_base_gun_t* m_weapon{};

		sdk::qang_t m_eye_angles{};
		
		sdk::vec3_t m_velocity{}, m_origin{}, m_obb_min{}, m_obb_max{};

		valve::anim_layers_t m_anim_layers{};

		std::array< anim_side_t, 3u > m_anim_sides{};
	};

	struct lag_anim_data_t {
		ALWAYS_INLINE constexpr lag_anim_data_t( ) = default;

		ALWAYS_INLINE lag_anim_data_t( const lag_record_t* const lag_record );

		struct anim_side_t {
			float	m_foot_yaw{}, m_prev_foot_yaw{},
				m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
				m_move_yaw{}, m_move_weight_smoothed{};
		};

		valve::weapon_cs_base_gun_t* m_weapon{};

		sdk::qang_t m_eye_angles{};

		sdk::vec3_t m_origin{}, m_velocity{};

		valve::e_ent_flags m_flags{};

		float m_lby{}, m_duck_amt{}, m_third_person_recoil{};

		bool m_dormant{};

		valve::anim_layers_t m_anim_layers{};

		float m_sim_time{};

		std::array< anim_side_t, 3u > m_anim_sides{};
	};

	struct player_entry_t {
		ALWAYS_INLINE void reset( );
		
		valve::cs_player_t* m_player{};
		
		float m_spawn_time{}, m_receive_time{}, m_alive_loop_cycle{}, m_alive_loop_rate{}, m_cur_sim{}, m_old_sim{},
			m_last_sim{}, m_broke_sim{};

		valve::bones_t m_bones{};

		std::array < sdk::vec3_t, valve::k_max_bones > m_bone_origins{};

		std::ptrdiff_t m_records_count{}, m_misses{}, m_last_anim_side{}, m_jitter_misses{};
		
		sdk::vec3_t m_render_origin{};

		std::optional< lag_anim_data_t > m_prev_anim_data{};

		std::deque < std::shared_ptr < lag_record_t > > m_lag_records{};
	};

	class c_lag_comp final {
	private:
		std::array< player_entry_t, 64u > m_entries{};
	public:
		void handle( );

		ALWAYS_INLINE float calc_time_delta( const float sim_time );

		ALWAYS_INLINE player_entry_t& entry( const std::size_t i );
	};

	inline const auto g_lag_comp = std::make_unique < c_lag_comp >( );
}

#include "impl/lag_comp.inl"