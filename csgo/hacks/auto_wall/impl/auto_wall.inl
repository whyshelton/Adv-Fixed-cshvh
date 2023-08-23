#pragma once

#include "../auto_wall.h"

namespace hacks {
	ALWAYS_INLINE bool c_auto_wall::breakable( valve::base_entity_t* const entity ) {
		if ( !entity )
			return false;

		using fn_t = bool( __thiscall* )( decltype( entity ) );

		if ( reinterpret_cast< fn_t >( g_ctx->offsets( ).m_addresses.m_is_breakable )( entity ) )
			return true;

		const auto cc = entity->networkable( )->client_class( );
		if ( !cc )
			return false;

		return ( *reinterpret_cast< const std::uint32_t* >( cc->m_network_name ) == 'erBC'
			&& *reinterpret_cast< const std::uint32_t* >( cc->m_network_name + 7 ) == 'Selb' )
			|| ( *reinterpret_cast< const std::uint32_t* >( cc->m_network_name ) == 'saBC'
				&& *reinterpret_cast< const std::uint32_t* >( cc->m_network_name + 7 ) == 'ytit' );
	}
}