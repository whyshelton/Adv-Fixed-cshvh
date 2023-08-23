#include "../../../csgo.hpp"

namespace hacks {
    bool c_models::on_draw_model( valve::studio_render_t* ecx, std::uintptr_t edx, std::uintptr_t results, const valve::draw_model_info_t& info,
        sdk::mat3x4_t* bones, float* flex_weights, float* flex_delayed_weights, const sdk::vec3_t& origin, int flags
    ) {
        const auto& cfg = g_menu->main( ).m_models_cfg.get( );

        if ( !valve::g_engine.get( )->in_game( )
            || !info.m_studio
            || !info.m_renderable 
            || ecx->is_forced_material_override ( ) )
            return false; 

        const auto entity = reinterpret_cast< valve::base_entity_t* const >( info.m_renderable - 0x4 );

        if ( !entity )
            return false;

        if ( g_visuals->m_rendering_shot_mdl )
            return false;

        const auto client_class = entity->networkable( )->client_class( );

        if ( !client_class )
            return false;

        static bool chams_inited{};

        if ( !chams_inited )
            create_materials( );

        chams_inited = true;

        const auto move_parent = valve::g_entity_list.get( )->get_entity( entity->move_parent( ) );
        const auto move_parent_player = move_parent && move_parent->networkable( )->index( ) >= 1 && move_parent->networkable( )->index( ) <= 64;
        const auto player_mdl = *reinterpret_cast< const std::uint32_t* >( info.m_studio->m_name + 14u ) == 'yalp';

        if ( player_mdl ) {
            if ( static_cast < valve::e_class_id >( client_class->m_class_id ) == valve::e_class_id::ragdoll ) {
                if ( cfg.m_ragdolls
                    && !g_local_player->self ( )->friendly ( ( valve::cs_player_t* )entity ) ) {
                    const auto col = sdk::col_t( cfg.m_ragdolls_clr.at( 0 ) * 255, cfg.m_ragdolls_clr.at( 1 ) * 255,
                        cfg.m_ragdolls_clr.at( 2 ) * 255, cfg.m_ragdolls_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_ragdolls_type, true );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_ragdolls_glow_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_ragdolls_glow_outline_clr.at( 0 ) * 255, cfg.m_ragdolls_glow_outline_clr.at( 1 ) * 255,
                            cfg.m_ragdolls_glow_outline_clr.at( 2 ) * 255, cfg.m_ragdolls_glow_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, true );

                        hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );

                    return true;
                }

                return false;
            }
            const auto is_friendly = g_local_player->self( )->friendly( ( valve::cs_player_t* )entity );
            const auto is_local = g_local_player->self( ) == entity;

            if ( !is_friendly ) {
                if ( cfg.m_player_occluded ) {
                    const auto col = sdk::col_t( cfg.m_player_occluded_clr.at( 0 ) * 255, cfg.m_player_occluded_clr.at( 1 ) * 255,
                        cfg.m_player_occluded_clr.at( 2 ) * 255, cfg.m_player_occluded_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_player_occluded_type, true );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_player_occluded_glow_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_player_glow_occluded_outline_clr.at( 0 ) * 255, cfg.m_player_glow_occluded_outline_clr.at( 1 ) * 255,
                            cfg.m_player_glow_occluded_outline_clr.at( 2 ) * 255, cfg.m_player_glow_occluded_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, true );

                        hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );
                }

                if ( cfg.m_player ) {
                    const auto col = sdk::col_t( cfg.m_player_clr.at( 0 ) * 255, cfg.m_player_clr.at( 1 ) * 255,
                        cfg.m_player_clr.at( 2 ) * 255, cfg.m_player_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_player_type, false );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_player_glow_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_player_glow_outline_clr.at( 0 ) * 255, cfg.m_player_glow_outline_clr.at( 1 ) * 255,
                            cfg.m_player_glow_outline_clr.at( 2 ) * 255, cfg.m_player_glow_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, false );

                        hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );
                    
                    return true;
                }
            }
            else if ( is_local ) {
                if ( cfg.m_local_player_fake ) {
                    const auto col = sdk::col_t( cfg.m_local_player_fake_clr.at( 0 ) * 255, cfg.m_local_player_fake_clr.at( 1 ) * 255,
                        cfg.m_local_player_fake_clr.at( 2 ) * 255, cfg.m_local_player_fake_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_local_player_fake_type, true );

                    hooks::o_draw_model( ecx, edx, results, info, g_local_player->anim_sync( ).m_anim_data.m_fake.m_bones.data( ), flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_local_player_fake_glow_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_local_player_fake_glow_outline_clr.at( 0 ) * 255, cfg.m_local_player_fake_glow_outline_clr.at( 1 ) * 255,
                            cfg.m_local_player_fake_glow_outline_clr.at( 2 ) * 255, cfg.m_local_player_fake_glow_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, true );

                        hooks::o_draw_model( ecx, edx, results, info, g_local_player->anim_sync( ).m_anim_data.m_fake.m_bones.data( ), flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );
                }

                if ( cfg.m_local_player ) {
                    const auto col = sdk::col_t( cfg.m_local_player_clr.at( 0 ) * 255, cfg.m_local_player_clr.at( 1 ) * 255,
                        cfg.m_local_player_clr.at( 2 ) * 255, cfg.m_local_player_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_local_player_type, false );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_local_player_glow_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_local_player_glow_outline_clr.at( 0 ) * 255, cfg.m_local_player_glow_outline_clr.at( 1 ) * 255,
                            cfg.m_local_player_glow_outline_clr.at( 2 ) * 255, cfg.m_local_player_glow_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, false );

                        hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );

                    return true;
                }
            }
        }
        else if ( *reinterpret_cast< const std::uint32_t* >( info.m_studio->m_name + 17u ) == 'smra' ) {
            if ( cfg.m_hands ) {
                const auto col = sdk::col_t( cfg.m_hands_clr.at( 0 ) * 255, cfg.m_hands_clr.at( 1 ) * 255,
                    cfg.m_hands_clr.at( 2 ) * 255, cfg.m_hands_clr.at( 3 ) * 255 );

                set_clr( col, cfg.m_hands_type, false );

                hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                if ( cfg.m_hands_glow_outline ) {
                    const auto outline_col = sdk::col_t( cfg.m_hands_glow_outline_clr.at( 0 ) * 255, cfg.m_hands_glow_outline_clr.at( 1 ) * 255,
                        cfg.m_hands_glow_outline_clr.at( 2 ) * 255, cfg.m_hands_glow_outline_clr.at( 3 ) * 255 );

                    set_clr( outline_col, 4, false );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                }

                valve::g_studio_render->forced_material_override( nullptr );

                return true;
            }
        }
        else if ( *reinterpret_cast< const std::uint32_t* >( info.m_studio->m_name ) == 'paew'
            && info.m_studio->m_name[ 8 ] == 'v' ) {
                if ( cfg.m_local_weapon ) {
                    const auto col = sdk::col_t( cfg.m_local_weapon_clr.at( 0 ) * 255, cfg.m_local_weapon_clr.at( 1 ) * 255,
                        cfg.m_local_weapon_clr.at( 2 ) * 255, cfg.m_local_weapon_clr.at( 3 ) * 255 );

                    set_clr( col, cfg.m_local_weapon_type, false );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                    if ( cfg.m_local_weapon_outline ) {
                        const auto outline_col = sdk::col_t( cfg.m_local_weapon_outline_clr.at( 0 ) * 255, cfg.m_local_weapon_outline_clr.at( 1 ) * 255,
                            cfg.m_local_weapon_outline_clr.at( 2 ) * 255, cfg.m_local_weapon_outline_clr.at( 3 ) * 255 );

                        set_clr( outline_col, 4, false );

                        hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                    }

                    valve::g_studio_render->forced_material_override( nullptr );

                    return true;
                }
        }
        else if ( move_parent_player ) {
            if ( cfg.m_weapons ) {
                const auto col = sdk::col_t( cfg.m_weapons_clr.at( 0 ) * 255, cfg.m_weapons_clr.at( 1 ) * 255,
                    cfg.m_weapons_clr.at( 2 ) * 255, cfg.m_weapons_clr.at( 3 ) * 255 );

                set_clr( col, cfg.m_weapons_type, false );

                hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );

                if ( cfg.m_weapons_outline ) {
                    const auto outline_col = sdk::col_t( cfg.m_weapons_outline_clr.at( 0 ) * 255, cfg.m_weapons_outline_clr.at( 1 ) * 255,
                        cfg.m_weapons_outline_clr.at( 2 ) * 255, cfg.m_weapons_outline_clr.at( 3 ) * 255 );

                    set_clr( outline_col, 4, false );

                    hooks::o_draw_model( ecx, edx, results, info, bones, flex_weights, flex_delayed_weights, origin, flags );
                }

                valve::g_studio_render->forced_material_override( nullptr );

                return true;
            }
        }

        return false;
    }
}