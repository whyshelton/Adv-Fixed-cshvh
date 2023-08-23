#pragma once

#include "../entities.hpp"

namespace valve {
    ALWAYS_INLINE weapon_info_t* base_weapon_t::info( ) {
        using fn_t = weapon_info_t*( __thiscall* )( std::uintptr_t, e_item_index );

        const auto weapon_system = g_ctx->offsets( ).m_weapon_system;

        return ( *weapon_system.as< fn_t** >( ) )[ 2u ]( weapon_system, item_index( ) );
    }

    ALWAYS_INLINE bool base_player_t::alive( ) {
        return life_state( ) == e_life_state::alive && health( ) > 0;
    }

    ALWAYS_INLINE std::optional< player_info_t > base_player_t::info( ) {
        player_info_t info{};
        if ( !g_engine.get( )->get_player_info( networkable( )->index( ), &info ) )
            return std::nullopt;

        return info;
    }

    ALWAYS_INLINE std::uintptr_t cs_player_t::player_resource( ) {
        for ( std::ptrdiff_t i{ 1u }; i < valve::g_entity_list.get( )->highest_index( ); ++i ) {
            const auto ent = valve::g_entity_list.get( )->get_entity( i );

            if ( !ent )
                continue;

            auto cl_class = ent->networkable( )->client_class( );

            if ( !cl_class )
                continue;

            if ( cl_class->m_class_id == 42 ) {
                return reinterpret_cast < std::uintptr_t >( ent );
            }
        }

        return {};
    }

    ALWAYS_INLINE bool cs_player_t::friendly( cs_player_t* const with ) {
        if ( with == this )
            return true;

#ifndef CSGO2018
        if ( g_game_types->game_type( ) == e_game_type::ffa )
            return survival_team( ) == with->survival_team( );
#endif

        if ( g_ctx->cvars( ).mp_teammates_are_enemies->get_int( ) )
            return false;

        return team( ) == with->team( );
    }
}