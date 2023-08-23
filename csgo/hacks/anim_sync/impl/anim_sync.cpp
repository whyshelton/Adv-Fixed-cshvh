#include "../../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"
namespace hacks {
	ALWAYS_INLINE sdk::vec3_t origin( sdk::mat3x4_t who ) {
		return sdk::vec3_t( who[ 0 ][ 3 ], who[ 1 ][ 3 ], who[ 2 ][ 3 ] );
	}

	void c_anim_sync::handle(
		player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, const sdk::qang_t& angles, const bool custom_angles
	) {
		struct anim_backup_t {
			__forceinline constexpr anim_backup_t( ) = default;

			__forceinline anim_backup_t( valve::cs_player_t* const player, lag_record_t* const current )
				: m_anim_state{ *player->anim_state( ) }, m_abs_yaw{ m_anim_state.m_foot_yaw },
				m_anim_layers{ current->m_anim_layers }, m_pose_params{ player->pose_params( ) } {}

			__forceinline void restore( valve::cs_player_t* const player, bool restore_state ) const {

				if ( restore_state ) {
					*player->anim_state( ) = m_anim_state;
					player->pose_params( ) = m_pose_params;
				}
				player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

				player->anim_layers( ) = m_anim_layers;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};
				
			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{ entry.m_player, current };

		if ( !entry.m_player
			|| !entry.m_player->anim_state( )
			|| !entry.m_player->alive( ) )
			return; // won't happen if callin` it in lag_comp::handle but might cause any troubles when calling it w/ custom angles

		if ( custom_angles )
			current->m_jitter_correction_applied = true;

		setup_anim_data( entry, current, previous, custom_angles );

		rotate( entry, current, previous, 0, angles, custom_angles );

		anim_backup.restore( entry.m_player, true );

		if ( !entry.m_player->friendly( g_local_player->self( ) ) ) {
			valve::player_info_t player_info{};
			const auto player_info_got = valve::g_engine.get( )->get_player_info( entry.m_player->networkable( )->index( ), &player_info );

			if ( player_info_got ) {
				rotate( entry, current, previous, 2, angles, custom_angles );

				anim_backup.restore( entry.m_player, true );

				rotate( entry, current, previous, 1, angles, custom_angles );

				anim_backup.restore( entry.m_player, false );
			}
		}

		if ( !custom_angles )
		find_server_foot_yaw( entry, current, previous );

		entry.m_player->anim_layers( ) = current->m_anim_layers;

		const auto& anim_side = current->m_anim_sides.at( current->m_anim_side );

		entry.m_player->pose_params( ) = anim_side.m_pose_params;

		if ( !custom_angles ) {
			for ( int i = 0; i < 256; ++i ) {
				entry.m_bone_origins.at( i ) = entry.m_player->abs_origin( ) - origin( anim_side.m_bones.at( i ) );
			}

			std::memcpy( entry.m_bones.data( ), anim_side.m_bones.data( ), anim_side.m_bones_count * sizeof( sdk::mat3x4_t ) );
		}
	}

	void c_anim_sync::rotate(
		player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, int side, const sdk::qang_t& angles, const bool custom_angles
	) {
		const auto anim_state = entry.m_player->anim_state( );

		const auto backup_recorded_eye_angles = current->m_eye_angles;

		current->m_eye_angles = angles;

		if ( previous ) {
			entry.m_player->anim_layers( ) = previous->m_anim_layers;
			auto& prev_anim_side = previous->m_anim_sides.at( side );

			anim_state->m_foot_yaw = prev_anim_side.m_foot_yaw;
			anim_state->m_last_foot_yaw = prev_anim_side.m_prev_foot_yaw;
			anim_state->m_move_yaw = prev_anim_side.m_move_yaw;
			anim_state->m_move_yaw_cur_to_ideal = prev_anim_side.m_move_yaw_cur_to_ideal;
			anim_state->m_move_yaw_ideal = prev_anim_side.m_move_yaw_ideal;
			anim_state->m_move_weight_smoothed = prev_anim_side.m_move_weight_smoothed;

			anim_state->m_primary_cycle = previous->m_anim_layers.at( 6u ).m_cycle;
			anim_state->m_move_weight = previous->m_anim_layers.at( 6u ).m_weight;

			const auto& layer7 = previous->m_anim_layers.at( 7u );

			anim_state->m_strafe_weight = layer7.m_weight;
			anim_state->m_strafe_sequence = layer7.m_seq;
			anim_state->m_strafe_cycle = layer7.m_cycle;
			anim_state->m_acceleration_weight = previous->m_anim_layers.at( 12u ).m_weight;
		}
		else {
			if ( current->m_flags & valve::e_ent_flags::on_ground ) {
				anim_state->m_on_ground = true;
				anim_state->m_landing = false;
			}

			anim_state->m_time_since_in_air = 0.f;
			anim_state->m_primary_cycle = current->m_anim_layers.at( 6u ).m_cycle;
			anim_state->m_move_weight = current->m_anim_layers.at( 6u ).m_weight;
			anim_state->m_strafe_weight = current->m_anim_layers.at( 7u ).m_weight;
			anim_state->m_strafe_sequence = current->m_anim_layers.at( 7u ).m_seq;
			anim_state->m_strafe_cycle = current->m_anim_layers.at( 7u ).m_cycle;
			anim_state->m_acceleration_weight = current->m_anim_layers.at( 12u ).m_weight;

			entry.m_player->pose_params( ).at( 6 ) = 0.f;
			entry.m_player->anim_layers( ) = current->m_anim_layers;

			auto walk_run_transition = current->m_velocity.length( 2u ) <= 135.f ? 0.f : 1.f;

			anim_state->m_walk_to_run_transition = walk_run_transition;
			anim_state->m_last_update_time = current->m_sim_time - valve::g_global_vars.get( )->m_interval_per_tick;
		}

		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t( valve::cs_player_t* const player, lag_record_t* record )
				: m_cur_time{ valve::g_global_vars.get( )->m_cur_time },
				m_frame_time{ valve::g_global_vars.get( )->m_frame_time },
				m_real_time{ valve::g_global_vars.get( )->m_real_time }, m_abs_frame_time{ valve::g_global_vars.get( )->m_abs_frame_time },
				m_frame_count{ valve::g_global_vars.get( )->m_frame_count }, m_tick_count{ valve::g_global_vars.get( )->m_tick_count }, m_interp_amt{ valve::g_global_vars.get( )->m_interp_amt },
				m_duck_amount{ player->duck_amt( ) }, m_eye_angles{ player->eye_angles( ) },
				m_eflags{ player->eflags( ) }, m_flags{ player->flags( ) },
				m_velocity{ player->velocity( ) }, m_lby{ player->lby( ) }, m_abs_velocity{ player->abs_velocity( ) } {}

			__forceinline void restore( valve::cs_player_t* const player, lag_record_t* record ) const {
				valve::g_global_vars.get( )->m_cur_time = m_cur_time;
				valve::g_global_vars.get( )->m_frame_time = m_frame_time;
				valve::g_global_vars.get( )->m_real_time = m_real_time;
				valve::g_global_vars.get( )->m_abs_frame_time = m_abs_frame_time;
				valve::g_global_vars.get( )->m_frame_count = m_frame_count;
				valve::g_global_vars.get( )->m_tick_count = m_tick_count;
				valve::g_global_vars.get( )->m_interp_amt = m_interp_amt;

				player->duck_amt( ) = m_duck_amount;
				player->eye_angles( ) = m_eye_angles;

				player->eflags( ) = m_eflags;
				player->flags( ) = m_flags;
				player->lby( ) = m_lby;
				player->velocity( ) = m_velocity;
				player->abs_velocity( ) = m_abs_velocity;
			}

			float				m_cur_time{}, m_frame_time{}, m_real_time{ }, m_abs_frame_time{ },
				m_duck_amount{}, m_interp_amt{ };

			sdk::qang_t              m_eye_angles{};

			int                 m_frame_count{ }, m_tick_count{ };

			std::size_t			m_eflags{};
			valve::e_ent_flags	m_flags{};
			float               m_lby{};
			sdk::vec3_t				m_velocity{}, m_abs_velocity{};
		} backup{ entry.m_player, current };

		if ( current->m_choked_cmds >= 2 
			&& previous ) {
			anim_state->m_last_update_time = current->m_anim_time;

			for ( std::ptrdiff_t i{ 1 }; i <= current->m_choked_cmds; ++i ) {
				auto anim_time = current->m_anim_time + valve::to_time( i );

				const auto cur_time = i < current->m_choked_cmds ? anim_time : current->m_sim_time;

				valve::g_global_vars.get( )->m_cur_time = cur_time;
				valve::g_global_vars.get( )->m_real_time = cur_time;
				valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
				valve::g_global_vars.get( )->m_abs_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
				valve::g_global_vars.get( )->m_frame_count = valve::to_ticks( cur_time );
				valve::g_global_vars.get( )->m_tick_count = valve::to_ticks( cur_time );
				valve::g_global_vars.get( )->m_interp_amt = 0.f;

				const auto lerp_tick = i / static_cast < float > ( current->m_choked_cmds );

				if ( i < current->m_choked_cmds ) {
					auto prev_lby = previous->m_lby;
					auto cur_lby = current->m_lby;

					if ( prev_lby != cur_lby ) {
						auto v40 = current->m_choked_cmds - i;
						bool use_new_lby = true;

						if ( current->m_lby_diff < 1.f )
							use_new_lby = v40 == 0;
						else
							use_new_lby = v40 < 2;

						if ( !use_new_lby )
							cur_lby = prev_lby;

						entry.m_player->lby( ) = cur_lby;
					}

					current->m_interp_flags = current->m_flags;
					process_air_data( entry, current, previous, valve::to_ticks( cur_time ) );
					
					entry.m_player->flags( ) = current->m_interp_flags;

					entry.m_player->duck_amt( ) = previous->m_duck_amt + current->m_duck_amt_diff * lerp_tick;

					if ( entry.m_player->duck_amt( ) <= 0.75f )
						entry.m_player->flags( ) &= ~valve::e_ent_flags::ducking;
					else
						entry.m_player->flags( ) |= valve::e_ent_flags::ducking;

					entry.m_player->update_collision_bounds( );

					current->m_obb_min = entry.m_player->obb_min( );
					current->m_obb_max = entry.m_player->obb_max( );

					entry.m_player->velocity( ) = entry.m_player->abs_velocity( ) = previous->m_velocity + current->m_velocity_diff * lerp_tick;

				    entry.m_player->eye_angles( ) = current->m_eye_angles;

					entry.m_player->eye_angles( ).z( ) = 0.f;

				}
				else {
					entry.m_player->flags( ) = current->m_flags;
					entry.m_player->duck_amt( ) = current->m_duck_amt;
					entry.m_player->velocity( ) = entry.m_player->abs_velocity( ) = current->m_velocity;

					entry.m_player->eye_angles( ) = current->m_eye_angles;
					entry.m_player->eye_angles( ).z( ) = 0.f;
				}

				entry.m_player->eflags( ) &= ~0x1000u;

				if ( side ) {
					if ( side == 2 )
						anim_state->m_foot_yaw = sdk::normalize_yaw( current->m_eye_angles.y( ) + crypt_float ( 60.f ) );
					else
						anim_state->m_foot_yaw = sdk::normalize_yaw( current->m_eye_angles.y( ) - crypt_float( 60.f ) );
				}

				anim_state->m_last_update_frame = valve::g_global_vars.get( )->m_frame_count - 1;

				const auto is_hltv = valve::g_client_state->m_is_hltv;

				anim_state->m_player = entry.m_player;
				anim_state->m_last_weapon = current->m_weapon;

				entry.m_player->client_side_anim( ) = g_ctx->allow_update_anim( ) = true;

				valve::g_client_state->m_is_hltv = true;

				entry.m_player->update_client_side_anim( );

				entry.m_player->client_side_anim( ) = g_ctx->allow_update_anim( ) = false;

				valve::g_client_state->m_is_hltv = is_hltv;
			}
		}
		else {
			if ( anim_state->m_last_update_time == current->m_sim_time )
				anim_state->m_last_update_time = current->m_sim_time + valve::g_global_vars.get( )->m_interval_per_tick;

			valve::g_global_vars.get( )->m_cur_time = current->m_sim_time;
			valve::g_global_vars.get( )->m_real_time = current->m_sim_time;
			valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
			valve::g_global_vars.get( )->m_abs_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
			valve::g_global_vars.get( )->m_frame_count = valve::to_ticks( current->m_sim_time );
			valve::g_global_vars.get( )->m_tick_count = valve::to_ticks( current->m_sim_time );
			valve::g_global_vars.get( )->m_interp_amt = 0.f;

			entry.m_player->eye_angles( ) = current->m_eye_angles;
			entry.m_player->eye_angles( ).z( ) = 0.f;
			entry.m_player->flags( ) = current->m_flags;
			entry.m_player->duck_amt( ) = current->m_duck_amt;
			entry.m_player->velocity( ) = entry.m_player->abs_velocity( ) = current->m_velocity;

			entry.m_player->eflags( ) &= ~0x1000u;
			
			anim_state->m_last_update_frame = valve::g_global_vars.get( )->m_frame_count - 1;

			anim_state->m_player = entry.m_player;
			anim_state->m_last_weapon = current->m_weapon;

			const auto is_hltv = valve::g_client_state->m_is_hltv;

			entry.m_player->client_side_anim( ) = g_ctx->allow_update_anim( ) = true;

			valve::g_client_state->m_is_hltv = true;

			entry.m_player->update_client_side_anim( );

			entry.m_player->client_side_anim( ) = g_ctx->allow_update_anim( ) = false;

			valve::g_client_state->m_is_hltv = is_hltv;
		}

		backup.restore( entry.m_player, current );

		reinterpret_cast < void( __thiscall* )( void*, int ) > ( g_ctx->offsets( ).m_addresses.m_invalidate_physics_recursive )( entry.m_player, 0x8 );

		auto& cur_anim_side = current->m_anim_sides.at( side );

		if ( !custom_angles ) {
			{
				cur_anim_side.m_foot_yaw = anim_state->m_foot_yaw;
				cur_anim_side.m_prev_foot_yaw = anim_state->m_last_foot_yaw;
				cur_anim_side.m_move_yaw_ideal = anim_state->m_move_yaw_ideal;
				cur_anim_side.m_move_yaw_cur_to_ideal = anim_state->m_move_yaw_cur_to_ideal;
				cur_anim_side.m_move_yaw = anim_state->m_move_yaw;
				cur_anim_side.m_move_weight_smoothed = anim_state->m_move_weight_smoothed;

				cur_anim_side.m_anim_layers = entry.m_player->anim_layers( );
				cur_anim_side.m_pose_params = entry.m_player->pose_params( );
			}
		}

		entry.m_player->anim_layers( ) = current->m_anim_layers;
		entry.m_player->set_abs_angles( { 0.f, anim_state->m_foot_yaw, 0.f } );

		const auto backup_eye_angles = entry.m_player->eye_angles( );

		if ( custom_angles )
			entry.m_player->eye_angles( ) = current->m_eye_angles;

		setup_bones( entry.m_player, cur_anim_side.m_bones, current->m_sim_time, 0x0007ff00 );

		if ( custom_angles ) {
			current->m_eye_angles = backup_recorded_eye_angles;
			entry.m_player->eye_angles( ) = backup_eye_angles;

		}
		const auto mdl_data = entry.m_player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return;

		cur_anim_side.m_bones_count = mdl_data->m_studio_hdr->m_bones_count;
	}

	void c_anim_sync::select_anim_side( player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		current->m_anim_side = 0;
		current->m_anim_resolver = false;

		if ( current->m_velocity.length( 2u ) <= 0.1f )
			return;

		if ( !( current->m_flags & valve::e_ent_flags::on_ground )
			|| !( previous->m_flags & valve::e_ent_flags::on_ground ) )
			return;

		const auto not_accelerating = static_cast < int > ( current->m_anim_layers.at( 6u ).m_weight * 1000.f ) == static_cast < int > ( previous->m_anim_layers.at( 6u ).m_weight * 1000.f )
			&& !static_cast < int > ( current->m_anim_layers.at( 12u ).m_weight * 1000.f );

		if ( not_accelerating ) {
			const auto& move_layer = current->m_anim_layers.at( 6u );

			if ( ( current->m_velocity.length( 2u ) <= 30.f
				&& current->m_velocity.length( 2u ) > 15.f )
				|| ( current->m_velocity.length( 2u ) < 120.f
					&& previous->m_velocity.length( 2u ) >= 120.f
					&& previous->m_anim_layers.at( 12u ).m_weight < current->m_anim_layers.at( 12u ).m_weight ) )
				current->m_force_safe_points = true;

			const auto delta_0 = std::abs( move_layer.m_playback_rate - current->m_anim_sides.at( 0u ).m_anim_layers.at( 6u ).m_playback_rate );
			const auto delta_1 = std::abs( move_layer.m_playback_rate - current->m_anim_sides.at( 1u ).m_anim_layers.at( 6u ).m_playback_rate );
			const auto delta_2 = std::abs( move_layer.m_playback_rate - current->m_anim_sides.at( 2u ).m_anim_layers.at( 6u ).m_playback_rate );

			/* solve method when something not legit is happening */
			if ( !static_cast < int > ( delta_0 * 1000.f )
				&& delta_1 > 0.2f
				&& delta_2 == delta_1 ) {
				current->m_force_safe_points = true;
				current->m_anim_side = 0;
				current->m_anim_resolver = true;
				current->m_r_mode = e_r_mode::r_anim_logic;
				return;
			}

			/* solve method when delta_1 is an actual fake */
			if ( ( delta_0 > delta_1
				|| delta_2 > delta_1 )
				&& static_cast < int > ( delta_2 * 1000.f ) ) {
				if ( delta_2 <= delta_0
					|| !static_cast < int > ( delta_1 * 1000.f ) ) {
					if ( static_cast < int > ( delta_1 * 1000.f ) )
						current->m_force_safe_points = true;

					current->m_anim_side = 1;
					current->m_anim_resolver = true;
					current->m_r_mode = e_r_mode::r_anim_logic;
				}
			}
			else {
				/* solve method when delta_2 is an actual fake */
				if ( delta_2 < delta_1
					|| !static_cast < int > ( delta_2 * 1000.f ) ) {
					if ( static_cast < int > ( delta_2 * 1000.f ) )
						current->m_force_safe_points = true;

					current->m_anim_side = 2;
					current->m_anim_resolver = true;
					current->m_r_mode = e_r_mode::r_anim_logic;
				}
			}
		}
	}

	void c_anim_sync::find_server_foot_yaw(
		player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		const auto& right_side = current->m_anim_sides.at( 2 );

		auto delta = sdk::angle_diff( right_side.m_foot_yaw, current->m_eye_angles.y( ) );
		bool negative_delta = delta < 0.0f;

		if ( negative_delta ) {
			std::swap( current->m_anim_sides.at( 2 ).m_anim_layers, current->m_anim_sides.at( 1 ).m_anim_layers );
			std::swap( current->m_anim_sides.at( 2 ).m_pose_params, current->m_anim_sides.at( 1 ).m_pose_params );
			std::swap( current->m_anim_sides.at( 2 ).m_foot_yaw, current->m_anim_sides.at( 1 ).m_foot_yaw );
			std::swap( current->m_anim_sides.at( 2 ).m_bones, current->m_anim_sides.at( 1 ).m_bones );
		}

		auto speed_2d = current->m_velocity.length( 2u );

		if ( entry.m_misses >= 1
			|| !previous ) {
			current->m_force_safe_points = true;

			current->m_anim_side = entry.m_last_anim_side + 1;

			if ( current->m_anim_side > 2 )
				current->m_anim_side = 0;

			entry.m_last_anim_side = current->m_anim_side;

			return;
		}

		if ( current->m_sim_ticks <= 0
			|| current->m_choked_cmds <= 1
			|| !( current->m_flags & valve::e_ent_flags::on_ground )
			|| !( previous->m_flags & valve::e_ent_flags::on_ground ) )
		{
			current->m_force_safe_points = true;
			return;
		}

		if ( current->m_eye_angles.y( ) != previous->m_eye_angles.y( ) && current->m_lby != previous->m_lby ) {
			current->m_force_safe_points = true;
		}

		if ( std::abs( sdk::angle_diff( previous->m_eye_angles.y( ), current->m_eye_angles.y( ) ) ) > 35.f )
			current->m_force_safe_points = true;

		select_anim_side( entry, current, previous );

		if ( !current->m_anim_resolver ) {
			if ( speed_2d <= 0.1f ) {
				current->m_force_safe_points = true;
				current->m_anim_side = sdk::angle_diff( current->m_eye_angles.y( ), entry.m_player->lby( ) ) <= 0.f ? 1 : 2;
			}
		}

		entry.m_last_anim_side = current->m_anim_side;
	}

	bool c_anim_sync::setup_bones(
		valve::cs_player_t* const player, valve::bones_t& bones, const float time, int mask
	) {
		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t( valve::cs_player_t* const player )
				: m_cur_time{ valve::g_global_vars.get( )->m_cur_time },
				m_frame_time{ valve::g_global_vars.get( )->m_frame_time },
				m_frame_count{ valve::g_global_vars.get( )->m_frame_count },
				m_real_time{ valve::g_global_vars.get( )->m_real_time },
				m_tick_count{ valve::g_global_vars.get( )->m_tick_count },
				m_abs_frame_time{ valve::g_global_vars.get( )->m_abs_frame_time },
				m_interp_amt{ valve::g_global_vars.get( )->m_interp_amt },
				m_occlusion_frame{ player->occlusion_frame( ) },
				m_ent_client_flags{ player->ent_client_flags( ) },
				m_ik_context{ player->ik_context( ) }, m_effects{ player->effects( ) },
				m_occlusion_flags{ player->occlusion_flags( ) }, m_abs_origin{ player->abs_origin( ) } {}

			float					m_cur_time{}, m_frame_time{}, m_real_time{}, m_abs_frame_time{}, m_interp_amt{ };
			int						m_frame_count{}, m_occlusion_frame{}, m_tick_count{};
			std::uint8_t			m_ent_client_flags{};
			valve::ik_context_t* m_ik_context{};

			std::size_t				m_effects{}, m_occlusion_flags{};
			sdk::vec3_t m_abs_origin{};
		} backup{ player };

		valve::g_global_vars.get( )->m_cur_time = time;
		valve::g_global_vars.get( )->m_real_time = time;
		valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
		valve::g_global_vars.get( )->m_abs_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
		valve::g_global_vars.get( )->m_frame_count = std::numeric_limits< int >::max( );
		valve::g_global_vars.get( )->m_tick_count = valve::to_ticks( time );
		valve::g_global_vars.get( )->m_interp_amt = 0.f;

		player->mdl_bone_cnt( ) = 0ul;
		player->last_setup_bones_time( ) = std::numeric_limits< float >::lowest( );

		for ( auto& layer : player->anim_layers( ) )
			layer.m_owner = player;

		player->set_abs_origin( player->origin( ) );

		player->effects( ) |= 8u;
		player->occlusion_flags( ) &= ~0xau;
		player->occlusion_frame( ) = 0;

		player->ik_context( ) = nullptr;
		player->ent_client_flags( ) |= 2u;

		player->last_setup_bones_frame( ) = 0;

		static auto jiggle_bones = valve::g_cvar->find_var( MilfStr( "r_jiggle_bones" ) );

		const auto backup_jiggle_bones = jiggle_bones->get_int( );

		jiggle_bones->set_int( 0 );

		g_ctx->allow_setup_bones( ) = true;
		const auto ret = player->renderable( )->setup_bones( bones.data( ), valve::k_max_bones, mask, time );
		g_ctx->allow_setup_bones( ) = false;

		player->ik_context( ) = backup.m_ik_context;

		jiggle_bones->set_int( backup_jiggle_bones );

		player->effects( ) = backup.m_effects;
		player->occlusion_flags( ) = backup.m_occlusion_flags;
		player->occlusion_frame( ) = backup.m_occlusion_frame;

		player->ent_client_flags( ) = backup.m_ent_client_flags;

		player->set_abs_origin( backup.m_abs_origin );

		valve::g_global_vars.get( )->m_cur_time = backup.m_cur_time;
		valve::g_global_vars.get( )->m_real_time = backup.m_real_time;
		valve::g_global_vars.get( )->m_abs_frame_time = backup.m_abs_frame_time;
		valve::g_global_vars.get( )->m_frame_time = backup.m_frame_time;
		valve::g_global_vars.get( )->m_tick_count = backup.m_tick_count;
		valve::g_global_vars.get( )->m_frame_count = backup.m_frame_count;
		valve::g_global_vars.get( )->m_interp_amt = backup.m_interp_amt;

		return ret;
	}

	void c_anim_sync::setup_anim_data(
		player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, const bool custom_angles
	) {
		entry.m_player->set_abs_origin( current->m_origin );

		if ( !custom_angles ) {
			entry.m_player->update_collision_bounds( );

			current->m_obb_min = entry.m_player->obb_min( );
			current->m_obb_max = entry.m_player->obb_max( );
		}

		if ( !custom_angles )
		if ( previous ) {
			if ( std::abs( current->m_eye_angles.y( ) - previous->m_eye_angles.y( ) ) > crypt_float ( 10.f ) )
				current->m_jittering = true;
		}

		if ( current->m_choked_cmds >= 2 
			&& previous ) {
			if ( !custom_angles )
			if ( current->m_shot ) {
				current->m_lby = previous->m_lby;
			}

			if ( !custom_angles ) {
				current->m_duck_amt_diff = current->m_duck_amt - previous->m_duck_amt;
				current->m_lby_diff = std::abs( sdk::angle_diff( current->m_eye_angles.y( ), current->m_lby ) );
				current->m_eye_angles_diff = ( current->m_eye_angles - previous->m_eye_angles ).normalized( );
				current->m_velocity_diff = current->m_velocity - previous->m_velocity;
				current->m_time_diff = current->m_sim_time - previous->m_sim_time;
			}

			/* air data */
			if ( current->m_flags & valve::e_ent_flags::on_ground 
				&& !custom_angles ) { // store data for land sync
				if ( current->m_anim_layers.at( 5 ).m_weight >  0.f
					&& previous->m_anim_layers.at( 5 ).m_weight <= 0.f &&	
					!( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
					const auto land_seq = current->m_anim_layers.at( 5 ).m_seq;

					if ( land_seq >= 2 ) {
						const auto& land_act = entry.m_player->lookup_seq_act( land_seq );

						if ( land_act == crypt_int ( 988 ) ||
							land_act == crypt_int ( 989 ) ) {
							const auto cur_cycle = current->m_anim_layers.at( 5 ).m_cycle;
							const auto cur_rate = current->m_anim_layers.at( 5 ).m_playback_rate;

							if ( cur_cycle != 0.f &&
								cur_rate != 0.f ) {

								const auto land_time = cur_cycle / cur_rate;

								if ( land_time != 0.f ) {
									current->m_on_ground = true;
									current->m_act_time = current->m_sim_time - land_time;
								}
							}
						}
					}

					current->m_velocity.z( ) = 0.f;
				}
			}
			else if ( !custom_angles ) {
				const auto jump_seq = current->m_anim_layers.at( 4u ).m_seq;

				if ( !( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
					const auto jump_seq = current->m_anim_layers.at( 4 ).m_seq;

					if ( jump_seq >= 2 ) {
						const auto jump_act = entry.m_player->lookup_seq_act( jump_seq );

						if ( jump_act == 985 ) {
							const auto cur_cycle = current->m_anim_layers.at( 4 ).m_cycle;
							const auto cur_rate = current->m_anim_layers.at( 4 ).m_playback_rate;

							if ( cur_cycle != 0.f &&
								cur_rate != 0.f ) {

								const auto jump_time = cur_cycle / cur_rate;

								if ( jump_time != 0.f ) {
									current->m_on_ground = false;
									current->m_act_time = current->m_sim_time - jump_time;
								}
							}
						}
					}
				}

				if ( entry.m_player->lookup_seq_act( jump_seq ) == crypt_int ( 985 ) ) {
					if ( current->m_anim_layers.at( 4u ).m_weight > 0.f
						&& current->m_anim_layers.at( 4u ).m_playback_rate > 0.f ) {
						const auto jump_time = ( ( ( current->m_anim_layers.at( 4u ).m_cycle / current->m_anim_layers.at( 4u ).m_playback_rate )
							/ valve::g_global_vars.get( )->m_interval_per_tick ) + 0.5f ) * valve::g_global_vars.get( )->m_interval_per_tick;

						static auto sv_gravity = valve::g_cvar->find_var( MilfStr( "sv_gravity" ) );
						static auto sv_jump_impulse = valve::g_cvar->find_var( MilfStr( "sv_jump_impulse" ) );

						current->m_velocity.z( ) = sv_jump_impulse->get_float( ) - sv_gravity->get_float( ) * jump_time;
					}
				}
			}

			if ( !( current->m_flags & valve::e_ent_flags::on_ground ) ) {
				if ( current->m_anim_layers.at( 4 ).m_weight != 0.f
					&& current->m_anim_layers.at( 4 ).m_playback_rate != 0.f ) {
					const auto cur_seq = current->m_anim_layers.at( 4 ).m_seq;

					if ( entry.m_player->lookup_seq_act( cur_seq ) == crypt_int ( 985 ) ) {
						const auto cur_cycle = current->m_anim_layers.at( 4 ).m_cycle;
						const auto previous_cycle = previous->m_anim_layers.at( 4 ).m_cycle;

						const auto previous_seq = previous->m_anim_layers.at( 4 ).m_seq;

						if ( ( cur_cycle != previous_cycle || previous_seq != cur_seq )
							&& previous_cycle > cur_cycle ) {
							entry.m_player->pose_params( ).at( 6 ) = 0.f;
							entry.m_player->anim_state( )->m_time_since_in_air = cur_cycle / current->m_anim_layers.at( 4 ).m_playback_rate;
						}
					}
				}
			}
		}
	}

	void c_anim_sync::process_air_data( player_entry_t& entry, lag_record_t* const current,
		const lag_anim_data_t* const previous, const int anim_tick ) const {

		if ( current->m_on_ground.has_value( ) ) {
			if ( !current->m_on_ground.value( ) ) {
				const auto jump_tick = valve::to_ticks( current->m_act_time ) + 1;

				if ( jump_tick == anim_tick - 1 )
					current->m_interp_flags |= valve::e_ent_flags::on_ground; // not jumped yet
				else if ( jump_tick == anim_tick ) {
					entry.m_player->anim_layers( ).at( 4 ).m_cycle = 0.f;
					entry.m_player->anim_layers( ).at( 4 ).m_weight = 0.f;
					entry.m_player->anim_layers( ).at( 4 ).m_playback_rate = current->m_anim_layers.at( 4 ).m_playback_rate;
					entry.m_player->anim_layers( ).at( 4 ).m_seq = current->m_anim_layers.at( 4 ).m_seq;
					current->m_interp_flags &= ~valve::e_ent_flags::on_ground; // alright, player jumped - remove on_ground flag
				}
			}
			else {
				const auto land_tick = valve::to_ticks( current->m_act_time ) + 1;

				if ( land_tick == anim_tick - 1 )
					current->m_interp_flags &= ~valve::e_ent_flags::on_ground; // not landed yet
				else if ( land_tick == anim_tick ) {
					entry.m_player->anim_layers( ).at( 5 ).m_cycle = 0.f;
					entry.m_player->anim_layers( ).at( 5 ).m_weight = 0.f;
					entry.m_player->anim_layers( ).at( 5 ).m_playback_rate = current->m_anim_layers.at( 5 ).m_playback_rate;
					entry.m_player->anim_layers( ).at( 5 ).m_seq = current->m_anim_layers.at( 5 ).m_seq;

					current->m_interp_flags |= valve::e_ent_flags::on_ground; // alright, player landed - add on_ground flag
				}
			}
		}
	}
}