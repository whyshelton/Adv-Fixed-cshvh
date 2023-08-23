#include "../../csgo.hpp"
namespace hooks {
	void __fastcall override_view(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		static bool keep_height{};

		if ( valve::g_engine.get( )->in_game( ) ) {		
			if ( g_local_player->self( )->alive( ) ) {
				if ( hacks::g_move->should_fake_duck( ) ) {
					keep_height = true;

					setup->m_origin.z( ) = g_local_player->self( )->abs_origin( ).z( ) + 64.0625f;
				}
				else if ( keep_height ) {
					const auto& local_data = g_local_player->prediction( ).data( ).local_data( ).at( g_local_player->last_cmd_number( ) % 150 );
					if ( local_data.m_net_vars.m_duck_amount != local_data.m_pred_net_vars.m_duck_amount ) {
						if ( local_data.m_pred_net_vars.m_duck_amount > 0.05f )
							setup->m_origin.z( ) = g_local_player->self( )->abs_origin( ).z( ) + 64.0625f;
						else
							keep_height = false;
					}
					else
						keep_height = false;
				}
			}
			else {
				keep_height = false;
			}

			if ( setup )
				setup->m_fov = g_menu->main( ).m_misc_cfg.get( ).m_camera_distance;

			hacks::g_misc->third_person( );
		}
		else
			keep_height = false;

		o_override_view( ecx, edx, setup );
	}

	int __fastcall do_post_screen_space_effects(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		hacks::g_visuals->draw_shot_mdl( );

		return o_do_post_screen_space_effects( ecx, edx, setup );
	}
}