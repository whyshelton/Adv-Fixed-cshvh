#pragma once

using ui_clr_t = ImColor;

enum e_tabs {
    aim_bot,
    anti_aim,
    players,
    visuals,
    misc
};

struct tabs_data_t {
    __forceinline tabs_data_t( ) = default;
    int m_rage_txt_alpha{ 255 }, m_anti_aim_txt_alpha{ 190 }, m_players_txt_alpha{ 190 }, m_visuals_txt_alpha{ 190 }, m_misc_txt_alpha{ 190 };
    int m_rage_rect_alpha{ 255 }, m_anti_aim_rect_alpha{ 0 }, m_players_rect_alpha{ 0 }, m_visuals_rect_alpha{ 0 }, m_misc_rect_alpha{ 0 };

    struct sub_tabs_data_t {
        __forceinline sub_tabs_data_t( ) = default;

        struct rage_bot_t {
            int m_blue_rect_alpha_main{ 255 }, m_blue_rect_alpha_extra{ 0 };
            int m_main_rect_alpha_main{ 65 }, m_main_rect_alpha_extra{ 0 };
            int m_outline_rect_alpha_main{ 8 }, m_outline_rect_alpha_extra{ 0 };
            float m_alpha_main{ 0.f }, m_alpha_extra{ 0.f };
        } m_rage_bot{};

        struct anti_aim_t {
            int m_blue_rect_alpha_main{ 255 }, m_blue_rect_alpha_extra{ 0 };
            int m_main_rect_alpha_main{ 65 }, m_main_rect_alpha_extra{ 0 };
            int m_outline_rect_alpha_main{ 8 }, m_outline_rect_alpha_extra{ 0 };
            float m_alpha_main{ 0.f }, m_alpha_extra{ 0.f };
        } m_anti_aim{};

        struct misc_t {
            int m_blue_rect_alpha_main{ 255 }, m_blue_rect_alpha_extra{ 0 };
            int m_main_rect_alpha_main{ 65 }, m_main_rect_alpha_extra{ 0 };
            int m_outline_rect_alpha_main{ 8 }, m_outline_rect_alpha_extra{ 0 };
            float m_alpha_main{ 0.f }, m_alpha_extra{ 0.f };
        } m_misc{};

        struct visuals_t {
            int m_blue_rect_alpha_main{ 255 }, m_blue_rect_alpha_extra{ 0 };
            int m_main_rect_alpha_main{ 65 }, m_main_rect_alpha_extra{ 0 };
            int m_outline_rect_alpha_main{ 8 }, m_outline_rect_alpha_extra{ 0 };
            float m_alpha_main{ 0.f }, m_alpha_extra{ 0.f };
        } m_visuals{};

        struct players_t {
            int m_blue_rect_alpha_main{ 255 }, m_blue_rect_alpha_extra{ 0 };
            int m_main_rect_alpha_main{ 65 }, m_main_rect_alpha_extra{ 0 };
            int m_outline_rect_alpha_main{ 8 }, m_outline_rect_alpha_extra{ 0 };
            float m_alpha_main{ 0.f }, m_alpha_extra{ 0.f };
        } m_players{};

    } m_sub_tabs{};

    __forceinline void set_alpha( const e_tabs& cur_tab ) {
        if ( cur_tab == e_tabs::aim_bot ) {
            m_rage_rect_alpha += 4;
            m_rage_txt_alpha += 1;
        }
        else {
            m_rage_rect_alpha -= 4;
            m_rage_txt_alpha -= 1;
        }

        if ( cur_tab == e_tabs::anti_aim ) {
            m_anti_aim_rect_alpha += 4;
            m_anti_aim_txt_alpha += 1;
        }
        else {
            m_anti_aim_rect_alpha -= 4;
            m_anti_aim_txt_alpha -= 1;
        }

        if ( cur_tab == e_tabs::players ) {
            m_players_rect_alpha += 4;
            m_players_txt_alpha += 1;
        }
        else {
            m_players_rect_alpha -= 4;
            m_players_txt_alpha -= 1;
        }

        if ( cur_tab == e_tabs::visuals ) {
            m_visuals_rect_alpha += 4;
            m_visuals_txt_alpha += 1;
        }
        else {
            m_visuals_rect_alpha -= 4;
            m_visuals_txt_alpha -= 1;
        }

        if ( cur_tab == e_tabs::misc ) {
            m_misc_rect_alpha += 4;
            m_misc_txt_alpha += 1;
        }
        else {
            m_misc_rect_alpha -= 4;
            m_misc_txt_alpha -= 1;
        }


        /* когда переносить меню в хак просто вызывать std::clamp */
        if ( m_rage_txt_alpha > 255 )
            m_rage_txt_alpha = 255;
        else if ( m_rage_txt_alpha < 190 )
            m_rage_txt_alpha = 190;

        if ( m_anti_aim_txt_alpha > 255 )
            m_anti_aim_txt_alpha = 255;
        else if ( m_anti_aim_txt_alpha < 190 )
            m_anti_aim_txt_alpha = 190;

        if ( m_players_txt_alpha > 255 )
            m_players_txt_alpha = 255;
        else if ( m_players_txt_alpha < 190 )
            m_players_txt_alpha = 190;

        if ( m_visuals_txt_alpha > 255 )
            m_visuals_txt_alpha = 255;
        else if ( m_visuals_txt_alpha < 190 )
            m_visuals_txt_alpha = 190;

        if ( m_misc_txt_alpha > 255 )
            m_misc_txt_alpha = 255;
        else if ( m_misc_txt_alpha < 190 )
            m_misc_txt_alpha = 190;

        if ( m_rage_rect_alpha > 255 )
            m_rage_rect_alpha = 255;
        else if ( m_rage_rect_alpha < 0 )
            m_rage_rect_alpha = 0;

        if ( m_anti_aim_rect_alpha > 255 )
            m_anti_aim_rect_alpha = 255;
        else if ( m_anti_aim_rect_alpha < 0 )
            m_anti_aim_rect_alpha = 0;

        if ( m_players_rect_alpha > 255 )
            m_players_rect_alpha = 255;
        else if ( m_players_rect_alpha < 0 )
            m_players_rect_alpha = 0;

        if ( m_visuals_rect_alpha > 255 )
            m_visuals_rect_alpha = 255;
        else if ( m_visuals_rect_alpha < 0 )
            m_visuals_rect_alpha = 0;

        if ( m_misc_rect_alpha > 255 )
            m_misc_rect_alpha = 255;
        else if ( m_misc_rect_alpha < 0 )
            m_misc_rect_alpha = 0;

    }

} inline m_tabs_data{};

class c_menu {
private:
    using cfg_ui_clr_t = sdk::cfg_var_t< ui_clr_t >;

    struct main_t {
        struct move_t {
            bool m_bhop{ false }, m_auto_strafe{ false }, m_fast_stop{ false }, 
                m_infinity_duck{ false }, m_slide_walk{ false };
            key_bind_t m_slow_walk{ key_bind_t( ) }, m_fake_duck{ key_bind_t( ) }, m_auto_peek{ key_bind_t( ) };
        };

        sdk::cfg_var_t< move_t > m_move_cfg{ 0x05562b71u, {} };

        struct aim_bot_t { 
            float m_hit_chance_awp{ 0.f }, m_hit_chance_scout{ 0.f }, m_hit_chance_scar{ 0.f },
                m_hit_chance_other{ 0.f }, m_hit_chance_heavy_pistol{ 0.f }, m_hit_chance_pistol{ 0.f };

            float m_body_scale_awp{ 0.f }, m_body_scale_scout{ 0.f }, m_body_scale_scar{ 0.f },
                m_body_scale_other{ 0.f }, m_body_scale_heavy_pistol{ 0.f }, m_body_scale_pistol{ 0.f };

            float m_head_scale_awp{ 0.f }, m_head_scale_scout{ 0.f }, m_head_scale_scar{ 0.f },
                m_head_scale_other{ 0.f }, m_head_scale_heavy_pistol{ 0.f }, m_head_scale_pistol{ 0.f };

            bool m_static_point_scale_awp{ false }, m_static_point_scale_scout{false }, m_static_point_scale_scar{ false },
                m_static_point_scale_other{ false }, m_static_point_scale_heavy_pistol{ false }, m_static_point_scale_pistol{ false };

            bool m_rage_bot{ false }, m_auto_scope{ false }, m_multi_threading{ false };

            bool m_early_stop_awp{ false }, m_early_stop_scout{ false }, m_early_stop_scar{ false },
                m_early_stop_other{ false }, m_early_stop_heavy_pistol{ false }, m_early_stop_pistol{ false };

            int m_min_dmg_awp{ 0 }, m_min_dmg_scout{ 0 }, m_min_dmg_scar{ 0 },
                m_min_dmg_other{ 0 }, m_min_dmg_heavy_pistol{ 0 }, m_min_dmg_pistol{ 0 };

            int m_hit_groups_awp{ 0 }, m_hit_groups_scout{ 0 }, m_hit_groups_scar{ 0 },
                m_hit_groups_other{ 0 }, m_hit_groups_heavy_pistol{ 0 }, m_hit_groups_pistol{ 0 };

            int m_max_misses_awp{ 0 }, m_max_misses_scout{ 0 }, m_max_misses_scar{ 0 },
                m_max_misses_other{ 0 }, m_max_misses_heavy_pistol{ 0 }, m_max_misses_pistol{ 0 };

            int m_stop_type_awp{ 0 }, m_stop_type_scout{ 0 }, m_stop_type_scar{ 0 },
                m_stop_type_other{ 0 }, m_stop_type_heavy_pistol{ 0 }, m_stop_type_pistol{ 0 };

            int m_multi_points_awp{ 0 }, m_multi_points_scout{ 0 }, m_multi_points_scar{ 0 },
                m_multi_points_other{ 0 }, m_multi_points_heavy_pistol{ 0 }, m_multi_points_pistol{ 0 };

            int m_min_dmg_awp_override_val{ 0 }, m_min_dmg_scout_override_val{ 0 }, m_min_dmg_scar_override_val{ 0 },
                m_min_dmg_other_override_val{ 0 }, m_min_dmg_heavy_pistol_override_val{ 0 }, m_min_dmg_pistol_override_val{ 0 };

            key_bind_t m_min_dmg_override{ key_bind_t( ) }, m_force_safe_points{ key_bind_t( ) };
        };

        sdk::cfg_var_t< aim_bot_t > m_aim_bot_cfg{ 0x05562b72u, {} };

        struct models_t {
            bool m_player{ },
                m_player_occluded{ }, m_local_player{ }, m_local_player_fake{ },
                m_ragdolls{ }, m_hands{ }, m_local_weapon{ }, m_weapons{  },
                m_shot{ };

            std::array < float, 4 >  m_player_clr{  1.f, 1.f, 1.f, 1.f },
                m_player_occluded_clr{ 1.f, 1.f, 1.f, 1.f },
                m_local_player_clr{ 1.f, 1.f, 1.f, 1.f  },
                m_local_player_fake_clr{ 1.f, 1.f, 1.f, 1.f },
                m_ragdolls_clr{ 1.f, 1.f, 1.f, 1.f  },
                m_hands_clr{  1.f, 1.f, 1.f, 1.f },
                m_local_weapon_clr{ 1.f, 1.f, 1.f, 1.f },
                m_weapons_clr{ 1.f, 1.f, 1.f, 1.f },
                m_shot_clr{ 1.f, 1.f, 1.f, 1.f };

            int m_player_type{ 0 }, m_player_occluded_type{ 0 }, m_local_player_type{ 0 },
                m_local_player_fake_type{ 0 }, m_ragdolls_type{ }, m_hands_type{ },
                m_local_weapon_type{ }, m_weapons_type{ }, m_shot_type{  };

            bool m_player_glow_outline{ false }, m_player_occluded_glow_outline{ false },
                m_local_player_glow_outline{ false }, m_local_player_fake_glow_outline{ false },
                m_ragdolls_glow_outline{ false }, m_hands_glow_outline{ false },
                m_local_weapon_outline{ false }, m_weapons_outline{ false }, m_shot_outline{ false };

            std::array < float, 4 > m_player_glow_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_player_glow_occluded_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_local_player_glow_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_local_player_fake_glow_outline_clr{ 1.f, 1.f, 1.f, 1.f  },
                m_ragdolls_glow_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_hands_glow_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_local_weapon_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_weapons_outline_clr{ 1.f, 1.f, 1.f, 1.f },
                m_shot_outline_clr{ 1.f, 1.f, 1.f, 1.f };
        };

        sdk::cfg_var_t< models_t > m_models_cfg{ 0x05562b73u, {} };

        struct fake_lags_t {
            bool m_enabled{ false };
            int  m_ticks{ 0 }, m_variability{ 0 };
        };

        sdk::cfg_var_t< fake_lags_t > m_fake_lags_cfg{ 0x05562b74u, {} };

        struct anti_aim_t {
            bool m_enabled{ false }, m_jitter{ false }, m_jitter_side{ false }
            , m_at_targets{ false }, m_roll{ false };
             float m_pitch{ 0.f }, m_jitter_range{ 0.f }, m_yaw{ 0.f }, m_roll_range{ 0.f };
            key_bind_t m_flip_side{ key_bind_t( ) };
            key_bind_t m_left_manual{ key_bind_t( ) };
            key_bind_t m_right_manual{ key_bind_t( ) };
            key_bind_t m_back_manual{ key_bind_t( ) };
            key_bind_t m_auto_dir{ key_bind_t( ) };
        };

        sdk::cfg_var_t< anti_aim_t > m_anti_aim_cfg{ 0x05562b75u, {} };

        struct exploits_t {
            key_bind_t m_double_tap{ key_bind_t( ) };
            key_bind_t m_hide_shots{ key_bind_t( ) };
        };

        sdk::cfg_var_t< exploits_t > m_exploits_cfg{ 0x05562b76u, {} };

        struct misc_t {
           key_bind_t m_third_person{ key_bind_t( ) };
           float m_third_person_val{ 0.f }, m_aspect_ratio_val{ 0.f },
                m_camera_distance{ 85.f };
           bool  m_third_person_when_dead{ false }, m_clan_tag{ false }, m_aspect_ratio{ false },
                m_remove_scope{ false }, m_remove_flash{ false }, m_remove_smoke{ false },
                m_remove_hands_shaking{ false }, m_remove_post_processing{ false }, m_remove_view_kick{ false },
                m_remove_view_punch{ false }, m_remove_land_bob{ false }, m_force_crosshair{ false },
                m_buy_bot{ false }, m_hit_sound{ false };

            int m_buy_bot_snipers{ false }, m_buy_bot_pistols{ false }, m_buy_bot_additional{  false };
                 
            bool m_reset_tag{};
            int m_prev_tag{};
        };

        sdk::cfg_var_t< misc_t > m_misc_cfg{ 0x05562b77u, {} };

        struct visuals_t {
            int m_sky_box{ 0 }, m_bloom{ 0 }, m_exposure{ 0 }, m_fog_start{ 0 },
                m_fog_end{ 0 }, m_fog_density{ 0 }, m_blend_in_scope_val{ 0 }, m_player_flags{ 0 };

            std::array < float, 4 > m_world_modulation{ 1.f, 1.f, 1.f, 1.f }, m_sky_modulation{ 1.f, 1.f, 1.f, 1.f },
                m_props_modulation{ 1.f, 1.f, 1.f, 1.f }, m_fog_clr{ 1.f, 1.f, 1.f, 1.f }, m_grenade_pred_clr{ 1.f, 1.f, 1.f, 1.f },
                m_enemy_bullet_tracers_clr{ 1.f, 1.f, 1.f, 1.f }, m_bullet_tracers_clr{ 1.f, 1.f, 1.f, 1.f }, m_glow_clr{ 1.f, 1.f, 1.f, 1.f };

            bool m_tone_map_modulation{ false }, m_kill_feed{ false }, m_show_wpn_in_scope{false },
                m_shadows_modulation{ false }, m_fog{ false }, m_molotov_timer{ false },
                m_smoke_timer{ false }, m_grenade_projectiles{ false }, m_draw_wpn_proj{ false },
                m_draw_wpn_proj_icon{ false }, m_grenade_prediction{ false }, m_spectators_list{  false },
                m_key_binds_list{ false }, m_blend_in_scope{ false }, m_draw_name{ false },
                m_draw_flags{ false }, m_draw_health{ false }, m_draw_box{ false }, m_wpn_icon{ false },
                m_wpn_text{ false }, m_oof_indicator{ false }, m_wpn_ammo{ false }, m_hits_marker{ false },
                m_bullet_tracers{ false }, m_enemy_bullet_tracers{ false }, m_bullet_impacts{ false },
                m_glow{ false };

            float m_x_dir{ 0.f }, m_y_dir{ 0.f }, m_z_dir{ 0.f };
        };

        sdk::cfg_var_t< visuals_t > m_visuals_cfg{ 0x05562b78u, {} };

        bool m_hidden{};
    } m_main{};
public:
    void render( );

    ALWAYS_INLINE auto& main( );
};

inline const auto g_menu = std::make_unique< c_menu >( );

#include "impl/menu.inl"