#pragma once

#include "../local_player.hpp"

ALWAYS_INLINE valve::cs_player_t* c_local_player::self( ) const {
    return *g_ctx->offsets( ).m_local_player.as< valve::cs_player_t** >( );
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::recoil_index( ) {
    return m_recoil_index;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::accuracy_penalty( ) {
    return m_accuracy_penalty;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::velocity_modifier( ) {
    return m_velocity_modifier;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::is_out_of_epsilon( ) {
    return m_is_out_of_epsilon;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::spread( ) {
    return m_spread;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::inaccuracy( ) {
    return m_inaccuracy;
};

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::min_inaccuracy( ) {
    return m_min_inaccuracy;
}


ALWAYS_INLINE sdk::qang_t& c_local_player::prediction_t::data_t::shot_angles( ) {
	return m_shot_angles;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::shot_this_cycle( ) {
	return m_shot_this_cycle;
}

ALWAYS_INLINE valve::move_data_t& c_local_player::prediction_t::data_t::move_data( ) {
    return m_move_data;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::net_velocity_modifier( ) {
    return m_net_velocity_modifier;
}

ALWAYS_INLINE valve::bones_t& c_local_player::prediction_t::data_t::shoot_bones( ) {
	return m_shoot_bones;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::shot_valid_wpn( ) {
	return m_shot_valid_wpn;
}

ALWAYS_INLINE std::ptrdiff_t& c_local_player::prediction_t::data_t::shot_cmd_number( ) {
	return m_shot_cmd_number;
}

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::last_shot_time( ) {
	return m_last_shot_time;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::shot( ) {
	return m_shot;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::old_shot( ) {
	return m_old_shot;
}

ALWAYS_INLINE valve::user_cmd_t& c_local_player::last_user_cmd( ) {
	return m_last_user_cmd;
}

ALWAYS_INLINE bool& c_local_player::left_create_move( ) {
	return m_left_create_move;
}

ALWAYS_INLINE bool& c_local_player::prediction_t::data_t::r8_can_shoot( ) { return m_r8_can_shoot; }

ALWAYS_INLINE float& c_local_player::prediction_t::data_t::postpone_fire_ready_time( ) { return m_postpone_fire_ready_time; };

ALWAYS_INLINE int& c_local_player::last_sent_cmd_number( ) { return m_last_sent_cmd_number; }

ALWAYS_INLINE int& c_local_player::last_cmd_number( ) { return m_last_cmd_number; }

ALWAYS_INLINE c_local_player::anim_sync_t& c_local_player::anim_sync( ) { return m_anim_sync; }

ALWAYS_INLINE void c_local_player::anim_sync_t::update_local( const sdk::qang_t& view_angles, const bool no_view_model, const float time ) {
	const auto anim_state = g_local_player->self( )->anim_state( );
	if ( !anim_state )
		return;

	valve::anim_state_t backup_state;

	const auto backup_cur_time = valve::g_global_vars.get( )->m_cur_time;
	const auto backup_frame_time = valve::g_global_vars.get( )->m_frame_time;
	const auto backup_real_time = valve::g_global_vars.get( )->m_real_time;

	valve::g_global_vars.get( )->m_cur_time = time;
	valve::g_global_vars.get( )->m_real_time = time;
	valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;

	g_local_player->self( )->set_local_view_angles( view_angles );

	anim_state->m_last_update_frame = valve::g_global_vars.get( )->m_frame_count - 1;

	do_animation_event( 0 );

	const auto backup_abs_velocity = g_local_player->self( )->abs_velocity( );

	g_local_player->self( )->abs_velocity( ) = g_local_player->self( )->velocity( );

	const auto backup_eflags = g_local_player->self( )->eflags( );

	g_local_player->self( )->eflags( ) &= ~0x1000u;

	const auto backup_client_side_anim = g_local_player->self( )->client_side_anim( );

	g_local_player->self( )->client_side_anim( ) = g_ctx->allow_update_anim( ) = true;

	if ( no_view_model )
		anim_state->update( view_angles.x( ), view_angles.y( ), view_angles.z( ) );
	else
		g_local_player->self( )->update_client_side_anim( );

	g_local_player->self( )->client_side_anim( ) = backup_client_side_anim;

	g_ctx->allow_update_anim( ) = false;

	g_local_player->self( )->eflags( ) = backup_eflags;

	g_local_player->self( )->abs_velocity( ) = backup_abs_velocity;

	valve::g_global_vars.get( )->m_cur_time = backup_cur_time;
	valve::g_global_vars.get( )->m_real_time = backup_real_time;
	valve::g_global_vars.get( )->m_frame_time = backup_frame_time;
}

ALWAYS_INLINE bool c_local_player::anim_sync_t::setup_bones(
	valve::bones_t& bones, const int time, const int flags
) {
	struct backup_t {
		__forceinline constexpr backup_t( ) = default;

		__forceinline backup_t( valve::cs_player_t* const player )
			: m_cur_time{ valve::g_global_vars.get( )->m_cur_time },
			m_frame_time{ valve::g_global_vars.get( )->m_frame_time },
			m_frame_count{ valve::g_global_vars.get( )->m_frame_count },
			m_real_time{ valve::g_global_vars.get( )->m_real_time },
			m_tick_count{ valve::g_global_vars.get( )->m_tick_count },
			m_abs_frame_time{ valve::g_global_vars.get( )->m_abs_frame_time },
			m_interp_amt{ valve::g_global_vars.get( )->m_interp_amt },
			m_occlusion_frame{ player->occlusion_frame( ) },
			m_ent_client_flags{ player->ent_client_flags( ) },
			m_ik_context{ player->ik_context( ) }, m_effects{ player->effects( ) },
			m_occlusion_flags{ player->occlusion_flags( ) } {}

		float					m_cur_time{}, m_frame_time{}, m_real_time{}, m_abs_frame_time{}, m_interp_amt{ };
		int						m_frame_count{}, m_occlusion_frame{}, m_tick_count{};
		std::uint8_t			m_ent_client_flags{};
		valve::ik_context_t* m_ik_context{};

		std::size_t				m_effects{}, m_occlusion_flags{};
	} backup{ g_local_player->self( ) };

	valve::g_global_vars.get( )->m_cur_time = time;
	valve::g_global_vars.get( )->m_real_time = time;
	valve::g_global_vars.get( )->m_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
	valve::g_global_vars.get( )->m_abs_frame_time = valve::g_global_vars.get( )->m_interval_per_tick;
	valve::g_global_vars.get( )->m_frame_count = valve::to_ticks( time );
	valve::g_global_vars.get( )->m_tick_count = valve::to_ticks( time );
	valve::g_global_vars.get( )->m_interp_amt = 0.f;

	if ( flags & 8 ) {
		g_local_player->self( )->effects( ) |= 8u;
		g_local_player->self( )->occlusion_flags( ) &= ~0xau;
		g_local_player->self( )->occlusion_frame( ) = 0;
	}

	if ( flags & 4 ) {
		g_local_player->self( )->ik_context( ) = nullptr;
		g_local_player->self( )->ent_client_flags( ) |= 2u;
	}

	if ( flags & 2 )
		g_local_player->self( )->last_setup_bones_frame( ) = 0;

	if ( flags & 1 ) {
		g_local_player->self( )->mdl_bone_cnt( ) = 0ul;
		g_local_player->self( )->last_setup_bones_time( ) = std::numeric_limits< float >::lowest( );

		auto& bone_accessor = g_local_player->self( )->bone_accessor( );

		bone_accessor.m_writable_bones = bone_accessor.m_readable_bones = 0;
	}

	static auto jiggle_bones = valve::g_cvar->find_var( "r_jiggle_bones" );

	const auto backup_jiggle_bones = jiggle_bones->get_int( );

	jiggle_bones->set_int( false );

	g_ctx->allow_setup_bones( ) = true;
	const auto ret = g_local_player->self( )->renderable( )->setup_bones( bones.data( ), 256, ( ( ( flags >> 4 ) & 1 ) << 9 ) + 0xffd00, time );
	g_ctx->allow_setup_bones( ) = false;

	if ( flags & 4 ) {
		g_local_player->self( )->ik_context( ) = backup.m_ik_context;
	}

	jiggle_bones->set_int( backup_jiggle_bones );

	if ( flags & 8 ) {
		g_local_player->self( )->effects( ) = backup.m_effects;
		g_local_player->self( )->occlusion_flags( ) = backup.m_occlusion_flags;
		g_local_player->self( )->occlusion_frame( ) = backup.m_occlusion_frame;
	}

	if ( flags & 4 ) {
		g_local_player->self( )->ent_client_flags( ) = backup.m_ent_client_flags;
	}

	valve::g_global_vars.get( )->m_cur_time = backup.m_cur_time;
	valve::g_global_vars.get( )->m_real_time = backup.m_real_time;
	valve::g_global_vars.get( )->m_abs_frame_time = backup.m_abs_frame_time;
	valve::g_global_vars.get( )->m_frame_time = backup.m_frame_time;
	valve::g_global_vars.get( )->m_tick_count = backup.m_tick_count;
	valve::g_global_vars.get( )->m_frame_count = backup.m_frame_count;
	valve::g_global_vars.get( )->m_interp_amt = backup.m_interp_amt;

	return ret;
}

ALWAYS_INLINE bool& c_local_player::can_shoot( ) { return m_can_shoot; }

ALWAYS_INLINE bool c_local_player::can_shoot(
	bool skip_r8, const int shift_amt, const bool what
) {
	if ( !g_local_player->self( )
		|| !g_local_player->self( )->alive( ) )
		return false;

	auto weapon = g_local_player->self( )->weapon( );
	if ( !weapon )
		return false;

	auto weapon_data = weapon->info( );
	if ( !weapon_data )
		return false;

	if ( g_local_player->self( )->flags( ) & valve::e_ent_flags::frozen )
		return false;

	if ( g_local_player->self( )->wait_for_no_attack( ) )
		return false;

	if ( g_local_player->self( )->defusing( ) )
		return false;

	auto anim_layer = g_local_player->self( )->anim_layers( ).at( 1u );

	if ( anim_layer.m_owner ) {
		if ( g_local_player->self( )->lookup_seq_act( anim_layer.m_seq ) == 967
			&& anim_layer.m_weight != 0.f )
			return false;
	}

	if ( weapon_data->m_type >= valve::e_weapon_type::pistol && weapon_data->m_type <= valve::e_weapon_type::machine_gun && weapon->clip1( ) < 1 )
		return false;

	float curtime = valve::to_time( g_local_player->self( )->tick_base( ) - shift_amt );
	if ( curtime < g_local_player->self( )->next_attack( ) )
		return false;

	if ( ( weapon->item_index( ) == valve::e_item_index::glock || weapon->item_index( ) == valve::e_item_index::famas ) && weapon->burst_shots_remaining( ) > 0 ) {
		if ( curtime >= weapon->next_burst_shot( ) )
			return true;
	}

	if ( self( )->weapon( )->item_index( ) == valve::e_item_index::c4 )
		return true;

	if ( self( )->weapon( )->item_index( ) >= valve::e_item_index::flashbang
		&& self( )->weapon( )->item_index( ) <= valve::e_item_index::inc_grenade )
		return  ( ( valve::base_grenade_t* ) g_local_player->weapon( ) )->throw_time( ) > 0.f && curtime > ( ( valve::base_grenade_t* ) g_local_player->weapon( ) )->throw_time( );

	if ( curtime < weapon->next_primary_attack( ) )
		return false;

	if ( weapon->item_index( ) != valve::e_item_index::revolver )
		return true;

	if ( skip_r8 )
		return true;

	return curtime >= weapon->postpone_fire_ready_time( );
}

ALWAYS_INLINE std::array < c_local_player::prediction_t::data_t::local_t, 150u >& c_local_player::prediction_t::data_t::local_data( ) {
    return m_local_data;
}

ALWAYS_INLINE void c_local_player::prediction_t::reset( ) {
	data( ).net_velocity_modifier( ) = 1.f;

	std::memset( data( ).local_data( ).data( ), 0, sizeof( prediction_t::data_t::local_t ) * data( ).local_data( ).size( ) );
}

ALWAYS_INLINE std::array < c_local_player::prediction_t::data_t::net_vars_t, 150u >& c_local_player::prediction_t::data_t::net_vars( ) {
    return m_net_vars;
}


ALWAYS_INLINE bool c_local_player::will_shoot(
	valve::base_weapon_t* const weapon, const valve::user_cmd_t& user_cmd
) const {
	if ( !weapon )
		return false;

	const auto item_index = weapon->item_index( );
	const auto wpn_data = weapon->info( );

	if ( ( item_index < valve::e_item_index::flashbang || item_index > valve::e_item_index::inc_grenade )
		&& !( user_cmd.m_buttons & valve::e_buttons::in_attack )
		&& ( !( user_cmd.m_buttons & valve::e_buttons::in_attack2 )
			|| weapon->next_secondary_attack( ) >= valve::g_global_vars.get( )->m_cur_time
			|| ( item_index != valve::e_item_index::revolver && ( !wpn_data || wpn_data->m_unk_type != 1 ) )
			)
		)
		return false;

	return true;
}

ALWAYS_INLINE c_local_player::prediction_t& c_local_player::prediction( ) { return m_prediction; }

ALWAYS_INLINE const void c_local_player::prediction_t::data_t::net_vars_t::store( const std::ptrdiff_t cmd_number ) {
    m_cmd_number = cmd_number;

    m_view_punch = g_local_player->self( )->view_punch( );
    m_aim_punch = g_local_player->self( )->aim_punch( );
    m_aim_punch_vel = g_local_player->self( )->aim_punch_vel( );
    m_view_offset_z = std::clamp( g_local_player->self( )->view_offset( ).z( ), 0.f, 64.f );
    m_velocity = g_local_player->self( )->velocity( );
    m_origin = g_local_player->self( )->origin( );
    m_velocity_modifier = g_local_player->self( )->velocity_modifier( );
    m_fall_velocity = g_local_player->self( )->fall_velocity( );
}

ALWAYS_INLINE sdk::vec3_t& c_local_player::shoot_pos( ) { return m_shoot_pos; }

ALWAYS_INLINE c_local_player::anti_aim_t& c_local_player::anti_aim( ) { return m_anti_aim; }

ALWAYS_INLINE const void c_local_player::prediction_t::data_t::net_vars_t::restore( const std::ptrdiff_t cmd_number ) const {
    if ( m_cmd_number != cmd_number )
        return;

	int counter{};

	if ( std::abs( g_local_player->self( )->view_offset( ).z( ) - m_view_offset_z ) <= 0.03125f )
		g_local_player->self( )->view_offset( ).z( ) = m_view_offset_z;

	const auto aim_punch_delta = g_local_player->self( )->aim_punch( ) - m_aim_punch;

	if ( std::abs( aim_punch_delta.x( ) ) <= 0.5f
		&& std::abs( aim_punch_delta.y( ) ) <= 0.5f
		&& std::abs( aim_punch_delta.z( ) ) <= 0.5f )
		g_local_player->self( )->aim_punch( ) = m_aim_punch;
	else
		counter++;

	const auto aim_punch_vel_delta = g_local_player->self( )->aim_punch_vel( ) - m_aim_punch_vel;
	if ( std::abs( aim_punch_vel_delta.x( ) ) <= 0.5f
		&& std::abs( aim_punch_vel_delta.y( ) ) <= 0.5f
		&& std::abs( aim_punch_vel_delta.z( ) ) <= 0.5f )
		g_local_player->self( )->aim_punch_vel( ) = m_aim_punch_vel;
	else
		counter++;

	const auto view_punch_delta = g_local_player->self( )->view_punch( ) - m_view_punch;
	if ( std::abs( view_punch_delta.x( ) ) <= 0.5f
		&& std::abs( view_punch_delta.y( ) ) <= 0.5f
		&& std::abs( view_punch_delta.z( ) ) <= 0.5f )
		g_local_player->self( )->view_punch( ) = m_view_punch;
	else
		counter++;

	const auto velocity_modifier_delta = g_local_player->self( )->velocity_modifier( ) - m_velocity_modifier;
	if ( std::abs( velocity_modifier_delta ) <= 0.00625f )
		g_local_player->self( )->velocity_modifier( ) = m_velocity_modifier;
	else
		counter++;

	const auto fall_velocity_delta = g_local_player->self( )->fall_velocity( ) - m_fall_velocity;
	if ( std::abs( fall_velocity_delta ) <= 0.5f )
		g_local_player->self( )->fall_velocity( ) = m_fall_velocity;
	else
		counter++;

	const auto velocity_delta = g_local_player->self( )->velocity( ) - m_velocity;
	if ( std::abs( velocity_delta.x( ) ) <= 0.5f
		&& std::abs( velocity_delta.y( ) ) <= 0.5f
		&& std::abs( velocity_delta.z( ) ) <= 0.5f )
		g_local_player->self( )->velocity( ) = m_velocity;
	else
		counter++;

	const auto origin_delta = g_local_player->self( )->origin( ) - m_origin;
	if ( std::abs( origin_delta.x( ) ) <= 0.1f
		&& std::abs( origin_delta.y( ) ) <= 0.1f
		&& std::abs( origin_delta.z( ) ) <= 0.1f )
		g_local_player->self( )->origin( ) = m_origin;
	else
		counter++;

	g_local_player->prediction( ).data( ).is_out_of_epsilon( ) = counter > 0 ? true : false;
}

ALWAYS_INLINE c_local_player::prediction_t::data_t& c_local_player::prediction_t::data ( ) { return m_data; }

ALWAYS_INLINE c_local_player::prediction_t::view_model_t& c_local_player::prediction_t::view_model( ) { return m_view_model; }

ALWAYS_INLINE std::ptrdiff_t& c_local_player::cur_seq( ) { return m_cur_seq; }

ALWAYS_INLINE void c_local_player::prediction_t::data_t::local_t::init( const valve::user_cmd_t& user_cmd ) {
    std::memset( this, 0, sizeof( local_t ) );

	g_local_player->prediction( ).data( ).velocity_modifier( ) = g_local_player->self( )->velocity_modifier( );

    if ( const auto weapon = g_local_player->self( )->weapon( ) ) {
		g_local_player->prediction( ).data( ).recoil_index( ) = weapon->recoil_index( );
		g_local_player->prediction( ).data( ).accuracy_penalty( ) = weapon->accuracy_penalty( );
    }

    m_spawn_time = g_local_player->self( )->spawn_time( );
    m_tick_base = m_adjusted_tick_base = g_local_player->self( )->tick_base( );

    m_user_cmd = user_cmd;

	store_anim( );

	m_net_vars.m_eye_angles = g_local_player->self( )->eye_angles( );
	m_net_vars.m_aim_punch = g_local_player->self( )->aim_punch( );
	m_net_vars.m_view_punch = g_local_player->self( )->view_punch( );

	m_net_vars.m_origin = g_local_player->self( )->origin( );
	m_net_vars.m_abs_origin = g_local_player->self( )->abs_origin( );
	m_net_vars.m_obb_min = g_local_player->self( )->obb_min( );
	m_net_vars.m_obb_max = g_local_player->self( )->obb_max( );
	m_net_vars.m_velocity = g_local_player->self( )->velocity( );
	m_net_vars.m_view_offset = g_local_player->self( )->view_offset( );
	m_net_vars.m_aim_punch_vel = g_local_player->self( )->aim_punch_vel( );
	m_net_vars.m_abs_velocity = g_local_player->self( )->abs_velocity( );
	m_net_vars.m_velocity_modifier = g_local_player->self( )->velocity_modifier( );

	m_net_vars.m_scoped = g_local_player->self( )->scoped( );
	m_net_vars.m_walking = g_local_player->self( )->walking( );
	m_net_vars.m_fall_velocity = g_local_player->self( )->fall_velocity( );
	m_net_vars.m_fall_velocity = g_local_player->self( )->fall_velocity( );
	m_net_vars.m_tick_base = g_local_player->self( )->tick_base( );

	m_net_vars.m_duck_amount = g_local_player->self( )->duck_amt( );
	m_net_vars.m_duck_speed = g_local_player->self( )->duck_speed( );
	m_net_vars.m_third_person_recoil = g_local_player->self( )->third_person_recoil( );
	m_net_vars.m_lby = g_local_player->self( )->lby( );

	m_net_vars.m_flags = g_local_player->self( )->flags( );
	m_net_vars.m_move_type = g_local_player->self( )->move_type( );

	m_pred_net_vars = m_net_vars;
}

ALWAYS_INLINE void c_local_player::prediction_t::data_t::local_t::save( const int cmd_number ) {
	if ( cmd_number != m_user_cmd.m_number )
		return;

	store_anim( );

	m_pred_net_vars.m_eye_angles = g_local_player->self( )->eye_angles( );
	m_pred_net_vars.m_aim_punch = g_local_player->self( )->aim_punch( );
	m_pred_net_vars.m_view_punch = g_local_player->self( )->view_punch( );

	m_pred_net_vars.m_origin = g_local_player->self( )->origin( );
	m_pred_net_vars.m_abs_origin = g_local_player->self( )->abs_origin( );
	m_pred_net_vars.m_obb_min = g_local_player->self( )->obb_min( );
	m_pred_net_vars.m_obb_max = g_local_player->self( )->obb_max( );
	m_pred_net_vars.m_velocity = g_local_player->self( )->velocity( );
	m_pred_net_vars.m_view_offset = g_local_player->self( )->view_offset( );
	m_pred_net_vars.m_aim_punch_vel = g_local_player->self( )->aim_punch_vel( );
	m_pred_net_vars.m_abs_velocity = g_local_player->self( )->abs_velocity( );

	m_pred_net_vars.m_velocity_modifier = g_local_player->self( )->velocity_modifier( );
	m_pred_net_vars.m_scoped = g_local_player->self( )->scoped( );
	m_pred_net_vars.m_walking = g_local_player->self( )->walking( );
	m_pred_net_vars.m_fall_velocity = g_local_player->self( )->fall_velocity( );

	m_pred_net_vars.m_tick_base = g_local_player->self( )->tick_base( );

	m_pred_net_vars.m_duck_amount = g_local_player->self( )->duck_amt( );
	m_pred_net_vars.m_duck_speed = g_local_player->self( )->duck_speed( );
	m_pred_net_vars.m_third_person_recoil = g_local_player->self( )->third_person_recoil( );
	m_pred_net_vars.m_lby = g_local_player->self( )->lby( );

	m_pred_net_vars.m_flags = g_local_player->self( )->flags( );
	m_pred_net_vars.m_move_type = g_local_player->self( )->move_type( );
}

ALWAYS_INLINE void c_local_player::prediction_t::data_t::local_t::store_anim( ) {
	if ( const auto anim_state = g_local_player->self( )->anim_state( ) ) {
		m_anim_state = *anim_state;
		m_abs_yaw = anim_state->m_foot_yaw;
	}

	m_anim_layers = g_local_player->self( )->anim_layers( );
	m_pose_params = g_local_player->self( )->pose_params( );
}

ALWAYS_INLINE void c_local_player::prediction_t::data_t::local_t::restore_anim( const bool layers ) const {
	if ( const auto anim_state = g_local_player->self( )->anim_state( ) )
		*anim_state = m_anim_state;

	if ( layers )
		g_local_player->self( )->anim_layers( ) = m_anim_layers;

	g_local_player->self( )->pose_params( ) = m_pose_params;

	g_local_player->self( )->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );
}

ALWAYS_INLINE valve::weapon_cs_base_gun_t* c_local_player::weapon( ) const { return m_weapon; }

ALWAYS_INLINE valve::weapon_info_t* c_local_player::weapon_info( ) const { return m_weapon_info; }

ALWAYS_INLINE valve::e_frame_stage& c_local_player::last_frame_stage( ) { return m_last_frame_stage; }

ALWAYS_INLINE c_local_player::net_data_t& c_local_player::net_data( ) { return m_net_data; }

ALWAYS_INLINE std::vector < int >& c_local_player::net_cmds( ) { return m_net_cmds; }

ALWAYS_INLINE c_local_player::net_info_t& c_local_player::net_info( ) { return m_net_info; }

ALWAYS_INLINE void c_local_player::net_data_t::add_net_cmd( const std::ptrdiff_t cmd_number, const bool& send_packet ) {
	if ( !send_packet )
		return;

	g_local_player->net_cmds( ).emplace_back( cmd_number );
}

ALWAYS_INLINE const void c_local_player::net_data_t::process_net( const bool& send_packet ) const {
	if ( send_packet 
		|| !valve::g_client_state->m_net_chan )
		return;

	const auto backup_choked_packets = valve::g_client_state->m_net_chan->m_choked_packets;

	valve::g_client_state->m_net_chan->m_choked_packets = 0;
	valve::g_client_state->m_net_chan->send_datagram( );
	--valve::g_client_state->m_net_chan->m_out_seq;

	valve::g_client_state->m_net_chan->m_choked_packets = backup_choked_packets;
};


ALWAYS_INLINE void c_local_player::prediction_t::correct_view_offset( ) {
	if ( !g_local_player->self( ) 
		|| !g_local_player->self( )->alive( ) 
		|| valve::g_movement.get( )->player( ) != g_local_player->self( ) )
		return;

	if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::ducking ) && !g_local_player->self( )->ducking( ) && !g_local_player->self( )->ducked( ) )
		g_local_player->self( )->view_offset( ) = { 0, 0, 64 };
	else if ( g_local_player->self( )->duck_until_on_ground( ) )
	{
		sdk::vec3_t hull_size_based = sdk::vec3_t( 16, 16, 72 ) - sdk::vec3_t( -16, -16, 0 );
		sdk::vec3_t hull_size_duck = sdk::vec3_t( 16, 16, 36 ) - sdk::vec3_t( -16, -16, 0 );
		sdk::vec3_t lower_clearence = hull_size_based - hull_size_duck;
		sdk::vec3_t duck_height = valve::g_movement.get( )->player_view_offset( false ) - lower_clearence;

		g_local_player->self( )->view_offset( ) = duck_height;
	}
	else if ( g_local_player->self( )->ducked( )
		&& !g_local_player->self( )->ducking( ) )
		g_local_player->self( )->view_offset( ) = { 0, 0, 46 };

}

ALWAYS_INLINE void c_local_player::prediction_t::velocity_modifier_to_data_map( ) {
	static const auto& net_var = g_ctx->offsets( ).m_cs_player.m_velocity_modifier;

	valve::data_map_t* data_map = g_local_player->self( )->pred_desc_map( );

	valve::type_desc_t* type_desc = g_local_player->self( )->data_map_entry( data_map, ( "m_chief_keef" ) );

	if ( net_var > 0 )
	{
		if ( type_desc )
		{
			const auto recovery_rate = 1.f / 2.5f;
			const auto tolerance = recovery_rate * valve::g_global_vars.get( )->m_interval_per_tick;

			if ( type_desc->m_tolerance != tolerance )
			{
				int offset = net_var;

				type_desc->m_type = 1;
				type_desc->m_tolerance = tolerance;
				type_desc->m_offset = offset;
				type_desc->m_field_size_in_bytes = sizeof( float );
				type_desc->m_flat_offset[ 0 ] = offset;

				data_map->m_packed_offsets_computed = false;
				data_map->m_packed_size = 0;
			}
		}
	}
}

ALWAYS_INLINE void c_local_player::prediction_t::third_person_recoil_to_data_map( ) {
	static const auto& net_var = g_ctx->offsets( ).m_cs_player.m_third_person_recoil;

	valve::data_map_t* data_map = g_local_player->self( )->pred_desc_map( );
	valve::type_desc_t* type_desc = g_local_player->self( )->data_map_entry( data_map, ( "m_bitch_love_sosa" ) );

	if ( net_var > 0 )
	{
		if ( type_desc )
		{
			const auto tolerance = 0.5f;

			if ( type_desc->m_tolerance != tolerance )
			{
				int offset = net_var;

				type_desc->m_type = 1;
				type_desc->m_tolerance = tolerance;
				type_desc->m_offset = offset;
				type_desc->m_field_size_in_bytes = sizeof( float );
				type_desc->m_flat_offset[ 0 ] = offset;

				data_map->m_packed_offsets_computed = false;
				data_map->m_packed_size = 0;
			}
		}
	}
}

ALWAYS_INLINE const void c_local_player::prediction_t::view_model_t::store_view_model( ) {
	auto view_model = valve::g_entity_list.get( )->get_entity( g_local_player->self( )->view_mdl_handle( ) );

	if ( !view_model )
		return;

	m_animation_parity = ( ( valve::base_view_model_t* ) view_model )->anim_parity( );
	m_view_sequence = ( ( valve::base_view_model_t* ) view_model )->sequence( );
	m_view_cycle = view_model->cycle( );
	m_anim_time = view_model->anim_time( );
}

ALWAYS_INLINE const void c_local_player::prediction_t::view_model_t::restore_view_model( ) {
	auto view_model = valve::g_entity_list.get( )->get_entity( g_local_player->self( )->view_mdl_handle( ) );

	if ( !view_model )
		return;

	if ( m_view_sequence != ( ( valve::base_view_model_t* ) view_model )->sequence( ) || m_animation_parity != ( ( valve::base_view_model_t* ) view_model )->anim_parity( ) )
		return;

	view_model->cycle( ) = m_view_cycle;
	view_model->anim_time( ) = m_anim_time;
}

__forceinline void set_origin( sdk::mat3x4_t& who, const sdk::vec3_t p ) {
	who[ 0 ][ 3 ] = p.x( );
	who[ 1 ][ 3 ] = p.y( );
	who[ 2 ][ 3 ] = p.z( );
}

ALWAYS_INLINE int c_local_player::anti_aim_t::select_side( ) {
	const auto& cfg = g_menu->main( ).m_anti_aim_cfg.get( );

	int side{};

	static bool invert_side{};

	if ( !valve::g_client_state->m_choked_cmds )
		invert_side = !invert_side;

	if ( cfg.m_jitter_side ) {
		if ( invert_side )
			side = 2;
		else
			side = 1;
	}
	else {
		if ( g_key_binds->get_keybind_state( &cfg.m_flip_side ) )
			side = 2;
		else
			side = 1;
	}

	return side;
}

ALWAYS_INLINE void c_local_player::anti_aim_t::select_yaw( float& yaw, const int side, valve::user_cmd_t& user_cmd ) {
	const auto& cfg = g_menu->main( ).m_anti_aim_cfg.get( );

	yaw = valve::g_engine.get( )->view_angles( ).y( );

	if ( user_cmd.m_buttons & valve::e_buttons::in_use )
		return;

	if ( !freestanding( yaw ) ) {
		if ( manual_yaw( ) != std::numeric_limits < float >::max( ) ) {
			yaw += manual_yaw( );
		}
		else {
			if ( cfg.m_at_targets
				&& !cfg.m_roll )
				at_target( yaw );
			else
				yaw += cfg.m_yaw;
		}
	}
	else {
		if ( m_auto_dir_side ) {
			if ( m_auto_dir_side == 2 ) {
				yaw += 90.f;
			}
			else {
				yaw -= 90.f;
			}
		}
	}

	static bool invert_jitter{};

	float random_val{ g_ctx->offsets( ).m_addresses.m_random_float( -30, 30 ) };

	if ( !valve::g_client_state->m_choked_cmds )
		invert_jitter = !invert_jitter;

	if ( cfg.m_jitter ) {
		if ( invert_jitter ) {
			yaw = yaw - cfg.m_jitter_range;
		}
		else
			yaw = yaw + cfg.m_jitter_range;

		yaw += random_val;
	}
}

ALWAYS_INLINE void c_local_player::anti_aim_t::handle( valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds ) {
	const auto anim_state = g_local_player->self( )->anim_state( );

	if ( !should_run( user_cmd )
		|| !anim_state )
		return;

	const auto& cfg = g_menu->main( ).m_anti_aim_cfg.get( );

	const auto shot_cmd_number = g_local_player->prediction( ).data( ).shot_cmd_number( );
	if ( user_cmd.m_number == shot_cmd_number )
		return;

	if ( !side ) {
		user_cmd.m_view_angles.y( ) = std::remainder( yaw, 360.f );

		return;
	}

	const auto in_shot = shot_cmd_number > valve::g_client_state->m_last_cmd_out
		&& shot_cmd_number <= ( valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds + 1 );

	const auto speed = ( ( anim_state->m_walk_to_run_transition * 20.f ) + 30.f ) * valve::g_global_vars.get( )->m_interval_per_tick;

	auto delta = side == 1 ? 60.f : -60.f;
	if ( !in_shot ) {
		const auto upper_limit = 60.f + speed;
		if ( delta > upper_limit )
			delta = upper_limit;
		else {
			const auto lower_limit = ( 60.f * -1.f ) - speed;
			if ( lower_limit > delta )
				delta = lower_limit;
		}
	}

	if ( choked_cmds <= 0 ) {
		user_cmd.m_view_angles.y( ) = std::remainder( yaw, 360.f );

		return;
	}

	const auto& local_data = g_local_player->prediction( ).data( ).local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );
	if ( std::abs( delta - 5.f ) > std::abs( sdk::angle_diff( local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw ) ) )
		delta = std::copysign( 120.f, delta );


	user_cmd.m_view_angles.y( ) = std::remainder( yaw - delta, 360.f );
}

ALWAYS_INLINE bool c_local_player::anti_aim_t::freestanding( float& yaw ) {
	if ( g_local_player->self( )->move_type( ) == valve::e_move_type::ladder )
		return false;

	const auto& cfg = g_menu->main( ).m_anti_aim_cfg.get( );

	if ( !g_key_binds->get_keybind_state( &cfg.m_auto_dir ) )
		return false;

	valve::cs_player_t* best_player{};
	auto best_fov = std::numeric_limits< float >::max( );

	const auto view_angles = valve::g_engine.get( )->view_angles( );

	for ( auto i = 1; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
		const auto player = static_cast< valve::cs_player_t* >(
			valve::g_entity_list.get( )->get_entity( i )
			);

		if ( !player
			|| player->networkable( )->dormant( )
			|| !player->alive( )
			|| player->friendly( g_local_player->self( ) ) )
			continue;

		const auto fov = sdk::calc_fov( view_angles, g_local_player->shoot_pos( ), player->world_space_center( ) );
		if ( fov >= best_fov )
			continue;

		best_fov = fov;
		best_player = player;
	}

	if ( !best_player )
		return false;

	auto& best_origin = best_player->origin( );

	const auto angle_yaw = sdk::calc_ang( g_local_player->self( )->origin( ), best_origin );

	sdk::vec3_t fwd{}, right{}, up{};

	sdk::ang_vecs( angle_yaw, &fwd, &right, &up );

	sdk::vec3_t start = g_local_player->shoot_pos( );
	sdk::vec3_t end = start + fwd * 100.0f;

	valve::ray_t right_ray( start + right * 35.0f, end + right * 35.0f );
	valve::ray_t left_ray( start - right * 35.0f, end - right * 35.0f );

	valve::trace_filter_t filter{};
	filter.m_ignore_entity = g_local_player->self( );

	valve::trace_t trace{};

	valve::g_engine_trace->trace_ray( right_ray, valve::e_mask::solid, &filter, &trace );
	float right_length = ( trace.m_end - trace.m_start ).length( 3u );

	valve::g_engine_trace->trace_ray( left_ray, valve::e_mask::solid, &filter, &trace );
	float left_length = ( trace.m_end - trace.m_start ).length( 3u );

	static auto left_ticks = 0;
	static auto right_ticks = 0;
	static auto back_ticks = 0;

	if ( right_length - left_length > 15.f )
		left_ticks++;
	else
		left_ticks = 0;

	if ( left_length - right_length > 15.f )
		right_ticks++;
	else
		right_ticks = 0;

	if ( fabs( right_length - left_length ) <= 15.f )
		back_ticks++;
	else
		back_ticks = 0;

	if ( right_ticks > 10 ) {
		m_auto_dir_side = 1;
	}
	else {
		if ( left_ticks > 10 ) {
			m_auto_dir_side = 2;
		}
		else {
			if ( back_ticks > 10 )
				return false;
		}
	}

	return true;
}

ALWAYS_INLINE void c_local_player::anti_aim_t::at_target( float& yaw ) const {
	valve::cs_player_t* best_player{};
	auto best_value = std::numeric_limits< float >::max( );

	const auto view_angles = valve::g_engine.get( )->view_angles( );

	for ( auto i = 1; i <= valve::g_global_vars.get( )->m_max_clients; ++i ) {
		const auto player = static_cast< valve::cs_player_t* >(
			valve::g_entity_list.get( )->get_entity( i )
			);
		if ( !player
			|| player->networkable( )->dormant ( )
			|| !player->alive( )
			|| player->friendly( g_local_player->self ( ) ) )
			continue;

		const auto fov = sdk::calc_fov( view_angles, g_local_player->shoot_pos( ), player->world_space_center( ) );
		if ( fov >= best_value )
			continue;

		best_value = fov;
		best_player = player;
	}


	if ( !best_player ) {
		yaw += g_menu->main( ).m_anti_aim_cfg.get( ).m_yaw;
		return;
	}

	const auto x = best_player->origin( ).x( ) - g_local_player->self( )->origin( ).x( );
	const auto y = best_player->origin( ).y( ) - g_local_player->self( )->origin( ).y( );

	yaw = x == 0.f && y == 0.f ? 0.f : sdk::to_deg( std::atan2( y, x ) ) + 180.f;
}

ALWAYS_INLINE float c_local_player::anti_aim_t::manual_yaw( ) {
	auto& cfg = g_menu->main( ).m_anti_aim_cfg.get( );
	static std::ptrdiff_t type{};	
	auto current = -1;

	if ( g_key_binds->get_keybind_state( &cfg.m_left_manual ) )
		current = 2;
	else if ( g_key_binds->get_keybind_state( &cfg.m_right_manual ) )
		current = 1;
	else if ( g_key_binds->get_keybind_state( &cfg.m_back_manual ) )
		current = 0;

	static bool prev_state;
	const auto state = current >= 0;
	if ( prev_state != state ) {
		if ( state ) {
			if ( current == type )
				type = -1;
			else
				type = current;
		}

		prev_state = state;
	}

	if ( type == 0 ) {
		return std::numeric_limits < float > ::max( );
	}

	if ( type != -1 )
		return ( type == 1 ) ? -90.f : 90.f;
	else {
		return std::numeric_limits < float > ::max( );
	}
}

ALWAYS_INLINE void c_local_player::anti_aim_t::fake_move( valve::user_cmd_t& user_cmd ) {
	if ( !should_run( user_cmd )
		|| user_cmd.m_buttons & valve::e_buttons::in_jump )
		return;

	if ( g_local_player->self( )->velocity( ).length_sqr( 2u ) > 2.f
		|| !g_local_player->self( )->anim_state( ) )
		return;

	user_cmd.m_buttons &= ~valve::e_buttons::in_speed;

	float duck_amount{};
	if ( user_cmd.m_buttons & valve::e_buttons::in_duck )
		duck_amount = std::min(
			1.f,
			g_local_player->self( )->duck_amt( )
			+ ( valve::g_global_vars.get( )->m_interval_per_tick * 0.8f ) * g_local_player->self( )->duck_speed( )
		);
	else
		duck_amount =
		g_local_player->self( )->duck_amt( )
		- std::max( 1.5f, g_local_player->self( )->duck_speed( ) ) * valve::g_global_vars.get( )->m_interval_per_tick;

	float move{};
	if ( user_cmd.m_buttons & valve::e_buttons::in_duck
		|| g_local_player->self( )->flags( ) & valve::e_ent_flags::anim_ducking )
		move = 1.1f / ( ( ( duck_amount * 0.34f ) + 1.f ) - duck_amount );
	else
		move = 1.1f;

	if ( std::abs( user_cmd.m_move.x( ) ) > move
		|| std::abs( user_cmd.m_move.y( ) ) > move )
		return;

	if ( !( user_cmd.m_number & 1 ) )
		move *= -1.f;

	user_cmd.m_move.x( ) = move;
}
