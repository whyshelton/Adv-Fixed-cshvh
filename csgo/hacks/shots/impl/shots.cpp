#include "../csgo/csgo.hpp"
#include "../../../valve/other/hit_sounds.h"
#include <playsoundapi.h>
#pragma comment(lib, "Winmm.lib")
__forceinline constexpr std::uint32_t hash_1( const char* str )
{
	auto hash = 0x811c9dc5u;

	char v4{};
	do {
		v4 = *str++;
		hash = 0x1000193u * ( hash ^ v4 );
	} while ( v4 );

	return hash;
}

namespace hacks {
	void c_shots::on_new_event( valve::game_event_t* const event ) {
		if ( !g_local_player
			|| !valve::g_engine.get( )->in_game( ) )
			return;

		switch ( hash_1( event->name( ) ) ) {
		case 0xf8dba51u/* player_footstep */: {


		} break;

		case 0xbded60d0u/* player_hurt */: {
			if ( valve::g_engine.get( )->index_for_uid( event->get_int( ( "attacker" ) ) ) != g_local_player->self( )->networkable( )->index( ) )
				return;

			g_shot_construct->on_hurt( event );

			const auto victim = valve::g_entity_list.get( )->get_entity( valve::g_engine.get( )->index_for_uid( event->get_int( ( "userid" ) ) ) );
			if ( !victim )
				return;

			const auto hitgroup = event->get_int( ( "hitgroup" ) );
			if ( hitgroup == 10 )
				return;

			if ( g_menu->main( ).m_misc_cfg.get( ).m_hit_sound ) {
				PlaySoundA( reinterpret_cast< char* > ( skeet_sound ), NULL, SND_ASYNC | SND_MEMORY );
			}

			const auto shot = last_unprocessed( );
			if ( !shot
				|| ( shot->m_target.m_entry && shot->m_target_index != victim->networkable( )->index( ) ) )
				return;

			shot->m_server_info.m_hitgroup = hitgroup;
			shot->m_server_info.m_dmg = event->get_int( ( "dmg_health" ) );
			shot->m_server_info.m_hurt_tick = valve::g_client_state->m_server_tick;

		} break;
		case 0xe64effdau/* weapon_fire */: {
			g_shot_construct->on_fire( event );

			if ( !event || !g_local_player->self( ) )
				return;

			auto attacker = valve::g_engine.get( )->index_for_uid( event->get_int( ( "userid" ) ) );
			if ( attacker != valve::g_engine.get( )->get_local_player( ) )
				return;

			if ( m_elements.empty( ) )
				return;

			const auto shot = std::find_if(
				m_elements.begin( ), m_elements.end( ),
				[ ]( const shot_t& shot ) {
					return shot.m_cmd_number != -1 && !shot.m_server_info.m_fire_tick
						&& std::abs( valve::g_client_state->m_cmd_ack - shot.m_cmd_number ) <= 20;
				}
			);

			if ( shot == m_elements.end( ) )
				return;

			shot->m_process_tick = valve::g_global_vars.get( )->m_real_time + 2.5f;
			shot->m_server_info.m_fire_tick = valve::g_client_state->m_server_tick;
		} break;
		case 0x9b5f9138u/* bullet_impact */: {
			g_shot_construct->on_impact( event );
			if ( !event || !g_local_player->self( ) )
				return;

			// get attacker, if its not us, screw it.
			auto attacker = valve::g_engine.get( )->index_for_uid( event->get_int( ( "userid" ) ) );
			if ( attacker != valve::g_engine.get( )->get_local_player( ) )
				return;

			// decode impact coordinates and convert to vec3.
			sdk::vec3_t pos = {
				event->get_float( ( "x" ) ),
				event->get_float( ( "y" ) ),
				event->get_float( ( "z" ) )
			};

			if ( const auto shot = last_unprocessed( ) )
				shot->m_server_info.m_impact_pos = pos;

		} break;
		case 0xd9dda907u: { // item_equip

			const auto idx = valve::g_engine.get( )->index_for_uid( event->get_int( ( "userid" ) ) );

			hacks::g_visuals->m_dormant_data[ idx ].m_weapon_id = event->get_int( ( "defindex" ) );
			hacks::g_visuals->m_dormant_data[ idx ].m_weapon_type = event->get_int( ( "weptype" ) );
		}break;

		case 0x2df1832d: {
			//const auto entity = valve::g_entity_list.get( )->get_entity( valve::g_engine.get( )->index_for_uid( event->get_int( xor_str( "userid" ) ) ) );
			//if ( !entity
			//	|| entity == g_local_player->self( )
			//	|| ( ( static_cast < valve::cs_player_t* > ( entity ) )->friendly( g_local_player->self( ) ) ) )
			//	return;
			//hacks::g_visuals->m_dormant_data.at( entity->networkable( )->index( ) ).m_receive_time = valve::g_global_vars.get( )->m_real_time;
			//hacks::g_visuals->m_dormant_data.at( entity->networkable( )->index( ) ).m_origin = entity->origin( );

		}break;

		case 0x19180a27u/* round_freeze_end */: /*g_context->freeze_time ( ) = false;*/ break;
		case 0x2301969du/* round_prestart */:
			hacks::g_visuals->reset_kill_feed( ) = true;
			hacks::g_misc->buy_bot_count( ) = 2;
			for ( std::size_t i{}; i < valve::g_global_vars.get( )->m_max_clients; ++i ) {
				hacks::g_visuals->m_dormant_data[ i ].m_origin = {};
				hacks::g_visuals->m_dormant_data[ i ].m_receive_time = 0.f;
				hacks::g_visuals->m_dormant_data[ i ].m_alpha = 0.f;
				hacks::g_visuals->m_dormant_data[ i ].m_alpha = std::clamp( hacks::g_visuals->m_dormant_data[ i ].m_alpha, 0.f, 255.f );
				hacks::g_visuals->m_dormant_data[ i ].m_use_shared = false;
				hacks::g_visuals->m_dormant_data[ i ].m_weapon_id = 0;
				hacks::g_visuals->m_dormant_data[ i ].m_weapon_type = -1;
				hacks::g_visuals->m_dormant_data.at( i ).m_last_shared_time = 0.f;
			}

			g_local_player->anim_sync( ).m_anim_data.reset( );

			hacks::g_visuals->m_bullet_tracers.clear( );
			hacks::g_visuals->m_hit_markers.clear( );

			g_shots->m_elements.clear( );

			g_local_player->prediction( ).reset( );

			//g_local_player->anim_sync( ).data( ).reset( );

			/*
			hacks::g_eng_pred->reset_on_spawn( );
			g_ctx->anim_data( ).m_local_data.reset( );

			g_shots->m_elements.clear( );

			hacks::g_local_sync->m_anim_layers = {};
			hacks::g_local_sync->m_old_layers = {};

			hacks::g_local_sync->m_pose_params = {};
			hacks::g_local_sync->m_old_params = {};

			hacks::g_visuals->m_bullet_tracers.clear( );
			hacks::g_visuals->m_hit_markers.clear( );

			for ( std::size_t i{}; i < 64u; ++i ) {
				auto& entry = g_lag_comp->entry( i );

				entry.m_air_misses = 0;
				entry.m_lby_misses = 0;
				entry.m_moving_misses = 0;
				entry.m_moved = false;
				entry.m_stand_moved_misses = 0;
				entry.m_stand_not_moved_misses = 0;
			}

			break;*/
		}
	}

	float get_absolute_time( )
	{
		return ( float ) ( clock( ) / ( float ) 1000.f );
	}

	void c_logs::draw_data( )
	{
		for ( int i = 0u; i < m_logs.size( ); i++ )
		{
			log_data_t* log_data = &m_logs[ i ];
			if ( !log_data )
				continue;

			float_t time_delta = get_absolute_time( ) - log_data->m_creation_time;
			if ( time_delta >= 5.0f )
			{
				m_logs.erase( m_logs.begin( ) + i );
				continue;
			}

			if ( time_delta >= 4.75f )
			{
				log_data->m_text_alpha = std::clamp( ( ( 5.0f - time_delta ) / 0.25f ) * 255.0f, 0.0f, 255.0f );
				log_data->m_back_alpha = std::clamp( ( ( 5.0f - time_delta ) / 0.25f ) * 50.0f, 0.0f, 50.0f );
				log_data->m_spacing = std::clamp( ( ( 5.0f - time_delta ) / 0.25f ) * 24.0f, 0.0f, 24.0f );
			}
			else if ( time_delta <= 0.25f )
			{
				log_data->m_text_alpha = std::clamp( ( 1.0f - ( 0.25f - time_delta ) / 0.25f ) * 255.0f, 0.0f, 255.0f );
				log_data->m_back_alpha = std::clamp( ( 1.0f - ( 0.25f - time_delta ) / 0.25f ) * 50.0f, 0.0f, 50.0f );
				log_data->m_spacing = std::clamp( ( 1.0f - ( 0.25f - time_delta ) / 0.25f ) * 24.0f, 0.0f, 24.0f );
			}

			float spacing = 0.0f;
			if ( i )
			{
				for ( int i2 = 0; i2 < i; i2++ )
					spacing += m_logs[ i2 ].m_spacing;
			}

			uint8_t r_clr = log_data->m_color.r( );
			uint8_t g_clr = log_data->m_color.g( );
			uint8_t b_clr = log_data->m_color.b( );

			uint8_t white_clr[ 4 ] = { r_clr, g_clr, b_clr, 255 };

			sdk::col_t icon_color = sdk::col_t( 122, 139, 165, ( int ) ( log_data->m_text_alpha ) );
			if ( !log_data->m_printed )
			{
				valve::g_cvar->con_print( false, *white_clr, "[ advance.tech ] " );
				valve::g_cvar->con_print( false, *white_clr, log_data->m_string.c_str( ) );
				valve::g_cvar->con_print( false, *white_clr, ( "\n" ) );
	
				log_data->m_printed = true;
			}

			csgo::g_render->text( "A", sdk::vec2_t( 4, spacing + 3 ), icon_color, csgo::g_render->m_fonts.m_museo_sans.m_300_16, true, false, false );

			csgo::g_render->text( log_data->m_string, sdk::vec2_t( 18, spacing + 2.5f ), sdk::col_t( log_data->m_color.r( ), log_data->m_color.g( ), log_data->m_color.b( ), ( int ) ( log_data->m_text_alpha ) ), csgo::g_render->m_fonts.m_museo_sans.m_300_16, true, false, false );
		}
	}

	void c_logs::push_log( std::string string, std::string icon, sdk::col_t color )
	{
		log_data_t data;

		data.m_creation_time = get_absolute_time( );
		data.m_spacing = 0.f;
		data.m_text_alpha = 0.f;
		data.m_icon = icon;
		data.m_string = string;
		data.m_color = color;

		m_logs.emplace_back( data );

		while ( m_logs.size( ) > 10.f )
			m_logs.pop_back( );
	}

	void c_shot_construct::on_rage_bot( aim_target_t* target, float damage, int bullets, std::shared_ptr < lag_record_t > record, int hitbox, const sdk::vec3_t& shoot_pos, int cmd_number ) {
		// setup new shot data.
		c_shot_record shot;
		shot.m_target = target;
		shot.m_record = record;
		shot.m_shot_time = valve::g_global_vars.get( )->m_real_time;
		shot.m_damage = damage;
		shot.m_pos = shoot_pos;
		shot.m_hitbox = hitbox;
		shot.m_impacted = false;
		shot.m_confirmed = false;
		shot.m_hurt = false;
		shot.m_cmd_num = cmd_number;
		shot.m_lat = g_local_player->net_info( ).m_latency.m_out;

		if ( g_local_player->self( )->weapon( )
			&& g_local_player->self( )->weapon( )->info( ) ) {
			shot.m_weapon_range = g_local_player->self( )->weapon( )->info( )->m_range;
		}

		if ( target
			&& record ) {
			const auto cur_bones = record->m_anim_sides.at( record->m_anim_side ).m_bones;
			auto matrix = cur_bones;

			shot.m_matrix = matrix;
		}

		// add to tracks.
		m_shots.push_front( shot );

		while ( m_shots.size( ) > 128 )
			m_shots.pop_back( );
	}

	void c_shot_construct::on_impact( valve::game_event_t* evt )
	{
		int        attacker;
		sdk::vec3_t     pos, dir, start, end;
		float      time;
		valve::trace_t trace;

		// screw this.
		if ( !evt || !g_local_player->self( ) )
			return;

		// get attacker, if its not us, screw it.
		attacker = valve::g_engine.get( )->index_for_uid( evt->get_int( ( "userid" ) ) );
		if ( attacker != valve::g_engine.get( )->get_local_player( ) ) {
			if ( ( static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( attacker ) ) )->team( ) == g_local_player->self( )->team( ) )
				return;

			pos = {
			evt->get_float( ( "x" ) ),
			evt->get_float( ( "y" ) ),
			evt->get_float( ( "z" ) )
			};

			c_visuals::bullet_trace_data_t enemy_trace_data{};

			enemy_trace_data.m_start_pos = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( attacker ) )->abs_origin( ) + static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( attacker ) )->view_offset( );
			enemy_trace_data.m_spawn_time = valve::g_global_vars.get( )->m_cur_time;
			enemy_trace_data.m_end_pos = pos;

			g_visuals->m_enemy_bullet_tracers.emplace_front( enemy_trace_data );

			return;
		}

		// decode impact coordinates and convert to vec3.
		pos = {
			evt->get_float( ( "x" ) ),
			evt->get_float( ( "y" ) ),
			evt->get_float( ( "z" ) )
		};

		// add to visual impacts if we have features that rely on it enabled.
		// todo - dex; need to match shots for this to have proper GetShootPosition, don't really care to do it anymore.
		//if( g_menu.main.visuals.impact_beams.get ( ) )
		//	m_vis_impacts.push_back ( { pos, g_cl.m_local->GetShootPosition ( ), g_cl.m_local->m_nTickBase ( ) } );

		auto& vis_impacts = hacks::g_visuals->m_bullet_impacts;

		c_visuals::bullet_trace_data_t trace_data{};
		trace_data.m_start_pos = hacks::g_aim_bot->shoot_pos( );
		trace_data.m_spawn_time = valve::g_global_vars.get( )->m_cur_time;
		trace_data.m_end_pos = pos;

		g_visuals->m_bullet_tracers.emplace_front( trace_data );

		if ( !vis_impacts.empty( )
			&& vis_impacts.back( ).m_time == valve::g_global_vars.get( )->m_cur_time )
			vis_impacts.back( ).m_final = false;

		vis_impacts.emplace_back(
			valve::g_global_vars.get( )->m_cur_time,
			hacks::g_aim_bot->shoot_pos( ),
			pos
		);
	}

	void c_shot_construct::on_hurt( valve::game_event_t* evt ) {

		int         attacker, victim, group, hp;
		float       damage;
		std::string name;

		if ( !evt || !g_local_player->self( ) )
			return;

		attacker = valve::g_engine.get( )->index_for_uid( evt->get_int( ( "attacker" ) ) );
		victim = valve::g_engine.get( )->index_for_uid( evt->get_int( ( "userid" ) ) );

		if ( attacker != g_local_player->self( )->networkable( )->index( )
			|| victim == g_local_player->self( )->networkable( )->index( ) )
			return;

		group = evt->get_int( ( "hitgroup" ) );

		if ( group == static_cast < int > ( valve::e_hitgroup::gear ) )
			return;

		valve::cs_player_t* target = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( victim ) );
		if ( !target )
			return;

		valve::player_info_t info;

		auto find = valve::g_engine.get( )->get_player_info( victim, &info );

		if ( !find )
			return;

		name = std::string( info.m_name ).substr( 0, 24 );

		damage = ( float ) evt->get_int( ( "dmg_health" ) );

		hp = evt->get_int( ( "health" ) );

		auto out = std::format( ( "hurt {} in the {} for {} damage ({} hp remaining)" ), name, m_groups[ group ], ( int ) damage, hp ).data( );

		g_logs->push_log( out, ( "d" ), sdk::col_t( 255, 255, 255, 255 ) );

		static auto get_hitbox_by_hitgroup = [ ]( int hitgroup ) -> valve::e_hitbox {
			switch ( hitgroup )
			{
			case 1:
				return valve::e_hitbox::head;
			case 2:
				return valve::e_hitbox::chest;
			case 3:
				return valve::e_hitbox::stomach;
			case 4:
				return valve::e_hitbox::left_hand;
			case 5:
				return valve::e_hitbox::right_hand;
			case 6:
				return valve::e_hitbox::right_calf;
			case 7:
				return valve::e_hitbox::left_calf;
			default:
				return valve::e_hitbox::pelvis;
			}
		};

		if ( group == static_cast < int > ( valve::e_hitgroup::generic ) )
			return;

		if ( g_shots->m_elements.empty( ) )
			return;

		auto& shot = g_shots->m_elements.back( );

		if ( !shot.m_target.m_lag_record )
			return;

		const auto cur_bones = shot.m_target.m_lag_record->m_anim_sides.at( shot.m_target.m_lag_record->m_anim_side ).m_bones;

		c_visuals::hit_marker_data_t hit_marker_data{};

		hit_marker_data.m_spawn_time = valve::g_global_vars.get( )->m_cur_time;
		hit_marker_data.m_pos = target->who_tf_asked( static_cast < int > ( get_hitbox_by_hitgroup( group ) ), cur_bones );

		hacks::g_visuals->m_hit_markers.emplace_back( hit_marker_data );
	}

	void c_shot_construct::on_fire( valve::game_event_t* evt )
	{
		int attacker;

		// screw this.
		if ( !evt || !g_local_player->self( ) )
			return;

		// get attacker, if its not us, screw it.
		attacker = valve::g_engine.get( )->index_for_uid( evt->get_int( ( "userid" ) ) );
		if ( attacker != valve::g_engine.get( )->get_local_player( ) )
			return;
	}
	ALWAYS_INLINE float dist_to( const sdk::vec3_t& from, const sdk::vec3_t& other ) {
		sdk::vec3_t delta;

		delta.x( ) = from.x( ) - other.x( );
		delta.y( ) = from.y( ) - other.y( );
		delta.z( ) = from.z( ) - other.z( );

		return delta.length( 2u );
	}

	void c_shot_construct::on_render_start( ) {
		if ( !valve::g_engine.get( )->in_game( ) ) {
			return g_shots->m_elements.clear( );
		}

		for ( auto& shot : g_shots->m_elements ) {
			if ( shot.m_processed
				|| valve::g_global_vars.get( )->m_real_time > shot.m_process_tick )
				continue;

			if ( shot.m_target.m_entry
				&& shot.m_target.m_entry->m_player ) {

				if ( !shot.m_target.m_entry->m_player->alive( ) ) {
				}
				else {
					lag_backup_t lag_data{ shot.m_target.m_entry->m_player };

					if ( shot.m_server_info.m_hurt_tick ) {
					}
					else {
						shot.m_target.m_lag_record->setup( shot.m_target.m_entry->m_player, shot.m_target.m_lag_record->m_anim_side );

						valve::trace_t trace{};

						valve::g_engine_trace->clip_ray_to_entity(
							{ shot.m_src, shot.m_server_info.m_impact_pos },
							valve::e_mask::shot_player, shot.m_target.m_entry->m_player, &trace
						);

						if ( trace.m_entity != shot.m_target.m_entry->m_player ) {

							if ( ( ( shot.m_src - shot.m_target.m_pos ).length( ) - 32.f ) > ( shot.m_src - shot.m_server_info.m_impact_pos ).length( ) )
								g_logs->push_log( ( "missed shot due to occlusion" ), ( "d" ), sdk::col_t( 224, 145, 27, 255 ) );
							else
								g_logs->push_log( ( "missed shot due to spread" ), ( "d" ), sdk::col_t( 224, 145, 27, 255 ) );
						}
						else {
							{
								std::string out = "missed shot due to animation desync";

								if ( shot.m_target.m_lag_record->m_jitter_correction_applied )
									out = "missed shot due to jitter correction";

								g_logs->push_log( out, ( "d" ), sdk::col_t( 207, 91, 28, 255 ) );

							}

							if ( shot.m_target.m_lag_record->m_jitter_correction_applied )
								++shot.m_target.m_entry->m_jitter_misses;
							else
								++shot.m_target.m_entry->m_misses;
						}
					}
					lag_data.restore( shot.m_target.m_entry->m_player );
				}
			}

			shot.m_processed = true;
		}

		g_shots->m_elements.erase( std::remove_if(
			g_shots->m_elements.begin( ), g_shots->m_elements.end( ),
			[ & ]( const shot_t& shot ) {
				if ( !shot.m_processed
					&& shot.m_process_tick )
					return false;

				if ( std::abs( valve::g_global_vars.get( )->m_real_time - shot.m_shot_time ) >= 2.5f )
					return true;

				if ( shot.m_processed
					|| shot.m_process_tick
					|| shot.m_target_index == -1
					|| shot.m_cmd_number == -1
					|| std::abs( valve::g_client_state->m_cmd_ack - shot.m_cmd_number ) <= 20 )
					return false;

				const auto delta = std::abs( valve::g_global_vars.get( )->m_real_time - shot.m_sent_time );
				if ( delta > 2.5f )
					return true;

				if ( delta <= shot.m_latency )
					return false;

				return true;
			}
		),
			g_shots->m_elements.end( )
				);
	}
}