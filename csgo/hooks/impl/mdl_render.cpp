#include "../../csgo.hpp"

namespace hooks {
    void __stdcall draw_mdl_exec(
        uintptr_t ctx, const valve::draw_model_state_t& state, const valve::model_render_info_t& info, sdk::mat3x4_t* bones
    ) {
        if ( !info.m_renderable )
            return o_draw_mdl_exec( valve::g_mdl_render, ctx, state, info, bones );

        const auto entity = reinterpret_cast< valve::base_entity_t* const >( info.m_renderable - 0x4 );

        if ( entity ) {
            if ( entity->is_player( ) ) {
                auto player = static_cast < valve::cs_player_t* > ( entity );

                if ( player == g_local_player->self( ) ) {
                    if ( valve::g_input->m_camera_in_third_person && g_local_player->self( )->scoped( )
                        && g_menu->main( ).m_visuals_cfg.get( ).m_blend_in_scope ) {
                        valve::g_render_view->set_blend( g_menu->main( ).m_visuals_cfg.get( ).m_blend_in_scope_val / 100.f );
                    }
                }
            }
        }

        o_draw_mdl_exec( valve::g_mdl_render, ctx, state, info, bones );
    }
}