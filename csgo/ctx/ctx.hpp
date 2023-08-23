#pragma once

namespace valve {
    struct cvar_t;

    struct recv_prop_t;

    struct cs_player_t;

    struct data_map_t;
}

class c_ctx {
private:
    using modules_t = std::unordered_map< sdk::hash_t, sdk::x86_pe_image_t* >;

    using interfaces_t = std::unordered_map< sdk::hash_t, sdk::address_t >;

    using random_float_t = float( __cdecl* )( const float, const float );
    using random_int_t = int( __cdecl* )( const int, const int );
    using random_seed_t = void( __cdecl* )( const int );
    using angle_matrix_t = void( __fastcall* )( const sdk::qang_t&, sdk::mat3x4_t& );
    using cl_read_packets_t = void( __cdecl* ) ( bool final_tick );

    struct ent_offset_t {
        valve::recv_prop_t* m_prop{};
        std::uint32_t       m_offset{};
    };

    bool m_allow_update_anim{}, m_allow_setup_bones{};

    using ent_offsets_t = std::unordered_map< sdk::hash_t, ent_offset_t >;

    __forceinline bool wait_for_all_modules( modules_t& modules ) const;

    __forceinline void init_imgui( const modules_t& modules ) const;

    __forceinline void parse_interfaces( sdk::x86_pe_image_t* const image, interfaces_t& interfaces ) const;

    __forceinline void init_interfaces( const modules_t& modules ) const;

    __forceinline bool parse_ent_offsets( ent_offsets_t& offsets, const modules_t& modules ) const;

    __forceinline void init_offsets( const modules_t& modules );

    __forceinline void init_cvars( );

    __forceinline void init_hooks( const modules_t& modules ) const;

    struct {
        sdk::address_t  m_local_player{},
                        m_weapon_system{},
                        m_user_cmd_checksum{};

        struct {
            int* m_pred_seed{};
            valve::cs_player_t* m_pred_player{};
        } m_prediction{};

        struct {
            sdk::address_t m_init{}, m_load_from_buffer{};
        }               m_key_values{};

        struct {
            sdk::address_t m_reset{}, m_update{};
        }               m_anim_state{};

        struct {
            std::uint32_t m_bone_cache{}, m_mdl_bone_cnt{};
        }               m_renderable{};

        struct {
            std::uint32_t   m_health{}, m_team_num{}, m_anim_time{},
                m_sim_time{}, m_flags{}, m_effects{},
                m_origin{}, m_velocity{}, m_eflags{},
                m_abs_origin{}, m_abs_velocity{},
                m_abs_rotation{}, m_move_type{},
                m_mins{}, m_maxs{},
                m_rgfl{ }, m_move_parent{},
                m_owner_entity{};
        }               m_base_entity{};

        struct {
            std::uint32_t   m_sequence{}, m_cycle{}, m_b_cl_side_anim{},
                            m_hitbox_set_index{}, m_studio_hdr{},
                            m_pose_params{}, m_anim_layers{},
                            m_flash_dur{};
        }               m_base_animating{};

        struct {
            std::uint32_t   m_pin_pulled{},
                m_throw_time{}, m_throw_strength{},
                m_smoke_effect_tick_begin{},
                m_did_smoke_effect{}, m_thrower_handle{}, m_explode_effect_tick_begin{},
                m_explode_effect_origin{};
        }               m_base_grenade{};

        struct {
            std::uint32_t   m_item_index{};
        }               m_base_attributable_item{};

        struct {
            std::uint32_t   m_clip1{}, m_primary_reserve_ammo_count{},
                            m_next_primary_attack{}, m_next_secondary_attack{};
        }               m_base_weapon{};

        struct {
            std::uint32_t   m_burst_mode{},
                m_last_shot_time{}, m_recoil_index{}, m_accuracy_penalty{},
                            m_postpone_fire_ready_time{};
        }               m_weapon_cs_base{};

        struct {
            std::uint32_t   m_zoom_lvl{},
                            m_burst_shots_remaining{}, m_next_burst_shot{};
        }               m_weapon_cs_base_gun{};

        struct {
            std::uint32_t   m_weapon_handle{}, m_next_attack{};
        }               m_base_combat_character{};

        struct {
            std::uint32_t   m_tick_base{}, m_life_state{},
                m_duck_amt{}, m_duck_speed{},
                m_spawn_time{}, m_aim_punch{},
                m_view_punch{}, m_aim_punch_vel{},
                m_view_offset{}, m_fall_velocity{},
                m_surface_friction{}, m_ground_entity{},
                m_observer_mode{}, m_observer_target{},
                m_max_speed{}, m_ducked{}, m_ducking{};
        }               m_base_player{};

        struct {
            std::uint32_t   m_lby{}, m_eye_angles{}, m_velocity_modifier{}, m_view_model{},
#ifndef CSGO2018
                            m_survival_team{},
#endif
                m_anim_state{}, m_defusing{}, m_third_person_recoil{}, m_wait_for_no_attack{}, m_scoped{},            
                m_armor_val, m_has_heavy_armor{},
                m_has_helmet{}, m_money{},
                m_walking{};
        }               m_cs_player{};

        struct {
            std::uint32_t   m_warmup_period{}, m_freeze_period{},
                            m_valve_ds{}, m_bomb_planted{};
        }               m_game_rules{};

        struct {
            std::uint32_t  m_sequence{}, m_animation_parity{};
        }               m_c_base_view_model{};

        struct {
            std::uint32_t m_use_custom_bloom_scale{}, m_user_custom_auto_exposure_min{}, m_user_custom_auto_exposure_max{}, m_custom_bloom_scale{}, m_auto_custom_exposure_min{}, m_auto_custom_exposure_max{};
        }m_tone_map{};

        struct {
            std::uint32_t m_shadow_dir{};
        } m_cascade_light{};

        struct {
            std::uint32_t m_ping{};
        } m_player_resource{};

        struct addresses_t {
            std::uintptr_t m_set_abs_origin{}, m_attachment_helper{}, m_lookup_seq_act{}, m_write_user_cmd{}, m_trace_filter_simple_vtable{},
                m_set_clan_tag{}, m_smoke_count{}, m_post_process{}, m_sky_box{}, m_hud{}, m_find_hud_element{}, m_clear_hud_wpn{}, m_v_matrix{},
                m_trace_filter_skip_two_entities_vtable{}, m_is_breakable{}, m_compute_hitbox_surround_box{}, m_voice_msg_ctor{}, m_set_collision_bounds{},
                m_set_abs_angles{}, m_invalidate_bone_cache{}, m_invalidate_physics_recursive{}, m_calc_shotgun_spread{}, m_allow_to_extrapolate{},
                m_ret_to_maintain_seq_transitions{}, m_ret_to_accumulate_layers{}, m_ret_to_setup_velocity{}, m_ret_to_extrapolation{},
                m_ret_insert_into_tree{};

            random_float_t		m_random_float{};
            random_int_t	    m_random_int{};
            random_seed_t	    m_random_seed{};
            angle_matrix_t 	    m_angle_matrix{};
            cl_read_packets_t   m_cl_read_packets{};
        } m_addresses{};
    } m_offsets{};

    struct {
        valve::cvar_t*  cl_forwardspeed{}, *cl_backspeed{},
                        *cl_sidespeed{}, *cl_upspeed{},
                        *cl_pitchdown{}, *cl_pitchup{},
                        *mp_teammates_are_enemies{};
    } m_cvars{};

public:
    void init( );

    ALWAYS_INLINE auto& offsets( );

    ALWAYS_INLINE bool& allow_update_anim( );
    
    ALWAYS_INLINE bool& allow_setup_bones( );

    ALWAYS_INLINE const auto& cvars( ) const;
};

inline auto g_ctx = std::make_unique< c_ctx >( );

struct key_bind_t {
    key_bind_t( )
    {
        m_code = 0;
        m_mode = 0;
        m_is_binding = false;
    }

    key_bind_t( char code, int mode )
    {
        m_code = code;
        m_mode = mode;
        m_is_binding = false;
    }

    char m_code;
    int m_mode;
    bool m_is_binding;
};

class c_key_binds {
public:

    bool get_keybind_state( const key_bind_t* bind );
    void set_keybind_state( key_bind_t* bind, int val );
    int get_keybind_mode( const key_bind_t* bind );
    void KeybindNelfo( const char* label, key_bind_t* keybind, bool is_manual = false, int x_bind = 300, float x_pos = 193 );

public:
    char m_last_code = NULL;
};

inline std::unique_ptr < c_key_binds > g_key_binds = std::make_unique < c_key_binds >( );


#include "impl/ctx.inl"