#include "../../csgo.hpp"
const char* skybox_list[ ] =
{
     ( "none" ),
     ( "tibet" ),
     ( "baggage" ),
     ( "italy" ),
     ( "aztec" ),
     ( "vertigo" ),
     ( "daylight" ),
     ( "daylight 2" ),
     ( "clouds" ),
     ( "clouds 2" ),
     ( "gray" ),
     ( "clear" ),
     ( "canals" ),
     ( "cobblestone" ),
     ( "assault" ),
     ( "clouds dark" ),
     ( "night" ),
     ( "night 2" ),
     ( "night flat" ),
     ( "dusty" ),
     ( "rainy" )
};

const char* hitboxes[ ] = {
    "head",
    "body",
    "stomach",
    "arms",
    "legs",
    "feet"
};

const char* snipers_arr[ ] = {
    "none",
    "auto",
    "awp",
    "ssg 08"
};

const char* stop_type[ ] = {
    "minimal",
    "full"
};

const char* pistols_arr[ ] = {
    "none",
    "dual berettas",
    "heavy pistol"
};

const char* additional_arr[ ] = {
    "grenades",
    "armor",
    "taser",
    "defuser"
};

inline const char* esp_flags[ ] = {
    "money",
    "armor",
    "scoped",
    "fake duck",
    "lc"
};

inline const char* materials[ ] = {
    "flat",
    "regular",
    "metallic",
    "glow",
    "glow overlay"
};

inline const char* weapons[ ] = {
    "scar",
    "scout",
    "awp",
    "heavy pistols",
    "pistols",
    "other"
};

inline const char* cfg_slots[ ] = {
    "slot 1",
    "slot 2",
    "slot 3",
    "slot 4",
    "slot 5",
    "slot 6"
};
int current_weapon{};

std::array< float, 6 > anim_multi_points{ -27.f, -27.f, -27.f, -27.f, -27.f, -27.f };
std::array< float, 6 > anim_hit_groups{ -27.f, -27.f, -27.f, -27.f, -27.f, -27.f };
std::array < std::array< std::string, 6 >, 6 > multi_points{};
std::array < std::array< std::string, 6 >, 6 > hit_groups{};
float add_x_rage{};
float add_y_rage{};

float add_x_aa{};
float add_y_aa{};

float add_x_visuals{};
float add_y_visuals{};

float add_x_players{};
float add_y_players{};

float add_x_misc{};
float add_y_misc{};

e_tabs tab{};
int sub_tab_rage{};
int sub_tab_aa{};
int sub_tab_visuals{};
int sub_tab_players{};
int sub_tab_misc{};

ALWAYS_INLINE void draw_rage_body( ImDrawList* draw, int sub_tab ) {

    if ( sub_tab == 0 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( "enabled", &g_menu->main( ).m_aim_bot_cfg.get( ).m_rage_bot );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 117 ) );
        ImGui::Checkbox( "multi thread", &g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_threading );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 145 ) );
        ImGui::Combo( "current weapon", &current_weapon, weapons, IM_ARRAYSIZE( weapons ) );
        ImGui::PopFont( );

        if ( current_weapon == 0 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_scar, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_scar, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_scar, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_scar );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_scar, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );

            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at ( 0 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 0 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at ( 0 ) += 1.f;

                anim_multi_points.at( 0 ) = std::clamp( anim_multi_points.at( 0 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scar & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 0 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 0 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scar |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 0 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scar &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 0 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );

            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at ( 0 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 0 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scar & ( 1 << i );

                    anim_hit_groups.at( 0 ) += 1.f;

                    anim_hit_groups.at( 0 ) = std::clamp( anim_hit_groups.at( 0 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 0 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 0 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scar |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 0 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scar &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 0 ) = -27.f;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_scar_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_scar, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_scar, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_scar );
            ImGui::PopFont( );
        }
        else if ( current_weapon == 1 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_scout, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_scout, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_scout, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_scout );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_scout, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );

            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at( 1 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 1 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at ( 1 ) += 1.f;

                anim_multi_points.at( 1 ) = std::clamp( anim_multi_points.at( 1 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scout & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 1 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 1 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scout |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 1 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_scout &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 1 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );

            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at( 1 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 1 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scout & ( 1 << i );

                    anim_hit_groups.at( 1 ) += 1.f;

                    anim_hit_groups.at( 1 ) = std::clamp( anim_hit_groups.at( 1 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 1 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 1 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scout |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 1 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_scout &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 1 ) = -27.f;


            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_scout_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_scout, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_scout, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_scout );
            ImGui::PopFont( );
        }
        else if ( current_weapon == 2 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_awp, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_awp, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_awp, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_awp );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_awp, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );
            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at( 2 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 2 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at( 2 ) += 1.f;

                anim_multi_points.at( 2 ) = std::clamp( anim_multi_points.at( 2 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_awp & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 2 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 2 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_awp |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 2 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_awp &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 2 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );
            
            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at( 2 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 2 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_awp & ( 1 << i );

                    anim_hit_groups.at( 2 ) += 1.f;

                    anim_hit_groups.at( 2 ) = std::clamp( anim_hit_groups.at( 2 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 2 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 2 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_awp |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 2 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_awp &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 2 ) = -27.f;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_awp_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_awp, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_awp, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_awp );
            ImGui::PopFont( );
        }
        else if ( current_weapon == 3 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_heavy_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_heavy_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_heavy_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_heavy_pistol );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_heavy_pistol, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );
            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at( 3 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 3 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at( 3 ) += 1.f;

                anim_multi_points.at( 3 ) = std::clamp( anim_multi_points.at( 3 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_heavy_pistol & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 3 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 3 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_heavy_pistol |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 3 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_heavy_pistol &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 3 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );

            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at( 3 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 3 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_heavy_pistol & ( 1 << i );

                    anim_hit_groups.at( 3 ) += 1.f;

                    anim_hit_groups.at( 3 ) = std::clamp( anim_hit_groups.at( 3 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 3 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 3 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_heavy_pistol |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 3 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_heavy_pistol &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 3 ) = -27.f;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_heavy_pistol_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_heavy_pistol, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_heavy_pistol, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_heavy_pistol );
            ImGui::PopFont( );
        }
        else if ( current_weapon == 4 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_pistol, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_pistol );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_pistol, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );

            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at( 4 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 4 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at( 4 ) += 1.f;

                anim_multi_points.at( 4 ) = std::clamp( anim_multi_points.at( 4 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_pistol & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 4 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 4 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_pistol |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 4 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_pistol &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 4 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );

            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at( 4 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 4 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_pistol & ( 1 << i );

                    anim_hit_groups.at( 4 ) += 1.f;

                    anim_hit_groups.at( 4 ) = std::clamp( anim_hit_groups.at( 4 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 4 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 4 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_pistol |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 4 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_pistol &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 4 ) = -27.f;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_pistol_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_pistol, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_pistol, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_pistol );
            ImGui::PopFont( );
        }
        else if ( current_weapon == 5 ) {
            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 136 + 31 ) );
            ImGui::SliderFloat( "hitchance", &g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_chance_other, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 169 + 31 ) );
            ImGui::SliderFloat( "head scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_head_scale_other, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 202 + 31 ) );
            ImGui::SliderFloat( "body scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_body_scale_other, 0.f, 100.f, "%.1f" );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 193, 241 + 31 ) );
            ImGui::Checkbox( "static point scale", &g_menu->main( ).m_aim_bot_cfg.get( ).m_static_point_scale_other );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 260 + 31 ) );
            ImGui::SliderInt( "min damage", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_other, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 302 + 31 ) );

            std::stringstream str_multi_points{};
            int iter_count_multi_points{};
            int wtf_multipoints{};

            for ( auto& cur_it : multi_points.at( 5 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_multipoints++;
            }

            for ( auto& cur_it : multi_points.at( 5 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_multi_points >= 4 ) {
                    str_multi_points << " ... ";
                    break;
                }

                str_multi_points << cur_it;

                if ( iter_count_multi_points < wtf_multipoints - 1
                    && wtf_multipoints > 1 )
                    str_multi_points << ", ";

                ++iter_count_multi_points;
            }


            if ( ImGui::BeginCombo( ( "multi points" ), str_multi_points.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                anim_multi_points.at( 5 ) += 1.f;

                anim_multi_points.at( 5 ) = std::clamp( anim_multi_points.at( 5 ), -27.f, 0.f );

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_other & ( 1 << i );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_multi_points.at( 5 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        multi_points.at( 5 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_other |= ( 1 << i );
                    }
                    else {
                        multi_points.at( 5 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_multi_points_other &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_multi_points.at( 5 ) = -27;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 333 + 31 ) );

            std::stringstream str_hit_groups{};
            int iter_count_hit_groups{};
            int wtf_hit_groups{};

            for ( auto& cur_it : hit_groups.at( 5 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                wtf_hit_groups++;
            }

            for ( auto& cur_it : hit_groups.at( 5 ) ) {
                if ( cur_it.empty( )
                    || cur_it == "" )
                    continue;

                if ( iter_count_hit_groups >= 4 ) {
                    str_hit_groups << " ... ";
                    break;
                }

                str_hit_groups << cur_it;

                if ( iter_count_hit_groups < wtf_hit_groups - 1
                    && wtf_hit_groups > 1 )
                    str_hit_groups << ", ";

                ++iter_count_hit_groups;
            }

            if ( ImGui::BeginCombo( ( "hit groups" ), str_hit_groups.str( ).c_str( ), 0, true ) ) {
                static bool hitgroups_vars[ IM_ARRAYSIZE( hitboxes ) ]{};

                for ( std::size_t i{}; i < IM_ARRAYSIZE( hitboxes ); ++i ) {
                    hitgroups_vars[ i ] = g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_other & ( 1 << i );

                    anim_hit_groups.at( 5 ) += 1.f;

                    anim_hit_groups.at( 5 ) = std::clamp( anim_hit_groups.at( 5 ), -27.f, 0.f );

                    ImGui::Selectable(
                        hitboxes[ i ], &hitgroups_vars[ i ],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_hit_groups.at( 5 )
                    );

                    if ( hitgroups_vars[ i ] ) {
                        hit_groups.at( 5 ).at( i ) = hitboxes[ i ];
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_other |= ( 1 << i );
                    }
                    else {
                        hit_groups.at( 5 ).at( i ) = "";
                        g_menu->main( ).m_aim_bot_cfg.get( ).m_hit_groups_other &= ~( 1 << i );
                    }
                }

                ImGui::EndCombo( );
            }
            else
                anim_hit_groups.at( 5 ) = -27.f;

            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 364 + 31 ) );
            g_key_binds->KeybindNelfo( "dmg override", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_override, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 409 + 31 ) );
            ImGui::SliderInt( "damage override val", &g_menu->main( ).m_aim_bot_cfg.get( ).m_min_dmg_other_override_val, 0, 100 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 482 ) );
            g_key_binds->KeybindNelfo( "force safe points", &g_menu->main( ).m_aim_bot_cfg.get( ).m_force_safe_points, false, 300, 197.f );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 196, 525 ) );
            ImGui::SliderInt( "max misses", &g_menu->main( ).m_aim_bot_cfg.get( ).m_max_misses_other, 0, 10 );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 586, 95 ) );
            ImGui::Combo( "stop type", &g_menu->main( ).m_aim_bot_cfg.get( ).m_stop_type_other, stop_type, IM_ARRAYSIZE( stop_type ) );
            ImGui::PopFont( );

            ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
            ImGui::SetCursorPos( ImVec2( 583, 123 ) );
            ImGui::Checkbox( "early stop", &g_menu->main( ).m_aim_bot_cfg.get( ).m_early_stop_other );
            ImGui::PopFont( );
        }

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 151 ) );
        g_key_binds->KeybindNelfo( "double tap", &g_menu->main( ).m_exploits_cfg.get( ).m_double_tap, false, 300, 587.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 205 ) );
        g_key_binds->KeybindNelfo( "hide shots", &g_menu->main( ).m_exploits_cfg.get( ).m_hide_shots, false, 300, 587.f );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
    else if ( sub_tab == 1 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra / 255;
       
        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
}

ALWAYS_INLINE void draw_antiaim_body( ImDrawList* draw, int sub_tab ) {
    if ( sub_tab == 0 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main / 255;
        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( "enabled", &g_menu->main( ).m_anti_aim_cfg.get( ).m_enabled );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 111 ) );
        ImGui::SliderFloat( "pitch", &g_menu->main( ).m_anti_aim_cfg.get( ).m_pitch, -89.f, 89.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 140 ) );
        ImGui::SliderFloat( "yaw range", &g_menu->main( ).m_anti_aim_cfg.get( ).m_yaw, 0.f, 180.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 182 ) );
        g_key_binds->KeybindNelfo( "flip", &g_menu->main( ).m_anti_aim_cfg.get( ).m_flip_side, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 233 ) );
        ImGui::Checkbox( "jitter", &g_menu->main( ).m_anti_aim_cfg.get( ).m_jitter );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 258 ) );
        ImGui::Checkbox( "jitter side", &g_menu->main( ).m_anti_aim_cfg.get( ).m_jitter_side );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 277 ) );
        ImGui::SliderFloat( "jitter range", &g_menu->main( ).m_anti_aim_cfg.get( ).m_jitter_range, 0.f, 100.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 316 ) );
        ImGui::Checkbox( "at targets", &g_menu->main( ).m_anti_aim_cfg.get( ).m_at_targets );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 340 ) );
        g_key_binds->KeybindNelfo( "left manual", &g_menu->main( ).m_anti_aim_cfg.get( ).m_left_manual, true, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 384 ) );
        g_key_binds->KeybindNelfo( "right manual", &g_menu->main( ).m_anti_aim_cfg.get( ).m_right_manual, true, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 424 ) );
        g_key_binds->KeybindNelfo( "back manual", &g_menu->main( ).m_anti_aim_cfg.get( ).m_back_manual, true, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 472 ) );
        g_key_binds->KeybindNelfo( "auto dir", &g_menu->main( ).m_anti_aim_cfg.get( ).m_auto_dir, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 92 ) );
        ImGui::Checkbox( "roll", &g_menu->main( ).m_anti_aim_cfg.get( ).m_roll );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 111 ) );
        ImGui::SliderFloat( "roll range", &g_menu->main( ).m_anti_aim_cfg.get( ).m_roll_range, 0.f, 50.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
    else if ( sub_tab == 1 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 190, 92 ) );
        ImGui::Checkbox( "fake lags", &g_menu->main( ).m_fake_lags_cfg.get( ).m_enabled );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 111 ) );
        ImGui::SliderInt( "ticks", &g_menu->main( ).m_fake_lags_cfg.get( ).m_ticks, 0, 14 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 142 ) );
        ImGui::SliderInt( "variability", &g_menu->main( ).m_fake_lags_cfg.get( ).m_variability, 0, 14 );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
}

ALWAYS_INLINE void draw_players_body( ImDrawList* draw, int sub_tab ) {
    if ( sub_tab == 0 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_players.m_alpha_main / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( ( "draw name" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_name );
        ImGui::PopFont( );
            
        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 117 ) );
        ImGui::Checkbox( ( "draw health" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_health );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 142 ) );
        ImGui::Checkbox( ( "draw box" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_box );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 167 ) );
        ImGui::Checkbox( ( "draw wpn icon" ), &g_menu->main( ).m_visuals_cfg.get( ).m_wpn_icon );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 192 ) );
        ImGui::Checkbox( ( "draw wpn txt" ), &g_menu->main( ).m_visuals_cfg.get( ).m_wpn_text );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 217 ) );
        ImGui::Checkbox( ( "draw ammo" ), &g_menu->main( ).m_visuals_cfg.get( ).m_wpn_ammo );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 242 ) );
        ImGui::Checkbox( ( "draw flags" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_flags );
        ImGui::PopFont( );

        static std::array< std::string, 5 > flagss{};
        std::stringstream str_flags{};
        int iter_count{};
        int wtf{};
  
        static float anim_flags{ -27 };

        for ( auto& cur_it : flagss ) {
            if ( cur_it.empty( )
                || cur_it == "" )
                continue;

            wtf++;
        }

        for ( auto& cur_it : flagss ) {
            if ( cur_it.empty( )
                || cur_it == "" )
                continue;

            if ( iter_count >= 3 ) {
                str_flags << " ... ";
                break;
            }

            str_flags << cur_it;

            if ( iter_count < wtf - 1
                && wtf > 1 )
                str_flags << ", ";

            ++iter_count;
        }

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 270 ) );
        if ( ImGui::BeginCombo( ( "flags" ), str_flags.str( ).c_str( ), 0, true ) ) {
            static bool hitgroups_vars[ IM_ARRAYSIZE( esp_flags ) ]{};

            anim_flags += 1.f;

            anim_flags = std::clamp( anim_flags, -27.f, 0.f );

            for ( std::size_t i{}; i < IM_ARRAYSIZE( esp_flags ); ++i ) {
                hitgroups_vars[ i ] = g_menu->main( ).m_visuals_cfg.get( ).m_player_flags & ( 1 << i );

                ImGui::Selectable(
                    esp_flags[ i ], &hitgroups_vars[ i ],
                    ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_flags
                );

                if ( hitgroups_vars[ i ] ) {
                    flagss.at( i ) = esp_flags[ i ];
                    g_menu->main( ).m_visuals_cfg.get( ).m_player_flags |= ( 1 << i );
                }
                else {
                    flagss.at( i ) = "";
                    g_menu->main( ).m_visuals_cfg.get( ).m_player_flags &= ~( 1 << i );
                }
            }

            ImGui::EndCombo( );
        }
        else
            anim_flags = -27;
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 298 ) );
        ImGui::Checkbox( ( "draw oof" ), &g_menu->main( ).m_visuals_cfg.get( ).m_oof_indicator );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 323 ) );
        ImGui::Checkbox( ( "glow" ), &g_menu->main( ).m_visuals_cfg.get( ).m_glow );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 326 ) );
        ImGui::ColorEdit4( ( "sdsd" ), g_menu->main( ).m_visuals_cfg.get( ).m_glow_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 92 ) );
        ImGui::Checkbox( "player model chams", &g_menu->main( ).m_models_cfg.get( ).m_player );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 95 ) );
        ImGui::ColorEdit4( "vavilon", g_menu->main( ).m_models_cfg.get( ).m_player_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 120 ) );
        ImGui::Combo( "material", &g_menu->main( ).m_models_cfg.get( ).m_player_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 148 ) );
        ImGui::Checkbox( "player model glow outline", &g_menu->main( ).m_models_cfg.get( ).m_player_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 151 ) );
        ImGui::ColorEdit4( "player model glow outline color", g_menu->main( ).m_models_cfg.get( ).m_player_glow_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 173 ) );
        ImGui::Checkbox( "player model occluded chams", &g_menu->main( ).m_models_cfg.get( ).m_player_occluded );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 176 ) );
        ImGui::ColorEdit4( "player model occluded chams color", g_menu->main( ).m_models_cfg.get( ).m_player_occluded_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 201 ) );
        ImGui::Combo( "occluded material", &g_menu->main( ).m_models_cfg.get( ).m_player_occluded_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 229 ) );
        ImGui::Checkbox( "player model occluded glow outline", &g_menu->main( ).m_models_cfg.get( ).m_player_occluded_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 232 ) );
        ImGui::ColorEdit4( "player model occluded glow outline color", g_menu->main( ).m_models_cfg.get( ).m_player_glow_occluded_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 254 ) );
        ImGui::Checkbox( "local player model chams", &g_menu->main( ).m_models_cfg.get( ).m_local_player );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 257 ) );
        ImGui::ColorEdit4( "local player model chams color", g_menu->main( ).m_models_cfg.get( ).m_local_player_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 282 ) );
        ImGui::Combo( "local material", &g_menu->main( ).m_models_cfg.get( ).m_local_player_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 310 ) );
        ImGui::Checkbox( "local player model glow outline", &g_menu->main( ).m_models_cfg.get( ).m_local_player_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 313 ) );
        ImGui::ColorEdit4( "local player model glow outline color", g_menu->main( ).m_models_cfg.get( ).m_local_player_glow_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 335 ) );
        ImGui::Checkbox( "local fake model chams", &g_menu->main( ).m_models_cfg.get( ).m_local_player_fake );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 338 ) );
        ImGui::ColorEdit4( "local fake model chams color", g_menu->main( ).m_models_cfg.get( ).m_local_player_fake_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 363 ) );
        ImGui::Combo( "fake material", &g_menu->main( ).m_models_cfg.get( ).m_local_player_fake_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 391 ) );
        ImGui::Checkbox( "local fake model glow outline", &g_menu->main( ).m_models_cfg.get( ).m_local_player_fake_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 394 ) );
        ImGui::ColorEdit4( "local fake model glow outline color", g_menu->main( ).m_models_cfg.get( ).m_local_player_fake_glow_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 416 ) );
        ImGui::Checkbox( "ragdoll model chams", &g_menu->main( ).m_models_cfg.get( ).m_ragdolls );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 419 ) );
        ImGui::ColorEdit4( "ragdoll model chams color", g_menu->main( ).m_models_cfg.get( ).m_ragdolls_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 444 ) );
        ImGui::Combo( "ragdoll material", &g_menu->main( ).m_models_cfg.get( ).m_ragdolls_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 472 ) );
        ImGui::Checkbox( "ragdoll model glow outline", &g_menu->main( ).m_models_cfg.get( ).m_ragdolls_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 475 ) );
        ImGui::ColorEdit4( "ragdoll model glow outline color", g_menu->main( ).m_models_cfg.get( ).m_ragdolls_glow_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
    else if ( sub_tab == 1 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_players.m_alpha_extra / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( "hands chams", &g_menu->main( ).m_models_cfg.get( ).m_hands );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 95 ) );
        ImGui::ColorEdit4( "hands chams color", g_menu->main( ).m_models_cfg.get( ).m_hands_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 120 ) );
        ImGui::Combo( "hands material", &g_menu->main( ).m_models_cfg.get( ).m_hands_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 148 ) );
        ImGui::Checkbox( "hands glow outline", &g_menu->main( ).m_models_cfg.get( ).m_hands_glow_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 151 ) );
        ImGui::ColorEdit4( "hands glow outline color", g_menu->main( ).m_models_cfg.get( ).m_hands_glow_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 173 ) );
        ImGui::Checkbox( "local wpn chams", &g_menu->main( ).m_models_cfg.get( ).m_local_weapon );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 176 ) );
        ImGui::ColorEdit4( "local wpn chams color", g_menu->main( ).m_models_cfg.get( ).m_local_weapon_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 201 ) );
        ImGui::Combo( "material", &g_menu->main( ).m_models_cfg.get( ).m_local_weapon_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 229 ) );
        ImGui::Checkbox( "local wpn glow outline", &g_menu->main( ).m_models_cfg.get( ).m_local_weapon_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 232 ) );
        ImGui::ColorEdit4( "local wpn glow outline color", g_menu->main( ).m_models_cfg.get( ).m_local_weapon_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 254 ) );
        ImGui::Checkbox( "wpns chams", &g_menu->main( ).m_models_cfg.get( ).m_weapons );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 257 ) );
        ImGui::ColorEdit4( "wpns chams color", g_menu->main( ).m_models_cfg.get( ).m_weapons_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 282 ) );
        ImGui::Combo( "wpns material", &g_menu->main( ).m_models_cfg.get( ).m_weapons_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 310 ) );
        ImGui::Checkbox( "wpns glow outline", &g_menu->main( ).m_models_cfg.get( ).m_weapons_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 313 ) );
        ImGui::ColorEdit4( "wpns glow outline color", g_menu->main( ).m_models_cfg.get( ).m_weapons_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 335 ) );
        ImGui::Checkbox( "shot chams", &g_menu->main( ).m_models_cfg.get( ).m_shot );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 338 ) );
        ImGui::ColorEdit4( "shot chams color", g_menu->main( ).m_models_cfg.get( ).m_shot_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 363 ) );
        ImGui::Combo( "shot material", &g_menu->main( ).m_models_cfg.get( ).m_shot_type, materials, IM_ARRAYSIZE( materials ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 391 ) );
        ImGui::Checkbox( "shot glow outline", &g_menu->main( ).m_models_cfg.get( ).m_shot_outline );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 394 ) );
        ImGui::ColorEdit4( "shot glow outline color", g_menu->main( ).m_models_cfg.get( ).m_shot_outline_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
}

ALWAYS_INLINE void draw_visuals_body( ImDrawList* draw, int sub_tab ) {
    if ( sub_tab == 0 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( ( "fog" ), &g_menu->main( ).m_visuals_cfg.get( ).m_fog );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 490, 95 ) );
        ImGui::ColorEdit4( ( "fog clr" ), g_menu->main( ).m_visuals_cfg.get( ).m_fog_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 111 ) );
        ImGui::SliderInt( "fog start", &g_menu->main( ).m_visuals_cfg.get( ).m_fog_start, 0, 1000 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 144 ) );
        ImGui::SliderInt( "fog end", &g_menu->main( ).m_visuals_cfg.get( ).m_fog_end, 100, 1100 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 177 ) );
        ImGui::SliderInt( "fog density", &g_menu->main( ).m_visuals_cfg.get( ).m_fog_density, 0, 100 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 216 ) );
        ImGui::Checkbox( "modulate shadows", &g_menu->main( ).m_visuals_cfg.get( ).m_shadows_modulation );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 235 ) );
        ImGui::SliderFloat( "x dir", &g_menu->main( ).m_visuals_cfg.get( ).m_x_dir, -100.f, 100.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 268 ) );
        ImGui::SliderFloat( "y dir", &g_menu->main( ).m_visuals_cfg.get( ).m_y_dir, -100.f, 100.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 301 ) );
        ImGui::SliderFloat( "z dir", &g_menu->main( ).m_visuals_cfg.get( ).m_z_dir, -100.f, 100.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 340 ) );
        ImGui::Checkbox( "tone map modulation", &g_menu->main( ).m_visuals_cfg.get( ).m_tone_map_modulation );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 359 ) );
        ImGui::SliderInt( "bloom", &g_menu->main( ).m_visuals_cfg.get( ).m_bloom, 0, 750 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 392 ) );
        ImGui::SliderInt( "exposure", &g_menu->main( ).m_visuals_cfg.get( ).m_exposure, 0, 2000 );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 434 ) );
        ImGui::ColorEdit4( "world modulation", g_menu->main( ).m_visuals_cfg.get( ).m_world_modulation.data( ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 460 ) );
        ImGui::ColorEdit4( "sky modulation", g_menu->main( ).m_visuals_cfg.get( ).m_sky_modulation.data( ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 486 ) );
        ImGui::ColorEdit4( "props modulation", g_menu->main( ).m_visuals_cfg.get( ).m_props_modulation.data( ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 512 ) );
        ImGui::Combo( ( "sky box" ), &g_menu->main( ).m_visuals_cfg.get( ).m_sky_box, skybox_list, IM_ARRAYSIZE( skybox_list ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 540 ) );
        ImGui::Checkbox( "show weapon in scope", &g_menu->main( ).m_visuals_cfg.get( ).m_show_wpn_in_scope );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 92 ) );
        ImGui::Checkbox( "remove flash", &g_menu->main( ).m_misc_cfg.get( ).m_remove_flash );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 117 ) );
        ImGui::Checkbox( "remove smoke", &g_menu->main( ).m_misc_cfg.get( ).m_remove_smoke );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 142 ) );
        ImGui::Checkbox( "remove land bob", &g_menu->main( ).m_misc_cfg.get( ).m_remove_land_bob );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 167 ) );
        ImGui::Checkbox( "remove view kick", &g_menu->main( ).m_misc_cfg.get( ).m_remove_view_kick );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 192 ) );
        ImGui::Checkbox( "remove view punch", &g_menu->main( ).m_misc_cfg.get( ).m_remove_view_punch );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 217 ) );
        ImGui::Checkbox( "remove post processing", &g_menu->main( ).m_misc_cfg.get( ).m_remove_post_processing );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 242 ) );
        ImGui::Checkbox( "remove hands shaking", &g_menu->main( ).m_misc_cfg.get( ).m_remove_hands_shaking );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 267 ) );
        ImGui::Checkbox( "remove scope", &g_menu->main( ).m_misc_cfg.get( ).m_remove_scope );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 292 ) );
        ImGui::Checkbox( "force crosshair", &g_menu->main( ).m_misc_cfg.get( ).m_force_crosshair );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 317 ) );
        ImGui::Checkbox( ( "molotov timer" ), &g_menu->main( ).m_visuals_cfg.get( ).m_molotov_timer );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 342 ) );
        ImGui::Checkbox( ( "smoke timer" ), &g_menu->main( ).m_visuals_cfg.get( ).m_smoke_timer );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 367 ) );
        ImGui::Checkbox( ( "grenade projectiles" ), &g_menu->main( ).m_visuals_cfg.get( ).m_grenade_projectiles );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 392 ) );
        ImGui::Checkbox( ( "draw wpn proj" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_wpn_proj );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 417 ) );
        ImGui::Checkbox( ( "draw wpn proj ( icon )" ), &g_menu->main( ).m_visuals_cfg.get( ).m_draw_wpn_proj_icon );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 442 ) );
        ImGui::Checkbox( ( "grenade prediction" ), &g_menu->main( ).m_visuals_cfg.get( ).m_grenade_prediction );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 445 ) );
        ImGui::ColorEdit4( "grenade prediction clr", g_menu->main( ).m_visuals_cfg.get( ).m_grenade_pred_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 467 ) );
        ImGui::Checkbox( "hits marker", &g_menu->main( ).m_visuals_cfg.get( ).m_hits_marker );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 492 ) );
        ImGui::Checkbox( "enemy bullets tracers", &g_menu->main( ).m_visuals_cfg.get( ).m_enemy_bullet_tracers );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 495 ) );
        ImGui::ColorEdit4( "enemy bullets tracers clr", g_menu->main( ).m_visuals_cfg.get( ).m_enemy_bullet_tracers_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 517 ) );
        ImGui::Checkbox( "bullets tracers", &g_menu->main( ).m_visuals_cfg.get( ).m_bullet_tracers );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 883, 520 ) );
        ImGui::ColorEdit4( "bullets tracers clr", g_menu->main( ).m_visuals_cfg.get( ).m_bullet_tracers_clr.data( ), ImGuiColorEditFlags_NoLabel );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 542 ) );
        ImGui::Checkbox( "bullet impacts", &g_menu->main( ).m_visuals_cfg.get( ).m_bullet_impacts );
        ImGui::PopFont( );

        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
    else if ( sub_tab == 1 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( "aspect ratio", &g_menu->main( ).m_misc_cfg.get( ).m_aspect_ratio );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 111 ) );
        ImGui::SliderFloat( "aspect ratio val", &g_menu->main( ).m_misc_cfg.get( ).m_aspect_ratio_val, 0.f, 3.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 144 ) );
        ImGui::SliderFloat( "camera distance", &g_menu->main( ).m_misc_cfg.get( ).m_camera_distance, 0.f, 140.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 183 ) );
        ImGui::Checkbox( "blend in scope", &g_menu->main( ).m_visuals_cfg.get( ).m_blend_in_scope );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 202 ) );
        ImGui::SliderInt( "blend in scope val", &g_menu->main( ).m_visuals_cfg.get( ).m_blend_in_scope_val, 0, 100 );
        ImGui::PopFont( );


        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
}

ALWAYS_INLINE void draw_misc_body( ImDrawList* draw, int sub_tab ) {
    if ( sub_tab == 0 ) {
        auto backup_alpha = ImGui::GetStyle( ).Alpha;
        ImGui::GetStyle( ).Alpha = m_tabs_data.m_sub_tabs.m_misc.m_alpha_main / 255;

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 92 ) );
        ImGui::Checkbox( "bhop", &g_menu->main( ).m_move_cfg.get( ).m_bhop );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 117 ) );
        ImGui::Checkbox( "strafe", &g_menu->main( ).m_move_cfg.get( ).m_auto_strafe );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 142 ) );
        ImGui::Checkbox( "fast stop", &g_menu->main( ).m_move_cfg.get( ).m_fast_stop );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 167 ) );
        ImGui::Checkbox( "infinity duck", &g_menu->main( ).m_move_cfg.get( ).m_infinity_duck );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 192 ) );
        ImGui::Checkbox( "slide walk", &g_menu->main( ).m_move_cfg.get( ).m_slide_walk );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 220 ) );
        g_key_binds->KeybindNelfo( "fake duck", &g_menu->main( ).m_move_cfg.get( ).m_fake_duck, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 274 ) );
        g_key_binds->KeybindNelfo( "slow walk", &g_menu->main( ).m_move_cfg.get( ).m_slow_walk, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 328 ) );
        g_key_binds->KeybindNelfo( "auto peek", &g_menu->main( ).m_move_cfg.get( ).m_auto_peek, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 382 ) );
        g_key_binds->KeybindNelfo( "third person", &g_menu->main( ).m_misc_cfg.get( ).m_third_person, false, 0, 197.f );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 196, 427 ) );
        ImGui::SliderFloat( "third person dist", &g_menu->main( ).m_misc_cfg.get( ).m_third_person_val, 55.f, 180.f, "%.1f" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 193, 466 ) );
        ImGui::Checkbox( "third person when dead", &g_menu->main( ).m_misc_cfg.get( ).m_third_person_when_dead );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 92 ) );
        ImGui::Checkbox( "clan tag", &g_menu->main( ).m_misc_cfg.get( ).m_clan_tag );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 117 ) );
        ImGui::Checkbox( "killfeed", &g_menu->main( ).m_visuals_cfg.get( ).m_kill_feed );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 142 ) );
        ImGui::Checkbox( ( "spectators list" ), &g_menu->main( ).m_visuals_cfg.get( ).m_spectators_list );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 167 ) );
        ImGui::Checkbox( ( "keys list" ), &g_menu->main( ).m_visuals_cfg.get( ).m_key_binds_list );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 192 ) );
        ImGui::Checkbox( "buy bot", &g_menu->main( ).m_misc_cfg.get( ).m_buy_bot );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 220 ) );
        ImGui::Combo( ( "snipers" ), &g_menu->main( ).m_misc_cfg.get( ).m_buy_bot_snipers, snipers_arr, IM_ARRAYSIZE( snipers_arr ) );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 251 ) );
        ImGui::Combo( ( "pistols" ), &g_menu->main( ).m_misc_cfg.get( ).m_buy_bot_pistols, pistols_arr, IM_ARRAYSIZE( pistols_arr ) );
        ImGui::PopFont( );

        static float anim_buybot{ -27 };

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 282 ) );

        static std::array< std::string, 4 > buybottt{};
        std::stringstream str_buybot{};
        int iter_count{};
        int wtf{};

        for ( auto& cur_it : buybottt ) {
            if ( cur_it.empty( )
                || cur_it == "" )
                continue;

            wtf++;
        }

        for ( auto& cur_it : buybottt ) {
            if ( cur_it.empty( ) 
                || cur_it == "" )
                continue;

            if ( iter_count >= 5 ) {
                str_buybot << " ... ";
                break;
            }

            str_buybot << cur_it;

            if ( iter_count < wtf - 1
                && wtf > 1 )
                str_buybot << ", ";

            ++iter_count;
        }

        if ( ImGui::BeginCombo( ( "additionals" ), str_buybot.str( ).c_str( ), 0, true ) ) {
            static bool hitgroups_vars[ IM_ARRAYSIZE( additional_arr ) ]{};

            anim_buybot += 1.f;

            anim_buybot = std::clamp( anim_buybot, -27.f, 0.f );

            for ( std::size_t i{}; i < IM_ARRAYSIZE( additional_arr ); ++i ) {
                hitgroups_vars[ i ] = g_menu->main( ).m_misc_cfg.get( ).m_buy_bot_additional & ( 1 << i );

                ImGui::Selectable(
                    additional_arr[ i ], &hitgroups_vars[ i ],
                    ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2( ), anim_buybot
                );

                if ( hitgroups_vars[ i ] ) {
                    buybottt.at( i ) = additional_arr[ i ];
                    g_menu->main( ).m_misc_cfg.get( ).m_buy_bot_additional |= ( 1 << i );
                }
                else {
                    buybottt.at( i ) = "";
                    g_menu->main( ).m_misc_cfg.get( ).m_buy_bot_additional &= ~( 1 << i );
                }
            }

            ImGui::EndCombo( );
        }
        else
            anim_buybot = -27;

        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 583, 310 ) );
        ImGui::Checkbox( "hit sound", &g_menu->main( ).m_misc_cfg.get( ).m_hit_sound );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 586, 338 ) );

        static int cfg_slot{};

        ImGui::Combo( ( "cfg slot" ), &cfg_slot, cfg_slots, IM_ARRAYSIZE( cfg_slots ) );

        ImGui::SetCursorPos( ImVec2( 587, 370 ) );

        if ( ImGui::Button( "load", ImVec2( 45, 20 ) ) )
            sdk::g_cfg->load( cfg_slots[ cfg_slot ] );

        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_verdana_15_regular );
        ImGui::SetCursorPos( ImVec2( 587, 402 ) );
        if ( ImGui::Button( "save", ImVec2( 45, 20 ) ) )
            sdk::g_cfg->save( cfg_slots[ cfg_slot ] );

        ImGui::PopFont( );
        ImGui::GetStyle( ).Alpha = backup_alpha;
    }
}

ALWAYS_INLINE void draw_rage_subtabs( ImDrawList* draw, int sub_tab, const ImVec2& pos ) {
    if ( sub_tab == 0 ) {
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main += 10;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main += 2;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main -= 10;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main -= 2;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main -= 1;
    }
    if ( sub_tab == 1 ) {
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra += 10;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra += 2;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra -= 10;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra -= 2;
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra -= 1;
    }

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main > 255 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main = 255;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main > 68 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main = 68;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main > 8 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main = 8;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra > 255 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra = 255;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra > 68 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra = 68;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra > 8 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra = 8;
    else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra = 0;


    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 112 ), ImVec2( pos.x + 10, pos.y + 80 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_main ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 112 ), ImVec2( pos.x + 139, pos.y + 80 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_main ) );

    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 152 ), ImVec2( pos.x + 10, pos.y + 120 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_rage_bot.m_blue_rect_alpha_extra ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 152 ), ImVec2( pos.x + 139, pos.y + 120 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_rage_bot.m_main_rect_alpha_extra ) );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_26 );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 82 ), ImColor( 255, 255, 255, 255 ), "Main" );
    ImGui::PopFont( );
}

ALWAYS_INLINE void draw_antiaim_subtabs( ImDrawList* draw, int sub_tab, const ImVec2& pos ) {
    if ( sub_tab == 0 ) {
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main += 10;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main += 2;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main -= 10;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main -= 2;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main -= 1;
    }
    if ( sub_tab == 1 ) {
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra += 10;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra += 2;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra -= 10;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra -= 2;
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra -= 1;
    }

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main > 255 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main = 255;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main > 68 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main = 68;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main > 8 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main = 8;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra > 255 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra = 255;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra > 68 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra = 68;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra > 8 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra = 8;
    else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra = 0;


    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 112 ), ImVec2( pos.x + 10, pos.y + 80 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_main ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 112 ), ImVec2( pos.x + 139, pos.y + 80 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_main ) );

    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 152 ), ImVec2( pos.x + 10, pos.y + 120 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_anti_aim.m_blue_rect_alpha_extra ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 152 ), ImVec2( pos.x + 139, pos.y + 120 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_anti_aim.m_main_rect_alpha_extra ) );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_26 );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 82 ), ImColor( 255, 255, 255, 255 ), "Main" );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 122 ), ImColor( 255, 255, 255, 225 ), "Extra" );
    ImGui::PopFont( );
}

ALWAYS_INLINE void draw_players_subtabs( ImDrawList* draw, int sub_tab, const ImVec2& pos ) {
    if ( sub_tab == 0 ) {
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main += 10;
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main += 2;
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main -= 10;
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main -= 2;
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main -= 1;
    }
    if ( sub_tab == 1 ) {
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra += 10;
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra += 2;
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra -= 10;
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra -= 2;
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra -= 1;
    }

    if ( m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main > 255 )
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main = 255;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main > 68 )
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main = 68;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main > 8 )
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main = 8;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra > 255 )
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra = 255;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra > 68 )
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra = 68;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra > 8 )
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra = 8;
    else if ( m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra = 0;


    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 112 ), ImVec2( pos.x + 10, pos.y + 80 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_main ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 112 ), ImVec2( pos.x + 139, pos.y + 80 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_main ) );

    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 152 ), ImVec2( pos.x + 10, pos.y + 120 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_players.m_blue_rect_alpha_extra ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 152 ), ImVec2( pos.x + 139, pos.y + 120 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_players.m_main_rect_alpha_extra ) );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_26 );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 82 ), ImColor( 255, 255, 255, 255 ), "Main" );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 122 ), ImColor( 255, 255, 255, 225 ), "Extra" );
    ImGui::PopFont( );
}

ALWAYS_INLINE void draw_visuals_subtabs( ImDrawList* draw, int sub_tab, const ImVec2& pos ) {
    if ( sub_tab == 0 ) {
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main += 10;
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main += 2;
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main -= 10;
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main -= 2;
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main -= 1;
    }
    if ( sub_tab == 1 ) {
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra += 10;
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra += 2;
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra -= 10;
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra -= 2;
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra -= 1;
    }

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main > 255 )
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main = 255;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main > 68 )
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main = 68;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main > 8 )
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main = 8;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra > 255 )
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra = 255;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra > 68 )
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra = 68;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra > 8 )
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra = 8;
    else if ( m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra = 0;


    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 112 ), ImVec2( pos.x + 10, pos.y + 80 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_main ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 112 ), ImVec2( pos.x + 139, pos.y + 80 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_main ) );

    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 152 ), ImVec2( pos.x + 10, pos.y + 120 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_visuals.m_blue_rect_alpha_extra ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 152 ), ImVec2( pos.x + 139, pos.y + 120 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_visuals.m_main_rect_alpha_extra ) );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_26 );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 82 ), ImColor( 255, 255, 255, 255 ), "Main" );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 122 ), ImColor( 255, 255, 255, 225 ), "Extra" );
    ImGui::PopFont( );
}

ALWAYS_INLINE void draw_misc_subtabs( ImDrawList* draw, int sub_tab, const ImVec2& pos ) {
    if ( sub_tab == 0 ) {
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main += 10;
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main += 2;
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main -= 10;
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main -= 2;
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main -= 1;
    }
    if ( sub_tab == 1 ) {
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra += 10;
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra += 2;
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra += 1;
    }
    else {
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra -= 10;
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra -= 2;
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra -= 1;
    }

    if ( m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main > 255 )
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main = 255;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main > 68 )
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main = 68;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main > 8 )
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main = 8;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main = 0;

    if ( m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra > 255 )
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra = 255;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra > 68 )
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra = 68;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra = 0;

    if ( m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra > 8 )
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra = 8;
    else if ( m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra < 0 )
        m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra = 0;


    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 112 ), ImVec2( pos.x + 10, pos.y + 80 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_main ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 112 ), ImVec2( pos.x + 139, pos.y + 80 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_main ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_main ) );

    draw->AddRectFilled( ImVec2( pos.x + 6, pos.y + 152 ), ImVec2( pos.x + 10, pos.y + 120 ), ImColor( 159, 182, 220, m_tabs_data.m_sub_tabs.m_misc.m_blue_rect_alpha_extra ) );
    draw->AddRectFilledMultiColor( ImVec2( pos.x + 7, pos.y + 152 ), ImVec2( pos.x + 139, pos.y + 120 ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_outline_rect_alpha_extra ), ImColor( 140, 140, 140, m_tabs_data.m_sub_tabs.m_misc.m_main_rect_alpha_extra ) );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_26 );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 82 ), ImColor( 255, 255, 255, 255 ), "Main" );
    ImGui::PopFont( );
}

ALWAYS_INLINE void draw_body( ImDrawList* draw, const ImVec2& pos ) {
    /* bg */
    draw->AddRectFilled( ImVec2( pos.x, pos.y ), ImVec2( pos.x + 957, pos.y + 612 ), ImColor( 27, 28, 32, 255 ) );

    /* top */
    draw->AddRectFilled( ImVec2( pos.x, pos.y ), ImVec2( pos.x + 956, pos.y + 57 ), ImColor( 33, 34, 38, 255 ) );

    /* top line */
    draw->AddRectFilled( ImVec2( pos.x, pos.y + 57 ), ImVec2( pos.x + 956, pos.y + 61 ), ImColor( 109, 130, 162, 255 ) );

    /* down */
    draw->AddRectFilled( ImVec2( pos.x, pos.y + 589 ), ImVec2( pos.x + 956, pos.y + 612 ), ImColor( 33, 34, 38, 255 ) );

    /* sub tab line */
    draw->AddRectFilled( ImVec2( pos.x + 139, pos.y + 61 ), ImVec2( pos.x + 140, pos.y + 589 ), ImColor( 40, 41, 45, 255 ) );

    /* sub tabs */
    draw->AddRectFilled( ImVec2( pos.x + 1, pos.y + 61 ), ImVec2( pos.x + 139, pos.y + 589 ), ImColor( 30, 31, 35, 255 ) );

    /* tabs */
    draw->AddRectFilled( ImVec2( pos.x + 181, pos.y + 83 ), ImVec2( pos.x + 532, pos.y + 565 ), ImColor( 30, 31, 35, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 574, pos.y + 83 ), ImVec2( pos.x + 925, pos.y + 565 ), ImColor( 30, 31, 35, 255 ) );

    /* tabs outline */
    draw->AddRectFilled( ImVec2( pos.x + 181, pos.y + 83 ), ImVec2( pos.x + 532, pos.y + 84 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 180, pos.y + 83 ), ImVec2( pos.x + 181, pos.y + 565 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 180, pos.y + 565 ), ImVec2( pos.x + 532, pos.y + 566 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 532, pos.y + 83 ), ImVec2( pos.x + 533, pos.y + 566 ), ImColor( 40, 41, 45, 255 ) );

    /* second tab */
    draw->AddRectFilled( ImVec2( pos.x + 574, pos.y + 83 ), ImVec2( pos.x + 574.f + 351.f, pos.y + 84 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 573, pos.y + 83 ), ImVec2( pos.x + 574, pos.y + 565 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 573, pos.y + 565 ), ImVec2( pos.x + 574.f + 351.f, pos.y + 566 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 924, pos.y + 83 ), ImVec2( pos.x + 925, pos.y + 83 + 483 ), ImColor( 40, 41, 45, 255 ) );

    /* outline lines */
    draw->AddRectFilled( ImVec2( pos.x, pos.y ), ImVec2( pos.x + 6, pos.y + 612 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x, pos.y + 610 ), ImVec2( pos.x + 957, pos.y + 612 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x + 954, pos.y ), ImVec2( pos.x + 957, pos.y + 612 ), ImColor( 40, 41, 45, 255 ) );
    draw->AddRectFilled( ImVec2( pos.x, pos.y + 3 ), ImVec2( pos.x + 957, pos.y ), ImColor( 40, 41, 45, 255 ) );

    /* drawings */
    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_700_39 );
    draw->AddText( ImVec2( pos.x + 22, pos.y + 9 ), ImColor( 30, 30, 30, 130 ), "A" );
    draw->AddText( ImVec2( pos.x + 21, pos.y + 8 ), ImColor( 159, 182, 220, 255 ), "A" );
    ImGui::PopFont( );

    ImGui::PushFont( csgo::g_render->m_fonts.m_museo_sans.m_300_30 );
    draw->AddText( ImVec2( pos.x + 42, pos.y + 15 ), ImColor( 255, 255, 255, 255 ), "dvance" );
    ImGui::PopFont( );

    ImGui::PushFont( csgo::g_render->m_fonts.m_tahoma_14_regular );
    draw->AddText( ImVec2( pos.x + 13, pos.y + 592 ), ImColor( 255, 255, 255, 255 ), "advance.tech for cs:go" );
    //draw->AddText( ImVec2( pos.x + 873, pos.y + 592 ), ImColor( 255, 255, 255, 255 ), "current date" );
    ImGui::PopFont( );
}


void c_menu::render( ) {
    if ( m_main.m_hidden )
        return;

    auto is_valid_cursor = [ ]( int max_x, int max_y, int min_x, int min_y ) {
        auto mouse_pos = ImGui::GetMousePos( );
        auto win_pos = ImGui::GetWindowPos( );
        if ( mouse_pos.x <= win_pos.x + max_x
            && mouse_pos.x > win_pos.x + min_x
            && mouse_pos.y <= win_pos.y + max_y
            && mouse_pos.y > win_pos.y + min_y )
            return true;

        return false;
    };
    ImGui::Begin( "got em", false, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar );
    {

        bool clicked{ ImGui::GetIO( ).MouseClicked[ 0 ] };
        ImVec2 pos;
        ImDrawList* draw;
        pos = ImGui::GetWindowPos( );
        draw = ImGui::GetWindowDrawList( );
        ImGui::SetWindowSize( ImVec2( 1000, 1000 ) );

        draw_body( draw, pos );

        if ( clicked
            && is_valid_cursor( 925, 50, 850, 12 ) ) {
            tab = e_tabs::aim_bot;
        }

        if ( clicked
            && is_valid_cursor( 832, 50, 751, 12 ) ) {
            tab = e_tabs::anti_aim;
        }

        if ( clicked
            && is_valid_cursor( 731, 50, 654, 12 ) ) {
            tab = e_tabs::players;
        }

        if ( clicked
            && is_valid_cursor( 639, 50, 562, 12 ) ) {
            tab = e_tabs::visuals;
        }

        if ( clicked
            && is_valid_cursor( 547, 50, 499, 12 ) ) {
            tab = e_tabs::misc;
        }


        m_tabs_data.set_alpha( tab );

        switch ( tab ) {
        case e_tabs::aim_bot:
            if ( clicked &&
                is_valid_cursor( 70, 112, 20, 80 ) ) {
                sub_tab_rage = 0;
            }

            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::anti_aim:
            if ( clicked &&
                is_valid_cursor( 70, 112, 20, 80 ) ) {
                sub_tab_aa = 0;
            }

            if ( clicked &&
                is_valid_cursor( 70, 152, 20, 120 ) ) {
                sub_tab_aa = 1;
            }

            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::visuals:
            if ( clicked &&
                is_valid_cursor( 70, 112, 20, 80 ) ) {
                sub_tab_visuals = 0;
            }

            if ( clicked &&
                is_valid_cursor( 70, 152, 20, 120 ) ) {
                sub_tab_visuals = 1;
            }

            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::misc:
            if ( clicked &&
                is_valid_cursor( 70, 112, 20, 80 ) ) {
                sub_tab_misc = 0;
            }

            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::players:
            if ( clicked &&
                is_valid_cursor( 70, 112, 20, 80 ) ) {
                sub_tab_players = 0;
            }

            if ( clicked &&
                is_valid_cursor( 70, 152, 20, 120 ) ) {
                sub_tab_players = 1;
            }

            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            break;
        default:
            break;
        }

        switch ( tab ) {
        case e_tabs::aim_bot:
            draw_rage_subtabs( draw, sub_tab_rage, pos );
            break;
        case e_tabs::anti_aim:
            draw_antiaim_subtabs( draw, sub_tab_aa, pos );
            break;
        case e_tabs::visuals:
            draw_visuals_subtabs( draw, sub_tab_visuals, pos );
            break;
        case e_tabs::misc:
            draw_misc_subtabs( draw, sub_tab_misc, pos );
            break;
        case e_tabs::players:
            draw_players_subtabs( draw, sub_tab_players, pos );
            break;
        default:
            break;
        }

        switch ( tab ) {
        case e_tabs::aim_bot:
            add_x_rage += 2.f;
            add_y_rage += 0.81f;
            add_x_aa -= 2.f;
            add_y_aa -= 0.81f;
            add_x_players -= 2.f;
            add_y_players -= 0.81f;
            add_x_visuals -= 2.f;
            add_y_visuals -= 0.81f;
            add_x_misc -= 2.f;
            add_y_misc -= 0.81f;
            break;

        case e_tabs::anti_aim:
            add_x_rage -= 2.f;
            add_y_rage -= 0.81f;
            add_x_aa += 2.f;
            add_y_aa += 0.81f;
            add_x_players -= 2.f;
            add_y_players -= 0.81f;
            add_x_visuals -= 2.f;
            add_y_visuals -= 0.81f;
            add_x_misc -= 2.f;
            add_y_misc -= 0.81f;
            break;

        case e_tabs::players:
            add_x_rage -= 2.f;
            add_y_rage -= 0.81f;
            add_x_aa -= 2.f;
            add_y_aa -= 0.81f;
            add_x_players += 2.f;
            add_y_players += 0.81f;
            add_x_visuals -= 2.f;
            add_y_visuals -= 0.81f;
            add_x_misc -= 2.f;
            add_y_misc -= 0.81f;
            break;

        case e_tabs::visuals:
            add_x_rage -= 2.f;
            add_y_rage -= 0.81f;
            add_x_aa -= 2.f;
            add_y_aa -= 0.81f;
            add_x_players -= 2.f;
            add_y_players -= 0.81f;
            add_x_visuals += 2.f;
            add_y_visuals += 0.81f;
            add_x_misc -= 2.f;
            add_y_misc -= 0.81f;
            break;

        case e_tabs::misc:
            add_x_rage -= 2.f;
            add_y_rage -= 0.81f;
            add_x_aa -= 2.f;
            add_y_aa -= 0.81f;
            add_x_players -= 2.f;
            add_y_players -= 0.81f;
            add_x_visuals -= 2.f;
            add_y_visuals -= 0.81f;
            add_x_misc += 2.f;
            add_y_misc += 0.81f;
            break;
        }

        if ( add_x_rage > 86.f )
            add_x_rage = 86.f;
        else if ( add_x_rage < 0.f )
            add_x_rage = 0.f;

        if ( add_y_rage > 36.f )
            add_y_rage = 36.f;
        else if ( add_y_rage < 0.f )
            add_y_rage = 0.f;

        if ( add_x_aa > 85.f )
            add_x_aa = 85.f;
        else if ( add_x_aa < 0.f )
            add_x_aa = 0.f;

        if ( add_y_aa > 36.f )
            add_y_aa = 36.f;
        else if ( add_y_aa < 0.f )
            add_y_aa = 0.f;

        if ( add_x_visuals > 86.f )
            add_x_visuals = 86.f;
        else if ( add_x_visuals < 0.f )
            add_x_visuals = 0.f;

        if ( add_y_visuals > 36.f )
            add_y_visuals = 36.f;
        else if ( add_y_visuals < 0.f )
            add_y_visuals = 0.f;

        if ( add_x_players > 86.f )
            add_x_players = 86.f;
        else if ( add_x_players < 0.f )
            add_x_players = 0.f;

        if ( add_y_players > 36.f )
            add_y_players = 36.f;
        else if ( add_y_players < 0.f )
            add_y_players = 0.f;

        if ( add_x_misc > 59.f )
            add_x_misc = 59.f;
        else if ( add_x_misc < 0.f )
            add_x_misc = 0.f;

        if ( add_y_misc > 36.f )
            add_y_misc = 36.f;
        else if ( add_y_misc < 0.f )
            add_y_misc = 0.f;

        if ( add_x_rage != 0.f ) {
            draw->AddRectFilled( ImVec2( pos.x + 845.f, pos.y + 12.f ), ImVec2( pos.x + 845.f + add_x_rage, pos.y + 12.f + add_y_rage ), ImColor( 30, 31, 35, m_tabs_data.m_rage_rect_alpha ), 3.f );
            draw->AddRect( ImVec2( pos.x + 845.f, pos.y + 12.f ), ImVec2( pos.x + 845.f + add_x_rage, pos.y + 12.f + add_y_rage ), ImColor( 40, 41, 45, m_tabs_data.m_rage_rect_alpha ), 3.f );
        }

        if ( add_x_aa != 0.f ) {
            draw->AddRectFilled( ImVec2( pos.x + 751, pos.y + 12.f ), ImVec2( pos.x + 752.f + add_x_aa, pos.y + 12.f + add_y_aa ), ImColor( 30, 31, 35, m_tabs_data.m_anti_aim_rect_alpha ), 3.f );
            draw->AddRect( ImVec2( pos.x + 751, pos.y + 12.f ), ImVec2( pos.x + 752.f + add_x_aa, pos.y + 12.f + add_y_aa ), ImColor( 40, 41, 45, m_tabs_data.m_anti_aim_rect_alpha ), 3.f );
        }
        if ( add_x_players != 0.f ) {
            draw->AddRectFilled( ImVec2( pos.x + 654.f, pos.y + 12.f ), ImVec2( pos.x + 654.f + add_x_players, pos.y + 12.f + add_y_players ), ImColor( 30, 31, 35, m_tabs_data.m_players_rect_alpha ), 3.f );
            draw->AddRect( ImVec2( pos.x + 654.f, pos.y + 12.f ), ImVec2( pos.x + 654.f + add_x_players, pos.y + 12.f + add_y_players ), ImColor( 40, 41, 45, m_tabs_data.m_players_rect_alpha ), 3.f );

        }
        if ( add_x_visuals != 0.f ) {
            draw->AddRectFilled( ImVec2( pos.x + 559.f, pos.y + 12.f ), ImVec2( pos.x + 559.f + add_x_visuals, pos.y + 12.f + add_y_visuals ), ImColor( 30, 31, 35, m_tabs_data.m_visuals_rect_alpha ), 3.f );
            draw->AddRect( ImVec2( pos.x + 559.f, pos.y + 12.f ), ImVec2( pos.x + 559.f + add_x_visuals, pos.y + 12.f + add_y_visuals ), ImColor( 40, 41, 45, m_tabs_data.m_visuals_rect_alpha ), 3.f );

        }
        if ( add_x_misc != 0.f ) {
            draw->AddRectFilled( ImVec2( pos.x + 495, pos.y + 12 ), ImVec2( pos.x + 495 + add_x_misc, pos.y + 12 + add_y_misc ), ImColor( 30, 31, 35, m_tabs_data.m_misc_rect_alpha ), 3.f );
            draw->AddRect( ImVec2( pos.x + 495, pos.y + 12 ), ImVec2( pos.x + 495 + add_x_misc, pos.y + 12 + add_y_misc ), ImColor( 40, 41, 45, m_tabs_data.m_misc_rect_alpha ), 3.f );

        }

        ImGui::PushFont( csgo::g_render->m_fonts.m_roboto_22_regular );
        draw->AddText( ImVec2( pos.x + 853, pos.y + 18 ), ImColor( 255, 255, 255, m_tabs_data.m_rage_txt_alpha ), "AIMBOT" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_roboto_22_regular );
        draw->AddText( ImVec2( pos.x + 754, pos.y + 18 ), ImColor( 255, 255, 255, m_tabs_data.m_anti_aim_txt_alpha ), "ANTI AIM" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_roboto_22_regular );
        draw->AddText( ImVec2( pos.x + 657, pos.y + 18 ), ImColor( 255, 255, 255, m_tabs_data.m_players_txt_alpha ), "PLAYERS" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_roboto_22_regular );
        draw->AddText( ImVec2( pos.x + 565, pos.y + 18 ), ImColor( 255, 255, 255, m_tabs_data.m_visuals_txt_alpha ), "VISUALS" );
        ImGui::PopFont( );

        ImGui::PushFont( csgo::g_render->m_fonts.m_roboto_22_regular );
        draw->AddText( ImVec2( pos.x + 502, pos.y + 18 ), ImColor( 255, 255, 255, m_tabs_data.m_misc_txt_alpha ), "MISC" );
        ImGui::PopFont( );

        if ( tab == e_tabs::aim_bot ) {
            switch ( sub_tab_rage ) {
            case 0:
                m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main += 10.f;
                m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra -= 10.f;
                break;

            case 1:
                m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
                m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra += 10.f;
                break;
            default:
                break;
            }
        }
        else if ( tab == e_tabs::anti_aim ) {
            switch ( sub_tab_aa ) {
            case 0:
                m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main += 10.f;
                m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
                break;

            case 1:
                m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
                m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra += 10.f;
                break;
            default:
                break;
            }
        }
        else if ( tab == e_tabs::players ) {
            switch ( sub_tab_players ) {
            case 0:
                m_tabs_data.m_sub_tabs.m_players.m_alpha_main += 10.f;
                m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;
                break;

            case 1:
                m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
                m_tabs_data.m_sub_tabs.m_players.m_alpha_extra += 10.f;
                break;
            default:
                break;
            }
        }
        else if ( tab == e_tabs::visuals ) {
            switch ( sub_tab_visuals ) {
            case 0:
                m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main += 10.f;
                m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
                break;

            case 1:
                m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
                m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra += 10.f;
                break;
            default:
                break;
            }
        }
        else if ( tab == e_tabs::misc ) {
            switch ( sub_tab_misc ) {
            case 0:
                m_tabs_data.m_sub_tabs.m_misc.m_alpha_main += 10.f;
                m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
                break;

            case 1:
                m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
                m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra += 10.f;
                break;
            default:
                break;
            }
        }

        switch ( tab ) {
        case e_tabs::aim_bot:
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::anti_aim:
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::visuals:
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::misc:
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra -= 10.f;

            break;
        case e_tabs::players:
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main -= 10.f;
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra -= 10.f;
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra -= 10.f;

            break;
        default:
            break;
        }

        if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra > 255.f )
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra < 0.f )
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_extra = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra > 255.f )
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra < 0.f )
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_extra = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra > 255.f )
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra < 0.f )
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_extra = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_players.m_alpha_extra > 255.f )
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_players.m_alpha_extra < 0.f )
            m_tabs_data.m_sub_tabs.m_players.m_alpha_extra = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra > 255.f )
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra < 0.f )
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_extra = 0.f;


        if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main > 255.f )
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main < 0.f )
            m_tabs_data.m_sub_tabs.m_anti_aim.m_alpha_main = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_misc.m_alpha_main > 255.f )
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_misc.m_alpha_main < 0.f )
            m_tabs_data.m_sub_tabs.m_misc.m_alpha_main = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main > 255.f )
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main < 0.f )
            m_tabs_data.m_sub_tabs.m_visuals.m_alpha_main = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_players.m_alpha_main > 255.f )
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_players.m_alpha_main < 0.f )
            m_tabs_data.m_sub_tabs.m_players.m_alpha_main = 0.f;

        if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main > 255.f )
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main = 255.f;
        else if ( m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main < 0.f )
            m_tabs_data.m_sub_tabs.m_rage_bot.m_alpha_main = 0.f;


        switch ( tab ) {
        case e_tabs::aim_bot:
            draw_rage_body( draw, sub_tab_rage );
            break;
        case e_tabs::anti_aim:
            draw_antiaim_body( draw, sub_tab_aa );
            break;
        case e_tabs::visuals:
            draw_visuals_body( draw, sub_tab_visuals );
            break;
        case e_tabs::misc:
            draw_misc_body( draw, sub_tab_misc );
            break;
        case e_tabs::players:
            draw_players_body( draw, sub_tab_players );
            break;
        }
    }
    ImGui::End( );

    /*ImGui::Begin( "wok sdk" );

    ImGui::End( );*/
}