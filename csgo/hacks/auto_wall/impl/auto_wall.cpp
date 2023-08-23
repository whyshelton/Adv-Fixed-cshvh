#include "../../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"
namespace hacks {
	bool c_auto_wall::trace_to_exit(
		const sdk::vec3_t& src, const sdk::vec3_t& dir,
		const valve::trace_t& enter_trace, valve::trace_t& exit_trace
	) const
	{
		float dist{};
		valve::e_mask first_contents{};

		constexpr auto k_step_size = 4.f;
		constexpr auto k_max_dist = 90.f;

		while ( dist <= k_max_dist ) {
			dist += k_step_size;

			const auto out = src + ( dir * dist );

			const auto cur_contents = valve::g_engine_trace->get_point_contents( out, valve::e_mask::shot_player );

			if ( !( ( int ) ( first_contents ) ) )
				first_contents = cur_contents;

			if ( cur_contents & valve::e_mask::shot_hull
				&& ( !( ( int )cur_contents & 0x40000000u ) || cur_contents == first_contents ) )
				continue;

			valve::g_engine_trace->trace_ray( { out, out - dir * k_step_size }, valve::e_mask::shot_player, nullptr, &exit_trace );

			if ( exit_trace.m_start_solid
				&& exit_trace.m_surface.m_flags & 0x8000u ) {
				valve::trace_filter_simple_t trace_filter{ exit_trace.m_entity, 0 };

				valve::g_engine_trace->trace_ray(
					{ out, src }, valve::e_mask::shot_hull,
					reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &exit_trace
				);

				if ( exit_trace.hit( )
					&& !exit_trace.m_start_solid )
					return true;

				continue;
			}

			if ( !exit_trace.hit( )
				|| exit_trace.m_start_solid ) {
				if ( enter_trace.m_entity
					&& enter_trace.m_entity->networkable( )->index( )
					&& g_auto_wall->breakable ( enter_trace.m_entity ) ) {
					exit_trace = enter_trace;
					exit_trace.m_end = src + dir;

					return true;
				}

				continue;
			}

			if ( exit_trace.m_surface.m_flags & 0x0080u ) {
				if ( g_auto_wall->breakable ( exit_trace.m_entity )
					&& g_auto_wall->breakable ( enter_trace.m_entity ) )
					return true;

				if ( !( enter_trace.m_surface.m_flags & 0x0080u ) )
					continue;
			}

			if ( exit_trace.m_plane.m_normal.dot( dir ) <= 1.f )
				return true;
		}

		return false;
	}

	void c_auto_wall::clip_trace_to_player(
		const sdk::vec3_t& src, const sdk::vec3_t& dst, valve::trace_t& trace,
		valve::cs_player_t* const player, const valve::should_hit_fn_t& should_hit_fn
	) const
	{
		if ( should_hit_fn
			&& !should_hit_fn( player, valve::e_mask::shot_player ) )
			return;

		const auto pos = player->origin( ) + ( player->obb_min( ) + player->obb_max( ) ) * 0.5f;
		const auto to = pos - src;

		auto dir = src - dst;
		const auto len = dir.normalize( );
		const auto range_along = dir.dot( to );

		const auto range =
			range_along < 0.f ? -( to ).length( )
			: range_along > len ? -( pos - dst ).length( )
			: ( pos - ( src + dir * range_along ) ).length( );

		if ( range > 60.f || range < 0.f )
			return;

		valve::trace_t new_trace{};

		valve::g_engine_trace->clip_ray_to_entity( { src, dst }, valve::e_mask::shot_player, player, &new_trace );

		if ( new_trace.m_frac >= trace.m_frac )
			return;

		trace = new_trace;
	}

	bool c_auto_wall::handle_bullet_penetration(
		valve::cs_player_t* const shooter, const valve::weapon_info_t* const wpn_data,
		const valve::trace_t& enter_trace, sdk::vec3_t& src, const sdk::vec3_t& dir, int& pen_count,
		float& cur_dmg, const float pen_modifier
	) const
	{
		if ( pen_count <= 0
			|| wpn_data->m_penetration <= 0.f )
			return false;

		valve::trace_t exit_trace{};

		if ( !trace_to_exit( enter_trace.m_end, dir, enter_trace, exit_trace )
			&& !( valve::g_engine_trace->get_point_contents( enter_trace.m_end, valve::e_mask::shot_hull ) & valve::e_mask::shot_hull ) )
			return false;

		auto final_dmg_modifier = 0.16f;
		float combined_pen_modifier{};

		static auto ff_dmg_reduction_bullets = valve::g_cvar->find_var( ( "ff_damage_reduction_bullets" ) );
		static auto ff_dmg_bullet_pen = valve::g_cvar->find_var( ( "ff_damage_bullet_penetration" ) );

		const auto exit_surface_data = valve::g_surface_data->get( exit_trace.m_surface.m_surface_props );
		const auto enter_surface_data = valve::g_surface_data->get( enter_trace.m_surface.m_surface_props );

		if ( enter_surface_data->m_game.m_material == 'G'
			|| enter_surface_data->m_game.m_material == 'Y' ) {
			final_dmg_modifier = 0.05f;
			combined_pen_modifier = 3.f;
		}
		else if ( enter_trace.m_contents & 0x8u
			|| enter_trace.m_surface.m_flags & 0x0080u ) {
			final_dmg_modifier = 0.16f;
			combined_pen_modifier = 1.f;
		}
		else if ( enter_trace.m_entity
			&& ff_dmg_reduction_bullets->get_float( ) == 0.f
			&& enter_surface_data->m_game.m_material == 'F'
			&& enter_trace.m_entity->team( ) == shooter->team( ) ) {
			const auto dmg_bullet_pen = ff_dmg_bullet_pen->get_float( );
			if ( dmg_bullet_pen == 0.f )
				return false;

			combined_pen_modifier = dmg_bullet_pen;
			final_dmg_modifier = 0.16f;
		}
		else {
			combined_pen_modifier = (
				enter_surface_data->m_game.m_pen_modifier
				+ exit_surface_data->m_game.m_pen_modifier
				) * 0.5f;

			final_dmg_modifier = 0.16f;
		}

		if ( enter_surface_data->m_game.m_material == exit_surface_data->m_game.m_material ) {
			if ( exit_surface_data->m_game.m_material == 'U'
				|| exit_surface_data->m_game.m_material == 'W' )
				combined_pen_modifier = 3.f;
			else if ( exit_surface_data->m_game.m_material == 'L' )
				combined_pen_modifier = 2.f;
		}

		const auto modifier = std::max( 1.f / combined_pen_modifier, 0.f );
		const auto pen_dist = ( exit_trace.m_end - enter_trace.m_end ).length( );

		const auto lost_dmg =
			cur_dmg * final_dmg_modifier
			+ pen_modifier * ( modifier * 3.f )
			+ ( ( pen_dist * pen_dist ) * modifier ) / 24.f;

		if ( lost_dmg > cur_dmg )
			return false;

		if ( lost_dmg > 0.f )
			cur_dmg -= lost_dmg;

		if ( cur_dmg < 1.f )
			return false;

		--pen_count;

		src = exit_trace.m_end;

		return true;
	}

	bool is_armored( valve::cs_player_t* player, int hit_group ) {
		const bool has_helmet = player->has_helmet( );
		const bool has_heavy_armor = player->has_heavy_armor( );
		const float armor_val = player->armor_val( );

		if ( armor_val > 0.f ) {
			switch ( hit_group ) {
			case 2:
			case 3:
			case 4:
			case 5:
				return true;
				break;
			case 1:
				return has_helmet || has_heavy_armor;
				break;
			default:
				return has_heavy_armor;
				break;
			}
		}

		return false;
	}

	void c_auto_wall::scale_dmg(
		valve::cs_player_t* const player, float& dmg,
		const float armor_ratio, const float headshot_mult, const int hitgroup
	) const
	{
		const auto team = player->team( );

		static auto mp_dmg_scale_ct_head = valve::g_cvar->find_var( MilfStr( "mp_damage_scale_ct_head" ) );
		static auto mp_dmg_scale_t_head = valve::g_cvar->find_var( MilfStr( "mp_damage_scale_t_head" ) );

		static auto mp_dmg_scale_ct_body = valve::g_cvar->find_var( MilfStr( "mp_damage_scale_ct_body" ) );
		static auto mp_dmg_scale_t_body = valve::g_cvar->find_var( MilfStr( "mp_damage_scale_t_body" ) );

		float head_dmg_scale = team == valve::e_team::ct ? mp_dmg_scale_ct_head->get_float( ) : mp_dmg_scale_t_head->get_float( );
		const float body_dmg_scale = team == valve::e_team::ct ? mp_dmg_scale_ct_body->get_float( ) : mp_dmg_scale_t_body->get_float( );

		const auto armored = is_armored( player, hitgroup );
		const bool has_heavy_armor = player->has_heavy_armor( );
		const bool is_zeus = g_local_player->weapon( ) ? g_local_player->weapon( )->item_index( ) == valve::e_item_index::taser : false;

		const auto armor_val = static_cast < float > ( player->armor_val( ) );

		if ( has_heavy_armor )
			head_dmg_scale *= 0.5f;

		if ( !is_zeus ) {
			switch ( hitgroup ) {
			case 1:
				dmg = ( dmg * 4.f ) * head_dmg_scale;
				break;
			case 3:
				dmg = ( dmg * 1.25f ) * body_dmg_scale;
				break;
			case 6:
			case 7:
				dmg = ( dmg * 0.75f ) * body_dmg_scale;
				break;
			default:
				break;
			}
		}

		if ( armored ) {
			float armor_scale = 1.f;
			float armor_bonus_ratio = 0.5f;
			float armor_ratio_calced = armor_ratio * 0.5f;
			float dmg_to_health = 0.f;

			if ( has_heavy_armor ) {
				armor_ratio_calced = armor_ratio * 0.25f;
				armor_bonus_ratio = 0.33f;
				armor_scale = 0.33f;

				dmg_to_health = ( dmg * armor_ratio_calced ) * 0.85f;
			}
			else
				dmg_to_health = dmg * armor_ratio_calced;

			float dmg_to_armor = ( dmg - dmg_to_health ) * ( armor_scale * armor_bonus_ratio );

			if ( dmg_to_armor > armor_val )
				dmg_to_health = dmg - ( armor_val / armor_bonus_ratio );

			dmg = dmg_to_health;
		}

		dmg = std::floor( dmg );
	}

	pen_data_t c_auto_wall::fire_bullet(
		valve::cs_player_t* const shooter, valve::cs_player_t* const target,
		const valve::weapon_info_t* const wpn_data, const bool is_taser, sdk::vec3_t src, const sdk::vec3_t& dst
	) const
	{
		const auto pen_modifier = std::max( ( 3.f / wpn_data->m_penetration ) * 1.25f, 0.f );

		float cur_dist{};

		pen_data_t data{};

		data.m_remaining_pen = 4;

		auto cur_dmg = static_cast< float >( wpn_data->m_dmg );

		auto dir = dst - src;

		dir.normalize( );

		valve::trace_t trace{};
		valve::trace_filter_skip_two_entities_t trace_filter{};

		valve::cs_player_t* last_hit_player{};

		auto max_dist = wpn_data->m_range;

		while ( cur_dmg > 0.f ) {
			max_dist -= cur_dist;

			const auto cur_dst = src + dir * max_dist;

			trace_filter.m_ignore_entity0 = shooter;
			trace_filter.m_ignore_entity1 = last_hit_player;

			valve::g_engine_trace->trace_ray(
				{ src, cur_dst }, valve::e_mask::shot_player,
				reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
			);

			if ( target )
				clip_trace_to_player( src, cur_dst + dir * 40.f, trace, target, trace_filter.m_should_hit_fn );

			if ( trace.m_frac == 1.f )
				break;

			cur_dist += trace.m_frac * max_dist;
			cur_dmg *= std::pow( wpn_data->m_range_modifier, cur_dist / 500.f );

			if ( trace.m_entity ) {
				const auto is_player = trace.m_entity->is_player( );
				if ( ( trace.m_entity == target || ( is_player && trace.m_entity->team( ) != shooter->team( ) ) )
					&& ( ( ( int )trace.m_hitgroup >= 1 && ( int )trace.m_hitgroup <= 7 ) || ( int )trace.m_hitgroup == 10 ) ) {
					data.m_hit_player = static_cast< valve::cs_player_t* >( trace.m_entity );
					data.m_hitbox = ( int )trace.m_hitbox;
					data.m_hitgroup = ( int )trace.m_hitgroup;

					if ( is_taser )
						data.m_hitgroup = 0;

					scale_dmg( data.m_hit_player, cur_dmg, wpn_data->m_armor_ratio, wpn_data->m_headshot_multiplier, data.m_hitgroup );

					data.m_dmg = static_cast< int >( cur_dmg );

					return data;
				}

				last_hit_player =
					is_player ? static_cast< valve::cs_player_t* >( trace.m_entity ) : nullptr;
			}
			else
				last_hit_player = nullptr;

			if ( is_taser
				|| ( cur_dist > 3000.f && wpn_data->m_penetration > 0.f ) )
				break;

			const auto enter_surface = valve::g_surface_data->get( trace.m_surface.m_surface_props );
			if ( enter_surface->m_game.m_pen_modifier < 0.1f
				|| !handle_bullet_penetration( shooter, wpn_data, trace, src, dir, data.m_remaining_pen, cur_dmg, pen_modifier ) )
				break;
		}

		return data;
	}

	pen_data_t c_auto_wall::fire_emulated(
		valve::cs_player_t* const shooter, valve::cs_player_t* const target, sdk::vec3_t src, const sdk::vec3_t& dst
	) const
	{
		static const auto wpn_data = [ ]( ) {
			valve::weapon_info_t wpn_data{};

			wpn_data.m_dmg = 115;
			wpn_data.m_range = 8192.f;
			wpn_data.m_penetration = 2.5f;
			wpn_data.m_range_modifier = 0.99f;
			wpn_data.m_armor_ratio = 1.95f;

			return wpn_data;
		}( );

		const auto pen_modifier = std::max( ( 3.f / wpn_data.m_penetration ) * 1.25f, 0.f );

		float cur_dist{};

		pen_data_t data{};

		data.m_remaining_pen = 4;

		auto cur_dmg = static_cast< float >( wpn_data.m_dmg );

		auto dir = dst - src;

		const auto max_dist = dir.normalize( );

		valve::trace_t trace{};
		valve::trace_filter_skip_two_entities_t trace_filter{};

		valve::cs_player_t* last_hit_player{};

		while ( cur_dmg > 0.f ) {
			const auto dist_remaining = wpn_data.m_range - cur_dist;

			const auto cur_dst = src + dir * dist_remaining;

			trace_filter.m_ignore_entity0 = shooter;
			trace_filter.m_ignore_entity1 = last_hit_player;

			valve::g_engine_trace->trace_ray(
				{ src, cur_dst }, valve::e_mask::shot_player,
				reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
			);

			if ( trace.m_frac == 1.f
				|| ( trace.m_end - src ).length( ) > max_dist )
				break;

			cur_dist += trace.m_frac * dist_remaining;
			cur_dmg *= std::pow( wpn_data.m_range_modifier, cur_dist / 500.f );

			if ( trace.m_entity ) {
				const auto is_player = trace.m_entity->is_player( );
				if ( trace.m_entity == target ) {
					data.m_hit_player = static_cast< valve::cs_player_t* >( trace.m_entity );
					data.m_hitbox = ( int )trace.m_hitbox;
					data.m_hitgroup = ( int )trace.m_hitgroup;
					data.m_dmg = static_cast< int >( cur_dmg );

					return data;
				}

				last_hit_player =
					is_player ? static_cast< valve::cs_player_t* >( trace.m_entity ) : nullptr;
			}
			else
				last_hit_player = nullptr;

			if ( cur_dist > 3000.f
				&& wpn_data.m_penetration > 0.f )
				break;

			const auto enter_surface = valve::g_surface_data->get( trace.m_surface.m_surface_props );
			if ( enter_surface->m_game.m_pen_modifier < 0.1f
				|| !handle_bullet_penetration( shooter, &wpn_data, trace, src, dir, data.m_remaining_pen, cur_dmg, pen_modifier ) )
				break;
		}

		return data;
	}
}