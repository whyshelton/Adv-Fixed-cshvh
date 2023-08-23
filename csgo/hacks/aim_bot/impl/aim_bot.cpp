#include "../../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"


namespace hacks {

	ALWAYS_INLINE float get_hit_chance( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_hit_chance_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_hit_chance_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_hit_chance_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_hit_chance_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_hit_chance_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_hit_chance_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE float get_head_scale( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_head_scale_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_head_scale_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_head_scale_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_head_scale_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_head_scale_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_head_scale_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE float get_body_scale( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_body_scale_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_body_scale_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_body_scale_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_body_scale_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_body_scale_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_body_scale_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE int get_min_dmg( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_min_dmg_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_min_dmg_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_min_dmg_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_min_dmg_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_min_dmg_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_min_dmg_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE int get_min_dmg_override_val( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_min_dmg_awp_override_val;
		case valve::e_item_index::ssg08:
			return cfg.m_min_dmg_scout_override_val;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_min_dmg_scar_override_val;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_min_dmg_other_override_val;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_min_dmg_heavy_pistol_override_val;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_min_dmg_pistol_override_val;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE bool static_point_scale( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return false;

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return false;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_static_point_scale_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_hit_chance_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_static_point_scale_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_static_point_scale_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_static_point_scale_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_static_point_scale_pistol;
		default:
			return false;
		}

		return false;
	}

	ALWAYS_INLINE int get_multi_points( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_multi_points_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_multi_points_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_multi_points_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_multi_points_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_multi_points_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_multi_points_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE int get_max_misses( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_max_misses_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_max_misses_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_max_misses_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_max_misses_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_max_misses_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_max_misses_pistol;
		default:
			return 0;
		}

		return 0;
	}

	ALWAYS_INLINE int get_hit_groups( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_hit_groups_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_hit_groups_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_hit_groups_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_hit_groups_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_hit_groups_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_hit_groups_pistol;
		default:
			return 0;
		}

		return 0;
	}


	ALWAYS_INLINE int get_stop_type( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return 0;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return cfg.m_stop_type_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_stop_type_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_stop_type_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_stop_type_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_stop_type_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_stop_type_pistol;
		default:
			return 0;
		}

		return 0;
	}

	void c_aim_bot::handle( valve::user_cmd_t& user_cmd, bool& send_packet ) {
		if ( g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_threading ) {
			static const auto once = [ ]( ) {
				const auto fn = reinterpret_cast< int( _cdecl* )( ) >(
					GetProcAddress( GetModuleHandle( ( "tier0.dll" ) ), ( "AllocateThreadID" ) )
					);

				std::counting_semaphore<> sem{ 0u };

				for ( std::size_t i{}; i < std::thread::hardware_concurrency( ); ++i )
					sdk::g_thread_pool->enqueue(
						[ ]( decltype( fn ) fn, std::counting_semaphore<>& sem ) {
							sem.acquire( );
							fn( );
						}, fn, std::ref( sem )
							);

				for ( std::size_t i{}; i < std::thread::hardware_concurrency( ); ++i )
					sem.release( );

				sdk::g_thread_pool->wait( );

				return true;
			}( );
		}

		m_last_target.m_lag_record.reset( );

		m_shooting = false;

		if ( !g_menu->main( ).m_aim_bot_cfg.get( ).m_rage_bot )
			return;

		if ( !g_local_player->weapon( )
			|| !g_local_player->weapon_info( ) 
			|| g_local_player->weapon_info( )->m_type == valve::e_weapon_type::knife 
			|| g_local_player->weapon_info( )->m_type == valve::e_weapon_type::grenade )
			return;

		g_move->allow_early_stop( ) = true;

		find_targets( );

		scan_targets( );

		const auto target = select_target( user_cmd.m_number );
		if ( !target )
			return m_targets.clear( );

		const auto point = select_point( *target, user_cmd.m_number );
		if ( !point
			|| !point->m_valid )
			return m_targets.clear( );

		lag_backup_t lag_backup{ target->m_entry->m_player };

		target->m_lag_record->setup( target->m_entry->m_player, target->m_lag_record->m_anim_side );

		sdk::qang_t angle{};
		
		g_local_player->prediction( ).update_shoot_pos( user_cmd );

		sdk::vec_angs( point->m_pos - g_local_player->shoot_pos( ), angle );

		const auto item_index = g_local_player->weapon( )->item_index( );

		target->m_pos = point->m_pos;

		if ( g_local_player->can_shoot( ) ) {
			if ( g_menu->main( ).m_aim_bot_cfg.get( ).m_auto_scope
				&& ( item_index == valve::e_item_index::g3sg1
					|| item_index == valve::e_item_index::scar20
					|| item_index == valve::e_item_index::awp
					|| item_index == valve::e_item_index::aug
					|| item_index == valve::e_item_index::sg553
					|| item_index == valve::e_item_index::ssg08
					)
				&& !g_local_player->self( )->scoped( )
				&& g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground )
				user_cmd.m_buttons |= valve::e_buttons::in_attack2;
			else if ( const auto hit_chance = calc_hit_chance( target, angle, point ); hit_chance >= ( item_index == valve::e_item_index::taser ? 66 : get_hit_chance( ) ) ) {
				static auto weapon_recoil_scale = valve::g_cvar->find_var( MilfStr( "weapon_recoil_scale" ) );
				user_cmd.m_view_angles = angle - g_local_player->self( )->aim_punch( ) * weapon_recoil_scale->get_float( );

				user_cmd.m_view_angles.x( ) = std::remainder( user_cmd.m_view_angles.x( ), 360.f );
				user_cmd.m_view_angles.y( ) = std::remainder( user_cmd.m_view_angles.y( ), 360.f );
				user_cmd.m_view_angles.z( ) = std::remainder( user_cmd.m_view_angles.z( ), 360.f );

				user_cmd.m_view_angles.x( ) = std::clamp( user_cmd.m_view_angles.x( ), -89.f, 89.f );
				user_cmd.m_view_angles.y( ) = std::clamp( user_cmd.m_view_angles.y( ), -180.f, 180.f );
				user_cmd.m_view_angles.z( ) = std::clamp( user_cmd.m_view_angles.z( ), -90.f, 90.f );

				user_cmd.m_buttons |= valve::e_buttons::in_attack;

				static auto cl_lagcompensation = valve::g_cvar->find_var( MilfStr( "cl_lagcompensation" ) );

				g_exploits->defensive_allowed( ) = false;

				if ( g_move->should_fake_duck( ) )
					send_packet = true;

				if ( cl_lagcompensation->get_int( ) != 0 )
					user_cmd.m_tick = valve::to_ticks( target->m_lag_record->m_sim_time + g_local_player->net_info( ).m_lerp );

				g_shots->add(
					g_local_player->shoot_pos( ), target,
					hacks::g_exploits->next_shift_amount( ), user_cmd.m_number, valve::g_global_vars.get( )->m_real_time, g_local_player->net_info( ).m_latency.m_out + g_local_player->net_info( ).m_latency.m_in
				);

				m_last_target.m_entry = target->m_entry;
				m_last_target.m_lag_record = target->m_lag_record;
				m_last_target.m_hitbox = point->m_hitbox;
				m_last_target.m_dmg = static_cast< int >( point->m_pen_data.m_dmg );
				m_last_target.m_hit_chance = hit_chance;
				m_last_target.m_point = *point;
				m_shooting = true;
				hacks::g_move->auto_peek_data( ).m_shot = true;
			}
			else {
				if ( !( user_cmd.m_buttons & valve::e_buttons::in_jump ) )
					m_stop_type = get_stop_type( ) + 1;
			}
		}

		lag_backup.restore( target->m_entry->m_player );

		m_targets.clear( );
	}

	aim_target_t* c_aim_bot::select_target( const int cmd_number ) {
		if ( m_targets.empty( ) )
			return nullptr;

		g_move->allow_early_stop( ) = false;

		auto best_target = &m_targets.front( );

		const auto end = m_targets.end( );
		for ( auto it = std::next( m_targets.begin( ) ); it != end; it = std::next( it ) ) {
			const auto hp = it->m_entry->m_player->health( );

			const auto& best_lag = best_target->m_lag_record;
			const auto& lag = it->m_lag_record;

			const auto& best_pen_data = best_target->m_best_point->m_pen_data;
			const auto& pen_data = it->m_best_point->m_pen_data;

			if ( hp <= pen_data.m_dmg )
				best_target = &*it;		
			else if ( lag->m_broke_lc == best_lag->m_broke_lc ) {

				if ( it->m_best_point->m_intersections == best_target->m_best_point->m_intersections ) {
					if ( best_pen_data.m_dmg <= hp
						&& pen_data.m_dmg > best_pen_data.m_dmg )
						best_target = &*it;
				}
				else if ( it->m_best_point->m_intersections > best_target->m_best_point->m_intersections ) {
					best_target = &*it;
				}
			}
			else if ( !lag->m_broke_lc )
			{
				best_target = &*it;
			}
		}

		return best_target;
	}

	aim_point_t* c_aim_bot::select_point( const aim_target_t& target, const int cmd_number ) const {
		if ( !target.m_best_body_point
			|| target.m_best_body_point->m_pen_data.m_dmg < 1 )
			return target.m_best_point;

		if ( target.m_entry->m_misses >= get_max_misses( )
			|| target.m_entry->m_jitter_misses >= get_max_misses( ) )
			return target.m_best_body_point;

		const auto& shots = g_shots->elements( );
		if ( !shots.empty( ) ) {
			const auto& last_shot = shots.back( );
			if ( last_shot.m_target.m_entry == target.m_entry
				&& std::abs( last_shot.m_cmd_number - cmd_number ) <= 150 ) {
				const auto after_shot_hp = last_shot.m_target.m_entry->m_player->health( ) - last_shot.m_dmg;
				if ( after_shot_hp > 0
					&& target.m_best_body_point->m_pen_data.m_dmg > after_shot_hp )
					return target.m_best_body_point;
			}
		}

		if ( target.m_best_body_point->m_pen_data.m_dmg >= target.m_entry->m_player->health( ) )
			return target.m_best_body_point;

		const auto dt_enabled = g_exploits->type( ) == 2 || g_exploits->type( ) == 3;

		if ( g_local_player->self( ) && g_local_player->weapon( ) && ( g_local_player->weapon( )->item_index( ) == valve::e_item_index::ssg08
			|| g_local_player->weapon( )->item_index( ) == valve::e_item_index::awp ) ) {
			if ( dt_enabled ) {
				const auto& shots = g_shots->elements( );
				if ( !shots.empty( ) ) {
					const auto& last_shot = shots.back( );
					if ( last_shot.m_next_shift_amount
						&& last_shot.m_target.m_entry == target.m_entry
						&& std::abs( last_shot.m_cmd_number - cmd_number ) <= 150
						&& ( target.m_best_body_point->m_pen_data.m_dmg + last_shot.m_dmg ) >= target.m_entry->m_player->health( ) )
						return target.m_best_body_point;
				}
			}
		}
		else {
			if ( dt_enabled
				&& ( target.m_best_body_point->m_pen_data.m_dmg * 2 ) >= target.m_entry->m_player->health( ) )
				return target.m_best_body_point;

			if ( target.m_best_body_point->m_pen_data.m_dmg + 20.f >= target.m_entry->m_player->health( ) )
				return target.m_best_body_point;

			if ( dt_enabled ) {
				const auto& shots = g_shots->elements( );
				if ( !shots.empty( ) ) {
					const auto& last_shot = shots.back( );
					if ( last_shot.m_next_shift_amount
						&& last_shot.m_target.m_entry == target.m_entry
						&& ( target.m_best_body_point->m_pen_data.m_dmg + last_shot.m_dmg ) >= target.m_entry->m_player->health( ) )
						return target.m_best_body_point;
				}
			}
		}

		return target.m_best_point;
	}

	std::optional< aim_record_t > c_aim_bot::select_latest_record( const player_entry_t& entry, const bool skip_is_valid ) const {
		if ( entry.m_lag_records.empty( ) )
			return std::nullopt;

		const auto& latest = entry.m_lag_records.back( );

		if ( latest->m_choked_cmds <= 0
			|| latest->m_choked_cmds >= 20
			|| latest->m_dormant
			|| ( !latest->valid( ) && !skip_is_valid )
			|| latest->m_invalid_sim )
			return std::nullopt;

		if ( latest->m_broke_lc ) {
			return std::nullopt;
		}

		aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

		return ret;
	}

	std::optional< aim_record_t > c_aim_bot::extrapolate( const player_entry_t& entry ) const {
		if ( entry.m_lag_records.empty( ) )
			return std::nullopt;

		const auto& latest = entry.m_lag_records.back( );
		if ( latest->m_sim_ticks < 0
			|| latest->m_sim_ticks > 20
			|| latest->m_dormant
			|| latest->m_choked_cmds <= 0 )
			return std::nullopt;

		const auto time_delta = g_lag_comp->calc_time_delta( latest->m_sim_time );
		if ( time_delta < 0.2f ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}

		const auto& net_info = g_local_player->net_info( );

		const auto latency_ticks = valve::to_ticks( net_info.m_latency.m_in + net_info.m_latency.m_out );
		const auto delta = ( valve::g_client_state->m_server_tick + latency_ticks - latest->m_receive_tick ) / latest->m_choked_cmds;
		if ( delta <= 0
			|| delta > 20 ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}

		const auto max = valve::to_ticks( time_delta - 0.2f ) / latest->m_choked_cmds;

		auto v18 = std::min( delta, max );
		if ( v18 <= 0 ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}

		extrapolation_data_t data{ entry.m_player, latest.get( ) };

		do {
			for ( int i{}; i < latest->m_choked_cmds; ++i ) {
				data.m_sim_time += valve::g_global_vars.get( )->m_interval_per_tick;

				player_move( data );
			}
		} while ( --v18 );

		aim_record_t ret{ const_cast< player_entry_t* >( &entry ), std::make_shared< lag_record_t >( ) };

		*ret.m_lag_record = *latest;

		ret.m_lag_record->m_sim_time = data.m_sim_time;
		ret.m_lag_record->m_flags = data.m_flags;

		ret.m_lag_record->m_origin = data.m_origin;
		ret.m_lag_record->m_velocity = data.m_velocity;

		const auto origin_delta = data.m_origin - latest->m_origin;

		for ( auto& anim_side : ret.m_lag_record->m_anim_sides )
			for ( std::size_t i{}; i < anim_side.m_bones_count; ++i ) {
				auto& bone = anim_side.m_bones.at( i );

				bone[ 0 ][ 3 ] += origin_delta.x( );
				bone[ 1 ][ 3 ] += origin_delta.y( );
				bone[ 2 ][ 3 ] += origin_delta.z( );
			}

		return ret;
	}


	void c_aim_bot::player_move( extrapolation_data_t& data ) const {
		static auto sv_gravity = valve::g_cvar->find_var( ( MilfStr( "sv_gravity" ) ) );
		static auto sv_enable_bhop = valve::g_cvar->find_var( ( MilfStr( "sv_enablebunnyhopping" ) ) );
		static auto sv_jump_impulse = valve::g_cvar->find_var( MilfStr( "sv_jump_impulse" ) );

		if ( !( data.m_flags & valve::e_ent_flags::on_ground ) ) {
			if ( !sv_enable_bhop->get_int( ) ) {
				const auto speed = data.m_velocity.length( );

				const auto max_speed = data.m_player->max_speed( ) * 1.1f;
				if ( max_speed > 0.f
					&& speed > max_speed )
					data.m_velocity *= ( max_speed / speed );
			}

			if ( data.m_was_in_air )
				data.m_velocity.z( ) = sv_jump_impulse->get_float( );
		}
		else
			data.m_velocity.z( ) -=
			sv_gravity->get_float( ) * valve::g_global_vars.get( )->m_interval_per_tick;

		valve::trace_t trace{};
		valve::trace_filter_world_only_t trace_filter{};

		valve::g_engine_trace->trace_ray(
			{
				data.m_origin,
				data.m_origin + data.m_velocity * valve::g_global_vars.get( )->m_interval_per_tick,
				data.m_obb_min, data.m_obb_max
			},
			( valve::e_mask ) ( 0x200400bu ), &trace_filter, &trace
		);

		if ( trace.m_frac != 1.f ) {
			for ( int i{}; i < 2; ++i ) {
				data.m_velocity -= trace.m_plane.m_normal * data.m_velocity.dot( trace.m_plane.m_normal );

				const auto adjust = data.m_velocity.dot( trace.m_plane.m_normal );
				if ( adjust < 0.f )
					data.m_velocity -= trace.m_plane.m_normal * adjust;

				valve::g_engine_trace->trace_ray(
					{
						trace.m_end,
						trace.m_end + ( data.m_velocity * ( valve::g_global_vars.get( )->m_interval_per_tick * ( 1.f - trace.m_frac ) ) ),
						data.m_obb_min, data.m_obb_max
					},
					( valve::e_mask ) ( 0x200400bu ), &trace_filter, &trace
				);

				if ( trace.m_frac == 1.f )
					break;
			}
		}

		data.m_origin = trace.m_end;

		valve::g_engine_trace->trace_ray(
			{
				trace.m_end,
				{ trace.m_end.x( ), trace.m_end.y( ), trace.m_end.z( ) - 2.f },
				data.m_obb_min, data.m_obb_max
			},
			( valve::e_mask ) ( 0x200400bu ), &trace_filter, &trace
		);

		data.m_flags &= ~valve::e_ent_flags::on_ground;

		if ( trace.m_frac != 1.f
			&& trace.m_plane.m_normal.z( ) > 0.7f )
			data.m_flags |= valve::e_ent_flags::on_ground;
	}


	std::optional< aim_record_t > c_aim_bot::select_record( player_entry_t& entry ) const {
		if ( entry.m_lag_records.empty( ) )
			return std::nullopt;

		const auto mdl_data = entry.m_player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return std::nullopt;

		static auto cl_lagcompensation = valve::g_cvar->find_var( ( "cl_lagcompensation" ) );

		const auto hitbox_set = mdl_data->m_studio_hdr->get_hitbox_set( entry.m_player->hitbox_set_index( ) );
		if ( !hitbox_set
			|| hitbox_set->m_hitboxes_count <= 0 )
			return std::nullopt;

		if ( cl_lagcompensation->get_int( ) == 0 )
			return extrapolate( entry );

		const auto latest_record = select_latest_record( entry, false );

		if ( entry.m_lag_records.size( ) == 1u )
			return select_latest_record( entry, false );

		int cfg_hitgroups{};

		cfg_hitgroups = 32 | 5;
		std::optional< aim_point_t > best_point{};
		std::shared_ptr< lag_record_t > best_record{}, pre_best_record{};
		lag_backup_t lag_backup{ entry.m_player };

		const auto hp = entry.m_player->health( );

		std::size_t scanned_count{};

		const auto rend = entry.m_lag_records.rend( );
		std::shared_ptr< lag_record_t > prev_lag_record;
		for ( auto i = entry.m_lag_records.rbegin( ); i != rend; i++ ) {	
			const auto& lag_record = *i;

			if ( ( i + 1 ) != rend )
				prev_lag_record = *( i + 1 );

			if ( !lag_record->valid( ) ) {
				continue;
			}

			if ( lag_record->m_broke_lc ) {
				best_record = {};
				break;
			}

			if ( lag_record->m_invalid_sim )
				continue;

			if ( lag_record->m_choked_cmds < 20
				&& !lag_record->m_dormant ) {
				aim_target_t target{};

				target.m_entry = const_cast < player_entry_t* > ( &entry );
				target.m_lag_record = lag_record;

				++scanned_count;
				auto hitgroups = 1;
				if ( ( scanned_count & 3 ) == 0 )
					hitgroups = cfg_hitgroups;

				scan_center_points( target, hitgroups, g_local_player->shoot_pos( ) );

				if ( !select_points( target, false ) ) {
					if ( !best_record ) {
						best_record = lag_record;
						pre_best_record = prev_lag_record;
					}

					continue;
				}

				if ( !best_record
					|| !best_point.has_value( ) ) {
					best_record = lag_record;
					best_point = *target.m_best_point;
					pre_best_record = prev_lag_record;

					continue;
				}

				const auto& pen_data = target.m_best_point->m_pen_data;
				const auto& best_pen_data = best_point.value( ).m_pen_data;

				if ( std::abs( pen_data.m_dmg - best_pen_data.m_dmg ) > 10 ) {
					if ( pen_data.m_dmg <= hp
						|| best_pen_data.m_dmg <= hp ) {
						if ( pen_data.m_dmg > best_pen_data.m_dmg ) {
							best_record = lag_record;
							best_point = *target.m_best_point;
							pre_best_record = prev_lag_record;
						}
					}
				}

				if ( pen_data.m_dmg > best_pen_data.m_dmg ) {
					best_record = lag_record;
					best_point = *target.m_best_point;
					pre_best_record = prev_lag_record;
				}
			}
		}

		lag_backup.restore( entry.m_player );

		if ( !best_record ) {
			return std::nullopt;
		}

		/*f( best_record->m_jittering )
			if ( latest_record.has_value( ) )
				if ( latest_record.value( ).m_lag_record->valid( ) ) {
					best_record = latest_record.value( ).m_lag_record;
					best_record->m_jitter_correction_applied = true;
				}
				*/

		if ( best_record->m_broke_lc )
			return std::nullopt;

		aim_record_t ret{ const_cast< player_entry_t* >( &entry ), best_record };

		return ret;
	}

	static std::vector<std::tuple<float, float, float>> precomputed_seeds = {};
	static const int total_seeds = 128;
	void build_seed_table( ) {
		if ( !precomputed_seeds.empty( ) )
			return;

		for ( auto i = 0; i < total_seeds; i++ ) {
			g_ctx->offsets( ).m_addresses.m_random_seed ( i + 1 );

			const auto pi_seed = g_ctx->offsets( ).m_addresses.m_random_float( 0.f, sdk::k_pi< float > * 2 );

			precomputed_seeds.emplace_back( g_ctx->offsets( ).m_addresses.m_random_float( 0.f, 1.f ),
				sin( pi_seed ), cos( pi_seed ) );
		}
	}

	sdk::vec2_t c_aim_bot::calc_spread_angle(
		const int bullets, const valve::e_item_index item_index,
		const float recoil_index, const std::size_t i
	) const {
		g_ctx->offsets( ).m_addresses.m_random_seed( i + 1u );

		auto v1 = g_ctx->offsets( ).m_addresses.m_random_float( 0.f, 1.f );
		auto v2 = g_ctx->offsets( ).m_addresses.m_random_float( 0.f, sdk::k_pi2<float> );

		float v3{}, v4{};

		using fn_t = void( __stdcall* )( valve::e_item_index, int, int, float*, float* );

		static auto weapon_accuracy_shotgun_spread_patterns = valve::g_cvar->find_var( ( "weapon_accuracy_shotgun_spread_patterns" ) );

		if ( weapon_accuracy_shotgun_spread_patterns->get_int( ) > 0 )
			reinterpret_cast< fn_t >( g_ctx->offsets( ).m_addresses.m_calc_shotgun_spread )( item_index, 0, static_cast< int >( bullets * recoil_index ), &v4, &v3 );
		else {
			v3 = g_ctx->offsets( ).m_addresses.m_random_float( 0.f, 1.f );
			v4 = g_ctx->offsets( ).m_addresses.m_random_float( 0.f, sdk::k_pi2<float> );
		}

		if ( recoil_index < 3.f
			&& item_index == valve::e_item_index::negev ) {
			for ( auto i = 3; i > recoil_index; --i ) {
				v1 *= v1;
				v3 *= v3;
			}

			v1 = 1.f - v1;
			v3 = 1.f - v3;
		}

		const auto inaccuracy = v1 * g_local_player->prediction( ).data( ).inaccuracy( );
		const auto spread = v3 * g_local_player->prediction( ).data( ).spread( );

		return {
			std::cos( v2 ) * inaccuracy + std::cos( v4 ) * spread,
			std::sin( v2 ) * inaccuracy + std::sin( v4 ) * spread
		};
	}

	void c_aim_bot::get_hitbox_data( 
		c_hitbox* rtn, valve::cs_player_t* ent, int ihitbox, valve::bones_t& matrix 
	) {
		if ( ihitbox < 0 || ihitbox > 19 ) return;

		if ( !ent ) return;

		const valve::model_t* const model = ent->renderable( )->model( );

		if ( !model )
			return;

		valve::studio_hdr_t* const studio_hdr = ent->mdl_data( )->m_studio_hdr;

		if ( !studio_hdr )
			return;

		valve::studio_bbox_t* const hitbox = studio_hdr->p_hitbox( ihitbox, ent->hitbox_set_index( ) );

		if ( !hitbox )
			return;

		const auto is_capsule = hitbox->m_radius != -1.f;

		sdk::vec3_t min, max;
		if ( is_capsule ) {
			min = hitbox->m_mins.transform( matrix[ hitbox->m_bone ] );
			max = hitbox->m_maxs.transform( matrix[ hitbox->m_bone ] );
		}
		else {
			min = sdk::vector_rotate( hitbox->m_mins, hitbox->m_rotation );
			max = sdk::vector_rotate( hitbox->m_maxs, hitbox->m_rotation );

			min = min.transform( matrix[ hitbox->m_bone ] );
			max = max.transform( matrix[ hitbox->m_bone ] );
		}

		rtn->m_hitbox_id = ihitbox;
		rtn->m_is_oob = !is_capsule;
		rtn->m_radius = hitbox->m_radius;
		rtn->m_mins = min;
		rtn->m_maxs = max;
		rtn->m_hitgroup = hitbox->m_group;
		rtn->m_hitbox = hitbox;
		rtn->m_start_scaled = g_local_player->shoot_pos( ).i_transform( matrix[ hitbox->m_bone ] );
		rtn->m_bone = hitbox->m_bone;
	}

	int c_aim_bot::calc_hit_chance(
		const aim_target_t* const target, const sdk::qang_t& angle, const aim_point_t* const aim_point
	) {
		build_seed_table( );

		if ( static_cast < int > ( g_local_player->prediction( ).data( ).inaccuracy( ) * 10000.f ) == 0 ) {
			return 100;
		}

		if ( ( g_local_player->weapon( )->item_index( ) == valve::e_item_index::ssg08 || g_local_player->weapon( )->item_index( ) == valve::e_item_index::revolver )
			&& !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) ) {
			if ( g_local_player->prediction( ).data( ).inaccuracy( ) < 0.009f )
				return 100;
		}

		const auto is_scope_able_weapon = g_local_player->weapon( )->item_index( ) == valve::e_item_index::scar20 || g_local_player->weapon( )->item_index( ) == valve::e_item_index::g3sg1;

		c_hitbox ht{};
		get_hitbox_data( &ht, target->m_entry->m_player, aim_point->m_hitbox, target->m_lag_record->m_anim_sides.at( target->m_lag_record->m_anim_side ).m_bones );

		sdk::vec3_t fwd{}, right{}, up{};
		sdk::ang_vecs( angle, &fwd, &right, &up );

		int hits{};

		constexpr auto k_max_seeds = 128u;

		auto min_dmg = get_min_dmg( ) + 1;

		if ( g_key_binds->get_keybind_state( &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override ) )
			min_dmg = get_min_dmg_override_val( );

		min_dmg = std::clamp( static_cast< int >( min_dmg * ( 2.f / 100.f ) ), 1, 100 );

		const auto trace_spread = [ ](
			const sdk::vec3_t& fwd, const sdk::vec3_t& right, const sdk::vec3_t& up,
			const aim_target_t* const target, const int accuracy_boost,
			const valve::e_item_index item_index, const float recoil_index,
			const int min_dmg, const std::size_t i, bool& hit ) {
				const auto spread_angle = g_aim_bot->calc_spread_angle( g_local_player->weapon_info( )->m_bullets, item_index, recoil_index, i );

				auto dir = fwd + ( right * spread_angle.x( ) ) + ( up * spread_angle.y( ) );

				dir.normalize( );

				const auto pen_data = g_auto_wall->fire_bullet(
					g_local_player->self( ), target->m_entry->m_player,
					g_local_player->weapon_info( ), item_index == valve::e_item_index::taser,
					g_local_player->shoot_pos( ), g_local_player->shoot_pos( ) + dir * g_local_player->weapon_info( )->m_range
				);

				hit = pen_data.m_dmg > 0;
		};

		const auto weapon_inaccuracy = g_local_player->prediction( ).data( ).inaccuracy( );

		sdk::vec3_t total_spread{}, end{};

		float inaccuracy{}, spread_x{}, spread_y{};

		std::tuple<float, float, float>* seed{};

		std::array< bool, k_max_seeds > seeds{};

		const auto item_index = g_local_player->weapon( )->item_index( );
		const auto recoil_index = g_local_player->weapon( )->recoil_index( );

		for ( std::size_t i{}; i < k_max_seeds; ++i ) {
			seed = &precomputed_seeds[ i ];

			inaccuracy = std::get<0>( *seed ) * weapon_inaccuracy;
			spread_x = std::get<2>( *seed ) * inaccuracy;
			spread_y = std::get<1>( *seed ) * inaccuracy;

			total_spread = ( fwd + right * spread_x + up * spread_y ).normalized( );

			end = g_local_player->shoot_pos( ) + ( total_spread * 8192.f );
			bool intersected = false;

			if ( ht.m_is_oob ) {
				sdk::vec3_t delta;
				delta = ( total_spread * 8192.f ).i_rotate( target->m_lag_record->m_anim_sides.at( target->m_lag_record->m_anim_side ).m_bones[ ht.m_bone ] );

				intersected = sdk::intersect_bb( ht.m_start_scaled, delta, ht.m_mins, ht.m_maxs );
			}
			else
			{
				intersected = sdk::intersect( g_local_player->shoot_pos( ), end, ht.m_mins, ht.m_maxs, ht.m_radius );
			}
			if ( intersected )
				trace_spread( fwd, right, up, target, 0, item_index, recoil_index, min_dmg, i, seeds.at( i ) );
		}

		for ( auto& hit : seeds )
			if ( hit )
				++hits;

		if ( static_cast< int >( ( hits / static_cast< float >( k_max_seeds ) ) * 100.f ) >= 30
			&& g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground
			&& is_scope_able_weapon
			&& !g_local_player->self( )->scoped( )
			&& g_local_player->can_shoot( ) ) {
			if ( g_local_player->prediction( ).data( ).min_inaccuracy( ) >= ( g_local_player->weapon( )->accuracy_penalty( ) * 0.02f ) )
				return 100;
		}

		return static_cast< int >( ( hits / static_cast< float >( k_max_seeds ) ) * 100.f );
	}


	void c_aim_bot::scan_point( const aim_target_t& target, aim_point_t& point, const bool ignore_dmg, sdk::vec3_t shoot_pos ) const {
		const auto hitbox_set = target.m_entry->m_player->mdl_data( )->m_studio_hdr->get_hitbox_set( target.m_entry->m_player->hitbox_set_index( ) );

		point.m_pen_data = g_auto_wall->fire_bullet(
			g_local_player->self( ), target.m_entry->m_player, g_local_player->weapon_info( ),
			g_local_player->weapon( )->item_index( ) == valve::e_item_index::taser, shoot_pos, point.m_pos
		);

		if ( point.m_pen_data.m_dmg < 1 )
			return;

		const auto& pen = point.m_pen_data;

		if ( !ignore_dmg ) {
			auto min_dmg = get_min_dmg( ) + 1;

			if ( g_key_binds->get_keybind_state( &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override ) )
				min_dmg = get_min_dmg_override_val( );

			if ( pen.m_dmg < target.m_entry->m_player->health( ) )
				if ( pen.m_dmg < min_dmg )
					return;
		}

		if ( point.m_needed_intersections <= 0 ) {
			point.m_valid = true;
			point.m_intersections = 3;

			return;
		}


		const auto intersect = [ ](
			const sdk::vec3_t& shoot_pos, const  sdk::vec3_t& point,
			const valve::studio_bbox_t* const hitbox, const  sdk::mat3x4_t& matrix
			) {
				sdk::vec3_t min{}, max{};

				min = hitbox->m_mins.transform( matrix );
				max = hitbox->m_maxs.transform( matrix );

				if ( hitbox->m_radius != 1.f )
					return sdk::segment_to_segment( shoot_pos, point, min, max ) < hitbox->m_radius;

				min = shoot_pos.i_transform( matrix );
				max = point.i_transform( matrix );

				return sdk::line_vs_bb( min, max, hitbox->m_mins, hitbox->m_maxs );
		};

		point.m_intersections = 1;
		point.m_head_intersections = 1;

		const auto& second_bones = target.m_lag_record->m_anim_sides.at( 2 ).m_bones;
		const auto& first_bones = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side ).m_bones;

		for ( int i{}; i < hitbox_set->m_hitboxes_count; ++i ) {
			const auto hitbox = hitbox_set->get_bbox( i );

			if ( !hitbox
				|| !intersect( shoot_pos, point.m_pos, hitbox, first_bones[ hitbox->m_bone ] ) || !intersect( shoot_pos, point.m_pos, hitbox, target.m_lag_record->m_anim_sides.at( 0 ).m_bones[ hitbox->m_bone ] ) )
				continue;

			point.m_intersections = 2;

			break;
		}

		for ( int i{}; i < hitbox_set->m_hitboxes_count; ++i ) {
			const auto hitbox = hitbox_set->get_bbox( i );
			if ( !hitbox
				|| !intersect( shoot_pos, point.m_pos, hitbox, second_bones[ hitbox->m_bone ] ) )
				continue;

			++point.m_intersections;

			break;
		}


		point.m_valid = point.m_intersections >= point.m_needed_intersections;

	}

	bool c_aim_bot::select_points( aim_target_t& target, bool additional_scan ) const {
		std::array< aim_point_t*, 11u > best_points{};

		const auto hp = target.m_entry->m_player->health( );
		lag_backup_t lag_backup{ target.m_entry->m_player };
		if ( additional_scan ) {
			target.m_lag_record->setup( target.m_entry->m_player, target.m_lag_record->m_anim_side );
		}

		for ( auto& point : target.m_points ) {
			if ( additional_scan )
				scan_point( target, point, false, g_local_player->shoot_pos( ) );

			if ( !point.m_valid
				|| point.m_pen_data.m_dmg < 1 )
				continue;

			auto& best_point = best_points.at( point.m_hitgroup );

			if ( !best_point ) {
				best_point = &point;

				continue;
			}

			const auto& best_pen_data = best_point->m_pen_data;
			const auto& pen_data = point.m_pen_data;

			if ( point.m_intersections == best_point->m_intersections ) {
				if ( point.m_center ) {
					if ( ( best_pen_data.m_hitgroup == pen_data.m_hitgroup && best_pen_data.m_remaining_pen == pen_data.m_remaining_pen )
						|| ( best_pen_data.m_remaining_pen == pen_data.m_remaining_pen && std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) <= 1 )
						|| ( best_pen_data.m_dmg > hp && pen_data.m_dmg > hp ) ) { 
						best_point = &point;

						continue;
					}
				}

				if ( pen_data.m_dmg > best_pen_data.m_dmg )
					best_point = &point;

				continue;
			}

			if ( best_pen_data.m_hitgroup != pen_data.m_hitgroup
				|| best_pen_data.m_remaining_pen != pen_data.m_remaining_pen ) {
				if ( best_pen_data.m_remaining_pen != pen_data.m_remaining_pen
					|| std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 ) {
					if ( best_pen_data.m_dmg <= hp && pen_data.m_dmg <= hp ) {
						if ( pen_data.m_dmg > best_pen_data.m_dmg )
							best_point = &point;

						continue;
					}
					else {
						if ( best_pen_data.m_dmg > hp )
							continue;

						best_point = &point;
					}
				}
			}

			if ( point.m_intersections > best_point->m_intersections )
				best_point = &point;
		}

		if ( additional_scan )
			lag_backup.restore( target.m_entry->m_player );

		{
			std::vector< aim_point_t > new_points{};

			for ( auto& best_point : best_points )
				if ( best_point )
					new_points.emplace_back( std::move( *best_point ) );

			target.m_points = new_points;
		}

		if ( target.m_points.empty( ) )
			return false;

		for ( auto& point : target.m_points ) {
			if ( !target.m_best_point )
				target.m_best_point = &point;
			else {
				const auto& best_pen_data = target.m_best_point->m_pen_data;
				const auto& pen_data = point.m_pen_data;

				bool should_change_point{};

				if ( std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 && ( pen_data.m_dmg <= hp || best_pen_data.m_dmg <= hp ) )
					should_change_point = pen_data.m_dmg > best_pen_data.m_dmg;

				if ( should_change_point )
					target.m_best_point = &point;
			}

			if ( point.m_hitgroup == 2
				|| point.m_hitgroup == 3 ) {
				if ( !target.m_best_body_point )
					target.m_best_body_point = &point;
				else {
					const auto& best_pen_data = target.m_best_body_point->m_pen_data;
					const auto& pen_data = point.m_pen_data;

					bool should_change_point{};
					if ( std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 && ( pen_data.m_dmg <= hp || best_pen_data.m_dmg <= hp ) )
						should_change_point = pen_data.m_dmg > best_pen_data.m_dmg;

					if ( should_change_point )
						target.m_best_body_point = &point;
				}
			}
		}

		return true;
	}

	std::size_t c_aim_bot::calc_points_count( const int hitgroups, const int multi_points ) const {
		std::size_t ret{};

		for ( std::size_t i{}, fl = 1; i < m_hitgroups.size( ); ++i, fl <<= 1 ) {
			if ( hitgroups & fl ) {
				if ( !( multi_points & fl ) )
					++ret;
				else {
					for ( auto& hitbox : m_hitgroups.at( i ) ) {
						if ( hitbox == 11 || hitbox == 12 )
							ret += 3u;
						else if ( hitbox ) {
							if ( hitbox == 3 || hitbox == 2 )
								ret += 4u;
							else if ( hitbox == 6 || hitbox == 4 || hitbox == 5 )
								ret += 2u;
							else
								ret += 1u;
						}
						else
							ret += 6u;
					}
				}
			}
		}

		return ret;
	}

	void c_aim_bot::scan_center_points( aim_target_t& target, const int hitgroups, sdk::vec3_t shoot_pos ) const {
		const auto& anim_side = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side );

		const auto hitbox_set = target.m_entry->m_player->mdl_data( )->m_studio_hdr->get_hitbox_set( target.m_entry->m_player->hitbox_set_index( ) );

		const auto points_count = calc_points_count( hitgroups, 0 );

		target.m_points.reserve( points_count );

		for ( std::size_t i{}; i < m_hitgroups.size( ); ++i ) {
			if ( !( ( 1 << i ) & hitgroups ) )
				continue;

			for ( const auto& index : m_hitgroups.at( i ) ) {
				const auto hitbox = hitbox_set->get_bbox( index );
				if ( !hitbox )
					continue;

				sdk::vec3_t point{};

				point = ( ( hitbox->m_mins + hitbox->m_maxs ) / 2.f ).transform( anim_side.m_bones[ hitbox->m_bone ] );

				target.m_points.emplace_back( std::move( point ), true, index, hitbox->m_group, 0 );
			}
		}

		target.m_lag_record->setup( target.m_entry->m_player, target.m_lag_record->m_anim_side );

		for ( auto& point : target.m_points )
			scan_point( target, point, true, shoot_pos );
	}

	void c_aim_bot::calc_capsule_points(
		aim_target_t& target, const valve::studio_bbox_t* const hitbox,
		const int index, const sdk::mat3x4_t& matrix, float scale
	) const {
		sdk::vec3_t min{}, max{};


		min = hitbox->m_mins.transform( matrix );
		max = hitbox->m_maxs.transform( matrix );

		static auto matrix0 = sdk::vector_matrix( { 0.f, 0.f, 1.f } );

		auto matrix1 = sdk::vector_matrix( ( max - min ).normalized( ) );

		for ( const auto& vertices : {
				sdk::vec3_t{ 0.95f, 0.f, 0.f },
				sdk::vec3_t{ -0.95f, 0.f, 0.f },
				sdk::vec3_t{ 0.f, 0.95f, 0.f },
				sdk::vec3_t{ 0.f, -0.95f, 0.f },
				sdk::vec3_t{ 0.f, 0.f, 0.95f },
				sdk::vec3_t{ 0.f, 0.f, -0.95f }
			} ) {
			sdk::vec3_t point{};

			point = vertices.rotate( matrix0 );
			point = point.rotate( matrix1 );

			point *= scale;

			if ( vertices.z( ) > 0.f )
				point += min - max;

			target.m_points.emplace_back( point + max, false, index, hitbox->m_group, 0 );
		}
	}

	float c_aim_bot::calc_point_scale(
		const float spread, const float max,
		const float dist, const sdk::vec3_t& dir,
		const sdk::vec3_t& right, const sdk::vec3_t& up
	) const {
		const auto v1 = g_local_player->prediction( ).data( ).inaccuracy( ) + spread;

		auto v28 = right * v1 + dir + up * v1;

		v28.normalize( );

		const auto delta = sdk::angle_diff( sdk::to_deg( std::atan2( dir.y( ), dir.x( ) ) ), sdk::to_deg( std::atan2( v28.y( ), v28.x( ) ) ) );

		const auto v23 = max + dist / std::tan( sdk::to_rad( 180.f - ( delta + 90.f ) ) );
		if ( v23 > max )
			return 1.f;

		float v25{};
		if ( v23 >= 0.f )
			v25 = v23;

		return v25 / max;
	}


	void c_aim_bot::calc_multi_points(
		aim_target_t& target, const valve::studio_bbox_t* const hitbox,
		const int index, const sdk::mat3x4_t& matrix, float scale
	) const {
		sdk::vec3_t point{};

		const auto center = ( hitbox->m_mins + hitbox->m_maxs ) / 2.f;

		point = center.transform( matrix );

		target.m_points.emplace_back( point, true, index, hitbox->m_group, 0 );

		if ( !static_point_scale( ) ) {
			const auto max = ( hitbox->m_maxs - hitbox->m_mins ).length( ) * 0.5f + hitbox->m_radius;

			auto dir = ( point - g_local_player->shoot_pos( ) );

			const auto dist = dir.normalize( );

			sdk::vec3_t right{}, up{};

			if ( dir.x( ) == 0.f
				&& dir.y( ) == 0.f ) {
				right = { 0.f, -1.f, 0.f };
				up = { -dir.z( ), 0.f, 0.f };
			}
			else {
				right = dir.cross( { 0.f, 0.f, 1.f } ).normalized( );
				up = right.cross( dir ).normalized( );
			}

			scale = calc_point_scale( g_local_player->prediction( ).data( ).spread( ), max, dist, dir, right, up );
			if ( scale <= 0.f
				&& g_local_player->prediction( ).data( ).spread( ) > g_local_player->prediction( ).data( ).min_inaccuracy( ) )
				scale = calc_point_scale( g_local_player->prediction( ).data( ).min_inaccuracy( ), max, dist, dir, right, up );
		}

		if ( scale <= 0.f )
			return;

		if ( hitbox->m_radius <=0.f ) {
			point = { center.x( ) + ( hitbox->m_mins.x( ) - center.x( ) ) * scale, center.y( ), center.z( ) };

			point = point.transform( matrix );

			target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

			point = { center.x( ) + ( hitbox->m_maxs.x( ) - center.x( ) ) * scale, center.y( ), center.z( ) };

			point = point.transform( matrix );

			target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

			return;
		}

		if ( index ) {
			if ( index == 3 ) {
				if ( !static_point_scale( ) && scale > 0.9f )
					scale = 0.9f;
			}
			else {
				if ( index != 2
					&& index != 6 ) {
					if ( index == 4
						|| index == 5 ) {
						if ( !static_point_scale( ) && scale > 0.9f )
							scale = 0.9f;

						point = { center.x( ), hitbox->m_maxs.y( ) - hitbox->m_radius * scale, center.z( ) };

						point = point.transform( matrix );

						target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );
					}

					return;
				}

				if ( !static_point_scale( ) && scale > 0.9f )
					scale = 0.9f;

				if ( index == 6 ) {
					point = { center.x( ), hitbox->m_maxs.y( ) - hitbox->m_radius * scale, center.z( ) };

					point = point.transform( matrix );

					target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

					return;
				}
			}

			return calc_capsule_points( target, hitbox, index, matrix, scale );
		}

		point = { hitbox->m_maxs.x( ) + 0.70710678f * ( hitbox->m_radius * scale ), hitbox->m_maxs.y( ) - 0.70710678f * ( hitbox->m_radius * scale ), hitbox->m_maxs.z( ) };

		point = point.transform( matrix );

		target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_maxs.x( ), hitbox->m_maxs.y( ), hitbox->m_maxs.z( ) + hitbox->m_radius * scale };

		point = point.transform( matrix );

		target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_maxs.x( ), hitbox->m_maxs.y( ), hitbox->m_maxs.z( ) - hitbox->m_radius * scale };

		point = point.transform( matrix );

		target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_maxs.x( ), hitbox->m_maxs.y( ) - hitbox->m_radius * scale, hitbox->m_maxs.z( ) };

		point = point.transform( matrix );

		target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

		if ( target.m_entry->m_player->anim_state( ) && target.m_lag_record->m_velocity.length( 2u ) <= 0.1f && target.m_lag_record->m_eye_angles.x( ) <= target.m_entry->m_player->anim_state( )->m_aim_pitch_max )
		{
			point = { hitbox->m_maxs.x( ) - hitbox->m_radius * scale, hitbox->m_maxs.y( ), hitbox->m_maxs.z( ) };

			point = point.transform( matrix );

			target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );
		}
	}
	
	void c_aim_bot::scan_points(
		aim_target_t& target, const int hitgroups, const int multi_points, const bool trace, sdk::vec3_t shoot_pos
	) const {
		const auto hp = target.m_entry->m_player->health( );

		const auto& anim_side = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side );

		const auto hitbox_set = target.m_entry->m_player->mdl_data( )->m_studio_hdr->get_hitbox_set( target.m_entry->m_player->hitbox_set_index( ) );

		const auto points_count = calc_points_count( hitgroups, multi_points );

		target.m_points.reserve( points_count );

		const auto force_safe_point = g_key_binds->get_keybind_state( &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points );

		const auto is_taser = g_local_player->weapon( )->item_index( ) == valve::e_item_index::taser;

		for ( std::size_t i{}; i < m_hitgroups.size( ); ++i ) {

			if ( !( ( 1 << i ) & hitgroups )
				&& ( i != 2 ) )
				continue;

			if ( ( ( 1 << i ) & multi_points ) ) {
				const auto scale = ( i ? get_body_scale( ) : get_head_scale( ) ) / 100.f;

				for ( const auto& index : m_hitgroups.at( i ) ) {
					const auto hitbox = hitbox_set->get_bbox( index );
					if ( !hitbox )
						continue;

					calc_multi_points( target, hitbox, index, anim_side.m_bones[ hitbox->m_bone ], scale );
				}
			}
			else {
				for ( const auto& index : m_hitgroups.at( i ) ) {
					const auto hitbox = hitbox_set->get_bbox( index );
					if ( !hitbox )
						continue;

					sdk::vec3_t point{};

					point = ( ( hitbox->m_mins + hitbox->m_maxs ) / 2.f ).transform( anim_side.m_bones[ hitbox->m_bone ] );

					target.m_points.emplace_back( std::move( point ), true, index, hitbox->m_group, 0 );
				}
			}
		}

		for ( auto& point : target.m_points ) {

			if ( is_taser )
				point.m_needed_intersections = 2;

			if ( force_safe_point ) {
				point.m_needed_intersections = 3;

				continue;
			}

			if ( target.m_lag_record->m_force_safe_points ) {
				point.m_needed_intersections = 3;
			}
		}

		if ( !trace )
			return;

		lag_backup_t lag_backup{ target.m_entry->m_player };

		target.m_lag_record->setup( target.m_entry->m_player, target.m_lag_record->m_anim_side );

		for ( auto& point : target.m_points )
			scan_point( target, point, false, shoot_pos );

		lag_backup.restore( target.m_entry->m_player );
	}


	void c_aim_bot::scan_targets( ) {
		for ( auto& target : m_targets ) {
			g_aim_bot->scan_points( target, get_hit_groups( ), get_multi_points( ), false );
		}

		m_targets.erase(
			std::remove_if(
				m_targets.begin( ), m_targets.end( ),
				[ & ]( aim_target_t& target ) {
					return !select_points( target, true );
				}
			),
			m_targets.end( )
					);
		
	}

	void c_aim_bot::find_targets( ) {
		std::vector< std::future< std::optional< aim_record_t > > > results{};
		std::vector < std::optional < aim_record_t > > results_no_thread{};

		m_targets.reserve( valve::g_global_vars.get( )->m_max_clients );

		for ( auto i = 1; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
			auto& entry = g_lag_comp->entry( i - 1 );
			if ( !entry.m_player
				|| entry.m_player->networkable( )->dormant( )
				|| !entry.m_player->alive( )
				|| entry.m_player->friendly( g_local_player->self( ) )
				|| entry.m_lag_records.empty( ) )
				continue;

			if ( g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_threading ) {
				results.emplace_back(
					sdk::g_thread_pool->enqueue(
						&c_aim_bot::select_record,
						g_aim_bot.get( ), std::ref( entry )
					)
				);

				sdk::g_thread_pool->wait( );
			}
			else
				results_no_thread.emplace_back( select_record( entry ) );

		}

		if ( g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_threading ) {
			for ( auto& result : results ) {
				const auto& record = result.get( );
				if ( !record.has_value( ) )
					continue;

				m_targets.emplace_back( record.value( ) );
			}
		}
		else {
			for ( auto& result : results_no_thread ) {
				const auto& record = result;
				if ( !record.has_value( ) )
					continue;

				m_targets.emplace_back( record.value( ) );
			}
		}
	}

	void c_knife_bot::handle_knife_bot( valve::user_cmd_t& cmd ) {
		m_best_index = -1;

		if ( !g_local_player->self( )->weapon( ) )
			return;

		if ( !g_local_player->self( )->weapon( )->is_knife( ) )
			return;

		if ( !select_target( ) )
			return;

		auto best_angle = get_hitbox_pos( 5, m_best_player );
		auto entity_angle = sdk::calc_ang( g_local_player->shoot_pos( ), best_angle );
		auto health = m_best_player->health( );

		auto stab = false;

		if ( m_best_player->has_heavy_armor( ) )
		{
			if ( health <= 55 && health > get_min_dmg( ) )
				stab = true;
		}
		else
		{
			if ( health <= 65 && health > get_min_dmg( ) )
				stab = true;
		}

		if ( health >= 90 )
			stab = true;

		stab = stab && m_best_distance < 60;

		cmd.m_view_angles = entity_angle;

		if ( stab )
			cmd.m_buttons |= valve::e_buttons::in_attack2;
		else
			cmd.m_buttons |= valve::e_buttons::in_attack;

		cmd.m_tick = valve::to_ticks( m_best_player->sim_time( ) + g_local_player->net_info( ).m_lerp );
	}
	__forceinline float dist_to( sdk::vec3_t& from, const sdk::vec3_t& value ) { return ( from - value ).length( ); }

	bool c_knife_bot::select_target( ) {
		float good_distance = 75.0f;

		for ( int i = 1; i <= valve::g_global_vars.get( )->m_max_clients; i++ )
		{
			valve::cs_player_t* player = ( valve::cs_player_t* ) valve::g_entity_list.get( )->get_entity( i );

			if ( !player || player == g_local_player->self( )
				|| player->networkable( )->dormant( ) || !player->alive( ) 
				|| player->team( ) == g_local_player->self( )->team( ) )
				continue;

			sdk::vec3_t local_position = g_local_player->self( )->origin( );
			local_position.z( ) += 50.0f;

			sdk::vec3_t entity_position = player->abs_origin( );
			entity_position.z( ) += 50.0f;

			float current_distance = dist_to( local_position, entity_position );

			if ( current_distance < good_distance )
			{
				good_distance = current_distance;
				m_best_index = i;
				m_best_player = player;
			}
		}

		m_best_distance = good_distance;

		return m_best_index != -1;
	}
}