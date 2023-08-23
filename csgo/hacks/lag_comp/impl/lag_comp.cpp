#include "../../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"
namespace hacks {
	void c_lag_comp::handle( ) {
		if ( !g_local_player->self( )
			|| !valve::g_engine.get( )->in_game( ) )
			return;

		const auto tick_rate = valve::to_ticks( 1.f );

		for ( std::ptrdiff_t i{ 1 }; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
			auto& entry = m_entries.at( i - 1 );

			const auto player = static_cast < valve::cs_player_t* >(
				valve::g_entity_list.get( )->get_entity( i )
				);

			if ( !player ) {
				entry.reset( );

				continue;
			}

			if ( player == g_local_player->self( ) ) {
				entry.reset( );

				continue;
			}

			if ( player->team( ) == g_local_player->self( )->team( ) ) {
				entry.reset( );
				
				continue;
			}

			if ( entry.m_player != player )
				entry.reset( );

			entry.m_player = player;

			if ( !player->alive( ) 
				|| !player ) {
				entry.reset( );

				if ( player )
					entry.m_player = player;

				continue;
			}

			const auto anim_state = player->anim_state( );
			if ( !anim_state ) {
				entry.reset( );

				continue;
			}

			if ( player->networkable( )->dormant( ) ) {
				entry.m_prev_anim_data = std::nullopt;
				if ( entry.m_lag_records.empty( ) ) {
					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player )
					);

					continue;
				}

				if ( !entry.m_lag_records.back( )->m_dormant ) {
					entry.m_lag_records.clear( );

					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player )
					);

					continue;
				}

				continue;
			}

			bool invalid_processing{};

			static auto cl_lagcompensation = valve::g_cvar->find_var( MilfStr( "cl_lagcompensation" ) );

			if ( player->sim_time( ) == 0.f )
				continue;

			if ( player->sim_time( ) == player->old_sim_time( ) ) {
				continue;
			}

			/* fake update -> restore previous simulation data */
			if ( player->anim_layers( ).at( 11u ).m_cycle == entry.m_alive_loop_cycle
				&& player->anim_layers( ).at( 11u ).m_playback_rate == entry.m_alive_loop_rate ) {
				player->sim_time( ) = entry.m_cur_sim;
				player->old_sim_time( ) = entry.m_old_sim;
				continue;
			}

			if ( player->sim_time( ) < entry.m_last_sim
				&& ( entry.m_last_sim != 0.f ) ) {
				entry.m_broke_sim = player->sim_time( );
			}

			if ( entry.m_broke_sim >= player->sim_time( ) 
				&& cl_lagcompensation->get_int ( ) != 0 )
				invalid_processing = true;

			++entry.m_records_count;

			entry.m_old_sim = entry.m_cur_sim;
			entry.m_cur_sim = player->sim_time( );

			if ( !invalid_processing )
				entry.m_last_sim = player->sim_time( );

			entry.m_alive_loop_cycle = player->anim_layers( ).at( 11 ).m_cycle;
			entry.m_alive_loop_rate = player->anim_layers( ).at( 11 ).m_playback_rate;

			while ( entry.m_lag_records.size( ) > tick_rate )
				entry.m_lag_records.pop_front( );

			entry.m_receive_time = valve::g_global_vars.get( )->m_real_time;

			if ( entry.m_spawn_time != player->spawn_time( ) ) {
				anim_state->reset( );

				entry.m_prev_anim_data = std::nullopt;
				entry.m_last_sim = 0.f;
				entry.m_cur_sim = 0.f;
				entry.m_alive_loop_cycle = 0.f;
				entry.m_alive_loop_rate = 0.f;
				entry.m_old_sim = 0.f;
				entry.m_misses = 0;

				entry.m_lag_records.clear( );
			}

			entry.m_spawn_time = player->spawn_time( );

			lag_anim_data_t* previous{};
			if ( entry.m_prev_anim_data.has_value( ) )
				previous = &entry.m_prev_anim_data.value( );

			const auto current = entry.m_lag_records.emplace_back(
				std::make_shared< lag_record_t >( player )
			).get( );

			entry.m_render_origin = current->m_origin;

			current->m_invalid_sim = invalid_processing;

			current->simulate( previous, entry );

			g_anim_sync->handle( entry, current, previous, current->m_eye_angles, false );
			entry.m_prev_anim_data.emplace( current );
		}
	}
}