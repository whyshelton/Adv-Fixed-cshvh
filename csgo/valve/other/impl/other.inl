#pragma once

namespace valve {
    ALWAYS_INLINE void user_cmd_t::sanitize( ) {
        m_view_angles.normalize( );

        const auto& cvars = g_ctx->cvars( );

        m_view_angles.x( ) = std::clamp( m_view_angles.x( ), -cvars.cl_pitchup->get_float( ), cvars.cl_pitchdown->get_float( ) );
        m_view_angles.z( ) = std::clamp( m_view_angles.z( ), -k_max_roll, k_max_roll );

        const auto max_fwd_speed = cvars.cl_forwardspeed->get_float( );
        const auto max_back_speed = cvars.cl_backspeed->get_float( );

        const auto max_side_speed = cvars.cl_sidespeed->get_float( );
        const auto max_up_speed = cvars.cl_upspeed->get_float( );

        m_move = {
            std::clamp( m_move.x( ), -max_back_speed, max_fwd_speed ),
            std::clamp( m_move.y( ), -max_side_speed, max_side_speed ),
            std::clamp( m_move.z( ), -max_up_speed, max_up_speed )
        };
    }

    ALWAYS_INLINE bool trace_t::hit( ) const {
        return m_frac < 1.f || m_all_solid || m_start_solid;
    }

    ALWAYS_INLINE const char* studio_hitbox_set_t::name( ) const {
        return !m_name_index ? nullptr
            : sdk::address_t{ this }.self_offset( m_name_index ).as< const char* >( );
    }

    ALWAYS_INLINE studio_bbox_t* studio_hitbox_set_t::get_bbox( const int i ) const {
        return i > m_hitboxes_count ? nullptr
            : sdk::address_t{ this }.self_offset( m_hitbox_index ).as< studio_bbox_t* >( ) + i;
    }

    ALWAYS_INLINE studio_bone_t* studio_hdr_t::get_bone( const int i ) const {
        return i > m_bones_count ? nullptr
            : sdk::address_t{ this }.self_offset( m_bone_index ).as< studio_bone_t* >( ) + i;
    }

    ALWAYS_INLINE studio_hitbox_set_t* studio_hdr_t::get_hitbox_set( const int i ) const {
        return i > m_hitbox_sets_count ? nullptr
            : sdk::address_t{ this }.self_offset( m_hitbox_set_index ).as< studio_hitbox_set_t* >( ) + i;
    }

	ALWAYS_INLINE std::ptrdiff_t anim_state_t::select_sequence_from_acitivty_modifier( std::ptrdiff_t activity ) {
		bool is_player_ducked = m_duck_amount > 0.55f;
		bool is_player_running = m_speed_as_portion_of_walk_speed > 0.25f;

		int32_t cur_sequence = -1;
		switch ( activity )
		{
		case 985:
		{
			cur_sequence = 15 + static_cast < int32_t >( is_player_running );
			if ( is_player_ducked )
				cur_sequence = 17 + static_cast < int32_t >( is_player_running );
		}
		break;

		case 981:
		{
			cur_sequence = 8;
			if ( m_last_weapon != m_weapon )
				cur_sequence = 9;
		}
		break;

		case 980:
		{
			cur_sequence = 6;
		}
		break;

		case 986:
		{
			cur_sequence = 14;
		}
		break;


		case 988:
		{
			cur_sequence = 20;
			if ( is_player_running )
				cur_sequence = 22;

			if ( is_player_ducked )
			{
				cur_sequence = 21;
				if ( is_player_running )
					cur_sequence = 19;
			}
		}
		break;

		case 989:
		{
			cur_sequence = 23;
			if ( is_player_ducked )
				cur_sequence = 24;
		}
		break;

		case 987:
		{
			cur_sequence = 13;
		}
		break;
		default: break;
		}

		return cur_sequence;
	}

	ALWAYS_INLINE unsigned int find_in_datamap( valve::data_map_t* map, const char* name ) {
		while ( map )
		{
			for ( auto i = 0; i < map->m_size; ++i )
			{
				if ( !map->m_descriptions[ i ].m_name )
					continue;

				if ( !strcmp( name, map->m_descriptions[ i ].m_name ) )
					return map->m_descriptions[ i ].m_flat_offset[ 0 ];

				if ( map->m_descriptions[ i ].m_type == 10 )
				{
					if ( map->m_descriptions[ i ].m_data_map )
					{
						unsigned int offset;

						if ( offset = find_in_datamap( map->m_descriptions[ i ].m_data_map, name ) )
							return offset;
					}
				}
			}

			map = map->m_base_map;
		}

		return 0;
	}
}