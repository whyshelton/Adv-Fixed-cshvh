#include "../../csgo.hpp"

namespace hooks {
    void __fastcall paint_traverse( const std::uintptr_t ecx, const std::uintptr_t edx,
        const std::uint32_t id, bool force_repaint, bool allow_force
    ) {
        std::string panel_name = valve::g_panel->get_name( id );
        if ( strstr( valve::g_panel->get_name( id ), ( "HudZoom" ) ) )
            if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_scope )
                return;

        o_paint_traverse( ecx, edx, id, force_repaint, allow_force );

        static auto draw_panel_id = 0u;

        if ( !draw_panel_id ) {
            if ( panel_name.compare( ( "MatSystemTopPanel" ) ) )
                return;
                
            draw_panel_id = id;
        }

        if ( id != draw_panel_id )
            return;

        csgo::g_render->m_draw_list->Clear( );
        csgo::g_render->m_draw_list->PushClipRectFullScreen( );

        csgo::g_render->m_screen_size = *reinterpret_cast< sdk::vec2_t* >( &ImGui::GetIO( ).DisplaySize );

        hacks::g_visuals->handle_player_drawings( );

        hacks::g_visuals->draw_bullet_tracers( );

        hacks::g_visuals->draw_scope_lines( );

        hacks::g_visuals->draw_enemy_bullet_tracers( );

        hacks::g_visuals->draw_hitmarkers( );

        hacks::g_logs->draw_data( );

        hacks::g_visuals->handle( );

        hacks::g_visuals->draw_auto_peek( );

        static auto last_server_tick = valve::g_client_state->m_server_tick;
        if ( valve::g_client_state->m_server_tick != last_server_tick ) {
            hacks::g_visuals->m_throwed_grenades.clear( );

            last_server_tick = valve::g_client_state->m_server_tick;
        }
        auto ent_id = valve::g_entity_list.get( )->highest_index( );

        for ( int i{}; i <= ent_id; ++i ) {

            const auto entity = valve::g_entity_list.get( )->get_entity( i );
            if ( !entity )
                continue;

            if ( entity->is_player( ) )
            {
                // who 
            }
            else if ( const auto client_class = entity->networkable( )->client_class( ) ) {
                if ( entity->networkable( )->dormant( ) )
                    continue;

                if ( entity->is_weapon( ) )
                {
                }
                else {
                    hacks::g_visuals->handle_warning_pred( entity, static_cast < valve::e_class_id > ( client_class->m_class_id ) );
                }
            }
        }

        hacks::g_visuals->add_grenade_simulation( hacks::g_visuals->m_grenade_trajectory, false, false );

        {
            const auto lock = std::unique_lock<std::mutex>( csgo::g_render->m_mutex );

            *csgo::g_render->m_data_draw_list = *csgo::g_render->m_draw_list;
        }
    }
}