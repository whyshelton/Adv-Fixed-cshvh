#include "../../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"
namespace hacks {
	__forceinline std::string get_weapon_name( valve::base_weapon_t* wpn ) {
		auto get_clean_name = [ ]( const char* name ) -> const char* {
			if ( name[ 0 ] == 'C' )
				name++;

			auto start = strstr( name, ( "Weapon" ) );
			if ( start != nullptr )
				name = start + 6;

			return name;
		};

		if ( !wpn )
			return "";

		valve::e_item_index weapon_index = wpn->item_index( );

		if ( !wpn->networkable( )->client_class( ) )
			return "";

		std::string str_result = "";
		switch ( weapon_index )
		{
		case valve::e_item_index::revolver: str_result = ( "R8" ); break;
		case valve::e_item_index::scar20: str_result = ( "SCAR20" ); break;
		case valve::e_item_index::deagle: str_result = ( "DEAGLE" ); break;
		case valve::e_item_index::elite: str_result = ( "DUAL ELITE" ); break;
		case valve::e_item_index::five_seven: str_result = ( "FIVE-SEVEN" ); break;
		case valve::e_item_index::frag_grenade: str_result = ( "FRAG GRENADE" ); break;
		case valve::e_item_index::smoke_grenade: str_result = ( "SMOKE GRENADE" ); break;
		case valve::e_item_index::decoy: str_result = ( "DECOY GRENADE" ); break;
		case valve::e_item_index::flashbang: str_result = ( "FLASHBANG" ); break;
		case valve::e_item_index::p2000: str_result = ( "P2000" ); break;
		case valve::e_item_index::inc_grenade: str_result = ( "FIRE GRENADE" ); break;
		case valve::e_item_index::molotov: str_result = ( "MOLOTOV" ); break;
		case valve::e_item_index::he_grenade: str_result = ( "FRAG GRENADE" ); break;
		default: str_result = get_clean_name( wpn->networkable( )->client_class( )->m_network_name );
		}

		if ( str_result == ( "HKP2000" ) )
			return ( "P2000" );

		std::transform( str_result.begin( ), str_result.end( ), str_result.begin( ), ::toupper );

		return str_result;
	}

	__forceinline std::string get_weapon_icon( valve::base_weapon_t* wpn )
	{
		if ( !wpn )
			return " ";

		if ( wpn->is_knife( ) )
		{
			return ( "[" );
		}

		std::string str_result = "";
		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::scar20: str_result = ( "Y" ); break;
		case valve::e_item_index::g3sg1: str_result = ( "X" ); break;
		case valve::e_item_index::awp: str_result = ( "Z" ); break;
		case valve::e_item_index::ssg08: str_result = ( "a" ); break;
		case valve::e_item_index::deagle: str_result = ( "A" ); break;
		case valve::e_item_index::revolver: str_result = ( "J" ); break;
		case valve::e_item_index::p2000: str_result = ( "E" ); break;
		case valve::e_item_index::glock: str_result = ( "D" ); break;
		case valve::e_item_index::usps: str_result = ( "G" ); break;
		case valve::e_item_index::elite: str_result = ( "B" ); break;
		case valve::e_item_index::c4: str_result = ( "o" ); break;
		case valve::e_item_index::p250: str_result = ( "F" ); break;
		case valve::e_item_index::aug: str_result = ( "U" ); break;
		case valve::e_item_index::five_seven: str_result = ( "C" ); break;
		case valve::e_item_index::ak47: str_result = ( "W" ); break;
		case valve::e_item_index::galil: str_result = ( "Q" ); break;
		case valve::e_item_index::cz75a: str_result = ( "I" ); break;
		case valve::e_item_index::famas: str_result = ( "R" ); break;
		case valve::e_item_index::tec9: str_result = ( "H" ); break;
		case valve::e_item_index::bizon: str_result = ( "M" ); break;
		case valve::e_item_index::m249: str_result = ( "g" ); break;
		case valve::e_item_index::negev: str_result = ( "f" ); break;
		case valve::e_item_index::nova: str_result = ( "e" ); break;
		case valve::e_item_index::mag7: str_result = ( "d" ); break;
		case valve::e_item_index::taser: str_result = ( "h" ); break;
		case valve::e_item_index::he_grenade: str_result = ( "j" ); break;
		case valve::e_item_index::smoke_grenade: str_result = ( "k" ); break;
		case valve::e_item_index::frag_grenade: str_result = ( "n" ); break;
		case valve::e_item_index::molotov: str_result = ( "l" ); break;
		case valve::e_item_index::sawed_off: str_result = ( "c" ); break;
		case valve::e_item_index::decoy: str_result = ( "m" ); break;
		case valve::e_item_index::flashbang: str_result = ( "i" ); break;
		case valve::e_item_index::m4a4: str_result = ( "S" ); break;
		case valve::e_item_index::m4a1s: str_result = ( "T" ); break;
		case valve::e_item_index::firebomb: str_result = ( "k" ); break;
		case valve::e_item_index::mac10: str_result = ( "K" ); break;
		case valve::e_item_index::ump45: str_result = ( "L" ); break;
		case valve::e_item_index::mp7: str_result = ( "N" ); break;
		case valve::e_item_index::p90: str_result = ( "P" ); break;
		case valve::e_item_index::mp9: str_result = ( "N" ); break;
		case valve::e_item_index::sg553: str_result = ( "V" ); break;
		case valve::e_item_index::xm1014: str_result = ( "e" ); break;
		case valve::e_item_index::ta_grenade: str_result = ( "i" ); break;
		}

		return str_result;
	}

	__forceinline void trace_hull( const sdk::vec3_t& src, const sdk::vec3_t& end, valve::trace_t& trace, valve::base_entity_t* entity, std::uint32_t mask, int col_group ) {
		static const sdk::vec3_t hull[ 2 ] = { sdk::vec3_t( -2.0f, -2.0f, -2.0f ), sdk::vec3_t( 2.0f, 2.0f, 2.0f ) };
		/*pred_trace_filter_t pred_filter;

		pred_filter.set_class_to_ignore( "CBaseCSGrenadeProjectile" );
		pred_filter.m_ignored_entities = ignored_entities;
*/
		valve::trace_filter_simple_t filter{ entity, col_group };
		valve::ray_t ray{ src, end, hull[ 0 ], hull[ 1 ] };

		valve::g_engine_trace->trace_ray_( ray, mask, reinterpret_cast< valve::base_trace_filter_t* > ( &filter ), &trace );
	}

	__forceinline void trace_line( const sdk::vec3_t& src, const sdk::vec3_t& end, valve::trace_t& trace, valve::base_entity_t* entity, std::uint32_t mask, int col_group ) {
		valve::trace_filter_simple_t filter{ entity, col_group };

		valve::ray_t ray{ src, end };

		valve::g_engine_trace->trace_ray_( ray, mask, reinterpret_cast< valve::base_trace_filter_t* > ( &filter ), &trace );
	}

	void c_visuals::handle( ) const {
		const std::ptrdiff_t entities = valve::g_entity_list.get( )->highest_index( );
		for ( std::ptrdiff_t i{}; i <= entities; ++i ) {
			const auto entity = valve::g_entity_list.get( )->get_entity( i );

			if ( !entity || entity->networkable( )->dormant( )
				|| entity->is_player( ) || !entity->networkable( )->client_class( ) )
				continue;

			if ( entity->is_weapon( ) ) {
				auto model = entity->renderable( )->model( );
				if ( model ) {
					auto weapon = ( valve::base_weapon_t* ) entity;
					sdk::vec3_t screen{};

					const auto& origin = weapon->origin( );
					if ( origin.x( ) == 0.f
						&& origin.y( ) == 0.f
						&& origin.z( ) == 0.f )
						continue;

					auto on_screen = csgo::g_render->world_to_screen( origin, screen );

					if ( !on_screen )
						continue;

					auto offset = 0;

					if ( g_menu->main( ).m_visuals_cfg.get( ).m_draw_wpn_proj_icon ) {
						csgo::g_render->text( get_weapon_icon( weapon ), sdk::vec2_t( screen.x( ), screen.y( ) ),
							sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_icons.m_icon_font, true, true, false );
						offset += 20;
					}

					if ( g_menu->main( ).m_visuals_cfg.get( ).m_draw_wpn_proj )
						csgo::g_render->text( get_weapon_name( weapon ), sdk::vec2_t( screen.x( ), screen.y( ) + offset ),
							sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );
				}
			}

			switch ( static_cast < valve::e_class_id > ( entity->networkable( )->client_class( )->m_class_id ) ) {
			case valve::e_class_id::tonemap_controller:
				tone_map_modulation( entity );
				break;
			case valve::e_class_id::cascade_light:
				change_shadows( entity );
				break;
			case valve::e_class_id::smoke_grenade_projectile:
				smoke_timer( entity );
				break;
			case valve::e_class_id::inferno:
				molotov_timer( entity );
				break;
			default:
				grenade_projectiles( entity );
				break;
			}
		}
	}

	__forceinline bool is_zero_vec3_t( sdk::vec3_t vec ) {
		return ( vec.x( ) > -0.01f && vec.x( ) < 0.01f &&
			vec.y( ) > -0.01f && vec.y( ) < 0.01f &&
			vec.z( ) > -0.01f && vec.z( ) < 0.01f );
	}

	void c_visuals::draw_auto_peek( ) {
		if ( !g_local_player->self( ) ||
			!g_local_player->self( )->alive( ) )
			return;

		if ( !g_local_player->self( )->weapon( ) )
			return;

		if ( g_local_player->self( )->weapon( )->is_knife( ) )
			return;

		if ( g_local_player->self( )->weapon( )->item_index( ) == valve::e_item_index::taser )
			return;

		if ( !g_local_player->self( )->weapon( )->info( ) )
			return;

		if ( g_local_player->self( )->weapon( )->info( )->m_type == static_cast < valve::e_weapon_type > ( 9 ) )
			return;


		static auto pos = sdk::vec3_t( );

		if ( !is_zero_vec3_t( g_move->auto_peek_data( ).m_pos ) )
			pos = g_move->auto_peek_data( ).m_pos;

		if ( is_zero_vec3_t( pos ) )
			return;

		static auto alpha = 4.f;

		if ( g_key_binds->get_keybind_state( &g_menu->main( ).m_move_cfg.get( ).m_auto_peek )
			|| alpha ) {
			if ( g_key_binds->get_keybind_state( &g_menu->main( ).m_move_cfg.get( ).m_auto_peek ) )
				alpha += 9.0f * valve::g_global_vars.get( )->m_frame_time;
			else
				alpha -= 9.0f * valve::g_global_vars.get( )->m_frame_time;;

			alpha = std::clamp( alpha, 0.0f, 1.0f );

			static constexpr float step_ = sdk::k_pi2< float > * 2.0f / 60;
			std::vector<ImVec2> points;
			for ( float lat = 0.f; lat <= sdk::k_pi2< float > * 2.0f; lat += step_ )
			{
				const auto point3d = sdk::vec3_t( sin( lat ), cos( lat ), 0.f ) * 13.f * alpha;

				sdk::vec3_t point2d;
				if ( csgo::g_render->world_to_screen( g_move->auto_peek_data( ).m_pos + point3d, point2d ) )
					points.push_back( ImVec2( point2d.x( ), point2d.y( ) ) );
			}
			auto flags_backup = csgo::g_render->m_draw_list->Flags;
			csgo::g_render->m_draw_list->Flags |= ImDrawListFlags_AntiAliasedFill;
			csgo::g_render->m_draw_list->AddConvexPolyFilled( points.data( ), points.size( ), sdk::col_t( 20, 20, 20, 127 * alpha ).hex( ) );
			csgo::g_render->m_draw_list->AddPolyline( points.data( ), points.size( ), sdk::col_t( 255, 255, 255, 127 * alpha ).hex( ), true, 2.f );
			csgo::g_render->m_draw_list->Flags = flags_backup;
		}
	}

	void c_visuals::skybox_changer( ) const {
		static auto sv_skyname = valve::g_cvar->find_var( ( "sv_skyname" ) );
		std::string skybox = sv_skyname->m_str;

		using sky_box_fn = void( __fastcall* ) ( const char* );

		static auto fn = reinterpret_cast < sky_box_fn > ( g_ctx->offsets( ).m_addresses.m_sky_box );

		switch ( g_menu->main( ).m_visuals_cfg.get( ).m_sky_box ) {
		case 1:
			skybox = ( "cs_tibet" );
			break;
		case 2:
			skybox = ( "cs_baggageskybox" );
			break;
		case 3:
			skybox = ( "italy" );
			break;
		case 4:
			skybox = ( "jungle" );
			break;
		case 5:
			skybox = ( "office" );
			break;
		case 6:
			skybox = ( "sky_cs15_daylight01_hdr" );
			break;
		case 7:
			skybox = ( "sky_cs15_daylight02_hdr" );
			break;
		case 8:
			skybox = ( "vertigoblue_hdr" );
			break;
		case 9:
			skybox = ( "vertigo" );
			break;
		case 10:
			skybox = ( "sky_day02_05_hdr" );
			break;
		case 11:
			skybox = ( "nukeblank" );
			break;
		case 12:
			skybox = ( "sky_venice" );
			break;
		case 13:
			skybox = ( "sky_cs15_daylight03_hdr" );
			break;
		case 14:
			skybox = ( "sky_cs15_daylight04_hdr" );
			break;
		case 15:
			skybox = ( "sky_csgo_cloudy01" );
			break;
		case 16:
			skybox = ( "sky_csgo_night02" );
			break;
		case 17:
			skybox = ( "sky_csgo_night02b" );
			break;
		case 18:
			skybox = ( "sky_csgo_night_flat" );
			break;
		case 19:
			skybox = ( "sky_dust" );
			break;
		case 20:
			skybox = ( "vietnam" );
			break;
		}

		fn( skybox.c_str( ) );
	}

	void c_visuals::tone_map_modulation( valve::base_entity_t* const entity ) const {
		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

		if ( !cfg.m_tone_map_modulation )
			return;

		valve::tone_map_t* const tone_map = static_cast < valve::tone_map_t* > ( entity );

		tone_map->use_custom_bloom_scale( ) = true;
		tone_map->custom_bloom_scale( ) = cfg.m_bloom * 0.01f;

		tone_map->user_custom_auto_exposure_min( ) = true;
		tone_map->auto_custom_exposure_min( ) = cfg.m_exposure * 0.001f;

		tone_map->user_custom_auto_exposure_max( ) = true;
		tone_map->auto_custom_exposure_max( ) = cfg.m_exposure * 0.001f;
	}
	
	__forceinline static DWORD* find_hud_element( const char* name ) {
		using find_t = std::uintptr_t* ( __thiscall* )( const std::uintptr_t, const char* );

		auto find_da_hud = reinterpret_cast< find_t >( g_ctx->offsets( ).m_addresses.m_find_hud_element ) ( g_ctx->offsets( ).m_addresses.m_hud, name );

		return ( DWORD* ) find_da_hud;
	}

	void c_visuals::kill_feed( ) {
		static DWORD* death_notice = nullptr;

		if ( valve::g_engine.get( )->in_game( ) ) {
			death_notice = find_hud_element( "CCSGO_HudDeathNotice" );

			if ( death_notice )
			{
				auto local_death_notice = ( float* ) ( ( uintptr_t ) death_notice + 0x50 );

				if ( local_death_notice )
					*local_death_notice = g_menu->main( ).m_visuals_cfg.get( ).m_kill_feed ? FLT_MAX : 1.5f;

				if ( m_reset_kill_feed )
				{
					using find_t = void( __thiscall* )( const std::uintptr_t );

					auto reset_kill_feed = reinterpret_cast < find_t >( g_ctx->offsets( ).m_addresses.m_clear_hud_wpn );

					reset_kill_feed( ( uintptr_t ) death_notice - 0x14 );

					m_reset_kill_feed = false;
				}
			}
		}
		else
			death_notice = 0;
	}


	void c_visuals::change_shadows( valve::base_entity_t* const entity ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_shadows_modulation )
			return;

		auto cascade_light = reinterpret_cast < valve::cascade_light_t* > ( entity );

		cascade_light->shadow_dir( ) = sdk::vec3_t( g_menu->main( ).m_visuals_cfg.get( ).m_x_dir, g_menu->main( ).m_visuals_cfg.get( ).m_y_dir, g_menu->main( ).m_visuals_cfg.get( ).m_z_dir );
	}

	void c_visuals::draw_scope_lines( ) const {
		if ( !g_local_player->self( )
			|| !g_local_player->self( )->alive( )
			|| !g_menu->main( ).m_misc_cfg.get( ).m_remove_scope )
			return;

		static int scope_alpha{};

		if ( g_local_player->self( )->scoped( ) )
			scope_alpha += 10;
		else
			scope_alpha -= 10;

		scope_alpha = std::clamp( scope_alpha, 0, 255 );

		auto wpn = g_local_player->self( )->weapon( );

		if ( wpn ) {
			auto wpn_data = wpn->info( );

			if ( wpn_data
				&& wpn_data->m_type == valve::e_weapon_type::sniper ) {
				int x, y;
				valve::g_engine.get( )->screen_size( x, y );
				csgo::g_render->line( sdk::vec2_t( 0, y / 2 ), sdk::vec2_t( x, y / 2 ), sdk::col_t( 0, 0, 0, scope_alpha ) );
				csgo::g_render->line( sdk::vec2_t( x / 2, 0 ), sdk::vec2_t( x / 2, y ), sdk::col_t( 0, 0, 0, scope_alpha ) );
			}
		}
	}


	void c_visuals::molotov_timer( valve::base_entity_t* const entity ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_molotov_timer )
			return;

		auto inferno = reinterpret_cast< valve::inferno_t* >( entity );
		auto origin = inferno->abs_origin( );

		sdk::vec3_t screen_origin;

		if ( !csgo::g_render->world_to_screen( origin, screen_origin ) )
			return;

		struct s_vecs {
			sdk::vec3_t a, b, c;
		};

		std::vector<int> excluded_ents{};
		std::vector<s_vecs> valid_molotovs{};

		auto spawn_time = inferno->get_spawn_time( );
		auto factor = ( spawn_time + valve::inferno_t::get_expiry_time( ) - valve::g_global_vars.get( )->m_cur_time )
			/ valve::inferno_t::get_expiry_time( );

		const auto thrower = static_cast< valve::cs_player_t* >( valve::g_entity_list.get( )->get_entity( entity->owner_entity_handle( ) ) );

		bool can_do_radius{ true };

		if ( !thrower )
			can_do_radius = false;

		if ( thrower &&
			thrower->team( ) == g_local_player->self( )->team( )
			&& thrower != g_local_player->self( ) )
			can_do_radius = false;

		if ( ( spawn_time + valve::inferno_t::get_expiry_time( ) - valve::g_global_vars.get( )->m_cur_time ) > 0.1f 
			&& can_do_radius ) {
			static const auto size = sdk::vec2_t( 70.f, 4.f );

			auto new_pos = sdk::vec2_t( screen_origin.x( ) - size.x( ) * 0.5, screen_origin.y( ) - size.y( ) * 0.5 );

			sdk::vec3_t min, max;
			entity->renderable( )->render_bounds( min, max );

			auto radius = ( max - min ).length( 2u ) * 0.5f;
			sdk::vec3_t bound_origin = sdk::vec3_t( ( min.x( ) + max.x( ) ) * 0.5f, ( min.y( ) + max.y( ) ) * 0.5f, min.z( ) + 5 ) + origin;
			const int accuracy = 25;
			const float step = sdk::k_pi2< float > / accuracy;
			for ( float a = 0.0f; a < sdk::k_pi2< float >; a += step ) {
				float a_c, a_s, as_c, as_s;
				sdk::sin_cos( a, a_s, a_c );
				sdk::sin_cos( a + step, as_s, as_c );

				sdk::vec3_t start = sdk::vec3_t( a_c * radius + bound_origin.x( ), a_s * radius + bound_origin.y( ), bound_origin.z( ) );
				sdk::vec3_t end = sdk::vec3_t( as_c * radius + bound_origin.x( ), as_s * radius + bound_origin.y( ), bound_origin.z( ) );

				sdk::vec3_t start2d, end2d, boundorigin2d;
				if ( !csgo::g_render->world_to_screen( start, start2d ) || !csgo::g_render->world_to_screen( end, end2d ) || !csgo::g_render->world_to_screen( bound_origin, boundorigin2d ) ) {
					excluded_ents.push_back( entity->networkable( )->index( ) );
					continue;
				}

				s_vecs n;
				n.a = start2d;
				n.b = end2d;
				n.c = boundorigin2d;
				valid_molotovs.push_back( n );
			}
		}

		if ( !excluded_ents.empty( ) ) {
			for ( int v = 0; v < excluded_ents.size( ); ++v ) {
				auto bbrr = excluded_ents[ v ];
				if ( bbrr == entity->networkable( )->index( ) )
					continue;

				if ( !valid_molotovs.empty( ) )
					for ( int m = 0; m < valid_molotovs.size( ); ++m ) {
						const auto& ba = valid_molotovs[ m ];
						csgo::g_render->triangle_filled( ba.c.x( ), ba.c.y( ), ba.a.x( ), ba.a.y( ), ba.b.x( ), ba.b.y( ), sdk::col_t( 255, 0, 0, 130 * factor ) ); 
						csgo::g_render->line( sdk::vec2_t( ba.a.x( ), ba.a.y( ) ), sdk::vec2_t( ba.b.x( ), ba.b.y( ) ), sdk::col_t::palette_t::white( 255 * factor ) );
					}
			}
		}
		else {
			if ( !valid_molotovs.empty( ) )
				for ( int m = 0; m < valid_molotovs.size( ); ++m ) {
					const auto& ba = valid_molotovs[ m ];
					csgo::g_render->triangle_filled( ba.c.x( ), ba.c.y( ), ba.a.x( ), ba.a.y( ), ba.b.x( ), ba.b.y( ), sdk::col_t( 255, 0, 0, 130 * factor ) );
					csgo::g_render->line( sdk::vec2_t( ba.a.x( ), ba.a.y( ) ), sdk::vec2_t( ba.b.x( ), ba.b.y( ) ), sdk::col_t::palette_t::white( 255 * factor ) );
				}
		}


		static auto size = sdk::vec2_t( 35.0f, 5.0f );

		csgo::g_render->draw_rect_filled( screen_origin.x( ) - size.x( ) * 0.5f,
			screen_origin.y( ) - size.y( ) * 0.5f - 1.0f, size.x( ), size.y( ), sdk::col_t( 37, 37, 37, 255 ), 0 );
		csgo::g_render->draw_rect_filled( screen_origin.x( ) - size.x( ) * 0.5f + 2.0f,
			screen_origin.y( ) - size.y( ) * 0.5f, ( size.x( ) - 4.0f ) * factor, size.y( ) - 2.0f, sdk::col_t( 255, 255, 255, 255 ), 0 );

		csgo::g_render->text( ( MilfStr( "MOLLY" ) ), sdk::vec2_t( screen_origin.x( ), screen_origin.y( ) - size.y( ) * 0.5f + 12.0f ),
			sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );
	}

	void c_visuals::grenade_projectiles( valve::base_entity_t* const entity ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_grenade_projectiles )
			return;

		auto client_class = entity->networkable( )->client_class( );

		auto model = entity->renderable( )->model( );

		if ( !model )
			return;

		if ( static_cast < valve::e_class_id > ( client_class->m_class_id ) == valve::e_class_id::base_cs_grenade_projectile
			|| static_cast < valve::e_class_id > ( client_class->m_class_id ) == valve::e_class_id::molotov_projectile )
		{
			auto name = ( std::string_view ) model->m_path;
			auto grenade_origin = entity->abs_origin( );
			auto grenade_position = sdk::vec3_t( );

			if ( !csgo::g_render->world_to_screen( grenade_origin, grenade_position ) )
				return;

			std::string grenade_name, grenade_icon;

			if ( name.find( "flashbang" ) != std::string::npos )
			{
				grenade_name = "FLASHBANG";
				grenade_icon = "i";
			}
			else if ( name.find( "smokegrenade" ) != std::string::npos )
			{
				grenade_name = "SMOKE";
				grenade_icon = "k";
			}
			else if ( name.find( "incendiarygrenade" ) != std::string::npos )
			{
				grenade_name = "INCENDIARY";
				grenade_icon = "n";
			}
			else if ( name.find( "molotov" ) != std::string::npos )
			{
				grenade_name = "MOLOTOV";
				grenade_icon = "l";
			}
			else if ( name.find( "fraggrenade" ) != std::string::npos )
			{
				grenade_name = "HE GRENADE";
				grenade_icon = "j";
			}
			else
				return;

			csgo::g_render->text( grenade_name.c_str( ), sdk::vec2_t( grenade_position.x( ), grenade_position.y( ) ),
				sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );
		}
	}

	void c_visuals::smoke_timer( valve::base_entity_t* const entity ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_smoke_timer )
			return;

		auto smoke = reinterpret_cast< valve::smoke_t* >( entity );

		if ( !smoke->smoke_effect_tick_begin( ) || !smoke->did_smoke_effect( ) )
			return;

		auto origin = smoke->abs_origin( );

		sdk::vec3_t screen_origin;

		if ( !csgo::g_render->world_to_screen( origin, screen_origin ) )
			return;

		auto spawn_time = valve::to_time( smoke->smoke_effect_tick_begin( ) );
		auto factor = ( spawn_time + valve::smoke_t::get_expiry_time( ) - valve::g_global_vars.get( )->m_cur_time ) /
			valve::smoke_t::get_expiry_time( );

		static auto size = sdk::vec2_t( 35.0f, 5.0f );

		csgo::g_render->draw_rect_filled( screen_origin.x( ) - size.x( ) * 0.5f, screen_origin.y( ) - size.y( ) * 0.5f - 1.0f, size.x( ), size.y( ),
			sdk::col_t( 37, 37, 37, 255 ), 0.f );
		csgo::g_render->draw_rect_filled( screen_origin.x( ) - size.x( ) * 0.5f + 2.0f, screen_origin.y( ) - size.y( ) * 0.5f, ( size.x( ) - 4.0f ) * factor, size.y( ) - 2.0f,
			sdk::col_t( 255, 255, 255, 255 ), 0.f );

		csgo::g_render->text( ( "SMOKE" ), sdk::vec2_t( screen_origin.x( ), screen_origin.y( ) - size.y( ) * 0.5f + 12.0f ),
			sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );
	}

	std::vector < valve::base_entity_t* > broken_entities{};

	void clear_broken_entities( ) { broken_entities.clear( ); }

	void mark_entity_as_broken( valve::base_entity_t* entity ) { broken_entities.emplace_back( entity ); }

	bool c_visuals::is_entity_broken( valve::base_entity_t* entity ) {
		return std::find( broken_entities.begin( ), broken_entities.end( ), entity ) != broken_entities.end( );
	}

	void c_visuals::on_create_move( const valve::user_cmd_t& cmd ) {
		m_grenade_trajectory = {};

		if ( !g_local_player->weapon( )
			|| !g_local_player->weapon_info( ) )
			return;

		sdk::vec3_t dir{};

		if ( 0
			&& g_local_player->weapon_info( )->m_type != static_cast < valve::e_weapon_type > ( 0 )
			&& g_local_player->weapon_info( )->m_type < static_cast < valve::e_weapon_type > ( 7 ) ) {
			sdk::ang_vecs( cmd.m_view_angles, &dir, nullptr, nullptr );

		}

		auto base_grenade = ( valve::base_grenade_t* ) ( g_local_player->weapon( ) );

		if ( g_local_player->weapon_info( )->m_type != static_cast < valve::e_weapon_type > ( 9 )
			|| ( !base_grenade->pin_pulled( ) && base_grenade->throw_time( ) == 0.f ) )
			return;

		m_grenade_trajectory.m_owner = g_local_player->self( );
		m_grenade_trajectory.m_index = g_local_player->weapon( )->item_index( );

		auto view_angles = cmd.m_view_angles;

		if ( view_angles.x( ) < -90.f )
			view_angles.x( ) += 360.f;
		else if ( view_angles.x( ) > 90.f )
			view_angles.x( ) -= 360.f;

		view_angles.x( ) -= ( 90.f - std::fabsf( view_angles.x( ) ) ) * 10.f / 90.f;

		sdk::ang_vecs( view_angles, &dir, nullptr, nullptr );

		const auto throw_strength = std::clamp( base_grenade->throw_strength( ), 0.f, 1.f );

		auto src = g_local_player->shoot_pos( );

		src.z( ) += throw_strength * 12.f - 12.f;

		valve::trace_t trace{};
		valve::trace_filter_simple_t trace_filter{ g_local_player->self( ), 0 };
		valve::g_engine_trace->trace_ray_(
			{ src, src + dir * 22.f, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f } },
			_MASK_SOLID | _CONTENTS_CURRENT_90,
			reinterpret_cast< valve::base_trace_filter_t* >( &trace_filter ), &trace
		);

		const auto velocity = std::clamp(
			g_local_player->weapon_info( )->m_throw_velocity * 0.9f, 15.f, 750.f
		) * ( throw_strength * 0.7f + 0.3f );

		m_grenade_trajectory.predict(
			trace.m_end - dir * 6.f,
			dir * velocity + g_local_player->self( )->velocity( ) * 1.25f,
			valve::g_global_vars.get( )->m_cur_time, 0
		);
	}

	void c_visuals::handle_warning_pred( valve::base_entity_t* const entity, const valve::e_class_id class_id ) {
		if ( !valve::g_engine.get( )->in_game( )
			|| !g_local_player->self( ) ) {
			return m_throwed_grenades.clear( );
		}
		if ( class_id == valve::e_class_id::molotov_projectile || class_id == valve::e_class_id::base_cs_grenade_projectile ) {

			bool can_do = true;
			bool no_dmg = false;

			if ( class_id == valve::e_class_id::base_cs_grenade_projectile ) {
				const auto studio_model = entity->renderable( )->model( );
				if ( !studio_model
					|| std::string_view( studio_model->m_path ).find( "fraggrenade" ) == std::string::npos )
					can_do = false;
			}


			if ( !entity->networkable( )->dormant( ) && can_do ) {
				const auto handle = ( static_cast < valve::cs_player_t* > ( entity ) )->ref_handle( );
				const auto thrower = static_cast< valve::cs_player_t* >( valve::g_entity_list.get( )->get_entity( entity->thrower_handle( ) ) );

				if ( thrower
					&& thrower->team( ) == g_local_player->self( )->team( )
					&& thrower != g_local_player->self( ) )
					no_dmg = true;

				if ( entity->explode_effect_tick_begin( ) & 0x020 )
					m_throwed_grenades.erase( handle );
				else {
					if ( m_throwed_grenades.find( handle ) == m_throwed_grenades.end( ) ) {
						m_throwed_grenades.emplace(
							std::piecewise_construct,
							std::forward_as_tuple( handle ),
							std::forward_as_tuple(
								static_cast< valve::cs_player_t* >( valve::g_entity_list.get( )->get_entity( entity->thrower_handle( ) ) ),
								class_id == valve::e_class_id::molotov_projectile ? valve::e_item_index::molotov : valve::e_item_index::he_grenade,
								entity->origin( ), entity->velocity( ),
								entity->grenade_spawn_time( ), valve::to_ticks( entity->sim_time( ) - entity->grenade_spawn_time( ) )
							)
						);
					}

					if ( !add_grenade_simulation( m_throwed_grenades.at( handle ), true, no_dmg ) )
						m_throwed_grenades.erase( handle );
				}
			}
		}
	}

	void c_visuals::grenade_simulation_t::predict( const sdk::vec3_t& origin, const sdk::vec3_t& velocity, const float throw_time, const int offset ) {
		m_origin = origin;
		m_velocity = velocity;
		m_collision_group = 13;
		clear_broken_entities( );

		const auto tick = valve::to_ticks( 1.f / 30.f );

		m_last_update_tick = -tick;
		static auto molotov_throw_detonate_time = valve::g_cvar->find_var( ( "molotov_throw_detonate_time" ) );
		switch ( m_index ) {
		case valve::e_item_index::smoke_grenade: m_next_think_tick = valve::to_ticks( 1.5f ); break;
		case valve::e_item_index::decoy: m_next_think_tick = valve::to_ticks( 2.f ); break;
		case valve::e_item_index::flashbang:
		case valve::e_item_index::he_grenade:
			m_detonate_time = 1.5f;
			m_next_think_tick = valve::to_ticks( 0.02f );

			break;
		case valve::e_item_index::molotov:
		case valve::e_item_index::inc_grenade:
			m_detonate_time = molotov_throw_detonate_time->get_float( );
			m_next_think_tick = valve::to_ticks( 0.02f );

			break;
		}

		m_source_time = throw_time;

		const auto max_sim_amt = valve::to_ticks( 60.f );
		for ( ; m_tick < max_sim_amt; ++m_tick ) {
			if ( m_next_think_tick <= m_tick )
				think( );

			if ( m_tick < offset )
				continue;

			if ( physics_simulate( ) )
				break;

			if ( ( m_last_update_tick + tick ) > m_tick )
				continue;

			update_path( false );
		}

		if ( ( m_last_update_tick + tick ) <= m_tick )
			update_path( false );

		m_expire_time = throw_time + valve::to_time( m_tick );
	}

	bool c_visuals::grenade_simulation_t::physics_simulate( ) {
		if ( m_detonated )
			return true;
		static auto sv_gravity = valve::g_cvar->find_var( ( "sv_gravity" ) );
		const auto new_velocity_z = m_velocity.z( ) - ( sv_gravity->get_float( ) * 0.4f ) * valve::g_global_vars.get( )->m_interval_per_tick;

		const auto move = sdk::vec3_t(
			m_velocity.x( ) * valve::g_global_vars.get( )->m_interval_per_tick,
			m_velocity.y( ) * valve::g_global_vars.get( )->m_interval_per_tick,
			( m_velocity.z( ) + new_velocity_z ) / 2.f * valve::g_global_vars.get( )->m_interval_per_tick
		);

		m_velocity.z( ) = new_velocity_z;

		valve::trace_t trace{};

		physics_push_entity( move, trace );

		if ( m_detonated )
			return true;

		if ( trace.m_frac != 1.f ) {
			update_path( true );

			perform_fly_collision_resolution( trace );
		}

		return false;
	}

	void c_visuals::grenade_simulation_t::perform_fly_collision_resolution( valve::trace_t& trace ) {
		auto surface_elasticity = 1.f;

		if ( trace.m_entity ) {
			if ( hacks::g_auto_wall->breakable( trace.m_entity ) ) {
				m_last_breakable = trace.m_entity;

				mark_entity_as_broken( trace.m_entity );

				m_velocity *= 0.4f;

				return;
			}

			const auto is_player = trace.m_entity->is_player( );
			if ( is_player )
				surface_elasticity = 0.3f;

			if ( trace.m_entity->networkable( )->index( ) ) {
				if ( is_player
					&& m_last_hit_entity == trace.m_entity ) {
					m_collision_group = 1;

					return;
				}

				m_last_hit_entity = trace.m_entity;
			}
		}

		sdk::vec3_t velocity{};

		const auto back_off = m_velocity.dot( trace.m_plane.m_normal ) * 2.f;

		for ( std::size_t i{}; i < 3u; ++i ) {
			const auto change = trace.m_plane.m_normal.at( i ) * back_off;

			velocity.at( i ) = m_velocity.at( i ) - change;

			if ( std::fabsf( velocity.at( i ) ) >= 1.f )
				continue;

			velocity.at( i ) = 0.f;
		}

		velocity *= std::clamp( surface_elasticity * 0.45f, 0.f, 0.9f );

		if ( trace.m_plane.m_normal.z( ) > 0.7f ) {
			const auto speed_sqr = velocity.length_sqr( 3u );
			if ( speed_sqr > 96000.f ) {
				const auto l = velocity.normalized( ).dot( trace.m_plane.m_normal );
				if ( l > 0.5f )
					velocity *= 1.f - l + 0.5f;
			}

			if ( speed_sqr < 400.f )
				m_velocity = {};
			else {
				m_velocity = velocity;

				physics_push_entity( velocity * ( ( 1.f - trace.m_frac ) * valve::g_global_vars.get( )->m_interval_per_tick ), trace );
			}
		}
		else {
			m_velocity = velocity;

			physics_push_entity( velocity * ( ( 1.f - trace.m_frac ) * valve::g_global_vars.get( )->m_interval_per_tick ), trace );
		}

		if ( m_bounces_count > 20 )
			return detonate( false );

		++m_bounces_count;
	}

	void c_visuals::grenade_simulation_t::think( ) {
		switch ( m_index ) {
		case valve::e_item_index::smoke_grenade:
			if ( m_velocity.length_sqr( ) <= 0.01f )
				detonate( false );

			break;
		case valve::e_item_index::decoy:
			if ( m_velocity.length_sqr( ) <= 0.04f )
				detonate( false );

			break;
		case valve::e_item_index::flashbang:
		case valve::e_item_index::he_grenade:
		case valve::e_item_index::molotov:
		case valve::e_item_index::inc_grenade:
			if ( valve::to_time( m_tick ) > m_detonate_time )
				detonate( false );

			break;
		}

		m_next_think_tick = m_tick + valve::to_ticks( 0.2f );
	}

	void c_visuals::grenade_simulation_t::detonate( const bool bounced ) {
		m_detonated = true;

		update_path( bounced );
	}

	void c_visuals::grenade_simulation_t::update_path( const bool bounced ) {
		m_last_update_tick = m_tick;

		if ( m_path.size( ) < 200 ) {
			m_path.emplace_back( m_origin, bounced );
		}
	}


	void c_visuals::grenade_simulation_t::physics_trace_entity(
		const sdk::vec3_t& src, const sdk::vec3_t& dst,
		const std::uint32_t mask, valve::trace_t& trace
	) {
		valve::trace_filter_skip_two_entities_t trace_filter{ m_owner, m_last_breakable, m_collision_group };

		trace_hull( src, dst, trace, g_local_player->self( ), 0x200400B, m_collision_group );

		if ( trace.m_start_solid
			&& ( trace.m_contents & valve::contents_current_90 ) ) {
			trace.clear( );

			trace_hull( src, dst, trace, g_local_player->self( ), mask & ~0x80000, m_collision_group );
		}

		if ( !trace.hit( )
			|| !trace.m_entity
			|| !trace.m_entity->is_player( ) )
			return;

		trace.clear( );

		trace_line( src, dst, trace, g_local_player->self( ), mask, m_collision_group );
	}

	void c_visuals::grenade_simulation_t::physics_push_entity( const sdk::vec3_t& push, valve::trace_t& trace ) {
		physics_trace_entity( m_origin, m_origin + push,
			m_collision_group == 1
			? ( _MASK_SOLID | _CONTENTS_CURRENT_90 ) & ~_CONTENTS_MONSTER
			: _MASK_SOLID | _CONTENTS_OPAQUE | _CONTENTS_IGNORE_NODRAW_OPAQUE | _CONTENTS_CURRENT_90 | _CONTENTS_HITBOX,
			trace
		);

		valve::trace_filter_skip_two_entities_t trace_filter{ m_owner, m_last_breakable, m_collision_group };

		if ( trace.m_start_solid ) {
			m_collision_group = 3;
			trace_line( m_origin - push, m_origin + push, trace, g_local_player->self( ), ( _MASK_SOLID | _CONTENTS_CURRENT_90 ) & ~_CONTENTS_MONSTER, m_collision_group );
		}

		if ( trace.m_frac != 0.f )
			m_origin = trace.m_end;

		static auto weapon_molotov_maxdetonateslope = valve::g_cvar->find_var( ( "weapon_molotov_maxdetonateslope" ) );

		if ( !trace.m_entity )
			return;

		if ( trace.m_entity->is_player( )
			|| m_index != valve::e_item_index::molotov && m_index != valve::e_item_index::inc_grenade
			|| trace.m_plane.m_normal.z( ) < std::cos( sdk::to_rad( weapon_molotov_maxdetonateslope->get_float( ) ) ) )
			return;

		detonate( true );
	}

	RECT get_bbox( valve::base_entity_t* ent ) {
		RECT rect{ };

		if ( ( ( valve::cs_player_t* ) ent )->is_player( ) ) {
			sdk::vec3_t origin, mins, maxs;
			sdk::vec3_t bottom, top;
			if ( ( ( valve::cs_player_t* ) ent )->alive( ) )
				origin = ent->networkable( )->dormant( ) ? ent->abs_origin( ) : g_lag_comp->entry( ent->networkable( )->index( ) - 1 ).m_render_origin;
			else
				origin = hacks::g_visuals->m_alive_origin.at( ent->networkable( )->index( ) );

			const auto on_screen = csgo::g_render->world_to_screen( origin, bottom );

			origin.z( ) += ent->obb_max( ).z( );

			if ( !csgo::g_render->world_to_screen( origin, top )
				&& !on_screen )
				return RECT{};

			float x, y, w, h;
			h = bottom.y( ) - top.y( );
			w = h / 2.f;
			x = bottom.x( ) - ( w / 2.f );
			y = bottom.y( ) - h;

			return RECT{ long( x ), long( y ), long( x + w ), long( y + h ) };
		}
		else
		{
			auto min = ent->obb_min( );
			auto max = ent->obb_max( );

			const sdk::mat3x4_t& trans = ent->rgfl( );
			sdk::vec3_t points[ ] = {
				sdk::vec3_t( min.x( ), min.y( ), min.z( ) ),
				sdk::vec3_t( min.x( ), max.y( ), min.z( ) ),
				sdk::vec3_t( max.x( ), max.y( ), min.z( ) ),
				sdk::vec3_t( max.x( ), min.y( ), min.z( ) ),
				sdk::vec3_t( max.x( ), max.y( ), max.z( ) ),
				sdk::vec3_t( min.x( ), max.y( ), max.z( ) ),
				sdk::vec3_t( min.x( ), min.y( ), max.z( ) ),
				sdk::vec3_t( max.x( ), min.y( ), max.z( ) )
			};

			sdk::vec3_t points_transformed[ 8 ];
			for ( int i = 0; i < 8; i++ )
				points_transformed[ i ] = points[ i ].transform( trans );

			sdk::vec3_t screen_points[ 8 ] = { };
			for ( int i = 0; i < 8; i++ )
				if ( !csgo::g_render->world_to_screen( points_transformed[ i ], screen_points[ i ] ) )
					return rect;

			auto left = screen_points[ 0 ].x( );
			auto top = screen_points[ 0 ].y( );
			auto right = screen_points[ 0 ].x( );
			auto bottom = screen_points[ 0 ].y( );

			for ( int i = 1; i < 8; i++ )
			{
				if ( left > screen_points[ i ].x( ) )
					left = screen_points[ i ].x( );
				if ( top < screen_points[ i ].y( ) )
					top = screen_points[ i ].y( );
				if ( right < screen_points[ i ].x( ) )
					right = screen_points[ i ].x( );
				if ( bottom > screen_points[ i ].y( ) )
					bottom = screen_points[ i ].y( );
			}

			return RECT{ ( long ) ( left ), ( long ) ( bottom ), ( long ) ( right ), ( long ) ( top ) };
		}
	}

	void c_visuals::handle_player_drawings( ) {
		g_dormant_esp->start( );

		for ( int i = 0u; i < valve::g_global_vars.get( )->m_max_clients; ++i )
		{
			auto player = ( valve::cs_player_t* ) valve::g_entity_list.get( )->get_entity( i );

			if ( !g_local_player->self( )
				|| !player
				|| player == g_local_player->self( )
				|| player->team( ) == g_local_player->self( )->team( )
				|| !player->is_player( ) )
				continue;

			bool alive_check{};

			if ( !player->alive( ) ) {
				m_dormant_data[ player->networkable( )->index( ) ].m_alpha -= 255.f / 1.f * valve::g_global_vars.get( )->m_frame_time;
				m_dormant_data[ player->networkable( )->index( ) ].m_alpha = std::clamp( m_dormant_data[ player->networkable( )->index( ) ].m_alpha, 0.f, 255.f );
				alive_check = true;
			}
			else {
				m_alive_origin.at( player->networkable( )->index( ) ) = g_lag_comp->entry( player->networkable( )->index( ) - 1 ).m_render_origin;
				using compute_hitbox_fn = bool( __thiscall* )( void*, sdk::vec3_t*, sdk::vec3_t* );
				static auto compute_hitbox = reinterpret_cast < compute_hitbox_fn > ( g_ctx->offsets( ).m_addresses.m_compute_hitbox_surround_box );

				if ( !player->networkable( )->dormant( ) )
					compute_hitbox( player, &m_alive_mins.at( player->networkable( )->index( ) ), &m_alive_maxs.at( player->networkable( )->index( ) ) );
			}

			if ( !m_dormant_data[ player->networkable( )->index( ) ].m_alpha
				&& alive_check )
				continue;

			if ( !alive_check ) {
				if ( player->networkable( )->dormant( ) ) {
					if ( !m_dormant_data.at( player->networkable( )->index( ) ).m_use_shared
						&& valve::g_global_vars.get( )->m_real_time - m_dormant_data.at( player->networkable( )->index( ) ).m_last_shared_time > 4.f )
						g_dormant_esp->adjust_sound( player );

					if ( !m_dormant_data.at( player->networkable( )->index( ) ).m_use_shared
						&& valve::g_global_vars.get( )->m_real_time - m_dormant_data.at( player->networkable( )->index( ) ).m_last_shared_time > 4.f ) {
						if ( m_dormant_data[ player->networkable( )->index( ) ].m_alpha < 160.f )
						{
							m_dormant_data[ player->networkable( )->index( ) ].m_alpha -= 255.f / 16.f * valve::g_global_vars.get( )->m_frame_time;
							m_dormant_data[ player->networkable( )->index( ) ].m_alpha = std::clamp( m_dormant_data[ player->networkable( )->index( ) ].m_alpha, 0.f, 160.f );
						}
						else
						{
							m_dormant_data[ player->networkable( )->index( ) ].m_alpha -= 255.f / 1.f * valve::g_global_vars.get( )->m_frame_time;
						}
					}
					else {
						m_dormant_data[ player->networkable( )->index( ) ].m_alpha = 190.f;
					}

					if ( player->weapon( ) ) {
						if ( m_dormant_data.at( i ).m_weapon_id > 0 )
							player->weapon( )->item_index( ) = static_cast < valve::e_item_index > ( m_dormant_data.at( i ).m_weapon_id );

						if ( player->weapon( )->info( ) )
							if ( m_dormant_data.at( i ).m_weapon_type > -1 )
								player->weapon( )->info( )->m_type = static_cast < valve::e_weapon_type > ( m_dormant_data.at( i ).m_weapon_type );
					}
				}
				else {
					g_dormant_esp->m_sound_players[ i ].reset( true, player->abs_origin( ), static_cast < int > ( player->flags( ) ) );
					m_dormant_data[ i ].m_origin = sdk::vec3_t( );
					m_dormant_data[ i ].m_receive_time = 0.f;
					m_dormant_data[ i ].m_alpha += 255.f / 0.68f * valve::g_global_vars.get( )->m_frame_time;
					m_dormant_data[ i ].m_alpha = std::clamp( m_dormant_data[ i ].m_alpha, 0.f, 255.f );
					m_dormant_data[ i ].m_weapon_id = 0;
					m_dormant_data[ i ].m_weapon_type = -1;
					m_dormant_data.at( i ).m_use_shared = false;
					m_dormant_data.at( player->networkable( )->index( ) ).m_last_shared_time = 0.f;
				}
			}
			int screen_x, screen_y;

			valve::g_engine.get( )->screen_size( screen_x, screen_y );

			sdk::vec3_t screen = sdk::vec3_t( );

			if ( g_local_player->self( )->alive( ) ) {
				if ( !csgo::g_render->world_to_screen( player->abs_origin( ), screen ) ) {
					oof_indicators( player );
					continue;
				}

				if ( screen.x( ) < 0 || screen.x( ) > screen_x || screen.y( ) < 0 || screen.y( ) > screen_y ) {
					oof_indicators( player );
					continue;
				}
			}

			auto rect = get_bbox( player );

			draw_name( player, rect );
			draw_health( player, rect );
			draw_box( player, rect );
			draw_wpn( player, rect );
			draw_ammo( player, rect );
			draw_flags( player, rect );
		}
	}

	void c_dormant_esp::start( )
	{
		m_cur_sound_list.remove_all( );
		valve::g_engine_sound->get_act_sounds( m_cur_sound_list );

		if ( !m_cur_sound_list.m_size )
			return;

		for ( auto i = 0; i < m_cur_sound_list.m_size; i++ )
		{
			auto& sound = m_cur_sound_list.at( i );

			if ( sound.m_sound_src < 1 || sound.m_sound_src > 64 )
				continue;

			if ( sound.m_origin->x( ) == 0.f && sound.m_origin->y( ) == 0.f && sound.m_origin->z( ) == 0.f )
				continue;

			if ( !valid_sound( sound ) )
				continue;

			auto player = static_cast< valve::cs_player_t* >( valve::g_entity_list.get( )->get_entity( sound.m_sound_src ) );


			if ( !player || !player->alive( ) || player->friendly( g_local_player->self( ) ) ||
				player == g_local_player->self( ) )
				continue;

			setup_adjust( player, sound );
			m_sound_players[ sound.m_sound_src ].override( sound );
		}

		m_sound_buffer = m_cur_sound_list;
	}

	void c_dormant_esp::setup_adjust( valve::cs_player_t* player, valve::snd_info_t& sound )
	{
		sdk::vec3_t src3D, dst3D;
		valve::trace_t tr;
		valve::trace_filter_simple_t filter;

		src3D = *sound.m_origin + sdk::vec3_t( 0.0f, 0.0f, 1.0f );
		dst3D = src3D - sdk::vec3_t( 0.0f, 0.0f, 100.0f );

		filter.m_ignore_entity = player;
		valve::ray_t ray = { src3D, dst3D };

		valve::g_engine_trace->trace_ray( ray, valve::e_mask::player_solid, reinterpret_cast < valve::base_trace_filter_t* > ( &filter ), &tr );

		if ( tr.m_all_solid )
			m_sound_players[ sound.m_sound_src ].m_receive_time = -1;

		*sound.m_origin = tr.m_frac <= 0.97f ? tr.m_end : *sound.m_origin;
		m_sound_players[ sound.m_sound_src ].m_flags = static_cast < int > ( player->flags( ) );
		m_sound_players[ sound.m_sound_src ].m_flags |= ( tr.m_frac < 0.50f ? valve::e_ent_flags::ducking : static_cast < valve::e_ent_flags > ( 0 ) ) | ( tr.m_frac < 1.0f ? valve::e_ent_flags::on_ground : static_cast < valve::e_ent_flags > ( 0 ) );
		m_sound_players[ sound.m_sound_src ].m_flags &= ( tr.m_frac >= 0.50f ? ~valve::e_ent_flags::ducking : 0 ) | ( tr.m_frac >= 1.0f ? ~valve::e_ent_flags::on_ground : 0 );
	}

	bool c_dormant_esp::adjust_sound( valve::cs_player_t* entity )
	{
		auto i = entity->networkable( )->index( );
		auto sound_player = m_sound_players[ i ];

		auto expired = false;

		if ( fabs( valve::g_global_vars.get( )->m_real_time - sound_player.m_receive_time ) > 10.0f )
			expired = true;

		//entity->spotted( ) = true;
		entity->flags( ) = ( valve::e_ent_flags ) sound_player.m_flags;
		entity->set_abs_origin( sound_player.m_origin );

		return !expired;
	}

	bool c_dormant_esp::valid_sound( valve::snd_info_t& sound )
	{
		for ( auto i = 0; i < m_sound_buffer.m_size; i++ )
			if ( m_sound_buffer.at( i ).m_guid == sound.m_guid )
				return false;

		return true;
	}

	void c_visuals::draw_shot_mdl( ) {
		if ( !valve::g_engine.get( )->in_game( ) )
			return m_shot_mdls.clear( );

		const auto& cfg = g_menu->main( ).m_models_cfg.get( );

		if ( !cfg.m_shot
			|| m_shot_mdls.empty( ) )
			return;

		const auto context = valve::g_material_system->render_context( );
		if ( !context )
			return;

		m_rendering_shot_mdl = true;

		for ( auto i = m_shot_mdls.begin( ); i != m_shot_mdls.end( ); ) {
			const auto delta = ( i->m_time + 2 ) - valve::g_global_vars.get( )->m_real_time;
			valve::player_info_t player_info_data;
			const auto player_info = valve::g_engine.get( )->get_player_info( i->m_player_index, &player_info_data );

			if ( delta <= 0.f || !player_info ) {
				i = m_shot_mdls.erase( i );

				continue;
			}

			sdk::col_t clr = sdk::col_t( cfg.m_shot_clr[ 0 ] * 255.f, cfg.m_shot_clr[ 1 ] * 255.f,
				cfg.m_shot_clr[ 2 ] * 255.f, cfg.m_shot_clr[ 3 ] * 255.f * ( delta / 2 ) );

			g_models->set_clr( clr, cfg.m_shot_type, true );

			hooks::o_draw_mdl_exec( valve::g_mdl_render, *context, i->m_state, i->m_info, i->m_bones.data( ) );

			if ( cfg.m_shot_outline ) {
				const auto outline_clr = sdk::col_t( cfg.m_shot_outline_clr[ 0 ] * 255.f, cfg.m_shot_outline_clr[ 1 ] * 255.f,
					cfg.m_shot_outline_clr[ 2 ] * 255.f, cfg.m_shot_outline_clr[ 3 ] * 255.f * ( delta / 2 ) );

				g_models->set_clr( outline_clr, 4, true );

				hooks::o_draw_mdl_exec( valve::g_mdl_render, *context, i->m_state, i->m_info, i->m_bones.data( ) );
			}

			valve::g_studio_render->forced_material_override( nullptr );

			i = std::next( i );
		}

		m_rendering_shot_mdl = false;
	}


	void c_visuals::add_shot_mdl( valve::cs_player_t* player, const sdk::mat3x4_t* bones ) {
		const auto model = player->renderable( )->model( );
		if ( !model )
			return;

		if ( !bones )
			return;

		if ( !player )
			return;

		const auto mdl_data = *( valve::studio_hdr_t** ) player->studio_hdr( );
		if ( !mdl_data )
			return;

		auto& shot_mdl = m_shot_mdls.emplace_back( );

		valve::player_info_t player_info;

		const auto who = valve::g_engine.get( )->get_player_info( player->networkable( )->index( ), &player_info );

		if ( !who )
			return;

		static int skin = valve::find_in_datamap( player->pred_desc_map( ), ( "m_nSkin" ) );
		static int body = valve::find_in_datamap( player->pred_desc_map( ), ( "m_nBody" ) );

		shot_mdl.m_player_index = player->networkable( )->index( );
		shot_mdl.m_time = valve::g_global_vars.get( )->m_real_time;
		shot_mdl.m_state.m_studio_hdr = mdl_data;
		shot_mdl.m_state.m_studio_hw_data = valve::g_mdl_cache->lookup_hw_data( model->m_studio );
		shot_mdl.m_state.m_renderable = ( std::uintptr_t )player->renderable( );
		shot_mdl.m_state.m_draw_flags = 0;
		shot_mdl.m_info.m_renderable = ( std::uintptr_t )player->renderable( );
		shot_mdl.m_info.m_model = model;
		shot_mdl.m_info.m_hitbox_set = player->hitbox_set_index( );
		shot_mdl.m_info.m_skin = *( int* ) ( uintptr_t( player ) + skin );
		shot_mdl.m_info.m_body = *( int* ) ( uintptr_t( player ) + body );
		shot_mdl.m_info.m_index = player->networkable( )->index( );
		shot_mdl.m_info.m_origin = player->origin( );
		shot_mdl.m_info.m_angles.y( ) = player->anim_state( )->m_foot_yaw;

		shot_mdl.m_info.m_instance = player->renderable( )->mdl_instance( );
		shot_mdl.m_info.m_flags = 1;

		std::memcpy( shot_mdl.m_bones.data( ), bones, sizeof( sdk::mat3x4_t ) * player->bone_cache( ).m_size );

		g_ctx->offsets( ).m_addresses.m_angle_matrix( shot_mdl.m_info.m_angles, shot_mdl.m_world_matrix );

		shot_mdl.m_world_matrix[ 0 ][ 3 ] = player->origin( ).x( );
		shot_mdl.m_world_matrix[ 1 ][ 3 ] = player->origin( ).y( );
		shot_mdl.m_world_matrix[ 2 ][ 3 ] = player->origin( ).z( );

		shot_mdl.m_info.m_model_to_world = shot_mdl.m_state.m_model_to_world = &shot_mdl.m_world_matrix;
	}

	void c_visuals::shared_t::send_net_data( valve::cs_player_t* const player ) {
		valve::player_info_t info{};

		const bool exists = valve::g_engine.get( )->get_player_info( player->networkable( )->index( ), &info );

		if ( !exists )
			return;

		if ( info.m_fake_player )
			return;

		valve::cclc_msg_data_legacy_t client_msg{};

		memset( &client_msg, 0, sizeof( client_msg ) );

		const auto func = ( std::uint32_t( __fastcall* )( void*, void* ) )g_ctx->offsets( ).m_addresses.m_voice_msg_ctor;

		func( ( void* ) &client_msg, nullptr );

		cheat_data_t* ptr = ( cheat_data_t* ) &client_msg.m_xuid_low;

		ptr->m_unique_key = 228;
		ptr->m_player_idx = static_cast < std::uint8_t > ( player->networkable( )->index( ) );
		ptr->m_x = static_cast < std::int16_t > ( player->origin( ).x( ) );
		ptr->m_y = static_cast < std::int16_t > ( player->origin( ).y( ) );
		ptr->m_z = static_cast < std::int16_t > ( player->origin( ).z( ) );

		client_msg.m_flags = 63;
		client_msg.m_format = 0;

		valve::g_client_state->m_net_chan->send_net_msg( &client_msg );
	}

	void c_visuals::draw_health( valve::cs_player_t* player, RECT& rect ) {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_draw_health )
			return;

		float box_height = static_cast< float >( rect.bottom - rect.top );

		int red = 150;
		int green = 0xFF;
		int blue = 0x50;

		if ( player->health( ) >= 27 )
		{
			if ( player->health( ) < 57 )
			{
				red = 0xD7;
				green = 0xC8;
				blue = 0x50;
			}
		}
		else
		{
			red = 0xFF;
			green = 0x32;
			blue = 0x50;
		}

		static float last_hp[ 65 ];

		if ( last_hp[ player->networkable( )->index( ) ] > player->health( ) )
			last_hp[ player->networkable( )->index( ) ] -= ( 255.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
		else
			last_hp[ player->networkable( )->index( ) ] = player->health( );

		sdk::col_t color = sdk::col_t( red, green, blue, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha );
		auto bg_alpha = std::clamp( ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha, 0, 140 );
		float colored_bar_height = ( ( box_height * std::min( last_hp[ player->networkable( )->index( ) ], 100.f ) ) / 100.0f );
		float colored_max_bar_height = ( ( box_height * 100.0f ) / 100.0f );

		csgo::g_render->rect_filled( sdk::vec2_t( rect.left - 5.0f, rect.top - 1 ), sdk::vec2_t( rect.left - 2.0f, rect.top + colored_max_bar_height + 1 ), sdk::col_t( 0.0f, 0.0f, 0.0f, ( float ) bg_alpha ) );
		csgo::g_render->rect_filled( sdk::vec2_t( rect.left - 4.0f, rect.top + ( colored_max_bar_height - colored_bar_height ) ), sdk::vec2_t( rect.left - 3.0f, rect.top + colored_max_bar_height ), color );

		if ( player->health( ) < 100 )
		{
			csgo::g_render->text( std::to_string( player->health( ) ), sdk::vec2_t( rect.left - 11.f,
				rect.top - 2.0f ), sdk::col_t( 255, 255, 255, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );
		}
	}

	void c_visuals::draw_name( valve::cs_player_t* player, RECT& rect ) {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_draw_name )
			return;

		valve::player_info_t info;

		valve::g_engine.get( )->get_player_info( player->networkable( )->index( ), &info );

		std::string name = info.m_name;

		if ( name.length( ) > 36 )
		{
			name.erase( 36, name.length( ) - 36 );
			name.append( ( "..." ) );
		}

		auto width = abs( rect.right - rect.left );

		auto size = csgo::g_render->m_fonts.m_04b03_8->CalcTextSizeA( 9.f, FLT_MAX, NULL, name.c_str( ) );

		csgo::g_render->text( name, sdk::vec2_t( rect.left + width * 0.5f, rect.top - size.y - 4 ), sdk::col_t( 255, 255, 255, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), csgo::g_render->m_fonts.m_tahoma, true, true, false, true );
	}

	void c_visuals::oof_indicators( valve::cs_player_t* player ) {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_oof_indicator )
			return;

		if ( !player->weapon( ) )
			return;

		float width = 5.f;
		sdk::qang_t viewangles = valve::g_engine.get( )->view_angles( );

		auto angle = viewangles.y( ) - sdk::calc_ang( g_local_player->shoot_pos( ), player->abs_origin( ) ).y( ) - 90.f;

		int screen_x, screen_y;
		valve::g_engine.get( )->screen_size( screen_x, screen_y );

		csgo::g_render->arc( screen_x / 2, screen_y / 2, 256, angle - width, angle + width, sdk::col_t( 255, 255, 255, m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), 4.f );
		csgo::g_render->arc( screen_x / 2, screen_y / 2, 250, angle - width, angle + width, sdk::col_t( 255, 255, 255, m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), 1.5f );
	}


	void c_visuals::draw_wpn( valve::cs_player_t* player, RECT& rect ) {
		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );
		if ( !cfg.m_wpn_icon && !cfg.m_wpn_text )
			return;

		if ( !player->weapon( ) || !player->weapon( )->info( ) )
			return;

		int offset{ 0 };

		bool has_something{};

		if ( cfg.m_wpn_ammo
			&& m_change_offset_due_to_ammo.at( player->networkable( )->index( ) ) )
			offset += 6;

		has_something = offset != -0;

		if ( !has_something )
			offset = -1;
		else
			offset -= 2;

		if ( cfg.m_wpn_text ) {
			csgo::g_render->text( get_weapon_name( player->weapon( ) ), sdk::vec2_t( rect.left + ( abs( rect.right - rect.left ) * 0.5f ), rect.bottom + offset + 3 ), sdk::col_t( 255, 255, 255, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), csgo::g_render->m_fonts.m_04b03_8, true, true, false );

			if ( has_something )
				offset += 8;
			else
				offset += 8;
		}

		if ( cfg.m_wpn_icon )
			csgo::g_render->text( get_weapon_icon( player->weapon( ) ), sdk::vec2_t( rect.left + ( abs( rect.right - rect.left ) * 0.5f ), rect.bottom + offset + 2 ), sdk::col_t( 255, 255, 255, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha ), csgo::g_render->m_fonts.m_icons.m_icon_font, true, true, false, true );
	}

	void c_visuals::draw_ammo( valve::cs_player_t* player, RECT& rect ) {
		auto wpn = player->weapon( );

		if ( !wpn )
			return;

		auto wpn_data = wpn->info( );

		if ( !wpn_data )
			return;

		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

		m_change_offset_due_to_ammo.at( player->networkable( )->index( ) ) = false;

		if ( wpn->clip1( ) <= -1 )
			wpn->clip1( ) = 0;

		static float prev_ammo[ 65 ];

		if ( cfg.m_wpn_ammo
			&& wpn_data
			&& wpn_data->m_type != valve::e_weapon_type::knife
			&& wpn_data->m_type < valve::e_weapon_type::c4
			&& wpn_data->m_max_clip1 != -1 ) {
			if ( prev_ammo[ player->networkable( )->index( ) ] > wpn->clip1( ) )
				prev_ammo[ player->networkable( )->index( ) ] -= 0.01f;
			else {
				if ( prev_ammo[ player->networkable( )->index( ) ] != wpn->clip1( ) )
					prev_ammo[ player->networkable( )->index( ) ] = wpn->clip1( );
			}

			m_change_offset_due_to_ammo.at( player->networkable( )->index( ) ) = true;

			if ( prev_ammo[ player->networkable( )->index( ) ] < wpn->clip1( ) || prev_ammo[ player->networkable( )->index( ) ] > wpn_data->m_max_clip1 )
				prev_ammo[ player->networkable( )->index( ) ] = wpn->clip1( );

			float box_width = std::abs( rect.right - rect.left );
			float current_box_width = ( box_width * prev_ammo[ player->networkable( )->index( ) ] ) / wpn_data->m_max_clip1;

			if ( player->lookup_seq_act( player->anim_layers( ).at( 1 ).m_seq ) == 967 )
				current_box_width = box_width * player->anim_layers( ).at( 1 ).m_cycle;

			auto clr = sdk::col_t( 255, 255, 255, m_dormant_data[ player->networkable( )->index( ) ].m_alpha );

			auto magic_clr = player->networkable( )->dormant( ) ? sdk::col_t( 0.f, 0.f, 0.f, m_dormant_data.at( player->networkable( )->index( ) ).m_alpha ) : sdk::col_t( 0.0f, 0.0f, 0.0f, 100.0f );

			csgo::g_render->rect_filled( sdk::vec2_t( rect.right + 1, rect.bottom + 2 ), sdk::vec2_t( rect.left - 1, rect.bottom + 6 ), magic_clr );
			csgo::g_render->rect_filled( sdk::vec2_t( rect.left, rect.bottom + 3 ), sdk::vec2_t( rect.left + current_box_width, rect.bottom + 5 ), clr );

			csgo::g_render->text( std::to_string( wpn->clip1( ) ), sdk::vec2_t( rect.right + 3, rect.bottom ), sdk::col_t( 255, 255, 255, m_dormant_data.at( player->networkable( )->index( ) ).m_alpha ), csgo::g_render->m_fonts.m_04b03_8, true, false, false );
		}
	}

	void c_visuals::draw_flags( valve::cs_player_t* player, RECT& rect ) {
		if ( !player->weapon( ) )
			return;
		
		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

		if ( !cfg.m_draw_flags )
			return;

		int count{ 1 };

		std::vector < flags_data_t > flags_data{};

		std::string money_str{ std::to_string( player->money( ) ) };

		money_str += "$";

		if ( cfg.m_player_flags & 1 )
			flags_data.push_back( { money_str, 1.f, sdk::col_t( 150, 200, 60, 255 ) } );

		const auto red_clr = sdk::col_t( 163, 56, 56, 255 );

		{
			static float kevlar_add_anim[ 65 ] = {};

			auto kevlar = player->armor_val( ) > 0;
			auto helmet = player->has_helmet( );

			std::string text;

			if ( helmet && kevlar ) {
				kevlar_add_anim[ player->networkable( )->index( ) ] += ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
				text = ( "HK" );
			}
			else if ( kevlar ) {
				kevlar_add_anim[ player->networkable( )->index( ) ] += ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
				text = ( "K" );
			}
			else {
				text = ( "" );
				kevlar_add_anim[ player->networkable( )->index( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
			}

			kevlar_add_anim[ player->networkable( )->index( ) ] = std::clamp( kevlar_add_anim[ player->networkable( )->index( ) ], 0.f, 255.f );
			if ( cfg.m_player_flags & 2 )
				flags_data.push_back( { text, kevlar_add_anim[ player->networkable( )->index( ) ], sdk::col_t( 240, 240, 240, static_cast < int > ( kevlar_add_anim[ player->networkable( )->index( ) ] ) ) } );
		}

		{
			static float scoped_alpha_anim[ 65 ] = {};

			std::string scoped_str{};

			if ( player->scoped( ) ) {
				scoped_str = ( "ZOOM" );
				scoped_alpha_anim[ player->networkable( )->index( ) ] += ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
			}
			else {
				scoped_alpha_anim[ player->networkable( )->index( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
				scoped_str = ( "" );
			}

			scoped_alpha_anim[ player->networkable( )->index( ) ] = std::clamp( scoped_alpha_anim[ player->networkable( )->index( ) ], 0.f, 255.f );
			if ( cfg.m_player_flags & 4 )
				flags_data.push_back( { scoped_str, scoped_alpha_anim[ player->networkable( )->index( ) ], sdk::col_t( 0, 153, 204, static_cast < int > ( scoped_alpha_anim[ player->networkable( )->index( ) ] ) ) } );
		}

		{
			static float fd_alpha_anim[ 65 ] = {};

			std::string fd_str{};

			auto animstate = player->anim_state( );

			if ( animstate ) {

				auto fakeducking = [ & ]( ) -> bool {

					static auto stored_tick = 0;
					static int crouched_ticks[ 65 ];

					if ( animstate->m_duck_amount ) {
						if ( animstate->m_duck_amount < 0.9f && animstate->m_duck_amount > 0.5f ) {
							if ( stored_tick != valve::g_global_vars.get( )->m_tick_count ) {
								crouched_ticks[ player->networkable( )->index( ) ]++;
								stored_tick = valve::g_global_vars.get( )->m_tick_count;
							}

							return crouched_ticks[ player->networkable( )->index( ) ] > 16;
						}
						else
							crouched_ticks[ player->networkable( )->index( ) ] = 0;
					}

					return false;
				};

				if ( fakeducking( ) && player->flags( ) & valve::e_ent_flags::on_ground && !animstate->m_landing ) {
					fd_str = ( "FD" );
					fd_alpha_anim[ player->networkable( )->index( ) ] += ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
				}
				else {
					fd_alpha_anim[ player->networkable( )->index( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars.get( )->m_frame_time;
					fd_str = ( "" );
				}
			}

			fd_alpha_anim[ player->networkable( )->index( ) ] = std::clamp( fd_alpha_anim[ player->networkable( )->index( ) ], 0.f, 255.f );

			if ( cfg.m_player_flags & 8 )
				flags_data.push_back( { fd_str, fd_alpha_anim[ player->networkable( )->index( ) ], sdk::col_t( 212, 219, 206, static_cast < int > ( fd_alpha_anim[ player->networkable( )->index( ) ] ) ) } );
		}
		static float lc_alpha_anim[ 65 ];
		std::string lc_str{};


		lc_alpha_anim[ player->networkable( )->index( ) ] = std::clamp( lc_alpha_anim[ player->networkable( )->index( ) ], 0.f, 255.f );

		if ( cfg.m_player_flags & 16 )
			flags_data.push_back( { lc_str, lc_alpha_anim[ player->networkable( )->index( ) ],
				sdk::col_t( 255, 16, 16, static_cast < std::ptrdiff_t > ( lc_alpha_anim[ player->networkable( )->index( ) ] ) ) } );


		for ( auto& it : flags_data ) {

			sdk::col_t clr = player->networkable( )->dormant( ) ? sdk::col_t( it.m_clr.r( ), it.m_clr.g( ), it.m_clr.b( ), m_dormant_data.at( player->networkable( )->index( ) ).m_alpha ) : it.m_clr;

			if ( !it.m_alpha )
				continue;

			csgo::g_render->text( it.m_name, sdk::vec2_t( rect.right + 5, rect.top + 9 * count - 4 - 6 ), clr, csgo::g_render->m_fonts.m_04b03_8, true, false, false );

			count++;
		}
	}

	void c_visuals::draw_box( valve::cs_player_t* player, RECT& rect ) {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_draw_box )
			return;

		auto bg_alpha = std::clamp( ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha, 0, 150 );

		auto color = sdk::col_t( 255, 255, 255, ( int ) m_dormant_data[ player->networkable( )->index( ) ].m_alpha );
		csgo::g_render->rect( sdk::vec2_t( rect.left + 1, rect.top + 1 ), sdk::vec2_t( rect.right - 1, rect.bottom - 1 ), sdk::col_t( 10, 10, 10, bg_alpha ) );
		csgo::g_render->rect( sdk::vec2_t( rect.left - 1, rect.top - 1 ), sdk::vec2_t( rect.right + 1, rect.bottom + 1 ), sdk::col_t( 10, 10, 10, bg_alpha ) );
		csgo::g_render->rect( sdk::vec2_t( rect.left, rect.top ), sdk::vec2_t( rect.right, rect.bottom ), color );
	}

	void c_visuals::draw_hitmarkers( ) {
		if ( !g_local_player->self( )
			|| !g_local_player->self( )->alive( ) )
			return;

		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_hits_marker ) {
			return m_hit_markers.clear( );
		}

		if ( !m_hit_markers.empty( ) ) {
			auto it = m_hit_markers.begin( );

			while ( it != m_hit_markers.end( ) ) {
				auto& cur_it = *it;
				const auto life_time = valve::g_global_vars.get( )->m_cur_time - cur_it.m_spawn_time;

				sdk::vec3_t on_screen{};
				if ( csgo::g_render->world_to_screen( cur_it.m_pos, on_screen ) ) {
					constexpr auto k_size = 6;

					csgo::g_render->line(
						{ on_screen.x( ) - k_size, on_screen.y( ) - k_size },
						{ on_screen.x( ) - ( k_size / 2 ), on_screen.y( ) - ( k_size / 2 ) }, sdk::col_t( 255, 255, 255, 255 )
					);
					csgo::g_render->line(
						{ on_screen.x( ) - k_size, on_screen.y( ) + k_size },
						{ on_screen.x( ) - ( k_size / 2 ), on_screen.y( ) + ( k_size / 2 ) }, sdk::col_t( 255, 255, 255, 255 )
					);
					csgo::g_render->line(
						{ on_screen.x( ) + k_size, on_screen.y( ) + k_size },
						{ on_screen.x( ) + ( k_size / 2 ), on_screen.y( ) + ( k_size / 2 ) }, sdk::col_t( 255, 255, 255, 255 )
					);
					csgo::g_render->line(
						{ on_screen.x( ) + k_size, on_screen.y( ) - k_size },
						{ on_screen.x( ) + ( k_size / 2 ), on_screen.y( ) - ( k_size / 2 ) }, sdk::col_t( 255, 255, 255, 255 )
					);
				}

				if ( life_time > 4.f )
					it = m_hit_markers.erase( it );
				else
					it++;
			}
		}
	}

	void c_visuals::draw_enemy_bullet_tracers( ) {
		if ( !g_local_player->self( )
			|| !g_local_player->self( )->alive( ) ) {
			return m_enemy_bullet_tracers.clear( );
		}

		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_enemy_bullet_tracers ) {
			return m_enemy_bullet_tracers.clear( );
		}

		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

		auto it = m_enemy_bullet_tracers.begin( );

		while ( it != m_enemy_bullet_tracers.end( ) ) {
			auto& cur_it = *it;
			auto next_it = ( it + 1 );
			const auto life_time = valve::g_global_vars.get( )->m_cur_time - cur_it.m_spawn_time;

			if ( next_it != m_enemy_bullet_tracers.end( )
				&& cur_it.m_spawn_time == ( *next_it ).m_spawn_time ) {
				it = m_enemy_bullet_tracers.erase( it );
				continue;
			}

			if ( std::abs( life_time ) > 1.f )
				it = m_enemy_bullet_tracers.erase( it );
			else
				it++;

			valve::beam_info_t info{};

			info.m_start = cur_it.m_start_pos;
			info.m_end = cur_it.m_end_pos;
			info.m_model_index = valve::g_model_info->model_index( ( "sprites/purplelaser1.vmt" ) );
			info.m_model_name = ( "sprites/purplelaser1.vmt" );
			info.m_life = 0.5f;
			info.m_width = 0.6f;
			info.m_end_width = 0.6f;
			info.m_fade_length = 0.f;
			info.m_amplitude = 0.f;   // beam 'jitter'.
			info.m_brightness = 255.f;
			info.m_speed = 0.5f;
			info.m_segments = 2;
			info.m_renderable = true;
			info.m_flags = 0;
			info.m_red = cfg.m_enemy_bullet_tracers_clr[ 0 ] * 255.f;
			info.m_green = cfg.m_enemy_bullet_tracers_clr[ 1 ] * 255.f;
			info.m_blue = cfg.m_enemy_bullet_tracers_clr[ 2 ] * 255.f;

			const auto beam = valve::g_beams->create_beam_points( info );
			if ( !beam )
				return;

			valve::g_beams->draw_beam( beam );
		}
	}

	void c_visuals::draw_bullet_tracers( ) {
		if ( !g_local_player->self( )
			|| !g_local_player->self( )->alive( ) ) {
			return m_bullet_tracers.clear( );
		}

		const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_bullet_tracers ) {
			return m_bullet_tracers.clear( );
		}

		if ( !m_bullet_tracers.empty( ) ) {
			if ( m_bullet_tracers.size( ) > 4 )
				m_bullet_tracers.pop_front( );

			auto it = m_bullet_tracers.begin( );

			while ( it != m_bullet_tracers.end( ) ) {
				auto& cur_it = *it;
				auto next_it = ( it + 1 );
				const auto life_time = valve::g_global_vars.get( )->m_cur_time - cur_it.m_spawn_time;

				if ( next_it != m_bullet_tracers.end( )
					&& cur_it.m_spawn_time == ( *next_it ).m_spawn_time ) {
					it = m_bullet_tracers.erase( it );
					continue;
				}

				if ( std::abs( life_time ) > 1.f )
					it = m_bullet_tracers.erase( it );
				else
					it++;

				auto start_pos = cur_it.m_start_pos;

				start_pos.z( ) -= 2;

				valve::beam_info_t info{};

				info.m_start = start_pos;
				info.m_end = cur_it.m_end_pos;
				info.m_model_index = valve::g_model_info->model_index( ( "sprites/purplelaser1.vmt" ) );
				info.m_model_name = ( "sprites/purplelaser1.vmt" );
				info.m_life = 0.5f;
				info.m_width = 0.6f;
				info.m_end_width = 0.6f;
				info.m_fade_length = 0.f;
				info.m_amplitude = 0.f;   // beam 'jitter'.
				info.m_brightness = 255.f;
				info.m_speed = 0.5f;
				info.m_segments = 2;
				info.m_renderable = true;
				info.m_flags = 0;
				info.m_red = cfg.m_bullet_tracers_clr[ 0 ] * 255.f;
				info.m_green = cfg.m_bullet_tracers_clr[ 1 ] * 255.f;
				info.m_blue = cfg.m_bullet_tracers_clr[ 2 ] * 255.f;

				const auto beam = valve::g_beams->create_beam_points( info );
				if ( !beam )
					return;

				valve::g_beams->draw_beam( beam );
			}
		}
	}


	void c_visuals::key_binds( ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_key_binds_list )
			return;

		static std::map < std::string, key_data_t* > data_map{ { "double tap", new key_data_t( ) } };
		data_map.insert( { "hide shots", new key_data_t( ) } );
		data_map.insert( { "damage override", new key_data_t( ) } );
		data_map.insert( { "force safe points", new key_data_t( ) } );
		data_map.insert( { "flip side", new key_data_t( ) } );
		data_map.insert( { "auto direction", new key_data_t( ) } );

		int count{ 0 };
		bool has_anything{};
		static int whole_shit_alphas{ 0 };

		const auto& exploits_cfg = g_menu->main( ).m_exploits_cfg.get( );
		const auto& rage_cfg = g_menu->main( ).m_aim_bot_cfg.get( );
		const auto& anti_aim_cfg = g_menu->main( ).m_anti_aim_cfg.get( );

		if ( g_key_binds->get_keybind_state( &exploits_cfg.m_double_tap ) ) {
			has_anything = true;
			data_map.find( "double tap" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "double tap" )->second->m_status = "disabled";
			data_map.find( "double tap" )->second->m_alpha -= 15.f;
		}

		if ( g_key_binds->get_keybind_state( &exploits_cfg.m_hide_shots ) ) {
			has_anything = true;
			data_map.find( "hide shots" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "hide shots" )->second->m_status = "disabled";
			data_map.find( "hide shots" )->second->m_alpha -= 15.f;
		}

		if ( g_key_binds->get_keybind_state( &rage_cfg.m_min_dmg_override ) ) {
			has_anything = true;
			data_map.find( "damage override" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "damage override" )->second->m_status = "disabled";
			data_map.find( "damage override" )->second->m_alpha -= 15.f;
		}

		if ( g_key_binds->get_keybind_state( &rage_cfg.m_force_safe_points ) ) {
			has_anything = true;
			data_map.find( "force safe points" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "force safe points" )->second->m_status = "disabled";
			data_map.find( "force safe points" )->second->m_alpha -= 15.f;
		}

		if ( g_key_binds->get_keybind_state( &anti_aim_cfg.m_auto_dir ) ) {
			has_anything = true;
			data_map.find( "auto direction" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "auto direction" )->second->m_status = "disabled";
			data_map.find( "auto direction" )->second->m_alpha -= 15.f;
		}

		if ( g_key_binds->get_keybind_state( &anti_aim_cfg.m_flip_side ) ) {
			has_anything = true;
			data_map.find( "flip side" )->second->m_alpha += 15.f;
		}
		else {
			data_map.find( "flip side" )->second->m_status = "disabled";
			data_map.find( "flip side" )->second->m_alpha -= 15.f;
		}

		if ( !has_anything ) {
			whole_shit_alphas -= 10;
		}
		else
			whole_shit_alphas += 10;

		whole_shit_alphas = std::clamp( whole_shit_alphas, 0, 255 );

		data_map.find( "double tap" )->second->m_alpha = std::clamp( data_map.find( "double tap" )->second->m_alpha, 0.f, 255.f );
		data_map.find( "hide shots" )->second->m_alpha = std::clamp( data_map.find( "hide shots" )->second->m_alpha, 0.f, 255.f );
		data_map.find( "damage override" )->second->m_alpha = std::clamp( data_map.find( "damage override" )->second->m_alpha, 0.f, 255.f );
		data_map.find( "force safe points" )->second->m_alpha = std::clamp( data_map.find( "force safe points" )->second->m_alpha, 0.f, 255.f );
		data_map.find( "auto direction" )->second->m_alpha = std::clamp( data_map.find( "auto direction" )->second->m_alpha, 0.f, 255.f );
		data_map.find( "flip side" )->second->m_alpha = std::clamp( data_map.find( "flip side" )->second->m_alpha, 0.f, 255.f );

		if ( whole_shit_alphas < 0.000002f )
			return;

		ImGui::Begin( "Hello, world!!!!!!!!!!!!!!!", 64, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar );
		{
			ImVec2 pos;
			ImDrawList* draw;
			pos = ImGui::GetWindowPos( );
			draw = ImGui::GetWindowDrawList( );

			ImGui::SetWindowSize( ImVec2( 300, 300 ) );

			draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 60 ), ImVec2( pos.x + 200 + 5, pos.y + 92 ), ImColor( 27, 28, 32, whole_shit_alphas ), 2.f );
			draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 90 ), ImVec2( pos.x + 200 + 5, pos.y + 92 ), ImColor( 159, 182, 220, whole_shit_alphas ), 2.f );

			ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_44 );
			draw->AddText( ImVec2( pos.x + 60 + 5, pos.y + 53 ), ImColor( 159, 182, 220, whole_shit_alphas ), "A" );
			ImGui::PopFont( );

			ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_38 );
			draw->AddText( ImVec2( pos.x + 75 + 5, pos.y + 57 ), ImColor( 255, 255, 255, whole_shit_alphas ), "dvance" );
			ImGui::PopFont( );

			for ( const auto& it : data_map ) {
				if ( it.second->m_alpha < 0.001f )
					continue;

				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_30 );
				draw->AddText( ImVec2( pos.x + 5 + 5, pos.y + 91 + ( 28 * count ) ), ImColor( 255, 255, 255, static_cast < int > ( it.second->m_alpha ) ), it.first.c_str( ) );
				ImGui::PopFont( );

				draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 96 + ( 28 * count ) ), ImVec2( pos.x + 5 + 3, pos.y + 115 + ( 28 * count ) ), ImColor( 159, 182, 220, static_cast < int > ( it.second->m_alpha ) ) );

				count += 1;
			}
		}
	}

	void c_visuals::spectators( ) const {
		if ( !g_menu->main( ).m_visuals_cfg.get( ).m_spectators_list )
			return;

		std::vector < std::string > spectator_list{};

		int count{ 0 };
		static int whole_shit_alphas{ 0 };

		if ( g_local_player && g_local_player->self( ) && g_local_player->self( )->alive( ) )
		{
			for ( int i = 1; i <= valve::g_global_vars.get( )->m_max_clients; i++ )
			{
				auto player = ( valve::cs_player_t* ) valve::g_entity_list.get( )->get_entity( i );

				if ( !player || player->alive( ) || !player->is_player( ) )
					continue;

				auto observer_target = valve::g_entity_list.get( )->get_entity( player->observer_target_handle( ) );

				if ( !observer_target || observer_target != g_local_player->self( ) )
					continue;

				valve::player_info_t info;

				valve::g_engine.get( )->get_player_info( player->networkable( )->index( ), &info );

				spectator_list.emplace_back( ( std::string ) ( info.m_name ) );
			}
		}

		if ( spectator_list.empty( ) ) {
			whole_shit_alphas -= 15;
		}
		else {
			whole_shit_alphas += 15;
		}

		whole_shit_alphas = std::clamp( whole_shit_alphas, 0, 255 );

		if ( whole_shit_alphas <= 0 )
			return;

		ImGui::Begin( "Hello, world!!!!!!!!!!!!!!!", 64, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar );
		{
			ImVec2 pos;
			ImDrawList* draw;
			pos = ImGui::GetWindowPos( );
			draw = ImGui::GetWindowDrawList( );

			ImGui::SetWindowSize( ImVec2( 300, 300 ) );

			draw->AddRectFilled( ImVec2( pos.x, pos.y + 60 ), ImVec2( pos.x + 150, pos.y + 90 ), ImColor( 27, 28, 32, whole_shit_alphas ), 2.f );
			draw->AddRectFilled( ImVec2( pos.x, pos.y + 88 ), ImVec2( pos.x + 150, pos.y + 91 ), ImColor( 159, 182, 220, whole_shit_alphas ), 2.f );

			ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_30 );
			draw->AddText( ImVec2( pos.x + 34, pos.y + 60 ), ImColor( 255, 255, 255, whole_shit_alphas ), "Spectators" );

			for ( const auto& it : spectator_list ) {
				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_30 );
				draw->AddText( ImVec2( pos.x + 5 + 5, pos.y + 91 + ( 28 * count ) ), ImColor( 255, 255, 255, whole_shit_alphas ), it.c_str( ) );
				ImGui::PopFont( );

				draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 96 + ( 28 * count ) ), ImVec2( pos.x + 5 + 3, pos.y + 115 + ( 28 * count ) ), ImColor( 159, 182, 220, whole_shit_alphas ) );

				count += 1;
			}
			ImGui::PopFont( );
		}
	}

	void c_visuals::water_mark( ) const {
		auto is_valid_cursor = [ ]( int max_x, int max_y, int min_x, int min_y ) {
			auto mouse_pos = ImGui::GetMousePos( );
			auto win_pos = ImGui::GetWindowPos( );
			if ( mouse_pos.x <= win_pos.x + max_x
				&& mouse_pos.x > win_pos.x + min_x
				&& mouse_pos.y <= win_pos.y + max_y
				&& mouse_pos.y > win_pos.y + min_y )
				return true;

			return false;
		};

		int ping{};

		auto net_channel = valve::g_engine.get( )->net_channel_info( );
		if ( valve::g_engine.get( )->in_game( ) ) {
			if ( net_channel )
			{
				auto latency = net_channel->avg_latency( 0 );

				if ( latency )
				{
					static auto cl_updaterate = valve::g_cvar->find_var( ( "cl_updaterate" ) );
					latency -= 0.5f / cl_updaterate->get_float( );
				}

				ping = ( int ) ( std::max( 0.0f, latency ) * 1000.0f );
			}
		}
		else
			ping = 0;
	
		static float current_time = 0.f;
		static int last_fps = ( int ) ( 1.0f / valve::g_global_vars.get( )->m_abs_frame_time );

		if ( current_time > 0.5f )
		{
			last_fps = ( int ) ( 1.0f / valve::g_global_vars.get( )->m_abs_frame_time );
			current_time = 0.f;
		}

		current_time += valve::g_global_vars.get( )->m_abs_frame_time;

		ImGui::Begin( "waafdaf", false, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( ImVec2( 300, 100 ) );
			bool clicked{ImGui::GetIO( ).MouseClicked[ 0 ]};
			ImVec2 pos;
			ImDrawList* draw;
			pos = ImGui::GetWindowPos( );
			draw = ImGui::GetWindowDrawList( );

			static bool is_opened{ false };
			static bool is_name_picked{ false };
			static bool is_fps_picked{ false };
			static bool is_ms_picked{ false };

			static int name_alpha{}, name_alpha_circle{};
			static int fps_alpha{}, fps_alpha_circle{};
			static int ms_alpha{}, ms_alpha_circle{};

			static int extra_spacing_name{};
			static int extra_spacing_fps{};
			static int extra_spacing_ms{};

			int ping_offset_correction{}, fps_offset_correction{};

			if ( ping > 9 )
				ping_offset_correction += 10;

			if ( ping > 99 )
				ping_offset_correction += 10;

			if ( ping > 999 )
				ping_offset_correction += 13;

			if ( last_fps > 9 )
				fps_offset_correction += 10;

			if ( last_fps > 99 )
				fps_offset_correction += 10;

			if ( last_fps > 999 )
				fps_offset_correction += 13;

			if ( is_valid_cursor( 85, 40, 10, 10 )
				&& clicked )
				is_opened = !is_opened;

			if ( is_opened ) {
				if ( is_valid_cursor( 60, 65, 5, 40 )
					&& clicked )
					is_name_picked = !is_name_picked;

				if ( is_valid_cursor( 60, 95, 5, 75 )
					&& clicked )
					is_fps_picked = !is_fps_picked;

				if ( is_valid_cursor( 60, 125, 5, 105 )
					&& clicked )
					is_ms_picked = !is_ms_picked;
			}

			std::string name = "killniggers";

			static int first_spacing{}, second_spacing{}, third_spacing{};

			int text_length = csgo::g_render->m_fonts.m_mukta.m_mukta_38->CalcTextSizeA( 38.f, FLT_MAX, NULL, name.c_str( ) ).x - 61;

			static int alpha{ 0 };

			if ( is_opened ) {
				alpha += 10;
			}
			else {
				alpha -= 10;
				name_alpha -= 10;
				name_alpha_circle -= 10;
				fps_alpha_circle -= 10;
				ms_alpha_circle -= 10;
				fps_alpha -= 10;
				ms_alpha -= 10;
			}

			if ( name_alpha < 0 )
				name_alpha = 0;

			if ( name_alpha > 255 )
				name_alpha = 255;

			if ( name_alpha_circle < 0 )
				name_alpha_circle = 0;

			if ( name_alpha_circle > 255 )
				name_alpha_circle = 255;

			if ( fps_alpha_circle < 0 )
				fps_alpha_circle = 0;

			if ( fps_alpha_circle > 255 )
				fps_alpha_circle = 255;

			if ( ms_alpha_circle < 0 )
				ms_alpha_circle = 0;

			if ( ms_alpha_circle > 255 )
				ms_alpha_circle = 255;

			if ( fps_alpha < 0 )
				fps_alpha = 0;

			if ( fps_alpha > 255 )
				fps_alpha = 255;

			if ( ms_alpha < 0 )
				ms_alpha = 0;

			if ( ms_alpha > 255 )
				ms_alpha = 255;

			if ( alpha > 255 )
				alpha = 255;
			if ( alpha < 0 )
				alpha = 0;

			static int elements_counter{};

			if ( is_opened ) {
				elements_counter = 0;
				if ( is_name_picked ) {
					name_alpha += 3;
					name_alpha_circle += 15;
					extra_spacing_name += 1;
					elements_counter += 1;
				}
				else {
					name_alpha -= 3;
					name_alpha_circle -= 15;
					extra_spacing_name -= 1;
				}

				if ( name_alpha < 190 )
					name_alpha = 190;

				if ( name_alpha > 255 )
					name_alpha = 255;

				if ( name_alpha_circle < 0 )
					name_alpha_circle = 0;

				if ( name_alpha_circle > 255 )
					name_alpha_circle = 255;


				if ( is_fps_picked ) {
					elements_counter += 1;
					fps_alpha += 3;
					fps_alpha_circle += 15;
					extra_spacing_fps += 1;
				}
				else {
					fps_alpha -= 3;
					fps_alpha_circle -= 15;
					extra_spacing_fps -= 1;
				}

				if ( fps_alpha < 190 )
					fps_alpha = 190;

				if ( fps_alpha > 255 )
					fps_alpha = 255;

				if ( is_ms_picked ) {
					elements_counter += 1;
					ms_alpha += 3;
					ms_alpha_circle += 15;
					extra_spacing_ms += 1;
				}
				else {
					ms_alpha -= 3;
					ms_alpha_circle -= 15;
					extra_spacing_ms -= 1;
				}

				if ( fps_alpha_circle < 0 )
					fps_alpha_circle = 0;

				if ( fps_alpha_circle > 255 )
					fps_alpha_circle = 255;

				if ( ms_alpha_circle < 0 )
					ms_alpha_circle = 0;

				if ( ms_alpha_circle > 255 )
					ms_alpha_circle = 255;

				if ( ms_alpha < 190 )
					ms_alpha = 190;

				if ( ms_alpha > 255 )
					ms_alpha = 255;

				if ( extra_spacing_fps > 12 )
					extra_spacing_fps = 12;

				if ( extra_spacing_fps < 0 )
					extra_spacing_fps = 0;

				if ( extra_spacing_ms > 12 )
					extra_spacing_ms = 12;

				if ( extra_spacing_ms < 0 )
					extra_spacing_ms = 0;

				if ( extra_spacing_name > 12 )
					extra_spacing_name = 12;

				if ( extra_spacing_name < 0 )
					extra_spacing_name = 0;
			}

			if ( elements_counter > 0 )
				first_spacing += 7;
			else
				first_spacing -= 7;

			if ( elements_counter >= 2 )
				second_spacing += 10;
			else
				second_spacing -= 10;

			if ( elements_counter == 3 )
				third_spacing += 7;
			else
				third_spacing -= 7;

			if ( first_spacing > 75 )
				first_spacing = 75;
			if ( first_spacing < 0 )
				first_spacing = 0;

			if ( second_spacing > 85 )
				second_spacing = 85;
			if ( second_spacing < 0 )
				second_spacing = 0;

			if ( third_spacing > 75 )
				third_spacing = 75;
			if ( third_spacing < 0 )
				third_spacing = 0;

			draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 10 ), ImVec2( pos.x + 85 + first_spacing + second_spacing + third_spacing + text_length + ping_offset_correction + fps_offset_correction, pos.y + 40 ), ImColor( 27, 28, 32, 255 ), 3.f );

			if ( alpha ) {
				draw->AddRectFilled( ImVec2( pos.x + 5, pos.y + 41 ), ImVec2( pos.x + 100 + 5, pos.y + 130 ), ImColor( 27, 28, 32, alpha ), 3.f );

				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_34 );
				draw->AddText( ImVec2( pos.x + 5 + 5 + extra_spacing_name, pos.y + 40 ), ImColor( 255, 255, 255, name_alpha ), "Name" );
				draw->AddText( ImVec2( pos.x + 5 + 5 + extra_spacing_fps, pos.y + 69 ), ImColor( 255, 255, 255, fps_alpha ), "Fps" );
				draw->AddText( ImVec2( pos.x + 5 + 5 + extra_spacing_ms, pos.y + 98 ), ImColor( 255, 255, 255, ms_alpha ), "Ping" );

				draw->AddCircleFilled( ImVec2( pos.x + 8 + 5, pos.y + 58 ), 5.f, ImColor( 159, 182, 220, name_alpha_circle ), 30 );

				draw->AddCircleFilled( ImVec2( pos.x + 8 + 5, pos.y + 87 ), 5.f, ImColor( 159, 182, 220, fps_alpha_circle ), 30 );

				draw->AddCircleFilled( ImVec2( pos.x + 8 + 5, pos.y + 116 ), 5.f, ImColor( 159, 182, 220, ms_alpha_circle ), 30 );
				ImGui::PopFont( );
			}

			ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_44 );
			draw->AddText( ImVec2( pos.x + 5 + 5, pos.y + 3 ), ImColor( 159, 182, 220, 255 ), "A" );
			ImGui::PopFont( );
			ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_38 );
			draw->AddText( ImVec2( pos.x + 21 + 5, pos.y + 7 ), ImColor( 255, 255, 255, 255 ), "dvance" );
			ImGui::PopFont( );

			if ( elements_counter > 0 )
				draw->AddRectFilled( ImVec2( pos.x + 98 + 5, pos.y + 18 ), ImVec2( pos.x + 101 + 5, pos.y + 35 ), ImColor( 122, 126, 134, 255 ) );

			if ( elements_counter >= 2 ) {
				draw->AddRectFilled( ImVec2( pos.x + 179 + 5 + text_length, pos.y + 18 ), ImVec2( pos.x + 182 + 5 + text_length, pos.y + 35 ), ImColor( 122, 126, 134, 255 ) );
			}

			if ( elements_counter == 3 )
				draw->AddRectFilled( ImVec2( pos.x + 237 + 5 + text_length + fps_offset_correction, pos.y + 18 ), ImVec2( pos.x + 240 + 5 + text_length + fps_offset_correction, pos.y + 35 ), ImColor( 122, 126, 134, 255 ) );

			int add_offset_fps{};

			int add_offset_ms{ 9 };

			if ( is_name_picked ) {
				add_offset_fps += 82;
				add_offset_ms += 79;
				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_38 );
				draw->AddText( ImVec2( pos.x + 109 + 5, pos.y + 7 ), ImColor( 255, 255, 255, 255 ), name.c_str( ) );
				ImGui::PopFont( );
			}

			if ( is_fps_picked ) {
				add_offset_ms += 79;
				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_38 );
				draw->AddText( ImVec2( pos.x + 109 + 5 + add_offset_fps + text_length, pos.y + 7 ), ImColor( 255, 255, 255, 255 ), std::to_string( last_fps ).c_str( ) );
				ImGui::PopFont( );

				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_30 );
				draw->AddText( ImVec2( pos.x + 124 + 5 + add_offset_fps + text_length + fps_offset_correction, pos.y + 12 ), ImColor( 255, 255, 255, 255 ), "fps" );
				ImGui::PopFont( );
			}

			if ( is_ms_picked ) {
				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_38 );
				draw->AddText( ImVec2( pos.x + 85 + 5 + text_length + add_offset_ms + fps_offset_correction, pos.y + 7 ), ImColor( 255, 255, 255, 255 ), std::to_string( ping ).c_str( ) );
				ImGui::PopFont( );

				ImGui::PushFont( csgo::g_render->m_fonts.m_mukta.m_mukta_30 );
				draw->AddText( ImVec2( pos.x + 100 + 5 + text_length + add_offset_ms + fps_offset_correction + ping_offset_correction, pos.y + 12 ), ImColor( 255, 255, 255, 255 ), "ms" );
				ImGui::PopFont( );
			}
			
		}
		ImGui::End( );
	}

	bool c_visuals::add_grenade_simulation( const grenade_simulation_t& sim, const bool warning, const bool no_dmg ) const {
		const auto points_count = sim.m_path.size( );
		if ( points_count < 2u
			|| valve::g_global_vars.get( )->m_cur_time >= sim.m_expire_time
			|| !g_menu->main( ).m_visuals_cfg.get( ).m_grenade_prediction )
			return false;

		const auto& clr = sdk::col_t( g_menu->main( ).m_visuals_cfg.get( ).m_grenade_pred_clr.at( 0 ) * 255, g_menu->main( ).m_visuals_cfg.get( ).m_grenade_pred_clr.at( 1 ) * 255,
			g_menu->main( ).m_visuals_cfg.get( ).m_grenade_pred_clr.at( 2 ) * 255, g_menu->main( ).m_visuals_cfg.get( ).m_grenade_pred_clr.at( 3 ) * 255 );

		const auto& screen_size = ImGui::GetIO( ).DisplaySize;
		if ( warning ) {
			const auto& explode_pos = sim.m_path.back( ).first;
			auto dist = ( g_local_player->self( )->origin( ) - explode_pos ).length( );

			sdk::vec3_t prev_screen_pos{};
			auto prev_on_screen = csgo::g_render->world_to_screen( sim.m_path.front( ).first, prev_screen_pos
			);

			const auto mod = std::clamp(
				( sim.m_expire_time - valve::g_global_vars.get( )->m_cur_time )
				/ valve::to_time( sim.m_tick ),
				0.f, 1.f
			);

			for ( auto i = 1u; i < points_count; ++i ) {
				sdk::vec3_t cur_screen_pos{};
				const auto cur_on_screen = csgo::g_render->world_to_screen( sim.m_path.at( i ).first, cur_screen_pos
				);
				if ( prev_on_screen
					&& cur_on_screen ) {
					csgo::g_render->line( sdk::vec2_t( prev_screen_pos.x( ), prev_screen_pos.y( ) ), sdk::vec2_t( cur_screen_pos.x( ), cur_screen_pos.y( ) ), sdk::col_t( clr.r( ), clr.g( ), clr.b( ), clr.a( ) * mod ) );

				}
				prev_screen_pos = cur_screen_pos;
				prev_on_screen = cur_on_screen;
			}

			sdk::vec3_t screen_pos{};
			const auto on_screen = csgo::g_render->world_to_screen( explode_pos, screen_pos );
			if ( !on_screen
				&& dist > 400.f )
				return true;

			const auto unk = sdk::vec2_t( screen_size.x / 18.f, screen_size.y / 18.f );
			if ( !on_screen
				|| screen_pos.x( ) < -unk.x( )
				|| screen_pos.x( ) > ( screen_size.x + unk.x( ) )
				|| screen_pos.y( ) < -unk.y( )
				|| screen_pos.y( ) > ( screen_size.y + unk.y( ) ) ) {
				sdk::vec3_t dir{};
				sdk::ang_vecs( valve::g_view_render->m_setup.m_angles, &dir, nullptr, nullptr );

				dir.z( ) = 0.f;
				dir.normalize( );

				const auto radius = 210.f * ( screen_size.y / 480.f );

				auto delta = explode_pos - valve::g_view_render->m_setup.m_origin;

				delta.normalize( );

				screen_pos.x( ) = radius * -delta.dot( sdk::vec3_t{ 0.f, 0.f, 1.f }.cross( dir ) );
				screen_pos.y( ) = radius * -delta.dot( dir );

				const auto radians = sdk::to_rad(
					-sdk::to_deg( std::atan2( screen_pos.x( ), screen_pos.y( ) + 3.141592653589793 ) )
				);

				screen_pos.x( ) = static_cast< int >( screen_size.x / 2.f + radius * std::sin( radians ) );
				screen_pos.y( ) = static_cast< int >( screen_size.y / 2.f - radius * std::cos( radians ) );
			}

			csgo::g_render->m_draw_list->AddCircleFilled( ImVec2( screen_pos.x( ), screen_pos.y( ) ), 18.f, ImColor( 27, 28, 32, 230 ), 50 );

			if ( dist < 370.f && !no_dmg ) { // we can take damage

				/* ЕБАТЬ Я МАТЕМАТИК ХАХАХАХАХА */
				const float wtf = dist / 370.f;
				const auto reverted_alpha = wtf * 230;
				const auto alpha = 230 - reverted_alpha;
			
				csgo::g_render->m_draw_list->AddCircleFilled( ImVec2( screen_pos.x( ), screen_pos.y( ) ), 18.f, ImColor( 255, 0, 0, static_cast < int > ( alpha ) ), 50 );
			}

			csgo::g_render->m_draw_list->AddCircle( ImVec2( screen_pos.x( ), screen_pos.y( ) ), 18.f, ImColor( 40, 41, 45, 255 ), 50 );

			std::string icon = "";
			switch ( sim.m_index )
			{
			case valve::e_item_index::he_grenade: icon = ( "j" ); break;
			case valve::e_item_index::smoke_grenade: icon = ( "k" ); break;
			case valve::e_item_index::flashbang: icon = ( "i" ); break;
			case valve::e_item_index::decoy: icon = ( "m" ); break;
			case valve::e_item_index::inc_grenade: icon = ( "n" ); break;
			case valve::e_item_index::molotov: icon = ( "l" ); break;
			}

			csgo::g_render->text( icon, sdk::vec2_t( screen_pos.x( ) - 8, screen_pos.y( ) - 12 ), sdk::col_t( 255, 255, 255, 255 ), csgo::g_render->m_fonts.m_icons.m_warning_icon_font, true, false, false );

			return true;
		}

		sdk::vec3_t prev_screen_pos{};
		auto prev_on_screen = csgo::g_render->world_to_screen( sim.m_path.front( ).first, prev_screen_pos
		);

		for ( auto i = 1u; i < points_count; ++i ) {
			sdk::vec3_t cur_screen_pos{};
			const auto cur_on_screen = csgo::g_render->world_to_screen( sim.m_path.at( i ).first, cur_screen_pos
			);

			if ( prev_on_screen
				&& cur_on_screen ) {
				csgo::g_render->line( sdk::vec2_t( prev_screen_pos.x( ), prev_screen_pos.y( ) ), sdk::vec2_t( cur_screen_pos.x( ), cur_screen_pos.y( ) ), clr );

				const auto ayo = sdk::vec2_t( std::get < sdk::vec3_t>( sim.m_path.at( i ) ).x( ), std::get < sdk::vec3_t>( sim.m_path.at( i ) ).y( ) );

				if ( i == points_count - 1 )
					csgo::g_render->render_3d_circle( std::get < sdk::vec3_t >( sim.m_path.at( i ) ), 32, clr );
			}

			prev_screen_pos = cur_screen_pos;
			prev_on_screen = cur_on_screen;
		}

		return true;
	}

}