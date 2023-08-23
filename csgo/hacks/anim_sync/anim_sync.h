#pragma once

namespace hacks {
	class c_anim_sync {
	public:
		void handle( 
			player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, const sdk::qang_t& angles, const bool custom_angles 
		);

	private:
		void setup_anim_data( 
			player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, const bool custom_angles
		);

		void rotate(
			player_entry_t& entry, lag_record_t* current, lag_anim_data_t* previous, int side, const sdk::qang_t& angles, const bool custom_angles
		);

		void process_air_data(
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous, const int anim_tick
		) const;

		bool setup_bones(
			valve::cs_player_t* const player, valve::bones_t& bones, const float time, int mask
		);

		void find_server_foot_yaw(
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous
		) const;

		void select_anim_side(
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous
		) const;
	};

	inline const auto g_anim_sync = std::make_unique < c_anim_sync >( );
}

#include "impl/anim_sync.inl"