#include "../../csgo.hpp"

namespace hooks {
	void __fastcall process_movement( std::uintptr_t ecx, std::uintptr_t edx, valve::cs_player_t* player, valve::move_data_t* move_data 
	) {
		move_data->m_game_code_moved_player = false;
		return o_process_movement( ecx, edx, player, move_data );
	}
}