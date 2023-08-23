#include "../../csgo.hpp"

namespace hooks {
    NAKED void __stdcall create_move_proxy( int seq_number, float input_sample_frame_time, bool active ) {
        __asm {
            push ebp
            mov ebp, esp

            push ebx
            push esp
            push dword ptr[ ebp + 16 ]
            push dword ptr[ ebp + 12 ]
            push dword ptr[ ebp + 8 ]

            call create_move

            pop ebx

            pop ebp
            retn 12
        }
    }

    void __stdcall create_move( int seq_number, float input_sample_frame_time, bool active, bool& send_packet ) {
        o_create_move( valve::g_client, seq_number, input_sample_frame_time, active );

        if ( !valve::g_engine.get( )->in_game( )
            || !g_local_player->self( )
            || !g_local_player->self( )->alive( ) ) {
            g_local_player->net_cmds( ).clear( );
            return;
        }

        const auto slot = seq_number % valve::k_mp_backup;

        g_local_player->create_move( send_packet,
            valve::g_input->m_cmds[ slot ], valve::g_input->m_vfyd_cmds[ slot ]
        );
    }

    void __cdecl velocity_modifier( valve::recv_proxy_data_t* const data, valve::base_entity_t* const entity, void* const out ) {
        o_velocity_modifier( data, entity, out );

        if ( !g_local_player || !g_local_player->self( )
            || entity->networkable( )->index( ) != g_local_player->self( )->networkable( )->index( )
            || data->m_value.m_float == g_local_player->prediction( ).data( ).net_velocity_modifier ( ) )
            return;

        g_local_player->prediction( ).data( ).net_velocity_modifier( ) = data->m_value.m_float;

        valve::g_prediction.get( )->m_prev_ack_had_errors = true;
    }

    void __fastcall physics_simulate( valve::cs_player_t* const ecx, const std::uintptr_t edx ) { 
        if ( ecx != g_local_player->self( )
            || !ecx->alive( )
            || ecx->sim_tick( ) == valve::g_global_vars.get( )->m_tick_count )
            return o_physics_simulate( ecx, edx );

        auto& user_cmd = ecx->cmd_context( ).m_user_cmd;

        if ( user_cmd.m_tick >= std::numeric_limits< int >::max( ) ) {
            ecx->sim_tick( ) = valve::g_global_vars.get( )->m_tick_count;

            return g_local_player->prediction( ).data( ).net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );
        }

        if ( user_cmd.m_tick >= ( valve::g_global_vars.get( )->m_tick_count + ( 1.f / valve::g_global_vars.get( )->m_interval_per_tick ) + 8 ) ) {
            user_cmd.m_predicted = true;
            return;
        }

        if ( user_cmd.m_number == ( valve::g_client_state->m_cmd_ack + 1 ) ) {
            ecx->velocity_modifier( ) = g_local_player->prediction( ).data( ).net_velocity_modifier ( );
        }

        bool in_attack = user_cmd.m_buttons & ( valve::e_buttons::in_attack | valve::e_buttons::in_attack2 );

        g_local_player->prediction( ).data( ).net_vars( ).at( user_cmd.m_number % 150 ).m_r8 = { ecx->tick_base( ), in_attack,
                                                                                 g_local_player->can_shoot( true, 0, true ) };
        const auto weapon = ecx->weapon( );

        if ( weapon ) {
            weapon->postpone_fire_ready_time( ) = std::numeric_limits< float >::max( );

            if ( weapon->item_index( ) == valve::e_item_index::revolver ) {
                const auto max_cmds = ( int( 1.0f / valve::g_global_vars.get( )->m_interval_per_tick ) ) / 2;
                if ( max_cmds > 1 ) {
                    auto v27 = 0;

                    auto v15 = user_cmd.m_number - 1;

                    for ( auto i = 1u; i < max_cmds; ++i ) {
                        v27 = v15;

                        const auto& r8_data = g_local_player->prediction( ).data( ).net_vars( ).at( v15 % 150 ).m_r8;

                        if ( !r8_data.m_in_attack
                            || !r8_data.m_can_shoot )
                            break;

                        --v15;
                    }

                    if ( v27 ) {
                        const auto v17 = 1 + static_cast< int >( 0.03348f / valve::g_global_vars.get( )->m_interval_per_tick );
                        if ( user_cmd.m_number - v27 >= v17 )
                            weapon->postpone_fire_ready_time( ) = valve::to_time( g_local_player->prediction( ).data( ).net_vars( ).at( ( v27 + v17 ) % 150 ).m_r8.m_tick ) + 0.2f;
                    }
                }
            }
        }

        const auto backup_tick_base = ecx->tick_base( );

        const auto& local_data = g_local_player->prediction( ).data( ).local_data ( ).at( user_cmd.m_number % 150 );
        if ( local_data.m_spawn_time == ecx->spawn_time( ) && local_data.m_override_tick_base ) {
            ecx->tick_base( ) = local_data.m_adjusted_tick_base;
        }

        valve::g_global_vars.get( )->m_cur_time = valve::to_time( ecx->tick_base( ) );

        g_local_player->prediction( ).data( ).net_vars( ).at( ( user_cmd.m_number - 1 ) % 150 ).restore( user_cmd.m_number - 1 );

        o_physics_simulate( ecx, edx );

        if ( local_data.m_spawn_time == ecx->spawn_time( )
            && local_data.m_override_tick_base && local_data.m_restore_tick_base ) {
            ecx->tick_base( ) = backup_tick_base + ecx->tick_base( ) - local_data.m_adjusted_tick_base;
        }

        g_local_player->prediction( ).data( ).net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );
        g_local_player->prediction( ).view_model( ).store_view_model( );
    }

    void __stdcall frame_stage_notify( const valve::e_frame_stage stage ) {
        if ( !g_local_player->self( ) )
            return o_frame_stage_notify( stage );

        g_local_player->last_frame_stage( ) = stage;

        const auto in_game = valve::g_engine.get( )->in_game( );

        if ( stage == valve::e_frame_stage::render_start ) {
            if ( in_game ) {
                hacks::g_exploits->skip_lag_interpolation( false );
                hacks::g_misc->clan_tag( );
                hacks::g_misc->removals( );
                hacks::g_visuals->skybox_changer( );
                hacks::g_shot_construct->on_render_start( );

                const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

                static auto enable_fog = valve::g_cvar->find_var( ( "fog_enable" ) );
                static auto override_fog = valve::g_cvar->find_var( ( "fog_override" ) );
                static auto fog_clr = valve::g_cvar->find_var( ( "fog_color" ) );
                static auto fog_start = valve::g_cvar->find_var( ( "fog_start" ) );
                static auto fog_end = valve::g_cvar->find_var( ( "fog_end" ) );
                static auto fog_density = valve::g_cvar->find_var( ( "fog_maxdensity" ) );

                enable_fog->set_int( cfg.m_fog );
                override_fog->set_int( cfg.m_fog );
                fog_clr->set_str( std::string( std::to_string( cfg.m_fog_clr[ 0 ] * 255.f ) + " " + std::to_string( cfg.m_fog_clr[ 1 ] * 255.f ) + " " + std::to_string( cfg.m_fog_clr[ 2 ] * 255.f ) ).c_str( ) );
                fog_start->set_int( cfg.m_fog_start );
                fog_end->set_int( cfg.m_fog_end );
                fog_density->set_float( cfg.m_fog_density / 100.f );


                static int last_impacts_count{};

                struct client_hit_verify_t {
                    sdk::vec3_t	m_pos{};
                    float	m_time{};
                    float	m_expires{};
                };

                /* FF 71 0C F3 0F 11 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? */
                const auto& client_impacts_list = *reinterpret_cast< valve::utl_vec_t< client_hit_verify_t >* >(
                    reinterpret_cast< std::uintptr_t >( g_local_player->self( ) ) + 0x11C50u
                    );

                if ( cfg.m_bullet_impacts )
                    for ( auto i = client_impacts_list.m_size; i > last_impacts_count; --i )
                        valve::g_debug_overlay->add_box_overlay( client_impacts_list.at( i - 1 ).m_pos, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f }, {}, 255, 0, 0, 127, 4.f );

                last_impacts_count = client_impacts_list.m_size;

                if ( cfg.m_bullet_impacts )
                    for ( auto i = hacks::g_visuals->m_bullet_impacts.begin( ); i != hacks::g_visuals->m_bullet_impacts.end( ); i = hacks::g_visuals->m_bullet_impacts.erase( i ) ) {
                        valve::g_debug_overlay->add_box_overlay( i->m_pos, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f }, {}, 0, 0, 255, 127, 4.f );
                    }
                else
                    hacks::g_visuals->m_bullet_impacts.clear( );


                for ( size_t i{ 1 }; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
                    const auto player = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( i ) );

                    if ( !player ||
                        !player->alive( )
                        || player->networkable( )->dormant( )
                        || player->friendly( g_local_player->self( ) ) )
                        continue;

                    auto& entry = hacks::g_lag_comp->entry( i - 1 );

                    if ( entry.m_lag_records.empty( ) )
                        continue;

                    if ( player->sim_time( ) <= player->old_sim_time( ) )
                        continue;

                    auto& var_mapping = player->var_mapping( );

                    for ( size_t j{}; j < var_mapping.m_interpolated_entries; ++j )
                        var_mapping.m_entries.at( j ).m_needs_to_interpolate = false;
                }

                for ( std::size_t i{ 1 }; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
                    const auto player = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( i ) );

                    if ( !player ||
                        !player->alive( )
                        || player->networkable( )->dormant( ) )
                        continue;

                    hacks::g_visuals->m_shared.send_net_data( player );
                }
            }
        }

        o_frame_stage_notify( stage );

        if ( stage == valve::e_frame_stage::render_start )
            hacks::g_exploits->skip_lag_interpolation( true );

        if ( stage == valve::e_frame_stage::net_update_end ) {
            if ( in_game ) {
                valve::g_engine.get( )->fire_events( );

                g_local_player->prediction( ).velocity_modifier_to_data_map( );
                g_local_player->prediction( ).third_person_recoil_to_data_map( );

                hacks::g_lag_comp->handle( );

                const auto correction_ticks = hacks::g_exploits->calc_correction_ticks( );
                if ( correction_ticks == -1 )
                    hacks::g_exploits->correction_amount( ) = 0;
                else {
                    if ( g_local_player->self( )->sim_time( ) > g_local_player->self( )->old_sim_time( ) ) {
                        const auto delta = valve::to_ticks( g_local_player->self( )->sim_time( ) ) - valve::g_client_state->m_server_tick;
                        if ( std::abs( delta ) <= correction_ticks )
                            hacks::g_exploits->correction_amount( ) = delta;
                    }
                }
            }
        }

        if ( in_game ) {
            if ( const auto view_model = valve::g_entity_list.get( )->get_entity( g_local_player->self( )->view_mdl_handle( ) ) ) {
                if ( stage == valve::e_frame_stage::post_data_update_start )
                    g_local_player->prediction( ).view_model( ).restore_view_model( );
            }
        }

        hacks::g_visuals->kill_feed( );
    }

    void __fastcall do_extra_bones_processing(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, int a0, int a1, int a2, int a3, int a4, int a5
    ) {}

    void __fastcall accumulate_layers(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, int a0, int a1, float a2, int a3
    ) {
        if ( ecx->networkable( )->index ( ) < 1
            || ecx->networkable( )->index( ) > 64 )
            return o_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

        if ( const auto anim_state = ecx->anim_state( ) ) {
            const auto backup_first_update = anim_state->m_first_update;

            anim_state->m_first_update = true;

            o_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

            anim_state->m_first_update = backup_first_update;

            return;
        }

        o_accumulate_layers( ecx, edx, a0, a1, a2, a3 );
    }

    void __fastcall standard_blending_rules(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, valve::model_data_t* const mdl_data, int a1, int a2, float a3, int mask
    ) {
        if ( !ecx || ecx->networkable ( )->index( ) < 1
            || ecx->networkable( )->index( ) > 64 )
            return o_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

        if ( !( ecx->effects( ) & 8u ) )
            ecx->effects( ) |= 8u;

        o_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

        if ( ( ecx->effects( ) & 8u ) )
            ecx->effects( ) &= ~8u;
    }

    bool __fastcall setup_bones(
        const std::uintptr_t ecx, const std::uintptr_t edx, sdk::mat3x4_t* const bones, int max_bones, int mask, float time
    ) {
        const auto player = reinterpret_cast< valve::cs_player_t* >( ecx - sizeof( std::uintptr_t ) );
        if ( !player || !player->alive( )
            || player->networkable ( )->index( ) < 1
            || player->networkable( )->index( ) > 64 )
            return o_setup_bones( ecx, edx, bones, max_bones, mask, time );

        if ( player->team( ) == g_local_player->self( )->team( ) )
            if ( player != g_local_player->self( ) )
                return o_setup_bones( ecx, edx, bones, max_bones, mask, time );


        if ( !g_ctx->allow_setup_bones( ) ) {
            if ( !bones
                || max_bones == -1 )
                return true;

            if ( player == g_local_player->self( ) )
            std::memcpy(
                bones, g_local_player->anim_sync( ).m_anim_data.m_real.m_bones.data( ),
                std::min( max_bones, 256 ) * sizeof( sdk::mat3x4_t )
            );
            else {
                const auto& entry = hacks::g_lag_comp->entry( player->networkable( )->index( ) - 1 );

                std::memcpy(
                    bones, entry.m_bones.data( ),
                    std::min( max_bones, 256 ) * sizeof( sdk::mat3x4_t )
                );
            }

            return true;
        }

        return o_setup_bones( ecx, edx, bones, max_bones, mask, time );
    }

    void __fastcall clamp_bones_in_box(
        valve::cs_player_t* ecx, void* edx, sdk::mat3x4_t* matrix, int mask
    ) {
        const auto backup_cur_time = valve::g_global_vars.get( )->m_cur_time;

        valve::g_global_vars.get( )->m_cur_time = ecx->sim_time( );

        o_clamp_bones_in_box( ecx, edx, matrix, mask );

        valve::g_global_vars.get( )->m_cur_time = backup_cur_time;
    }

    __forceinline void set_origin( sdk::mat3x4_t& who, const sdk::vec3_t p ) {
        who[ 0 ][ 3 ] = p.x( );
        who[ 1 ][ 3 ] = p.y( );
        who[ 2 ][ 3 ] = p.z( );
    }

    bool __cdecl glow_effect_spectator(
        valve::cs_player_t* player, valve::cs_player_t* local,
        int& style, sdk::vec3_t& clr, float& alpha_from, float& alpha_to,
        float& time_from, float& time_to, bool& animate
    ) {
        if ( !g_menu->main( ).m_visuals_cfg.get( ).m_glow
            || player->friendly( g_local_player->self( ) ) )
            return o_glow_effect_spectator(
                player, local, style, clr, alpha_from, alpha_to,
                time_from, time_to, animate
            );

        style = 0;

        clr.x( ) = g_menu->main( ).m_visuals_cfg.get( ).m_glow_clr[ 0u ];
        clr.y( ) = g_menu->main( ).m_visuals_cfg.get( ).m_glow_clr[ 1u ];
        clr.z( ) = g_menu->main( ).m_visuals_cfg.get( ).m_glow_clr[ 2u ];

        alpha_to = g_menu->main( ).m_visuals_cfg.get( ).m_glow_clr[ 3u ];

        return true;
    }

    void __fastcall update_client_side_anim( valve::cs_player_t* const ecx, const std::uintptr_t edx ) {
        if ( !ecx || !ecx->alive( )
            || ecx->networkable( )->index( ) < 1
            || ecx->networkable( )->index( ) > 64 )
            return o_update_client_side_anim( ecx, edx );

        if ( ecx->team( ) == g_local_player->self( )->team( ) )
            if ( ecx != g_local_player->self( ) )
                return o_update_client_side_anim( ecx, edx );

        if ( !g_ctx->allow_update_anim( ) ) {
            if ( ecx->team( ) != g_local_player->self( )->team( ) ) {
                if ( ecx != g_local_player->self( ) ) {
                    auto& entry = hacks::g_lag_comp->entry( ecx->networkable( )->index( ) - 1 );
                    for ( int i = 0; i < 256; i++ )
                        set_origin( entry.m_bones.at( i ), ecx->abs_origin( ) - entry.m_bone_origins.at( i ) );

                }
            }
            else {
                const auto mdl_bone_counter = **reinterpret_cast< unsigned long** >(
                    g_ctx->offsets( ).m_addresses.m_invalidate_bone_cache + 0xau
                    );

                static auto prev_mdl_bone_counter = ecx->mdl_bone_cnt( );

                if ( mdl_bone_counter != prev_mdl_bone_counter )
                    g_local_player->anim_sync( ).setup_local_bones( );

                prev_mdl_bone_counter = mdl_bone_counter;

            }

            return;
        }

        for ( auto& layer : ecx->anim_layers( ) ) {
            layer.m_owner = ecx;
        }

        o_update_client_side_anim( ecx, edx );
    }

    void interpolate_server_entities( const std::uintptr_t ecx, const std::uintptr_t edx ) {
        o_interpolate_server_entities( ecx, edx );
    }

    int process_interpolated_list( ) {
        static auto allow_to_extrp = *( bool** ) ( g_ctx->offsets( ).m_addresses.m_allow_to_extrapolate + 0x1 );

        if ( allow_to_extrp )
            *allow_to_extrp = false;

        return o_process_interp_list( );
    }

    void __fastcall on_latch_interpolated_vars(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, const int flags
    ) {
        if ( !valve::g_engine.get( )->in_game( )
            || ecx != g_local_player->self( ) )
            return o_on_latch_interpolated_vars( ecx, edx, flags );

        const auto backup_sim_time = ecx->sim_time( );

        const auto& local_data = g_local_player->prediction ( ).data ( ).local_data ( ).at( valve::g_client_state->m_cmd_ack % 150 );
        if ( local_data.m_spawn_time == g_local_player->self( )->spawn_time( )
            && local_data.m_shift_amount > 0 )
            ecx->sim_time( ) = ecx->sim_time( ) + ( local_data.m_shift_amount * valve::g_global_vars.get( )->m_interval_per_tick );

        o_on_latch_interpolated_vars( ecx, edx, flags );

        ecx->sim_time( ) = backup_sim_time;
    }

    bool __fastcall write_user_cmd_delta_to_buffer(
        const std::uintptr_t ecx, const std::uintptr_t edx,
        const int slot, valve::bf_write_t* const buffer,
        int from, int to, const bool is_new_cmd
    ) {
        if ( !g_local_player->self( ) )
            return o_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );

        const auto move_msg = reinterpret_cast< valve::move_msg_t* >(
            *reinterpret_cast< std::uintptr_t* >(
                reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) - sizeof( std::uintptr_t )
                ) - 0x58u
            );

        if ( hacks::g_exploits->cur_shift_amount( )
            || valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( )
            || hacks::g_exploits->type( ) == 4 ) {
            if ( from == -1 ) {
                if ( valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
                    move_msg->m_new_cmds = 1;
                    move_msg->m_backup_cmds = 0;

                    const auto next_cmd_number = valve::g_client_state->m_choked_cmds + valve::g_client_state->m_last_cmd_out + 1;

                    for ( to = next_cmd_number - move_msg->m_new_cmds + 1; to <= next_cmd_number; ++to ) {
                        if ( !o_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, true ) )
                            break;

                        from = to;
                    }
                }
                else if ( hacks::g_exploits->type( ) == 5 )
                {
                    hacks::g_exploits->handle_break_lc( ecx, edx, slot, buffer, from, to, move_msg );
                    return 1;
                }
                else
                    hacks::g_exploits->process_real_cmds( ecx, edx, slot, buffer, from, to, move_msg );
            }

            return true;
        }
        else
        {
            if ( from == -1 ) {
                const auto v86 = std::min( move_msg->m_new_cmds + hacks::g_exploits->ticks_allowed( ), 16 );

                int v12{};

                const auto v70 = v86 - move_msg->m_new_cmds;
                if ( v70 >= 0 )
                    v12 = v70;

                hacks::g_exploits->ticks_allowed( ) = v12;
            }
        }

        return o_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );
    }

    void __fastcall player_move( const std::uintptr_t ecx, const std::uintptr_t edx ) {
        o_player_move( ecx, edx );
        g_local_player->prediction( ).correct_view_offset( );
    }

    void __fastcall calc_view(
        void* ecx, const std::uintptr_t edx, sdk::vec3_t& eye_origin, const sdk::qang_t& eye_ang, float& z_near, float& z_far, float& fov
    ) {
        if ( !g_local_player->self( )
            || ecx != g_local_player->self( ) )
            return o_calc_view( ecx, edx, eye_origin, eye_ang, z_near, z_far, fov );

        sdk::qang_t aim_punch_ang = g_local_player->self( )->aim_punch( );
        sdk::qang_t view_punch_ang = g_local_player->self( )->view_punch( );
        const auto backup_use_new_anim_state = g_local_player->self( )->use_new_anim_state( );

        g_local_player->self( )->use_new_anim_state( ) = false;

        if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_view_punch )
            g_local_player->self( )->aim_punch( ) = sdk::qang_t( );

        if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_view_kick )
            g_local_player->self( )->view_punch( ) = sdk::qang_t( );

        o_calc_view( ecx, edx, eye_origin, eye_ang, z_near, z_far, fov );

        if ( hacks::g_move->should_fake_duck( ) )
            eye_origin.z( ) = g_local_player->self( )->abs_origin( ).z( ) + 64.f;

        g_local_player->self( )->use_new_anim_state( ) = backup_use_new_anim_state;

        if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_view_punch )
            g_local_player->self( )->aim_punch( ) = aim_punch_ang;

        if ( g_menu->main( ).m_misc_cfg.get( ).m_remove_view_kick )
            g_local_player->self( )->view_punch( ) = view_punch_ang;
    }

    void __fastcall calc_viewmodel_bob( valve::cs_player_t* ecx, const std::uintptr_t edx, sdk::vec3_t& view_bob ) {
        if ( ecx != g_local_player->self( )
            || !g_menu->main ( ).m_misc_cfg.get( ).m_remove_land_bob );
        return o_calc_viewmodel_bob( ecx, edx, view_bob );

        return;
    }

    void __fastcall modify_eye_pos( valve::anim_state_t* ecx, std::uintptr_t edx, sdk::vec3_t& pos ) {
        if ( g_local_player->left_create_move( ) )
            return o_modify_eye_pos( ecx, edx, pos );

        return;
    }

    bool __fastcall should_draw_view_model( std::uintptr_t ecx, std::uintptr_t edx ) {
        if ( !valve::g_engine.get( )->in_game( )
            || !g_local_player->self( )->alive( ) )
            return o_should_draw_view_model( ecx, edx );

        if ( g_local_player->self( )->scoped( )
            && g_menu->main( ).m_visuals_cfg.get( ).m_show_wpn_in_scope )
            return true;

        return o_should_draw_view_model( ecx, edx );
    }

    void event_listener_t::fire_game_event( valve::game_event_t* const event ) {
        hacks::g_shots->on_new_event( event );
    }

    bool __fastcall svc_msg_voice_data( std::uintptr_t ecx, std::uintptr_t edx, void* msg ) {
        if ( !msg )
            return o_svc_msg_voice_data( ecx, edx, msg );

        auto msg_ptr = ( valve::csvc_msg_data_legacy_t* ) msg;

        if ( msg_ptr->m_client + 1 == valve::g_engine.get( )->get_local_player( ) ) {
            return o_svc_msg_voice_data( ecx, edx, msg );
        }

        cheat_data_t* ptr = ( cheat_data_t* ) &msg_ptr->m_xuid_low;

        if ( ptr->m_unique_key == 228 ) {
            const auto player = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( static_cast < int > ( ptr->m_player_idx ) ) );
            const auto sender = static_cast < valve::cs_player_t* > ( valve::g_entity_list.get( )->get_entity( static_cast < int > ( msg_ptr->m_client + 1 ) ) );

            if ( sender ) {
                if ( sender->team( ) == g_local_player->self( )->team( ) )
                    return o_svc_msg_voice_data( ecx, edx, msg );
            }

            if ( player ) {
                if ( player->alive( ) ) {
                    if ( player->networkable( )->dormant( ) ) {
                        hacks::g_visuals->m_dormant_data.at( player->networkable( )->index( ) ).m_origin = sdk::vec3_t( ptr->m_x, ptr->m_y, ptr->m_z );
                        hacks::g_visuals->m_dormant_data.at( player->networkable( )->index( ) ).m_use_shared = true;
                        player->origin( ) = sdk::vec3_t( ptr->m_x, ptr->m_y, ptr->m_z );
                        player->set_abs_origin( sdk::vec3_t( ptr->m_x, ptr->m_y, ptr->m_z ) );
                        hacks::g_visuals->m_dormant_data.at( player->networkable( )->index( ) ).m_last_shared_time = valve::g_global_vars.get( )->m_real_time;
                    }
                }
            }
        }

        return o_svc_msg_voice_data( ecx, edx, msg );
    }
}