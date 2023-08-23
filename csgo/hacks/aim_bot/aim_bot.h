#pragma once

namespace hacks {
	struct extrapolation_data_t {
		__forceinline constexpr extrapolation_data_t( ) = default;

		__forceinline extrapolation_data_t(
			valve::cs_player_t* const player, const lag_record_t* const lag_record
		) : m_player{ player }, m_sim_time{ lag_record->m_sim_time }, m_flags{ lag_record->m_flags },
			m_was_in_air{ !( lag_record->m_flags & valve::e_ent_flags::on_ground ) }, m_origin{ lag_record->m_origin },
			m_velocity{ lag_record->m_velocity }, m_obb_min{ lag_record->m_obb_min }, m_obb_max{ lag_record->m_obb_max } {}

		valve::cs_player_t* m_player{};

		float				m_sim_time{};

		valve::e_ent_flags	m_flags{};
		bool				m_was_in_air{};

		sdk::vec3_t				m_origin{}, m_velocity{},
			m_obb_min{}, m_obb_max{};
	};

	struct aim_point_t {
		__forceinline constexpr aim_point_t( ) = default;

		__forceinline constexpr aim_point_t(
			const sdk::vec3_t& pos, const bool center,
			const int hitbox, const int hitgroup,
			const int needed_intersections
		) : m_pos{ pos }, m_center{ center },
			m_hitbox{ hitbox }, m_hitgroup{ hitgroup },
			m_needed_intersections{ needed_intersections } {}

		sdk::vec3_t		m_pos{};
		pen_data_t	m_pen_data{};
		bool		m_center{}, m_valid{}, m_safe_head{ };
		int			m_intersections{},
			m_hitbox{}, m_hitgroup{},
			m_head_intersections{},
			m_needed_intersections{};
	};

	struct aim_record_t {
		player_entry_t* m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};
	};

	struct aim_target_t {
		__forceinline constexpr aim_target_t( ) = default;

		__forceinline aim_target_t( const aim_record_t& record )
			: m_entry{ record.m_entry }, m_lag_record{ record.m_lag_record } {}

		player_entry_t* m_entry{};
		std::shared_ptr< lag_record_t >	m_lag_record{};

		sdk::vec3_t m_pos{};

		std::vector< aim_point_t > m_points{};
		aim_point_t* m_best_point{};
		aim_point_t* m_best_body_point{};
	};

	struct aim_last_target_t {
		player_entry_t* m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};

		int								m_hitbox{}, m_dmg{},
			m_hit_chance{};

		aim_point_t						m_point{};
	};

	class c_hitbox {
	public:
		int m_hitbox_id = 0;
		bool m_is_oob;
		sdk::vec3_t m_mins;
		sdk::vec3_t m_maxs;
		sdk::vec3_t m_start_scaled;
		float m_radius;
		valve::studio_bbox_t* m_hitbox;
		int m_bone;
		int m_hitgroup;
	};

	class c_aim_bot {
	private:
		void find_targets( );

		void scan_targets( );

		aim_target_t* select_target( const int cmd_number );

		aim_point_t* select_point( const aim_target_t& target, const int cmd_number ) const;

		std::optional< aim_record_t > select_latest_record( const player_entry_t& entry, const bool skip_is_valid ) const;

		std::optional< aim_record_t > select_record( player_entry_t& entry ) const;

		std::size_t calc_points_count( const int hitgroups, const int multi_points ) const;

		void calc_multi_points(
			aim_target_t& target, const valve::studio_bbox_t* const hitbox,
			const int index, const sdk::mat3x4_t& matrix, float scale
		) const;

		std::optional< aim_record_t > extrapolate( const player_entry_t& entry ) const;

		void player_move( extrapolation_data_t& data ) const;

		void get_hitbox_data( c_hitbox* rtn, valve::cs_player_t* ent, int ihitbox, valve::bones_t& matrix );

		void calc_capsule_points(
			aim_target_t& target, const valve::studio_bbox_t* const hitbox,
			const int index, const sdk::mat3x4_t& matrix, float scale
		) const;

		float calc_point_scale(
			const float spread, const float max,
			const float dist, const sdk::vec3_t& dir,
			const sdk::vec3_t& right, const sdk::vec3_t& up
		) const;

		sdk::vec2_t calc_spread_angle(
			const int bullets, const valve::e_item_index item_index,
			const float recoil_index, const std::size_t i
		) const;

		int calc_hit_chance(
			const aim_target_t* const target, const sdk::qang_t& angle, const aim_point_t* const aim_point
		);

		sdk::vec3_t m_shoot_pos{};

		std::vector< aim_target_t > m_targets{};

		bool m_shooting{};

		std::array< std::vector< int >, 6u > m_hitgroups{
			std::vector< int >{ 0 },
			{ 2, 4, 5, 6 },
			{ 3 },
			{ 13, 14, 15, 16, 17, 18 },
			{ 7, 8, 9, 10 },
			{ 11, 12 }
		};

		int m_stop_type{};

	public:
		void handle( valve::user_cmd_t& user_cmd, bool& send_packet );

		void scan_points(
			aim_target_t& target, const int hitgroups, const int multi_points, const bool trace, sdk::vec3_t shoot_pos = g_local_player->shoot_pos( )
		) const;

		void scan_point( const aim_target_t& target, aim_point_t& point, const bool ignore_dmg, sdk::vec3_t shoot_pos = g_local_player->shoot_pos( ) ) const;

		void scan_center_points( aim_target_t& target, const int hitgroups, sdk::vec3_t shoot_pos ) const;

		bool select_points( aim_target_t& target, bool additional_scan ) const;

		ALWAYS_INLINE int& stop_type( );

		ALWAYS_INLINE sdk::vec3_t& shoot_pos( );

		ALWAYS_INLINE bool& shooting( );

		aim_last_target_t m_last_target{};
	};

	class c_knife_bot
	{
	public:
		void handle_knife_bot( valve::user_cmd_t& cmd );
		bool select_target( );

	protected:

		__forceinline int get_min_dmg( ) {
			if ( !g_local_player->self( ) || !g_local_player->self( )->weapon( ) )
				return -1;

			if ( valve::to_time( g_local_player->self( )->tick_base( ) ) > ( g_local_player->self( )->weapon( )->next_primary_attack( ) + 0.4f ) )
				return 34;

			return 21;
		}

		__forceinline sdk::vec3_t get_hitbox_pos( int hitbox_id, valve::cs_player_t* player ) {
			if ( !this )
				return sdk::vec3_t( );

			auto hdr = player->mdl_data( )->m_studio_hdr;

			if ( !hdr )
				return sdk::vec3_t( );

			auto hitbox_set = hdr->get_hitbox_set( player->hitbox_set_index( ) );

			if ( !hitbox_set )
				return sdk::vec3_t( );

			auto hitbox = hitbox_set->get_bbox( hitbox_id );

			if ( !hitbox )
				return sdk::vec3_t( );

			sdk::vec3_t min, max;

			min = hitbox->m_mins.transform( player->bone_cache( ).m_elements[ hitbox->m_bone ] );
			max = hitbox->m_maxs.transform( player->bone_cache( ).m_elements[ hitbox->m_bone ] );

			return ( min + max ) * 0.5f;
		}


		__forceinline bool is_visible( const sdk::vec3_t& start, const sdk::vec3_t& end, valve::cs_player_t* player, valve::cs_player_t* local ) {
			valve::trace_t trace;

			valve::ray_t ray{ start, end };

			valve::trace_filter_simple_t filter{ player, 0 };
			filter.m_ignore_entity = local;

			valve::g_engine_trace->trace_ray( ray, valve::e_mask::shot_player, reinterpret_cast< valve::trace_filter_t* >( &filter ), &trace );

			return trace.m_entity == player || trace.m_frac == 1.0f;
		}

		valve::cs_player_t* m_best_player;
		int m_best_distance;
		int m_best_index;
	};

	inline const auto g_knife_bot = std::make_unique < c_knife_bot >( );
	inline const auto g_aim_bot = std::make_unique< c_aim_bot >( );
}

#include "impl/aim_bot.inl"