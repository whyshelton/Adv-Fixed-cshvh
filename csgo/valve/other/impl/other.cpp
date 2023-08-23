#include "../csgo/csgo.hpp"

namespace valve {
	void anim_state_t::set_layer_seq( valve::anim_layer_t& layer, std::ptrdiff_t act ) {
		int32_t sequence = select_sequence_from_acitivty_modifier( act );
		if ( sequence < 2 )
			return;

		layer.m_cycle = 0.0f;
		layer.m_weight = 0.0f;
		layer.m_seq = sequence;
		layer.m_playback_rate = g_local_player->self ( )->get_layer_seq_cycle_rate( &layer, sequence );
	}

	bool trace_filter_t::should_hit_entity( base_entity_t* entity, e_mask ) const {
		auto ent_cc = entity->networkable( )->client_class( );
		if ( ent_cc && strcmp( m_ignore_cc, "" ) ) {
			if ( ent_cc->m_network_name == m_ignore_cc )
				return false;
		}

		return !( entity == m_ignore_entity );
	}
}