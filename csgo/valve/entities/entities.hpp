#pragma once

namespace valve {
    struct renderable_t {
        VFUNC( bool( __thiscall* )( decltype( this ), sdk::mat3x4_t*, int, int, float ),
            setup_bones( sdk::mat3x4_t* bones, int max_bones, int mask, float time ), 13u, bones, max_bones, mask, time
        );
        VFUNC( valve::model_t* ( __thiscall* ) ( decltype ( this ) ), model( ), 8u );

        VFUNC( std::uint16_t( __thiscall* ) ( decltype ( this ) ), mdl_instance( ), 30u );

        VFUNC( void( __thiscall* )( 
            decltype( this ), sdk::vec3_t& mins, sdk::vec3_t& maxs ), 
            render_bounds( sdk::vec3_t& mins, sdk::vec3_t& maxs ), 17u, mins, maxs );
    };

    struct networkable_t {
        VFUNC( bool( __thiscall* )( decltype( this ) ), dormant( ), 9u );

        VFUNC( int( __thiscall* )( decltype( this ) ), index( ), 10u );

        VFUNC( valve::client_class_t* ( __thiscall* ) ( decltype ( this ) ), client_class( ), 2u );
    };

    struct base_entity_t {
        POFFSET( renderable_t, renderable( ), sizeof( sdk::address_t ) );
        POFFSET( networkable_t, networkable( ), sizeof( sdk::address_t ) * 2u );

        OFFSET( int, health( ), g_ctx->offsets( ).m_base_entity.m_health );
        OFFSET( e_team, team( ), g_ctx->offsets( ).m_base_entity.m_team_num );
        OFFSET( std::uint32_t, effects( ), g_ctx->offsets( ).m_base_entity.m_effects );
        OFFSET( ent_handle_t, owner_entity_handle( ), g_ctx->offsets( ).m_base_entity.m_owner_entity );

        OFFSET( float, grenade_spawn_time( ), g_ctx->offsets( ).m_base_grenade.m_explode_effect_origin + 0xcu );

        OFFSET( float, sim_time( ), g_ctx->offsets( ).m_base_entity.m_sim_time );
        OFFSET( float, old_sim_time( ), g_ctx->offsets( ).m_base_entity.m_sim_time + sizeof( float ) );

        OFFSET( float, anim_time( ), g_ctx->offsets( ).m_base_entity.m_anim_time );
        OFFSET( float, cycle( ), g_ctx->offsets( ).m_base_animating.m_cycle ); // bro

        OFFSET( e_ent_flags, flags( ), g_ctx->offsets( ).m_base_entity.m_flags );

        OFFSET( std::size_t, eflags( ), g_ctx->offsets( ).m_base_entity.m_eflags );

        ALWAYS_INLINE var_mapping_t& var_mapping( ) {
            return *reinterpret_cast< var_mapping_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x24u
                );
        }

        ALWAYS_INLINE model_data_t*& mdl_data( ) {
            return *reinterpret_cast< model_data_t** >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2950u
                );
        }

        ALWAYS_INLINE float& obb_change_time( ) {
            return *reinterpret_cast< float* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x9924u
                );
        }

        ALWAYS_INLINE float& obb_change_height( ) {
            return *reinterpret_cast< float* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x9920u
                );
        }


        ALWAYS_INLINE void set_collision_bounds(
            const sdk::vec3_t& obb_min, const sdk::vec3_t& obb_max
        ) {
            using collideable_fn_t = std::uintptr_t( __thiscall* )( decltype( this ) );

            const auto collideable = ( *reinterpret_cast< collideable_fn_t** >( this ) )[ 3u ]( this );
            if ( !collideable )
                return;

            if ( !this )
                return;

            using fn_t = void( __thiscall* )( const std::uintptr_t, const sdk::vec3_t&, const sdk::vec3_t& );

            reinterpret_cast< fn_t >(
                g_ctx->offsets( ).m_addresses.m_set_collision_bounds
                )( collideable, obb_min, obb_max );
        }

        ALWAYS_INLINE void set_abs_angles( const sdk::qang_t& abs_angles ) {
            using fn_t = void( __thiscall* )( decltype( this ), const sdk::qang_t& );

            return reinterpret_cast< fn_t >(
                g_ctx->offsets( ).m_addresses.m_set_abs_angles
                )( this, abs_angles );
        }

        OFFSET( sdk::vec3_t, origin( ), g_ctx->offsets( ).m_base_entity.m_origin );
        OFFSET( sdk::vec3_t, velocity( ), g_ctx->offsets( ).m_base_entity.m_velocity );

        OFFSET( sdk::vec3_t, abs_origin( ), g_ctx->offsets( ).m_base_entity.m_abs_origin );
        OFFSET( sdk::vec3_t, abs_velocity( ), g_ctx->offsets( ).m_base_entity.m_abs_velocity );
        OFFSET( sdk::qang_t, abs_rotation( ), g_ctx->offsets( ).m_base_entity.m_abs_rotation );

        OFFSET( ent_handle_t, thrower_handle( ), g_ctx->offsets( ).m_base_grenade.m_thrower_handle );
        OFFSET( int, explode_effect_tick_begin( ), g_ctx->offsets( ).m_base_grenade.m_explode_effect_tick_begin );

        ALWAYS_INLINE bool& use_new_anim_state( ) {
            return *reinterpret_cast< bool* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x9b14u
                );
        }

        ALWAYS_INLINE void set_abs_origin( const sdk::vec3_t& abs_origin ) {
            using fn_t = void( __thiscall* )( decltype( this ), const sdk::vec3_t& );

            return reinterpret_cast< fn_t >(
                g_ctx->offsets ( ).m_addresses.m_set_abs_origin
                )( this, abs_origin );
        }

        ALWAYS_INLINE bool is_player( ) {
            using fn_t = bool( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 158u ]( this );
        }

        __forceinline bool is_weapon( ) {
            using fn_t = bool( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 166u ]( this );
        }

        __forceinline const sdk::vec3_t& world_space_center( ) {
            using fn_t = const sdk::vec3_t& ( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 79u ]( this );
        }

        OFFSET( e_move_type, move_type( ), g_ctx->offsets( ).m_base_entity.m_move_type );

        OFFSET( sdk::mat3x4_t, rgfl( ), g_ctx->offsets( ).m_base_entity.m_rgfl );

        OFFSET( valve::ent_handle_t, move_parent( ), g_ctx->offsets( ).m_base_entity.m_move_parent );
        
        OFFSET( sdk::vec3_t, obb_min( ), g_ctx->offsets( ).m_base_entity.m_mins );
        OFFSET( sdk::vec3_t, obb_max( ), g_ctx->offsets( ).m_base_entity.m_maxs );

        OFFSET( bones_t, bones( ), g_ctx->offsets( ).m_renderable.m_bone_cache + sizeof( std::uintptr_t ) );
        OFFSET( std::ptrdiff_t, bones_size( ), g_ctx->offsets( ).m_renderable.m_bone_cache + sizeof( std::uintptr_t ) + sizeof ( std::ptrdiff_t ) );

        __forceinline unsigned long& mdl_bone_cnt( ) {
            return *reinterpret_cast< unsigned long* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2690u
                );
        }

        ALWAYS_INLINE utl_vec_t< sdk::mat3x4_t >& bone_cache( ) {
            return *reinterpret_cast< utl_vec_t< sdk::mat3x4_t >* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2914u
                );
        }

        ALWAYS_INLINE std::uint32_t& occlusion_flags( ) {
            return *reinterpret_cast< std::uint32_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0xa28u
                );
        }

        ALWAYS_INLINE int& occlusion_frame( ) {
            return *reinterpret_cast< int* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0xa30u
                );
        }
        ALWAYS_INLINE std::uint8_t& ent_client_flags( ) {
            return *reinterpret_cast< std::uint8_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x68u
                );
        }

        ALWAYS_INLINE ik_context_t*& ik_context( ) {
            return *reinterpret_cast< ik_context_t** >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2670u
                );
        }

        ALWAYS_INLINE int& last_setup_bones_frame( ) {
            return *reinterpret_cast< int* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0xa68u
                );
        }

        ALWAYS_INLINE float& last_setup_bones_time( ) {
            return *reinterpret_cast< float* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2928u
                );
        }

        ALWAYS_INLINE bone_accessor_t& bone_accessor( ) {
            return *reinterpret_cast< bone_accessor_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x26a4u
                );
        }
    };

    struct base_animating_t : public base_entity_t {
        OFFSET( int, sequence( ), g_ctx->offsets( ).m_base_animating.m_sequence );

        OFFSET( int, hitbox_set_index( ), g_ctx->offsets( ).m_base_animating.m_hitbox_set_index );
        OFFSET( studio_hdr_t*, studio_hdr( ), g_ctx->offsets( ).m_base_animating.m_studio_hdr );

        OFFSET( bool, client_side_anim( ), g_ctx->offsets( ).m_base_animating.m_b_cl_side_anim );

        OFFSET( pose_params_t, pose_params( ), g_ctx->offsets( ).m_base_animating.m_pose_params );

        ALWAYS_INLINE anim_layers_t& anim_layers( ) {
            return **reinterpret_cast< anim_layers_t** >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2990u
                );
        }

        OFFSET( float, flash_dur( ), g_ctx->offsets( ).m_base_animating.m_flash_dur );

        ALWAYS_INLINE valve::data_map_t* pred_desc_map( ) {
            using fn_t = valve::data_map_t* ( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 17u ]( this );
        }

        ALWAYS_INLINE valve::type_desc_t* data_map_entry( valve::data_map_t* map, const char* name ) {
            while ( map )
            {
                for ( int i = 0; i < map->m_size; i++ )
                {
                    if ( map->m_descriptions[ i ].m_name == nullptr )
                        continue;

                    if ( strcmp( name, map->m_descriptions[ i ].m_name ) == 0 )
                        return &map->m_descriptions[ i ];
                }
                map = map->m_base_map;
            }

            return 0;
        }

    };

    struct base_attributable_item_t : public base_animating_t {
        OFFSET( e_item_index, item_index( ), g_ctx->offsets( ).m_base_attributable_item.m_item_index );
    };

    struct base_grenade_t : public base_attributable_item_t {
        OFFSET( bool, pin_pulled( ), g_ctx->offsets( ).m_base_grenade.m_pin_pulled );

        OFFSET( float, throw_time( ), g_ctx->offsets( ).m_base_grenade.m_throw_time );
        OFFSET( float, throw_strength( ), g_ctx->offsets( ).m_base_grenade.m_throw_strength );
    };

    struct base_weapon_t : public base_attributable_item_t {
        ALWAYS_INLINE weapon_info_t* info( );

        ALWAYS_INLINE float spread( ) {
            using fn_t = float( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 453u ]( this );
        }

        ALWAYS_INLINE float inaccuracy( ) {
            using fn_t = float( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 483u ]( this );
        }

        ALWAYS_INLINE void update_inaccuracy( ) {
            using fn_t = void( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 484u ]( this );
        }

        OFFSET( int, clip1( ), g_ctx->offsets( ).m_base_weapon.m_clip1 );
        OFFSET( int, primary_reserve_ammo_count( ), g_ctx->offsets( ).m_base_weapon.m_primary_reserve_ammo_count );

        OFFSET( float, next_primary_attack( ), g_ctx->offsets( ).m_base_weapon.m_next_primary_attack );
        OFFSET( float, next_secondary_attack( ), g_ctx->offsets( ).m_base_weapon.m_next_secondary_attack );

        ALWAYS_INLINE bool is_knife( ) {
            if ( !this )
                return false;

            auto idx = item_index( );

            return idx == e_item_index::knife_ct || idx == e_item_index::knife_bayonet || idx == e_item_index::knife_butterfly || idx == e_item_index::knife_falchion
                || idx == e_item_index::knife_flip || idx == e_item_index::knife_gut || idx == e_item_index::knife_karambit || idx == e_item_index::knife_m9_bayonet
                || idx == e_item_index::knife_t
                || idx == e_item_index::knife_stiletto
                || idx == e_item_index::knife_ursus || idx == e_item_index::knife_skeleton;
        }
    };

    struct weapon_cs_base_t : public base_weapon_t {
        OFFSET( bool, burst_mode( ), g_ctx->offsets( ).m_weapon_cs_base.m_burst_mode );
        OFFSET( float, last_shot_time( ), g_ctx->offsets( ).m_weapon_cs_base.m_last_shot_time );

        OFFSET( float, recoil_index( ), g_ctx->offsets( ).m_weapon_cs_base.m_recoil_index );
        OFFSET( float, accuracy_penalty( ), g_ctx->offsets( ).m_weapon_cs_base.m_accuracy_penalty );

        OFFSET( float, postpone_fire_ready_time( ), g_ctx->offsets( ).m_weapon_cs_base.m_postpone_fire_ready_time );
    };

    struct weapon_cs_base_gun_t : public weapon_cs_base_t {
        OFFSET( int, zoom_lvl( ), g_ctx->offsets( ).m_weapon_cs_base_gun.m_zoom_lvl );

        OFFSET( int, burst_shots_remaining( ), g_ctx->offsets( ).m_weapon_cs_base_gun.m_burst_shots_remaining );
        OFFSET( float, next_burst_shot( ), g_ctx->offsets( ).m_weapon_cs_base_gun.m_next_burst_shot );

        ALWAYS_INLINE float max_speed( bool scoped ) {
            float max_speed{ 260.f };

            if ( info( ) ) {
                max_speed = scoped ? info( )->m_max_speed_alt : info( )->m_max_speed;
            }

            return max_speed;
        }
    };

    struct base_combat_character_t : public base_animating_t {
        OFFSET( float, next_attack( ), g_ctx->offsets( ).m_base_combat_character.m_next_attack );
    };

    struct base_player_t : public base_combat_character_t {
        ALWAYS_INLINE bool alive( );

        ALWAYS_INLINE std::optional< player_info_t > info( );

        OFFSET( int, tick_base( ), g_ctx->offsets( ).m_base_player.m_tick_base );
        OFFSET( int, final_pred_tick( ), g_ctx->offsets( ).m_base_player.m_tick_base + 0x4u );

        OFFSET( bool, ducked( ), g_ctx->offsets( ).m_base_player.m_ducked );
        OFFSET( bool, ducking( ), g_ctx->offsets( ).m_base_player.m_ducking );

        OFFSET( int, observer_mode( ), g_ctx->offsets( ).m_base_player.m_observer_mode );

        OFFSET( float, surface_friction( ), g_ctx->offsets( ).m_base_player.m_surface_friction );

        OFFSET( ent_handle_t, ground_entity_handle( ), g_ctx->offsets( ).m_base_player.m_ground_entity );

        OFFSET( ent_handle_t, observer_target_handle( ), g_ctx->offsets( ).m_base_player.m_observer_target );

        OFFSET( float, max_speed( ), g_ctx->offsets( ).m_base_player.m_max_speed );

        ALWAYS_INLINE std::ptrdiff_t lookup_seq_act( std::ptrdiff_t seq ) {
            const auto model_data = mdl_data( );
            if ( !model_data
                || !model_data->m_studio_hdr )
                return -1;

            using fn_t = int( __fastcall* )( decltype( this ), studio_hdr_t* const, const int );

            return reinterpret_cast< fn_t >(
                g_ctx->offsets( ).m_addresses.m_lookup_seq_act
                )( this, model_data->m_studio_hdr, seq );
        }

        OFFSET( e_life_state, life_state( ), g_ctx->offsets( ).m_base_player.m_life_state );

        OFFSET( float, duck_amt( ), g_ctx->offsets( ).m_base_player.m_duck_amt );
        OFFSET( float, duck_speed( ), g_ctx->offsets( ).m_base_player.m_duck_speed );

        OFFSET( float, spawn_time( ), g_ctx->offsets( ).m_base_player.m_spawn_time );

        OFFSET( sdk::qang_t, aim_punch( ), g_ctx->offsets( ).m_base_player.m_aim_punch );
        OFFSET( sdk::qang_t, view_punch( ), g_ctx->offsets( ).m_base_player.m_view_punch );

        OFFSET( sdk::vec3_t, view_offset( ), g_ctx->offsets( ).m_base_player.m_view_offset );
        OFFSET( sdk::qang_t, aim_punch_vel( ), g_ctx->offsets( ).m_base_player.m_aim_punch_vel );

        OFFSET( float, fall_velocity( ), g_ctx->offsets( ).m_base_player.m_fall_velocity );
    };

    struct tone_map_t : public base_entity_t {
        OFFSET( bool, use_custom_bloom_scale( ), g_ctx->offsets( ).m_tone_map.m_use_custom_bloom_scale );
        OFFSET( bool, user_custom_auto_exposure_min( ), g_ctx->offsets( ).m_tone_map.m_user_custom_auto_exposure_min );
        OFFSET( bool, user_custom_auto_exposure_max( ), g_ctx->offsets( ).m_tone_map.m_user_custom_auto_exposure_max );
        OFFSET( float, custom_bloom_scale( ), g_ctx->offsets( ).m_tone_map.m_custom_bloom_scale );
        OFFSET( float, auto_custom_exposure_min( ), g_ctx->offsets( ).m_tone_map.m_auto_custom_exposure_min );
        OFFSET( float, auto_custom_exposure_max( ), g_ctx->offsets( ).m_tone_map.m_auto_custom_exposure_max );
    };

    struct cascade_light_t : public base_entity_t {
        OFFSET( sdk::vec3_t, shadow_dir( ), g_ctx->offsets( ).m_cascade_light.m_shadow_dir );
    };

    struct inferno_t : public base_entity_t {
        __forceinline float& get_spawn_time( ) {
            return *reinterpret_cast< float* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x20u
                );
        }

        static float get_expiry_time( )
        {
            return 7.03125f;
        }
    };

    struct smoke_t : public base_entity_t {
        OFFSET( std::ptrdiff_t, smoke_effect_tick_begin( ), g_ctx->offsets( ).m_base_grenade.m_smoke_effect_tick_begin );
        OFFSET( bool, did_smoke_effect( ), g_ctx->offsets( ).m_base_grenade.m_did_smoke_effect );

        static float get_expiry_time( ) {
            return 19.0f;
        }
    };


    struct cs_player_t : public base_player_t {
        ALWAYS_INLINE bool friendly( cs_player_t* const with );

        OFFSET( ent_handle_t, weapon_handle( ), g_ctx->offsets( ).m_base_combat_character.m_weapon_handle );

        ALWAYS_INLINE weapon_cs_base_gun_t* weapon( ) {
            const auto handle = weapon_handle( );

            return static_cast< weapon_cs_base_gun_t* >( g_entity_list.get( )->get_entity( handle ) );
        }

        ALWAYS_INLINE void set_local_view_angles( const sdk::qang_t& angle ) {
            using fn_t = void( __thiscall* )( decltype( this ), const sdk::qang_t& );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 373u ]( this, angle );
        }

        __forceinline bool duck_until_on_ground( ) {
            return *( bool* ) ( ( DWORD ) ( this ) + 0x10478 );
        }

        ALWAYS_INLINE sdk::vec3_t who_tf_asked( int hitbox_id, valve::bones_t bones ) {
            if ( !this )
                return sdk::vec3_t( );

            auto mdl_ = mdl_data( );

            if ( !mdl_ )
                return sdk::vec3_t( );

            auto hdr = mdl_->m_studio_hdr;

            auto hitbox_set = hdr->get_hitbox_set( hitbox_set_index( ) );

            if ( !hitbox_set )
                return sdk::vec3_t( );

            auto hitbox = hitbox_set->get_bbox( hitbox_id );

            if ( !hitbox )
                return sdk::vec3_t( );

            sdk::vec3_t min{}, max{};

            min = hitbox->m_mins.transform( bones[ hitbox->m_bone ] );
            max = hitbox->m_maxs.transform( bones[ hitbox->m_bone ] );

            return ( min + max ) * 0.5f;
        }

        ALWAYS_INLINE ent_handle_t ref_handle( ) {
            using fn_t = ent_handle_t( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 2u ]( this );
        }

        ALWAYS_INLINE void update_collision_bounds( ) {
            using fn_t = void( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 340u ]( this );
        }

        ALWAYS_INLINE std::uintptr_t player_resource( );

        ALWAYS_INLINE int ping( ) {
            auto res = player_resource( );

            return *( int* ) ( res + g_ctx->offsets( ).m_player_resource.m_ping + networkable( )->index( ) * 4 );
        }

        OFFSET( bool, has_helmet( ), g_ctx->offsets( ).m_cs_player.m_has_helmet );
        OFFSET( int, armor_val( ), g_ctx->offsets( ).m_cs_player.m_armor_val );
        OFFSET( bool, has_heavy_armor( ), g_ctx->offsets( ).m_cs_player.m_has_heavy_armor );
        OFFSET( bool, walking( ), g_ctx->offsets( ).m_cs_player.m_walking );
        OFFSET( int, money( ), g_ctx->offsets( ).m_cs_player.m_money );

        OFFSET( float, velocity_modifier( ), g_ctx->offsets( ).m_cs_player.m_velocity_modifier );
        OFFSET( float, third_person_recoil( ), g_ctx->offsets( ).m_cs_player.m_third_person_recoil );

        OFFSET( bool, wait_for_no_attack( ), g_ctx->offsets( ).m_cs_player.m_wait_for_no_attack );
        OFFSET( bool, scoped( ), g_ctx->offsets( ).m_cs_player.m_scoped );

        OFFSET( float, lby( ), g_ctx->offsets( ).m_cs_player.m_lby );

        OFFSET( sdk::qang_t, eye_angles( ), g_ctx->offsets( ).m_cs_player.m_eye_angles );
        OFFSET( valve::ent_handle_t, view_mdl_handle( ), g_ctx->offsets( ).m_cs_player.m_view_model );

        ALWAYS_INLINE sdk::qang_t& visual_angles( ) {
            return *( sdk::qang_t* ) ( ( DWORD ) ( this ) + 0x31E8 );
        }

        ALWAYS_INLINE int& sim_tick( ) {
            return *reinterpret_cast< int* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x2acu
                );
        }

        ALWAYS_INLINE void attachment_helper( ) {
            using fn = void( __thiscall* )( valve::cs_player_t*, valve::studio_hdr_t* );

            static fn attachment_help = reinterpret_cast < fn > ( g_ctx->offsets( ).m_addresses.m_attachment_helper );

            attachment_help( this, this->studio_hdr( ) );
        }

        ALWAYS_INLINE float get_layer_seq_cycle_rate( valve::anim_layer_t* layer, int seq ) {
            using fn_t = float( __thiscall* )( decltype( this ), valve::anim_layer_t*, int );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 223u ]( this, layer, seq );
        }

        ALWAYS_INLINE void update_client_side_anim( ) {
            using fn_t = void( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 224u ]( this );
        }

        ALWAYS_INLINE cmd_context_t& cmd_context( )
        {
            return *reinterpret_cast< cmd_context_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x350cu
                );
        }

#ifndef CSGO2018
        OFFSET( int, survival_team( ), g_ctx->offsets( ).m_cs_player.m_survival_team );
#endif

        OFFSET( anim_state_t*, anim_state( ), g_ctx->offsets( ).m_cs_player.m_anim_state );

        OFFSET( bool, defusing( ), g_ctx->offsets( ).m_cs_player.m_defusing );

        ALWAYS_INLINE user_cmd_t*& cur_user_cmd( ) {
            return *reinterpret_cast< user_cmd_t** >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x3348u );
        }

        ALWAYS_INLINE user_cmd_t& last_user_cmd( ) {
            return *reinterpret_cast< user_cmd_t* >(
                reinterpret_cast< std::uintptr_t >( this ) + 0x3298u );
        }
    };

    struct base_view_model_t : public base_entity_t {
        OFFSET( int, anim_parity( ), g_ctx->offsets( ).m_c_base_view_model.m_animation_parity );
        OFFSET( int, sequence( ), g_ctx->offsets( ).m_c_base_view_model.m_sequence );
    };

    class i_client_unknown;

    class i_client_renderable {
    public:
        virtual i_client_unknown* get_client_unknown( ) = 0;
    };
    class c_base_handle;
    class i_handle_entity {
    public:
        virtual							~i_handle_entity( ) = default;
        virtual void					set_ref_handle( const c_base_handle& handle ) = 0;
        virtual const c_base_handle& get_handle( ) const = 0;
    };

    class i_client_unknown : public i_handle_entity {
    public:
        virtual void* get_collideable( ) = 0;
        virtual void* get_client_networkable( ) = 0;
        virtual i_client_renderable* get_client_renderable( ) = 0;
        virtual void* get_client_entity( ) = 0;
        virtual base_entity_t* get_base_entity( ) = 0;
        virtual void* get_client_thinkable( ) = 0;
        virtual void* get_client_alpha_property( ) = 0;
    };

    class i_client_networkable {
    public:
        virtual i_client_unknown* get_client_unk( ) = 0;
        virtual void release( ) = 0;
        virtual void* client_class( ) = 0;
        virtual void who( int who ) = 0;
        virtual void on_pre_data_changed( int type ) = 0;
        virtual void on_data_changed( int type ) = 0;
        virtual void pre_data_update( int type ) = 0;
        virtual void post_data_update( int type ) = 0;
    };
}

#include "impl/entities.inl"