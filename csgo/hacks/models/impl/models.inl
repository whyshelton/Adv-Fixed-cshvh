#pragma once

#include "../models.hpp"

namespace hacks {
    ALWAYS_INLINE valve::material_t* c_models::create_material( const char* name, const char* shader, const char* settings ) const {
        const auto key_values = reinterpret_cast< valve::key_values_t* >(
            valve::g_mem_alloc->alloc( sizeof( valve::key_values_t ) )
        );

        key_values->init( shader );
        key_values->load_from_buffer( name, settings );

        return valve::g_material_system->create_material( name, key_values );
    }

    ALWAYS_INLINE void c_models::create_materials( ) {
        m_flat = create_material(
            "wok_flat.vmt", "UnlitGeneric",
            R"#("UnlitGeneric" {
                    "$basetexture"      "vgui/white_additive"
	                "$ignorez"          "0"
	                "$model"		    "1"
	                "$flat"			    "1"
	                "$nocull"		    "1"
	                "$selfillum"	    "1"
	                "$halflambert"      "1"
	                "$nofog"		    "1"
	                "$wireframe"	    "0"
                    "$znearer"          "0"
            })#"
        );

        m_regular = create_material( ( "wok_regular.vmt" ), ( "VertexLitGeneric" ), ( R"#("VertexLitGeneric" {
	            "$basetexture" "vgui/white_additive"
	            "$ignorez"      "0"
	            "$model"		"1"
	            "$flat"			"0"
	            "$nocull"		"1"
	            "$halflambert"	"1"
	            "$nofog"		"1"
	            "$wireframe"	"0"
            })#") 
        );

        m_metallic = create_material( ( "advance_metallic.vmt" ), ( "VertexLitGeneric" ), ( R"#("VertexLitGeneric" {
    "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
})#" ) );

        m_glow = create_material( ( "advance_glow.vmt" ), ( "VertexLitGeneric" ), ( R"#("VertexLitGeneric" {
          "$additive" "1"
          "$envmap" "models/effects/cube_white"
          "$envmaptint" "[1 1 1]"
          "$envmapfresnel" "1"
          "$envmapfresnelminmaxexp" "[0 1 2]"
})#" ) );

        m_glow_overlay = valve::g_material_system->find_mat( ( "dev/glow_armsrace" ), nullptr );

    }

    ALWAYS_INLINE void c_models::set_clr( const sdk::col_t& clr, int type, bool ignore_z ) const {
        valve::material_t* material{};

        switch ( type ) {
        case 0:
            material = m_flat;
            break;
        case 1:
            material = m_regular;
            break;
        case 2:
            material = m_metallic;
            break;
        case 3:
            material = m_glow;
            break;
        case 4:
            material = m_glow_overlay;
            break;
        default:
            break;
        }

        if ( !material )
            return;

        material->set_alpha( clr.a( ) / 255.f );
        material->set_clr( clr.r( ) / 255.f, clr.g( ) / 255.f, clr.b( ) / 255.f );
        material->set_flag( 1u << 15u, ignore_z );

        if ( const auto $envmaptint = material->find_var( ( "$envmaptint" ), nullptr, false ) )
            $envmaptint->set_vec( clr.r( ) / 255.f, clr.g( ) / 255.f, clr.b( ) / 255.f );

        valve::g_studio_render->forced_material_override( material );
    }

    ALWAYS_INLINE c_models::e_model_type c_models::get_model_type( const valve::draw_model_info_t& info ) const {
        if ( !info.m_studio
            || !info.m_renderable )
            return e_model_type::invalid;

        const auto name = info.m_studio->m_name;
        if ( !name )
            return e_model_type::invalid;

        if ( *reinterpret_cast< std::uint32_t* >( name + 14 ) == 'yalp' )
            return e_model_type::player;

        return e_model_type::invalid;
    }
}