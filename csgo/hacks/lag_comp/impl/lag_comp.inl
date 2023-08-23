#pragma once
#include "../csgo/security/Dependencies/Xor.hpp"
#include "../lag_comp.h"

namespace hacks {
	ALWAYS_INLINE void lag_record_t::setup( valve::cs_player_t* const player, const std::ptrdiff_t anim_idx ) const {
		player->origin( ) = m_origin;

		player->set_abs_origin( m_origin );

		const auto backup_cur_time = valve::g_global_vars.get( )->m_cur_time;

		valve::g_global_vars.get( )->m_cur_time = m_sim_time;

		player->set_collision_bounds( m_obb_min, m_obb_max );

		valve::g_global_vars.get( )->m_cur_time = backup_cur_time;

		const auto& anim_side = m_anim_sides.at( anim_idx );

		player->set_abs_angles( { 0.f, anim_side.m_foot_yaw, 0.f } );

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			anim_side.m_bones.data( ), anim_side.m_bones_count * sizeof( sdk::mat3x4_t )
		);

		player->mdl_bone_cnt( ) = **reinterpret_cast< unsigned long** >(
			g_ctx->offsets( ).m_addresses.m_invalidate_bone_cache + 0xau
			);
	}

	ALWAYS_INLINE void lag_record_t::simulate( lag_anim_data_t* previous, player_entry_t& entry ) {
		if ( previous ) {
			if ( ( m_origin - previous->m_origin ).length_sqr( 2u ) > 4096.f )
				m_broke_lc = true;
		}

		if ( !previous || previous->m_dormant ) {
			if ( !previous ) {
				if ( m_flags & valve::e_ent_flags::on_ground ) {
					auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( entry.m_player->scoped( ) ) ) : 260.f;

					if ( m_anim_layers.at( 6u ).m_weight > 0.f && m_anim_layers.at( 6u ).m_playback_rate > 0.f
						&& m_velocity.length( 2u ) > 0.f ) {
						if ( ( m_flags & valve::e_ent_flags::ducking ) )
							max_speed *= 0.34f;
						else if ( m_walking )
							max_speed *= 0.52f;

						const auto move_multiplier = m_anim_layers.at( 6u ).m_weight * max_speed;
						const auto speed_multiplier = move_multiplier / m_velocity.length( 2u );

						m_velocity_sync_stage = 1;

						m_velocity.x( ) *= speed_multiplier;
						m_velocity.y( ) *= speed_multiplier;
					}
				}
			}

			return;
		}

		const auto& cur_alive_loop_layer = m_anim_layers.at( 11u );
		const auto& prev_alive_loop_layer = previous->m_anim_layers.at( 11u );

		if ( m_weapon == previous->m_weapon 
			&& m_anim_layers.at( 11u ).m_playback_rate > 0.f && previous->m_anim_layers.at( 11u ).m_playback_rate > 0.f ) {

			const auto cur_11th_cycle = cur_alive_loop_layer.m_cycle;
			auto prev_11th_cycle = prev_alive_loop_layer.m_cycle;

			const auto cycles_delta = cur_11th_cycle - prev_11th_cycle;

			if ( cycles_delta != 0.f ) {
				const auto sim_ticks_delta = valve::to_ticks( m_sim_time - m_old_sim_time );

				if ( sim_ticks_delta != 1 ) {
					auto prev_11th_rate = prev_alive_loop_layer.m_playback_rate;
					std::ptrdiff_t resimulated_sim_ticks{};

					if ( cycles_delta >= 0.f ) {
						resimulated_sim_ticks = valve::to_ticks( cycles_delta / prev_11th_rate );
					}
					else {
						std::ptrdiff_t ticks_iterated{};
						float cur_simulated_cycle{ 0.f };
						while ( true ) {
							++resimulated_sim_ticks;

							if ( ( prev_11th_rate * valve::g_global_vars.get( )->m_interval_per_tick ) + prev_11th_cycle >= 1.f )
								prev_11th_rate = m_anim_layers.at( 11u ).m_playback_rate;

							cur_simulated_cycle = ( prev_11th_rate * valve::g_global_vars.get( )->m_interval_per_tick ) + prev_11th_cycle;
							prev_11th_cycle = cur_simulated_cycle;
							if ( cur_simulated_cycle > 0.f )
								break;

							if ( ++ticks_iterated >= 16 ) {
								goto leave_cycle;
							}
						}

						const auto first_val = prev_11th_cycle - cur_simulated_cycle;

						auto recalc_everything = cur_11th_cycle - first_val;
						recalc_everything /= cur_alive_loop_layer.m_playback_rate;
						recalc_everything /= valve::g_global_vars.get( )->m_interval_per_tick;
						recalc_everything += 0.5f;

						resimulated_sim_ticks += recalc_everything;
					}

				leave_cycle:
					if ( resimulated_sim_ticks < sim_ticks_delta ) {
						if ( resimulated_sim_ticks
							&& valve::g_client_state->m_server_tick - valve::to_ticks( m_sim_time ) == resimulated_sim_ticks ) {
							entry.m_player->sim_time( ) = entry.m_cur_sim = m_sim_time = ( resimulated_sim_ticks * valve::g_global_vars.get( )->m_interval_per_tick ) + m_old_sim_time;
						}
					}
				}
			}
		}

		auto sim_ticks = valve::to_ticks( m_sim_time - previous->m_sim_time );

		if ( sim_ticks - 1 > 31 || previous->m_sim_time == 0.f ) {
			sim_ticks = 1;
		}

		auto cur_cycle = cur_alive_loop_layer.m_cycle;
		auto prev_rate = prev_alive_loop_layer.m_playback_rate;

		if ( prev_rate > 0.f && cur_alive_loop_layer.m_playback_rate > 0.f
			&& m_weapon == previous->m_weapon ) {
			auto prev_cycle = prev_alive_loop_layer.m_cycle;
			sim_ticks = 0;

			if ( prev_cycle > cur_cycle )
				cur_cycle += 1.f;

			while ( cur_cycle > prev_cycle ) {
				const auto last_cmds = sim_ticks;

				const auto next_rate = valve::g_global_vars.get( )->m_interval_per_tick * prev_rate;
				prev_cycle += valve::g_global_vars.get( )->m_interval_per_tick * prev_rate;

				if ( prev_cycle >= 1.f )
					prev_rate = cur_alive_loop_layer.m_playback_rate;

				++sim_ticks;

				if ( prev_cycle > cur_cycle && ( prev_cycle - cur_cycle ) > ( next_rate * 0.5f ) )
					sim_ticks = last_cmds;
			}
		}

		m_choked_cmds = std::clamp( sim_ticks, 0, 17 );

		m_anim_time = m_sim_time - valve::to_time( m_choked_cmds );

		if ( m_choked_cmds >= 2 ) {
			auto origin_diff = m_origin - previous->m_origin;

			const auto chocked_cmds_time = valve::to_time( m_choked_cmds );
			if ( chocked_cmds_time > 0.f
				&& chocked_cmds_time < 1.f )
				m_velocity = origin_diff * ( 1.f / chocked_cmds_time );
		}

		if ( m_weapon
			&& m_last_shot_time > ( m_sim_time - valve::to_time( m_choked_cmds ) )
			&& m_sim_time >= m_last_shot_time )
			m_shot = true;

		if ( m_choked_cmds >= 2 ) {
			const auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( entry.m_player->scoped( ) ) ) : 260.f;

			if ( m_flags & valve::e_ent_flags::on_ground
				&& previous->m_flags & valve::e_ent_flags::on_ground ) {
				if ( m_weapon == previous->m_weapon ) {
					if ( cur_alive_loop_layer.m_weight > 0.f && cur_alive_loop_layer.m_weight < 1.f ) {
						const auto speed_2d = m_velocity.length( 2u );

						if ( speed_2d >= 0.1f ) {
							const auto reversed_val = ( 1.f - cur_alive_loop_layer.m_weight ) * 0.35f;

							if ( reversed_val > 0.f && reversed_val < 1.f ) {
								const auto speed_as_portion_of_run_top_speed = ( ( reversed_val + 0.55f ) * max_speed ) / speed_2d;
								if ( speed_as_portion_of_run_top_speed ) {
									m_velocity_sync_stage += 3;

									m_velocity.x( ) *= speed_as_portion_of_run_top_speed;
									m_velocity.y( ) *= speed_as_portion_of_run_top_speed;
								}
							}
						}
					}
				}
			}

			const auto weight_speed = ( ( ( ( max_speed * 0.34f ) - ( max_speed * 0.52f ) ) * ( m_duck_amt - 0.f ) ) + ( max_speed * 0.52f ) ) * m_anim_layers.at( 6u ).m_weight;
			const auto speed_2d = m_velocity.length( 2u );

			if ( m_anim_layers.at( 6u ).m_weight < 0.94999999 
				|| weight_speed > speed_2d ) {
				auto v247 = 1.f / ( speed_2d + 0.00000011920929 );
				m_velocity.x( ) = ( m_velocity.x( ) * v247 ) * weight_speed;
				m_velocity.y( ) = ( m_velocity.y( ) * v247 ) * weight_speed;
				return;
			}
		}
	}

	ALWAYS_INLINE lag_anim_data_t::lag_anim_data_t( const lag_record_t* const lag_record ) {
		m_weapon = lag_record->m_weapon;

		m_eye_angles = lag_record->m_eye_angles;
		m_origin = lag_record->m_origin;
		m_velocity = lag_record->m_velocity;

		m_flags = lag_record->m_flags;
		m_lby = lag_record->m_lby;
		m_third_person_recoil = lag_record->m_third_person_recoil;
		m_duck_amt = lag_record->m_duck_amt;

		m_anim_layers = lag_record->m_anim_layers;

		m_sim_time = lag_record->m_sim_time;

		m_dormant = lag_record->m_dormant;

		for ( std::size_t i{}; i < m_anim_sides.size( ); ++i ) {
			auto& to = m_anim_sides.at( i );
			const auto& from = lag_record->m_anim_sides.at( i );

			to.m_foot_yaw = from.m_foot_yaw;
			to.m_prev_foot_yaw = from.m_prev_foot_yaw;
			to.m_move_yaw_ideal = from.m_move_yaw_ideal;
			to.m_move_yaw_cur_to_ideal = from.m_move_yaw_cur_to_ideal;
			to.m_move_yaw = from.m_move_yaw;
			to.m_move_weight_smoothed = from.m_move_weight_smoothed;
		}
	}

	ALWAYS_INLINE bool lag_record_t::valid( ) const {
		if ( g_lag_comp->calc_time_delta( m_sim_time ) >= 0.2f )
			return false;

		return true;
	}

	ALWAYS_INLINE player_entry_t& c_lag_comp::entry( const std::size_t i ) {
		return m_entries.at( i );
	}

	ALWAYS_INLINE float c_lag_comp::calc_time_delta( const float sim_time ) {
		const auto& net_info = g_local_player->net_info( );
		static const auto sv_maxunlag = valve::g_cvar->find_var( MilfStr( "sv_maxunlag" ) );

		const auto correct = std::clamp(
			net_info.m_lerp + net_info.m_latency.m_in + net_info.m_latency.m_out,
			0.f, sv_maxunlag->get_float( )
		);

		auto tick_base = g_local_player->self( )->tick_base( );
		if ( g_exploits->next_shift_amount( ) > 0 )
			tick_base -= g_exploits->next_shift_amount( );


		return std::abs( correct - ( valve::to_time( tick_base ) - sim_time ) );
	}

	ALWAYS_INLINE void player_entry_t::reset( ) {
		m_player = nullptr;

		m_alive_loop_cycle = -1.f;
		m_alive_loop_rate = -1.f;
		m_render_origin = {};
		m_misses = 0;
		m_jitter_misses = 0;
		m_cur_sim = 0.f;
		m_old_sim = 0.f;
		m_last_sim = 0.f;
		m_broke_sim = 0.f;
		m_prev_anim_data = std::nullopt;
		m_records_count = 0;
		m_receive_time = 0.f;

		m_lag_records.clear( );
	}

	ALWAYS_INLINE lag_backup_t::lag_backup_t( valve::cs_player_t* const player ) {
		if ( const auto anim_state = player->anim_state( ) )
			m_foot_yaw = anim_state->m_foot_yaw;

		m_origin = player->origin( );
		m_abs_origin = player->abs_origin( );

		m_obb_min = player->obb_min( );
		m_obb_max = player->obb_max( );

		const auto& bone_accessor = player->bone_accessor( );

		m_readable_bones = bone_accessor.m_readable_bones;
		m_writable_bones = bone_accessor.m_writable_bones;

		const auto& bone_cache = player->bone_cache( );

		std::memcpy(
			m_bones.data( ),
			bone_cache.m_mem.m_ptr,
			bone_cache.m_size * sizeof( sdk::mat3x4_t )
		);

		m_bones_count = bone_cache.m_size;

		m_mdl_bone_counter = player->mdl_bone_cnt( );
	}

	ALWAYS_INLINE void lag_backup_t::restore( valve::cs_player_t* const player ) const {
		player->origin( ) = m_origin;

		player->set_abs_origin( m_abs_origin );

		player->set_collision_bounds( m_obb_min, m_obb_max );

		player->set_abs_angles( { 0.f, m_foot_yaw, 0.f } );

		auto& bone_accessor = player->bone_accessor( );

		bone_accessor.m_readable_bones = m_readable_bones;
		bone_accessor.m_writable_bones = m_writable_bones;

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			m_bones.data( ), m_bones_count * sizeof( sdk::mat3x4_t )
		);

		player->mdl_bone_cnt( ) = m_mdl_bone_counter;
	}
 }