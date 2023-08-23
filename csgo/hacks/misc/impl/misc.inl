#include "../csgo/hacks/misc/misc.h"

namespace hacks {
	ALWAYS_INLINE void c_misc::third_person( ) {
		bool is_enable = g_key_binds->get_keybind_state( &g_menu->main( ).m_misc_cfg.get( ).m_third_person );
		static float distance = 0.f;

		if ( !g_local_player || !g_local_player->self( ) )
			return;

		if ( !g_local_player->self( )->alive( ) )
		{
			if ( g_menu->main( ).m_misc_cfg.get( ).m_third_person_when_dead )
			{
				valve::g_input->m_camera_in_third_person = false;
				distance = 50.f;

				g_local_player->self( )->observer_mode( ) = 5;
			}

			return;
		}

		if ( is_enable )
			distance = g_menu->main( ).m_misc_cfg.get( ).m_third_person_val;
		else
			distance = std::max( distance - 6.0f, 30.0f );

		if ( distance <= ( is_enable ? 50.0f : 30.0f ) )
		{
			valve::g_input->m_camera_in_third_person = false;
			return;
		}

		const sdk::vec3_t& eye_pos = g_local_player->shoot_pos( );

		sdk::qang_t view_angles = valve::g_engine.get( )->view_angles( );

		valve::g_input->m_camera_in_third_person = true;
		valve::g_input->m_camera_offset = sdk::vec3_t( view_angles.x( ), view_angles.y( ), distance );

		valve::trace_t trace;

		float extent = 12.f + g_menu->main( ).m_misc_cfg.get( ).m_third_person_val / 4.8f - 18.f;

		sdk::vec3_t vec_forward = sdk::vec3_t( 0, 0, 0 );
		sdk::ang_vecs( sdk::qang_t( view_angles.x( ), view_angles.y( ), 0.0f ), &vec_forward, nullptr, nullptr );

		valve::trace_filter_world_only_t filter;

		valve::ray_t ray( eye_pos, eye_pos - vec_forward * valve::g_input->m_camera_offset.z( ) );
		ray.m_extents = sdk::vec3_t( extent, extent, extent );
		ray.m_ray = false;

		valve::g_engine_trace->trace_ray( ray, valve::e_mask::npc_world_static, &filter, &trace );

		valve::g_input->m_camera_offset.z( ) *= trace.m_frac;

		if ( valve::g_input->m_camera_offset.z( ) < std::min( 30.0f, g_menu->main( ).m_misc_cfg.get( ).m_third_person_val ) )
			valve::g_input->m_camera_in_third_person = false;
	}

	ALWAYS_INLINE void c_misc::clan_tag( ) {
		using set_clan_tag_t = int( __fastcall* )( const char*, const char* );

		if ( g_menu->main( ).m_misc_cfg.get( ).m_clan_tag ) {
			const auto i = ( valve::g_global_vars.get( )->m_tick_count / valve::to_ticks( 1.f ) ) % 34;
			if ( i != g_menu->main( ).m_misc_cfg.get( ).m_prev_tag ) {
				g_menu->main( ).m_misc_cfg.get( ).m_reset_tag = true;

				auto tag = "";

				switch ( i )
				{
				case 0:
					tag = ( "A" );
					break;
				case 1:
					tag = ( "A$%" );
					break;
				case 2:
					tag = ( "Ad" );
					break;
				case 3:
					tag = ( "Ad&$" );
					break;
				case 4:
					tag = ( "Adv" );
					break;
				case 5:
					tag = ( "Adv@*" );
					break;
				case 6:
					tag = ( "Adva" );
					break;
				case 7:
					tag = ( "Adva@%" );
					break;
				case 8:
					tag = ( "Advan" );
					break;
				case 9:
					tag = ( "Advan#^" );
					break;
				case 10:
					tag = ( "Advanc" );
					break;
				case 11:
					tag = ( "Advanc*#" );
					break;
				case 12:
					tag = ( "Advance" );
					break;
				case 13:
					tag = ( "Advance&@" );
					break;
				case 14:
					tag = ( "Advance." );
					break;
				case 15:
					tag = ( "Advance.#$" );
					break;
				case 16:
					tag = ( "Advance.t" );
					break;
				case 17:
					tag = ( "Advance.t*@" );
					break;
				case 18:
					tag = ( "Advance.te" );
					break;
				case 19:
					tag = ( "Advance.te#%" );
					break;
				case 20:
					tag = ( "Advance.tec" );
					break;
				case 21:
					tag = ( "Advance.tec&^" );
					break;
				case 22:
					tag = ( "Advance.tech" );
					break;
				case 23:
					tag = ( "Advance.teñ" );
					break;
				case 24:
					tag = ( "Advance.t" );
					break;
				case 25:
					tag = ( "Advance." );
					break;
				case 26:
					tag = ( "Advance" );
					break;
				case 27:
					tag = ( "Advanc" );
					break;
				case 28:
					tag = ( "Advan" );
					break;
				case 29:
					tag = ( "Adva" );
					break;
				case 30:
					tag = ( "Adv" );
					break;
				case 31:
					tag = ( "Ad" );
					break;
				case 32:
					tag = ( "A" );
					break;
				case 33:
					tag = ( "" );
					break;
				}

				reinterpret_cast< set_clan_tag_t >( g_ctx->offsets ( ).m_addresses.m_set_clan_tag )( tag, tag );

				g_menu->main( ).m_misc_cfg.get( ).m_prev_tag = i;
			}
		}
		else if ( g_menu->main( ).m_misc_cfg.get( ).m_reset_tag ) {
			g_menu->main( ).m_misc_cfg.get( ).m_reset_tag = false;

			reinterpret_cast< set_clan_tag_t >( g_ctx->offsets( ).m_addresses.m_set_clan_tag )( "", "" );
		}
	}

	ALWAYS_INLINE void c_misc::removals( ) {
		if ( g_local_player->self( ) ) {
			if ( g_local_player->self( )->flash_dur( ) && g_menu->main( ).m_misc_cfg.get( ).m_remove_flash )
				g_local_player->self( )->flash_dur( ) = 0.f;
		}

		static std::vector < std::string > smoke_str = {
		( "particle/vistasmokev1/vistasmokev1_smokegrenade" ),
		( "particle/vistasmokev1/vistasmokev1_emods" ),
		( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ),
		( "particle/vistasmokev1/vistasmokev1_fire" ),
		};

		for ( const auto& smoke_mat : smoke_str ) {
			auto cur_mat = valve::g_material_system->find_mat( smoke_mat.c_str( ), "Other textures" );

			if ( !cur_mat )
				continue;

			cur_mat->set_flag( 1 << 2, g_menu->main( ).m_misc_cfg.get( ).m_remove_smoke );
		}

		if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_smoke ) {
			if ( *reinterpret_cast < std::int32_t* > ( *reinterpret_cast < std::uint32_t** > ( reinterpret_cast < std::uint32_t > ( g_ctx->offsets( ).m_addresses.m_smoke_count ) ) ) != 0 ) {
				*reinterpret_cast < std::int32_t* > ( *reinterpret_cast < std::uint32_t** > ( reinterpret_cast < std::uint32_t > ( g_ctx->offsets( ).m_addresses.m_smoke_count ) ) ) = 0;
			}
		}

		**reinterpret_cast < bool** > ( reinterpret_cast < std::uint32_t > ( g_ctx->offsets( ).m_addresses.m_post_process ) ) = g_menu->main( ).m_misc_cfg.get( ).m_remove_post_processing;

		static auto cl_wpn_sway_amt = valve::g_cvar->find_var( ( "cl_wpn_sway_scale" ) );

		if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_hands_shaking ) {
			cl_wpn_sway_amt->set_float( 0.f );
		}
		else
			cl_wpn_sway_amt->set_float( 1.f );

		static auto blur_overlay = valve::g_material_system->find_mat( ( "dev/scope_bluroverlay" ), "Other textures" );
		static auto scope_dirt = valve::g_material_system->find_mat( ( "models/weapons/shared/scope/scope_lens_dirt" ), "Other textures" );

		blur_overlay->set_flag( ( 1 << 2 ), false );
		scope_dirt->set_flag( ( 1 << 2 ), false );

		if ( g_local_player->self( )
			&& g_local_player->self( )->alive( ) ) {
			auto wpn = g_local_player->self( )->weapon( );
			if ( wpn ) {
				if ( wpn->info( )
					&& wpn->info( )->m_type == valve::e_weapon_type::sniper ) {
					if ( g_local_player->self( )->scoped( ) ) {
						if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_scope ) {
							blur_overlay->set_flag( ( 1 << 2 ), true );
							scope_dirt->set_flag( ( 1 << 2 ), true );
						}
					}
				}
			}
		}
	}

	ALWAYS_INLINE int& c_misc::buy_bot_count( ) {
		return m_buy_bot_count;
	}
}