#include "../../csgo.hpp"

namespace hooks {
    void __fastcall get_color_modulation( void* ecx, void* edx, float* red, float* green, float* blue ) {
        auto material = ( valve::material_t* ) ( ecx );

        if ( !material || material->is_error_mat( ) )
            return o_get_clr_modulation( ecx, edx, red, green, blue );

        const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

        o_get_clr_modulation( ecx, edx, red, green, blue );

        sdk::col_t world_clr = sdk::col_t( cfg.m_world_modulation[ 0 ] * 255.f,
            cfg.m_world_modulation[ 1 ] * 255.f, cfg.m_world_modulation[ 2 ] * 255.f,
            cfg.m_world_modulation[ 3 ] * 255.f );

        sdk::col_t props_clr = sdk::col_t( cfg.m_props_modulation[ 0 ] * 255.f,
            cfg.m_props_modulation[ 1 ] * 255.f, cfg.m_props_modulation[ 2 ] * 255.f,
            cfg.m_props_modulation[ 3 ] * 255.f );

        sdk::col_t sky_clr = sdk::col_t( cfg.m_sky_modulation[ 0 ] * 255.f,
            cfg.m_sky_modulation[ 1 ] * 255.f, cfg.m_sky_modulation[ 2 ] * 255.f,
            cfg.m_sky_modulation[ 3 ] * 255.f );

        if ( strstr( material->texture_group_name( ), ( "World textures" ) ) )
        {
            *red = world_clr.r( ) / 255.f;
            *green = world_clr.g( ) / 255.f;
            *blue = world_clr.b( ) / 255.f;
        }
        else if ( strstr( material->texture_group_name( ), ( "StaticProp textures" ) ) )
        {
            *red = props_clr.r( ) / 255.f;
            *green = props_clr.g( ) / 255.f;
            *blue = props_clr.b( ) / 255.f;
        }
        else if ( strstr( material->texture_group_name( ), ( "SkyBox textures" ) ) )
        {
            *red = sky_clr.r( ) / 255.f;
            *green = sky_clr.g( ) / 255.f;
            *blue = sky_clr.b( ) / 255.f;
        }
    }

    float __fastcall get_alpha_modulation( void* ecx, void* edx ) {
        auto material = ( valve::material_t* ) ( ecx );
        if ( !material || material->is_error_mat( ) )
            return o_get_alpha_modulation( ecx, edx );

        const auto& cfg = g_menu->main( ).m_visuals_cfg.get( );

        sdk::col_t world_clr = sdk::col_t( cfg.m_world_modulation[ 0 ] * 255.f,
            cfg.m_world_modulation[ 1 ] * 255.f, cfg.m_world_modulation[ 2 ] * 255.f,
            cfg.m_world_modulation[ 3 ] * 255.f );

        sdk::col_t props_clr = sdk::col_t( cfg.m_props_modulation[ 0 ] * 255.f,
            cfg.m_props_modulation[ 1 ] * 255.f, cfg.m_props_modulation[ 2 ] * 255.f,
            cfg.m_props_modulation[ 3 ] * 255.f );

        sdk::col_t sky_clr = sdk::col_t( cfg.m_sky_modulation[ 0 ] * 255.f,
            cfg.m_sky_modulation[ 1 ] * 255.f, cfg.m_sky_modulation[ 2 ] * 255.f,
            cfg.m_sky_modulation[ 3 ] * 255.f );

        if ( strstr( material->texture_group_name( ), ( "World textures" ) ) )
            return world_clr.a( ) / 255.f;
        else if ( strstr( material->texture_group_name( ), ( "StaticProp textures" ) ) )
            return props_clr.a( ) / 255.f;
        else if ( strstr( material->texture_group_name( ), ( "SkyBox textures" ) ) )
            return sky_clr.a( ) / 255.f;

        return o_get_alpha_modulation( ecx, edx );
    }
}