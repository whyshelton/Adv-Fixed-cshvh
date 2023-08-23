#include "../../csgo.hpp"
__forceinline void normalize_ghetto( sdk::qang_t& vec )
{
    for ( auto i = 0; i < 2; i++ ) {
        while ( vec.at( i ) < -180.0f ) vec.at( i ) += 360.0f;
        while ( vec.at( i ) > 180.0f ) vec.at( i ) -= 360.0f;
    }

    while ( vec.at( 2 ) < -50.0f ) vec.at( 2 ) += 100.0f;
    while ( vec.at( 2 ) > 50.0f ) vec.at( 2 ) -= 100.0f;
}

__forceinline void clamp_angles( sdk::qang_t& angles )
{
    if ( angles.x( ) > 89.0f )
        angles.x( ) = 89.0f;
    else if ( angles.x( ) < -89.0f )
        angles.x( ) = -89.0f;

    if ( angles.y( ) > 180.0f )
        angles.y( ) = 180.0f;
    else if ( angles.y( ) < -180.0f )
        angles.y( ) = -180.0f;

    angles.z( ) = std::clamp( angles.z( ), -50.0f, 50.0f );
}

void c_local_player::prediction_t::start_prediction( ) {
    if ( g_local_player->last_frame_stage ( ) == valve::e_frame_stage::net_update_end ) {
        valve::g_prediction.get( )->update( valve::g_client_state->m_delta_tick,
            valve::g_client_state->m_delta_tick > 0,
            valve::g_client_state->m_last_cmd_ack,
            valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds );
    }

    if ( data( ).net_velocity_modifier ( ) < 1.f )
        valve::g_prediction.get( )->m_prev_ack_had_errors = true;

    if ( data( ).is_out_of_epsilon( ) ) {
        valve::g_prediction.get( )->m_cmds_predicted = 0;
        valve::g_prediction.get( )->m_prev_ack_had_errors = true;
    }

    if ( valve::g_client_state->m_delta_tick > 0 )
        data( ).is_out_of_epsilon( ) = false;

    m_prediction_backup.m_cur_time = valve::g_global_vars.get( )->m_cur_time;
    m_prediction_backup.m_frame_time = valve::g_global_vars.get( )->m_frame_time;

    valve::g_global_vars.get( )->m_cur_time = valve::to_time( g_local_player->self( )->tick_base( ) );
    valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
}

void c_local_player::prediction_t::update_shoot_pos( const valve::user_cmd_t& user_cmd ) {
    const auto anim_state = g_local_player->self( )->anim_state( );

    if ( !anim_state )
        return;

    sdk::vec3_t view_offset = g_local_player->self( )->view_offset( );
    if ( view_offset.z( ) <= 46.05f )
        view_offset.z( ) = 46.0f;
    else if ( view_offset.z( ) > 64.0f )
        view_offset.z( ) = 64.0f;

    bool landing{};

    const auto& local_data = data( ).local_data( ).at( user_cmd.m_number % 150 );

    if ( local_data.m_pred_net_vars.m_flags & valve::e_ent_flags::on_ground ) {
        if ( !( local_data.m_net_vars.m_flags & valve::e_ent_flags::on_ground ) )
            landing = true;
    }

    g_local_player->shoot_pos( ) = g_local_player->self( )->origin( ) + view_offset;

    if ( g_local_player->self( )->duck_amt( ) != 0.f
        || landing || hacks::g_move->should_fake_duck( ) ) {
        const auto old_body_pitch = g_local_player->self( )->pose_params( ).at( 12u );
        g_local_player->self( )->pose_params( ).at( 12u ) = ( user_cmd.m_view_angles.x( ) + 90.0f ) / 180.0f;

        auto& bones = data( ).shoot_bones( );
        g_local_player->anim_sync( ).setup_bones( bones, valve::to_time( g_local_player->self( )->tick_base( ) ), 15 );

        g_local_player->self( )->pose_params( ).at( 12u ) = old_body_pitch;

        sdk::vec3_t head_pos = sdk::vec3_t
        (
            bones[ 8 ][ 0 ][ 3 ],
            bones[ 8 ][ 1 ][ 3 ],
            bones[ 8 ][ 2 ][ 3 ] + 1.7f
        );

        if ( head_pos.z( ) > g_local_player->shoot_pos( ).z( ) )
            return;

        auto tmp = 0.0f;
        tmp = ( fabsf( g_local_player->shoot_pos( ).z( ) - head_pos.z( ) ) - 4.0f ) * 0.16666667f;

        if ( tmp >= 0.0f )
            tmp = fminf( tmp, 1.0f );

        g_local_player->shoot_pos( ).z( ) = ( ( head_pos.z( ) - g_local_player->shoot_pos( ).z( ) )
            * ( ( ( tmp * tmp ) * 3.0f ) - ( ( ( tmp * tmp ) * 2.0f ) * tmp ) ) )
            + g_local_player->shoot_pos( ).z( );
    }
}

void c_local_player::prediction_t::process_prediction( valve::user_cmd_t* const user_cmd ) {
    if ( user_cmd->m_number == -1 )
        return;

    data( ).local_data ( ).at( user_cmd->m_number % 150 ).init( *user_cmd );

    g_ctx->offsets( ).m_prediction.m_pred_player = g_local_player->self ( );
    *g_ctx->offsets( ).m_prediction.m_pred_seed = user_cmd->m_random_seed;

    g_local_player->self( )->cur_user_cmd( ) = user_cmd;
    g_local_player->self( )->last_user_cmd( ) = *user_cmd;

    m_prediction_backup.m_in_prediction = valve::g_prediction.get( )->m_in_prediction;
    m_prediction_backup.m_first_time_predicted = valve::g_prediction.get( )->m_first_time_predicted;

    valve::g_prediction.get( )->m_in_prediction = true;
    valve::g_prediction.get( )->m_first_time_predicted = false;

    valve::g_move_helper->set_host( g_local_player->self ( ) );

    valve::g_movement.get( )->start_track_pred_errors( g_local_player->self( ) );

    valve::g_prediction.get( )->setup_move( g_local_player->self( ), user_cmd, valve::g_move_helper, &data( ).move_data( ) );

    valve::g_movement.get( )->process_movement( g_local_player->self( ), &data( ).move_data ( ) );

    valve::g_prediction.get( )->finish_move( g_local_player->self ( ), user_cmd, &data( ).move_data( ) );

    g_local_player->self( )->set_abs_origin( g_local_player->self ( )->origin ( ) );

    valve::g_movement.get( )->finish_track_pred_errors( g_local_player->self( ) );

    valve::g_move_helper->set_host( nullptr );

    if ( const auto weapon = g_local_player->self( )->weapon( ) ) {
        weapon->update_inaccuracy( );

        data( ).inaccuracy( ) = weapon->inaccuracy( );
        data( ).spread( ) = weapon->spread( );

        const auto item_index = weapon->item_index( );
        const auto sniper =
            item_index == valve::e_item_index::awp || item_index == valve::e_item_index::g3sg1
            || item_index == valve::e_item_index::scar20 || item_index == valve::e_item_index::ssg08;
        const auto wpn_data = weapon->info( );

        if ( g_local_player->self( )->flags( ) & valve::e_ent_flags::ducking )
            data( ).min_inaccuracy( ) = sniper ? wpn_data->m_inaccuracy_crouch_alt : wpn_data->m_inaccuracy_crouch;
        else
            data( ).min_inaccuracy( ) = sniper ? wpn_data->m_inaccuracy_stand_alt : wpn_data->m_inaccuracy_stand;
    }
    else
        data( ).inaccuracy( ) = data( ).spread( ) = 0.f;

    m_data.local_data( ).at( user_cmd->m_number % 150 ).save( user_cmd->m_number );

    valve::g_prediction.get( )->m_in_prediction = m_prediction_backup.m_in_prediction;
    valve::g_prediction.get( )->m_first_time_predicted = m_prediction_backup.m_first_time_predicted;

    update_shoot_pos( *user_cmd );
}

void c_local_player::prediction_t::end_prediction( ) {
    g_ctx->offsets( ).m_prediction.m_pred_player = nullptr;
    *g_ctx->offsets( ).m_prediction.m_pred_seed = -1;

    valve::g_global_vars.get( )->m_cur_time = m_prediction_backup.m_cur_time;
    valve::g_global_vars.get( )->m_frame_time = m_prediction_backup.m_frame_time;
}

void c_local_player::anti_aim_t::choke( bool& send_packet ) {
    if ( !g_menu->main( ).m_fake_lags_cfg.get( ).m_enabled
        || g_local_player->self( )->flags( ) & valve::e_ent_flags::frozen )
        return;

    if ( hacks::g_move->should_fake_duck( ) )
        return;

    std::ptrdiff_t ticks{ g_menu->main( ).m_fake_lags_cfg.get( ).m_ticks + g_ctx->offsets( ).m_addresses.m_random_int( -g_menu->main( ).m_fake_lags_cfg.get( ).m_variability, g_menu->main( ).m_fake_lags_cfg.get( ).m_variability ) };

    if ( hacks::g_exploits->charged( )
        || hacks::g_exploits->shift( )
        || hacks::g_exploits->in_charge( )
        || !hacks::g_exploits->allow_choke( )
        || valve::g_client_state->m_choked_cmds > 14 ) {
        ticks = 1;
    }

    ticks = std::clamp( ticks, 0, 14 );

    send_packet = valve::g_client_state->m_choked_cmds >= ticks;
}

void c_local_player::anim_sync_t::do_animation_event( const std::ptrdiff_t const type ) {
    if ( g_local_player->self( )->flags( ) & valve::e_ent_flags::frozen )
    {
        m_anim_data.m_move_type[ type ] = valve::e_move_type::none;
        m_anim_data.m_flags[ type ] = valve::e_ent_flags::on_ground;
    }

    valve::anim_layer_t& land_or_climb_layer = g_local_player->self( )->anim_layers( ).at( 5 );

    valve::anim_layer_t& jump_or_fall_layer = g_local_player->self( )->anim_layers( ).at( 4 );


    if ( m_anim_data.m_move_type[ type ] != valve::e_move_type::ladder && g_local_player->self( )->move_type( ) == valve::e_move_type::ladder )
        g_local_player->self( )->anim_state( )->set_layer_seq( land_or_climb_layer, 987 );
    else if ( m_anim_data.m_move_type[ type ] == valve::e_move_type::ladder && g_local_player->self( )->move_type( ) != valve::e_move_type::ladder )
        g_local_player->self( )->anim_state( )->set_layer_seq( jump_or_fall_layer, 986 );
    else {
        if ( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) {
            if ( !( m_anim_data.m_flags[ type ] & valve::e_ent_flags::on_ground ) )
                g_local_player->self( )->anim_state( )->set_layer_seq( land_or_climb_layer, g_local_player->self( )->anim_state( )->m_time_since_in_air > 1.0f && type == 0 ? 989 : 988 );
        }
        else if ( m_anim_data.m_flags[ type ] & valve::e_ent_flags::on_ground ) {
            if ( g_local_player->self( )->velocity( ).z( ) > 0.0f )
                g_local_player->self( )->anim_state( )->set_layer_seq( jump_or_fall_layer, 985 );
            else
                g_local_player->self( )->anim_state( )->set_layer_seq( jump_or_fall_layer, 986 );
        }
    }

    m_anim_data.m_move_type[ type ] = g_local_player->self( )->move_type( );
    m_anim_data.m_flags[ type ] = g_local_player->self( )->flags( );
}

bool c_local_player::anti_aim_t::should_run( valve::user_cmd_t& user_cmd ) {
    if ( !g_menu->main( ).m_anti_aim_cfg.get( ).m_enabled )
        return false;

    const auto cur_move_type = g_local_player->self( )->move_type( );

    valve::e_move_type cmd_move_type{}, cmd_pred_move_type{};
    const auto& local_data = g_local_player->prediction( ).data( ).local_data( ).at( user_cmd.m_number % 150 );

    cmd_move_type = local_data.m_net_vars.m_move_type;
    cmd_pred_move_type = local_data.m_pred_net_vars.m_move_type;

    if ( cur_move_type == valve::e_move_type::ladder || cur_move_type == valve::e_move_type::noclip
        || cmd_move_type == valve::e_move_type::ladder || cmd_move_type == valve::e_move_type::noclip
        || cmd_pred_move_type == valve::e_move_type::ladder || cmd_pred_move_type == valve::e_move_type::noclip )
        return false;

    if ( g_local_player->self( )->flags( ) & valve::e_ent_flags::frozen )
        return false;

    if ( hacks::g_aim_bot->shooting( ) )
        return false;

    if ( g_local_player->self( )->defusing( ) )
        return false;

    return true;
}

void c_local_player::anim_sync_t::setup_local_bones( ) {
    const auto anim_state = g_local_player->self( )->anim_state( );
    if ( !anim_state )
        return;

    const auto& local_data = g_local_player->prediction( ).data( ).local_data( ).at( g_local_player->last_sent_cmd_number( ) % 150 );
    if ( local_data.m_spawn_time != g_local_player->self( )->spawn_time( ) )
        return;

    struct anim_backup_t {
        __forceinline anim_backup_t( )
            : m_anim_state{ *g_local_player->self( )->anim_state( ) },
            m_abs_yaw{ m_anim_state.m_foot_yaw },
            m_anim_layers{ g_local_player->self( )->anim_layers( ) },
            m_pose_params{ g_local_player->self( )->pose_params( ) } {}

        __forceinline void restore( ) const {
            g_local_player->self( )->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

            *g_local_player->self( )->anim_state( ) = m_anim_state;

            g_local_player->self( )->anim_layers( ) = m_anim_layers;
            g_local_player->self( )->pose_params( ) = m_pose_params;
        }

        valve::anim_state_t		m_anim_state{};

        float					m_abs_yaw{};

        valve::anim_layers_t	m_anim_layers{};
        valve::pose_params_t	m_pose_params{};
    } anim_backup{};

    local_data.restore_anim( true );

    if ( m_anim_data.m_fake.m_spawn_time == 0.f
        || m_anim_data.m_fake.m_spawn_time != g_local_player->self( )->spawn_time( ) ) {
        m_anim_data.m_fake.m_anim_state = *g_local_player->self( )->anim_state( );

        m_anim_data.m_fake.m_spawn_time = g_local_player->self( )->spawn_time( );
    }

    if ( !valve::g_client_state->m_choked_cmds
        && valve::g_global_vars.get( )->m_cur_time != m_anim_data.m_fake.m_anim_state.m_last_update_time ) {
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 4 ), &m_anim_data.m_fake.m_anim_layers.at( 4 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 5 ), &m_anim_data.m_fake.m_anim_layers.at( 5 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 6 ), &m_anim_data.m_fake.m_anim_layers.at( 6 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 7 ), &m_anim_data.m_fake.m_anim_layers.at( 7 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 11 ), &m_anim_data.m_fake.m_anim_layers.at( 11 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 12 ), &m_anim_data.m_fake.m_anim_layers.at( 12 ), sizeof( valve::anim_layer_t ) );

        *g_local_player->self( )->anim_state( ) = m_anim_data.m_fake.m_anim_state;

        anim_state->m_foot_yaw = local_data.m_user_cmd.m_view_angles.y( );

        const auto total_cmds = valve::g_client_state->m_choked_cmds + 1;

        for ( int i{ 1 }; i <= total_cmds; ++i ) {

            do_animation_event( 1 );

            const auto backup_abs_velocity = g_local_player->self( )->abs_velocity( );

            g_local_player->self( )->abs_velocity( ) = g_local_player->self( )->velocity( );

            const auto backup_eflags = g_local_player->self( )->eflags( );

            g_local_player->self( )->eflags( ) &= ~0x1000u;

            anim_state->m_last_update_frame = 0.f;

            const auto backup_client_side_anim = g_local_player->self( )->client_side_anim( );

            g_ctx->allow_update_anim( ) = g_local_player->self( )->client_side_anim( ) = true;

            anim_state->update(
                local_data.m_user_cmd.m_view_angles.x( ),
                local_data.m_user_cmd.m_view_angles.y( ),
                local_data.m_user_cmd.m_view_angles.z( )
            
            );

            g_ctx->allow_update_anim( ) = g_local_player->self( )->client_side_anim( ) = false;

            g_local_player->self( )->eflags( ) = backup_eflags;

            g_local_player->self( )->abs_velocity( ) = backup_abs_velocity;

            g_local_player->self( )->client_side_anim( ) = backup_client_side_anim;
        }

        m_anim_data.m_fake.m_abs_yaw = anim_state->m_foot_yaw;
        m_anim_data.m_fake.m_anim_state = *g_local_player->self( )->anim_state( );
        m_anim_data.m_fake.m_anim_layers = g_local_player->self( )->anim_layers( );
        m_anim_data.m_fake.m_pose_params = g_local_player->self( )->pose_params( );
    }

    local_data.restore_anim( true );

    g_local_player->self( )->anim_layers( ) = m_anim_data.m_fake.m_anim_layers;
    g_local_player->self( )->pose_params( ) = m_anim_data.m_fake.m_pose_params;

    g_local_player->self( )->set_abs_angles( { 0.f, m_anim_data.m_fake.m_abs_yaw, 0.f } );

    setup_bones( m_anim_data.m_fake.m_bones, valve::g_global_vars.get( )->m_cur_time, 11 );

    local_data.restore_anim( true );

    const auto backup_z = g_local_player->self( )->eye_angles( ).z( );
    const auto backup_visual_z = g_local_player->self( )->visual_angles( ).z( );

    if ( g_menu->main( ).m_anti_aim_cfg.get( ).m_roll ) {
        g_local_player->self( )->eye_angles( ).z( ) = g_menu->main( ).m_anti_aim_cfg.get( ).m_roll_range;
        g_local_player->self( )->visual_angles( ).z( ) = g_menu->main( ).m_anti_aim_cfg.get( ).m_roll_range;
    }

    setup_bones( m_anim_data.m_real.m_bones, valve::g_global_vars.get( )->m_cur_time, 27 );

    if ( g_menu->main( ).m_anti_aim_cfg.get( ).m_roll ) {
        g_local_player->self( )->visual_angles( ).z( ) = backup_visual_z;
        g_local_player->self( )->eye_angles( ).z( ) = backup_z;
    }

    anim_backup.restore( );
}

void c_local_player::anim_sync_t::update_local_real( valve::user_cmd_t& user_cmd, bool send_packet ) {
    const auto anim_state = g_local_player->self( )->anim_state( );
    if ( !anim_state )
        return;

    const auto backup_anim_layers = g_local_player->self( )->anim_layers( );

    if ( !g_local_player->anti_aim( ).should_run( user_cmd ) ) {
        if ( g_local_player->self( )->move_type( ) == valve::e_move_type::walk ) {
            user_cmd.m_buttons &= ~(
                valve::e_buttons::in_fwd
                | valve::e_buttons::in_back
                | valve::e_buttons::in_move_right
                | valve::e_buttons::in_move_left
                );

            if ( user_cmd.m_move.x( ) != 0.f )
                user_cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? user_cmd.m_move.x( ) < 0.f : user_cmd.m_move.x( ) > 0.f )
                ? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

            if ( user_cmd.m_move.y( ) != 0.f )
                user_cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? user_cmd.m_move.y( ) < 0.f : user_cmd.m_move.y( ) > 0.f )
                ? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
        }

        const auto& prev_local_data = g_local_player->prediction( ).data( ).local_data( ).at( ( user_cmd.m_number - 1 ) % 150 );
        if ( prev_local_data.m_spawn_time == g_local_player->self( )->spawn_time( ) )
            prev_local_data.restore_anim( false );

        update_local( user_cmd.m_view_angles, false, valve::to_time( g_local_player->self( )->tick_base( ) ) );

        g_local_player->prediction( ).data( ).local_data( ).at( user_cmd.m_number % 150 ).store_anim( );

        g_local_player->self( )->anim_layers( ) = backup_anim_layers;

        return;
    }

    const auto side = g_local_player->anti_aim( ).select_side( );

    auto yaw = user_cmd.m_view_angles.y( );
    g_local_player->anti_aim( ).select_yaw( yaw, side, user_cmd );

    const auto& first_local_data = g_local_player->prediction( ).data( ).local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );
    if ( first_local_data.m_spawn_time == g_local_player->self( )->spawn_time( ) )
        first_local_data.restore_anim( false );

    sdk::qang_t shot_cmd_view_angles{};

    auto i = 1;
    auto choked_cmds = valve::g_client_state->m_choked_cmds;

    const auto total_cmds = choked_cmds + 1;
    if ( total_cmds < 1 ) {
        g_local_player->self( )->anim_layers( ) = backup_anim_layers;

        return;
    }

    for ( ; i <= total_cmds; ++i, --choked_cmds ) {
        const auto& cur_local_data = g_local_player->prediction( ).data( ).local_data( ).at( ( valve::g_client_state->m_last_cmd_out + i ) % 150 );
        if ( cur_local_data.m_spawn_time == g_local_player->self( )->spawn_time( ) )
            break;
    }

    if ( send_packet ) {
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 4 ), &m_anim_data.m_real.m_anim_layers.at( 4 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 5 ), &m_anim_data.m_real.m_anim_layers.at( 5 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 6 ), &m_anim_data.m_real.m_anim_layers.at( 6 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 7 ), &m_anim_data.m_real.m_anim_layers.at( 7 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 11 ), &m_anim_data.m_real.m_anim_layers.at( 11 ), sizeof( valve::anim_layer_t ) );
        std::memcpy( &g_local_player->self( )->anim_layers( ).at( 12 ), &m_anim_data.m_real.m_anim_layers.at( 12 ), sizeof( valve::anim_layer_t ) );
    }

    auto shot_in_this_cycle =
        g_local_player->prediction( ).data( ).shot_cmd_number( ) > valve::g_client_state->m_last_cmd_out
        && g_local_player->prediction( ).data( ).shot_cmd_number( ) <= ( valve::g_client_state->m_last_cmd_out + total_cmds );

    for ( ; i <= total_cmds; ++i, --choked_cmds ) {
        const auto j = ( valve::g_client_state->m_last_cmd_out + i ) % 150;

        auto& cur_user_cmd = valve::g_input->m_cmds[ j ];
        auto& cur_local_data = g_local_player->prediction( ).data( ).local_data( ).at( j );

        if ( cur_local_data.m_net_vars.m_move_type != valve::e_move_type::ladder
            && cur_local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder ) {
            const auto old_view_angles = cur_user_cmd.m_view_angles;

            g_local_player->anti_aim( ).handle( cur_user_cmd, yaw, side, choked_cmds );

            if ( g_menu->main( ).m_anti_aim_cfg.get( ).m_roll )
                cur_user_cmd.m_view_angles.z( ) = g_menu->main( ).m_anti_aim_cfg.get( ).m_roll_range;

            if ( cur_user_cmd.m_view_angles.x( ) != old_view_angles.x( )
                || cur_user_cmd.m_view_angles.y( ) != old_view_angles.y( )
                || cur_user_cmd.m_view_angles.z( ) != old_view_angles.z( ) ) {
                hacks::g_move->rotate( cur_user_cmd, old_view_angles );
            }
        }

        if ( cur_local_data.m_net_vars.m_move_type == valve::e_move_type::walk ) {
            cur_user_cmd.m_buttons &= ~(
                valve::e_buttons::in_fwd
                | valve::e_buttons::in_back
                | valve::e_buttons::in_move_right
                | valve::e_buttons::in_move_left
                );

            if ( cur_user_cmd.m_move.x( ) != 0.f )
                cur_user_cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cur_user_cmd.m_move.x( ) < 0.f : cur_user_cmd.m_move.x( ) > 0.f )
                ? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

            if ( cur_user_cmd.m_move.y( ) != 0.f )
                cur_user_cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cur_user_cmd.m_move.y( ) < 0.f : cur_user_cmd.m_move.y( ) > 0.f )
                ? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
        }

        g_local_player->self( )->origin( ) = cur_local_data.m_pred_net_vars.m_origin;
        g_local_player->self( )->move_type( ) = cur_local_data.m_pred_net_vars.m_move_type;
        g_local_player->self( )->scoped( ) = cur_local_data.m_pred_net_vars.m_scoped;
        g_local_player->self( )->walking( ) = cur_local_data.m_pred_net_vars.m_walking;
        g_local_player->self( )->lby( ) = cur_local_data.m_pred_net_vars.m_lby;
        g_local_player->self( )->view_offset( ) = cur_local_data.m_pred_net_vars.m_view_offset;
        g_local_player->self( )->aim_punch( ) = cur_local_data.m_pred_net_vars.m_aim_punch;
        g_local_player->self( )->aim_punch_vel( ) = cur_local_data.m_pred_net_vars.m_aim_punch_vel;
        g_local_player->self( )->view_punch( ) = cur_local_data.m_pred_net_vars.m_view_punch;
        g_local_player->self( )->fall_velocity( ) = cur_local_data.m_pred_net_vars.m_fall_velocity;
        g_local_player->self( )->flags( ) = cur_local_data.m_pred_net_vars.m_flags;

        const auto backup_cur_time = valve::g_global_vars.get( )->m_cur_time;

        valve::g_global_vars.get( )->m_cur_time = valve::to_time( g_local_player->self( )->tick_base( ) );

        g_local_player->self( )->set_collision_bounds( 
            cur_local_data.m_pred_net_vars.m_obb_min, cur_local_data.m_pred_net_vars.m_obb_max 
        );

        valve::g_global_vars.get( )->m_cur_time = backup_cur_time;

        g_local_player->self( )->velocity( ) = cur_local_data.m_pred_net_vars.m_velocity;
        g_local_player->self( )->third_person_recoil( ) = cur_local_data.m_pred_net_vars.m_third_person_recoil;
        g_local_player->self( )->duck_amt( ) = cur_local_data.m_pred_net_vars.m_duck_amount;

        g_local_player->self( )->tick_base( ) = cur_local_data.m_pred_net_vars.m_tick_base;

        auto cur_view_angles = cur_user_cmd.m_view_angles;

        if ( shot_in_this_cycle ) {
            if ( cur_user_cmd.m_number == g_local_player->prediction( ).data( ).shot_cmd_number( ) )
                shot_cmd_view_angles = cur_view_angles;

            if ( cur_user_cmd.m_number > g_local_player->prediction( ).data( ).shot_cmd_number( ) )
                cur_view_angles = shot_cmd_view_angles;
        }

        const auto last_user_cmd = cur_user_cmd.m_number == user_cmd.m_number;

        int tick_base = g_local_player->self( )->tick_base( );

        if ( send_packet )
            update_local( cur_view_angles, !last_user_cmd, valve::to_time( tick_base ) );

        cur_local_data.m_user_cmd = cur_user_cmd;

        if ( shot_in_this_cycle ) {
            if ( cur_user_cmd.m_number == g_local_player->prediction( ).data( ).shot_cmd_number( ) )
                cur_local_data.m_choke_angle = cur_view_angles;

            if ( cur_user_cmd.m_number > g_local_player->prediction( ).data( ).shot_cmd_number( ) )
                cur_local_data.m_user_cmd.m_view_angles = shot_cmd_view_angles;
        }

        cur_local_data.store_anim( );

        if ( last_user_cmd )
            continue;

        valve::g_input->m_vfyd_cmds[ j ] = { cur_user_cmd, cur_user_cmd.checksum( ) };
    }

    if ( send_packet )
        m_anim_data.m_real.m_anim_layers = g_local_player->self( )->anim_layers( );

    g_local_player->self( )->anim_layers( ) = backup_anim_layers;
}

void c_local_player::create_move( bool& send_packet,
    valve::user_cmd_t& cmd, valve::vfyd_user_cmd_t& vfyd_cmd
) {
    send_packet = true;

    const auto old_angles = cmd.m_view_angles;
    auto old_angles_ = cmd.m_view_angles;

    cur_seq( ) = valve::g_client_state->m_net_chan->m_out_seq;

    hacks::g_move->should_fake_duck( ) = false;

    const auto net_channel_info = valve::g_engine.get( )->net_channel_info( );
    if ( !net_channel_info )
        return;

    {
        static auto cl_interp = valve::g_cvar->find_var( ( "cl_interp" ) );
        static auto cl_interp_ratio = valve::g_cvar->find_var( ( "cl_interp_ratio" ) );
        static auto cl_updaterate = valve::g_cvar->find_var( ( "cl_updaterate" ) );

        auto& net_info = g_local_player->net_info( );

        net_info.m_lerp = std::max(
            cl_interp->get_float( ),
            cl_interp_ratio->get_float( ) / cl_updaterate->get_float( )
        );
        net_info.m_latency = { net_channel_info->latency( 1 ), net_channel_info->latency( 0 ) };
    }
    
    static float prev_spawn_time = g_local_player->self( )->spawn_time( );
    if ( prev_spawn_time != g_local_player->self( )->spawn_time( ) ) {
        hacks::g_exploits->ticks_allowed( ) = 0;
        g_local_player->anim_sync( ).m_anim_data.reset( );
        g_local_player->prediction( ).reset( );

        prev_spawn_time = g_local_player->self( )->spawn_time( );
    }

    m_left_create_move = false;

    {
        if ( ( m_weapon = self( )->weapon( ) ) )
            m_weapon_info = m_weapon->info( );
        else
            m_weapon_info = nullptr;
    }

    static auto crosshair_data = valve::g_cvar->find_var( ( "weapon_debug_spread_show" ) );
    if ( g_menu->main ( ).m_misc_cfg.get( ).m_force_crosshair && crosshair_data ) {
        crosshair_data->set_int( !g_local_player->self( )->scoped( ) ? 3 : 0 );
    }
    else {
        if ( crosshair_data )
            crosshair_data->set_int( 0 );
    }

    bool break_lc{};
    hacks::g_move->should_fake_duck( ) = g_key_binds->get_keybind_state( &g_menu->main( ).m_move_cfg.get( ).m_fake_duck ) && ( self( )->flags( ) & valve::e_ent_flags::on_ground ) && !( cmd.m_buttons & valve::e_buttons::in_jump );

    hacks::g_exploits->defensive_allowed( ) = true;
 
    if ( !hacks::g_exploits->try_to_recharge( cmd ) ) {
        g_local_player->prediction( ).data( ).shot( ) = false;

        m_anti_aim.fake_move( cmd );

        m_prediction.start_prediction( );

        hacks::g_move->handle( cmd, send_packet );

        if ( valve::g_client_state->m_delta_tick > 0 ) {
            valve::g_prediction.get( )->update( valve::g_client_state->m_delta_tick,
                valve::g_client_state->m_delta_tick > 0,
                valve::g_client_state->m_last_cmd_ack,
                valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds );
        }

        m_prediction.process_prediction( &cmd );

        hacks::g_exploits->manage_wpn( cmd );

        hacks::g_move->auto_peek( old_angles_, cmd );

        m_anti_aim.choke( send_packet );

        hacks::g_visuals->on_create_move( cmd );

        if ( !m_can_shoot && m_weapon && !m_weapon->is_knife( )
            && m_weapon_info && m_weapon_info->m_type != valve::e_weapon_type::grenade
            && m_weapon->item_index( ) != valve::e_item_index::revolver )
            cmd.m_buttons &= ~valve::e_buttons::in_attack;

        if ( hacks::g_exploits->in_charge( ) && m_weapon
            && m_weapon_info && m_weapon_info->m_type != valve::e_weapon_type::grenade
            && m_weapon->item_index( ) != valve::e_item_index::revolver )
            cmd.m_buttons &= ~valve::e_buttons::in_attack;

        if ( hacks::g_exploits->in_charge( )
            && !hacks::g_move->should_fake_duck( ) && m_weapon_info 
            && m_weapon_info->m_type != valve::e_weapon_type::grenade ) {
            if ( !hacks::g_exploits->shift( )
                && !hacks::g_exploits->charged( ) )
                send_packet = true;

            cmd.m_buttons &= ~valve::e_buttons::in_attack;
        }

        hacks::g_aim_bot->handle( cmd, send_packet );

        hacks::g_knife_bot->handle_knife_bot( cmd );

        if ( valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) && m_weapon &&
            !m_weapon->is_knife( ) && hacks::g_exploits->is_peeking( const_cast < sdk::qang_t& > ( old_angles ), 6.f )
            && ( hacks::g_exploits->type( ) == 2 || hacks::g_exploits->type( ) == 3 ) && hacks::g_exploits->defensive_allowed( )
            && g_menu->main( ).m_aim_bot_cfg.get( ).m_rage_bot && m_weapon_info && m_weapon_info->m_type != valve::e_weapon_type::grenade ) {
            hacks::g_exploits->type( ) = 5;

            auto& local_data = prediction( ).data( ).local_data( ).at( cmd.m_number % 150 );

            local_data.m_override_tick_base = local_data.m_restore_tick_base = true;
            local_data.m_adjusted_tick_base = local_data.m_tick_base - hacks::g_exploits->next_shift_amount( ) - 1;

            break_lc = true;
            send_packet = true;
        }       

        bool can_run_pitch{ true };

        if ( m_can_shoot
            && will_shoot( m_weapon, cmd ) )
            can_run_pitch = false;

        if ( can_run_pitch
            && anti_aim( ).should_run( cmd ) 
            && !( cmd.m_buttons & valve::e_buttons::in_use ) ) {

            if ( !break_lc )
                cmd.m_view_angles.x( ) = g_menu->main( ).m_anti_aim_cfg.get( ).m_pitch;
            else
                cmd.m_view_angles.x( ) = g_ctx->offsets( ).m_addresses.m_random_float( -90.f, 90.f );
        }

        hacks::g_move->rotate( cmd, old_angles );

        if ( m_can_shoot
            && will_shoot( m_weapon, cmd ) ) {
            auto& data = prediction( ).data( );

            if ( m_weapon_info )
                data.shot_valid_wpn( ) = true;

            data.shot_cmd_number( ) = cmd.m_number;

            hacks::g_aim_bot->shoot_pos( ) = m_shoot_pos;

            if ( hacks::g_aim_bot->shooting( ) ) {
                const auto& aim_target = hacks::g_aim_bot->m_last_target;
                if ( g_menu->main( ).m_models_cfg.get( ).m_shot ) {
                    const auto& cur_bones = aim_target.m_lag_record->m_anim_sides.at( aim_target.m_lag_record->m_anim_side ).m_bones;

                    hacks::g_visuals->add_shot_mdl( aim_target.m_entry->m_player, cur_bones.data( ) );
                }
            }
            else {
                hacks::g_shots->add(
                    g_local_player->shoot_pos( ), nullptr,
                    hacks::g_exploits->next_shift_amount( ), cmd.m_number, valve::g_global_vars.get( )->m_real_time, g_local_player->net_info( ).m_latency.m_out + g_local_player->net_info( ).m_latency.m_in
                );
            }

            if ( !hacks::g_move->should_fake_duck( ) ) {
                if ( hacks::g_exploits->next_shift_amount( ) ) {
                    send_packet = true;
                }

                hacks::g_exploits->cur_shift_amount( ) = hacks::g_exploits->next_shift_amount( );
            }
        }

        anim_sync ( ).update_local_real( cmd, send_packet );

        if ( self( )->move_type( ) == valve::e_move_type::walk ) {
            cmd.m_buttons &= ~(
                valve::e_buttons::in_fwd
                | valve::e_buttons::in_back
                | valve::e_buttons::in_move_right
                | valve::e_buttons::in_move_left
                );

            if ( cmd.m_move.x( ) != 0.f )
                cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cmd.m_move.x( ) < 0.f : cmd.m_move.x( ) > 0.f )
                ? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

            if ( cmd.m_move.y( ) != 0.f )
                cmd.m_buttons |=
                ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cmd.m_move.y( ) < 0.f : cmd.m_move.y( ) > 0.f )
                ? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
        }

        if ( m_weapon ) {
            m_weapon->recoil_index( ) = prediction( ).data( ).recoil_index( );
            m_weapon->accuracy_penalty( ) = prediction( ).data( ).accuracy_penalty( );
        }
    }
    else {
        send_packet = false;

        hacks::g_exploits->manage_wpn( cmd );

        anim_sync( ).setup_local_bones( );

        cmd.m_tick = std::numeric_limits < int >::max( );

        auto& local_data = prediction( ).data( ).local_data( ).at( cmd.m_number % 150u );

        local_data.init( cmd );
        local_data.save( cmd.m_number );
    }

    hacks::g_move->prev_view_yaw( ) = old_angles.y( );

    if ( !g_menu->main( ).m_anti_aim_cfg.get( ).m_roll )
        cmd.sanitize( );
    else {
        normalize_ghetto( cmd.m_view_angles );
        clamp_angles( cmd.m_view_angles );
    }

    if ( self( )->move_type( ) == valve::e_move_type::walk ) {
        cmd.m_buttons &= ~(
            valve::e_buttons::in_fwd
            | valve::e_buttons::in_back
            | valve::e_buttons::in_move_right
            | valve::e_buttons::in_move_left
            );

        if ( cmd.m_move.x( ) != 0.f )
            cmd.m_buttons |=
            ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cmd.m_move.x( ) < 0.f : cmd.m_move.x( ) > 0.f )
            ? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

        if ( cmd.m_move.y( ) != 0.f )
        cmd.m_buttons |=
            ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk ? cmd.m_move.y( ) < 0.f : cmd.m_move.y( ) > 0.f )
            ? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
    }

    if ( valve::g_client_state->m_choked_cmds >= 14 )
        send_packet = true;

    if ( send_packet ) {
        if ( valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
            auto& local_data = prediction( ).data( ).local_data( ).at( cmd.m_number % 150 );

            local_data.m_override_tick_base = true;
            local_data.m_adjusted_tick_base = hacks::g_exploits->adjust_tick_base(
                valve::g_client_state->m_choked_cmds + 1, 1, -valve::g_client_state->m_choked_cmds
            );
        }
        else if ( break_lc ) {
            hacks::g_exploits->type( ) = 5;
            hacks::g_exploits->cur_shift_amount( ) = hacks::g_exploits->next_shift_amount( );
        }
    }

    m_net_data.process_net( send_packet );

    m_prediction.end_prediction( );

    hacks::g_exploits->charged( ) = false;

    m_net_data.add_net_cmd( cmd.m_number, send_packet );

    if ( cmd.m_tick != std::numeric_limits< int >::max( )
        && send_packet ) {
        if ( !hacks::g_exploits->cur_shift_amount( )
            || hacks::g_exploits->type( ) == 3
            || hacks::g_exploits->type( ) == 4 )
            if ( valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) )
                m_last_sent_cmd_number = cmd.m_number;
    }

    m_last_cmd_number = cmd.m_number;

    hacks::g_misc->buy_bot( );

    if ( cmd.m_buttons & valve::e_buttons::in_attack )
        prediction( ).data( ).last_shot_time( ) = valve::g_global_vars.get( )->m_cur_time;

    m_prediction.data( ).local_data( ).at( cmd.m_number % 150 ).m_move = cmd.m_move;

    m_left_create_move = true;

    vfyd_cmd.m_cmd = cmd;
    vfyd_cmd.m_checksum = cmd.checksum( );
}