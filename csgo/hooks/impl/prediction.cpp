#include "../../csgo.hpp"

namespace hooks {
	bool __stdcall in_prediction( ) {
		if ( g_ctx->allow_setup_bones( ) && *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->offsets( ).m_addresses.m_ret_to_maintain_seq_transitions )
			return true;

		return o_in_prediction( valve::g_prediction.get( ) );
	}
}