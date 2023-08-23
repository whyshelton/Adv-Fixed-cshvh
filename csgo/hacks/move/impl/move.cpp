#include "../../../csgo.hpp"

namespace hacks {
	ALWAYS_INLINE bool pred_auto_stop( ) {
		if ( !g_local_player->self( ) || !g_local_player->self( )->alive( ) )
			return false;

		auto wpn = g_local_player->self( )->weapon( );

		if ( !wpn )
			return false;

		const auto& cfg = g_menu->main( ).m_aim_bot_cfg.get( );

		switch ( wpn->item_index( ) ) {
		case valve::e_item_index::awp:
			return cfg.m_early_stop_awp;
		case valve::e_item_index::ssg08:
			return cfg.m_early_stop_scout;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return cfg.m_early_stop_scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a4:
		case valve::e_item_index::m4a1s:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawed_off:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return cfg.m_early_stop_other;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return cfg.m_early_stop_heavy_pistol;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::five_seven:
		case valve::e_item_index::p2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usps:
			return cfg.m_early_stop_pistol;
		default:
			return false;
		}

		return false;
	}

	void c_move::correct_mouse( valve::user_cmd_t& user_cmd ) {
		sdk::qang_t prev_view_angles{};
		prev_view_angles = valve::g_engine.get( )->view_angles( );

		auto pitch = valve::g_cvar->find_var( MilfStr( "m_pitch" ) );
		auto sensitivity = valve::g_cvar->find_var( MilfStr( "sensitivity" ) );
		auto yaw = valve::g_cvar->find_var( MilfStr( "m_yaw" ) );

		float delta_x = std::remainderf( user_cmd.m_view_angles.x( ) - prev_view_angles.x( ), 360.0f );
		float delta_y = std::remainderf( user_cmd.m_view_angles.y( ) - prev_view_angles.y( ), 360.0f );

		if ( delta_x != 0.0f ) {
			float mouse_y = -( ( delta_x / pitch->get_float( ) ) / sensitivity->get_float( ) );
			short mousedy;
			if ( mouse_y <= 32767.0f ) {
				if ( mouse_y >= -32768.0f ) {
					if ( mouse_y >= 1.0f || mouse_y < 0.0f ) {
						if ( mouse_y <= -1.0f || mouse_y > 0.0f )
							mousedy = static_cast< short >( mouse_y );
						else
							mousedy = -1;
					}
					else {
						mousedy = 1;
					}
				}
				else {
					mousedy = 0x8000u;
				}
			}
			else {
				mousedy = 0x7FFF;
			}

			user_cmd.m_mouse_accum.y( ) = mousedy;
		}

		if ( delta_y != 0.0f ) {
			float mouse_x = -( ( delta_y / yaw->get_float( ) ) / sensitivity->get_float( ) );
			short mousedx;
			if ( mouse_x <= 32767.0f ) {
				if ( mouse_x >= -32768.0f ) {
					if ( mouse_x >= 1.0f || mouse_x < 0.0f ) {
						if ( mouse_x <= -1.0f || mouse_x > 0.0f )
							mousedx = static_cast< short >( mouse_x );
						else
							mousedx = -1;
					}
					else {
						mousedx = 1;
					}
				}
				else {
					mousedx = 0x8000u;
				}
			}
			else {
				mousedx = 0x7FFF;
			}

			user_cmd.m_mouse_accum.x( ) = mousedx;
		}
	}

    void c_move::handle( valve::user_cmd_t& cmd, bool& send_packet ) {
		if ( g_local_player->weapon( )
			&& g_local_player->weapon_info( ) ) {
			m_max_player_speed = m_max_weapon_speed = g_local_player->self( )->scoped( ) ?
				g_local_player->weapon_info( )->m_max_speed_alt :
				g_local_player->weapon_info( )->m_max_speed;
		}
		else
			m_max_player_speed = m_max_weapon_speed = 260.f;

		const auto move_type = g_local_player->self( )->move_type( );

		if ( g_menu->main( ).m_move_cfg.get( ).m_bhop ) {
			if ( cmd.m_buttons & valve::e_buttons::in_jump ) {
				if ( move_type != valve::e_move_type::ladder
					|| move_type != valve::e_move_type::noclip ) {
					if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) )
						cmd.m_buttons &= ~valve::e_buttons::in_jump;
				}
			}
		}

		correct_mouse( cmd );

        auto_strafe( cmd );

		fast_stop( cmd );

		slow_walk( cmd );

		if ( g_menu->main( ).m_move_cfg.get( ).m_infinity_duck )
			cmd.m_buttons |= valve::e_buttons::in_bullrush;

		fake_duck( cmd, send_packet );

		auto_stop( cmd );

		cmd.m_buttons &= ~(
			valve::e_buttons::in_fwd
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
			);

		if ( g_menu->main( ).m_move_cfg.get( ).m_slide_walk
			&& g_local_player->self( )->move_type( ) == valve::e_move_type::walk ) {
			if ( cmd.m_move.x( ) != 0.f )
				cmd.m_buttons |=
				cmd.m_move.x( ) < 0.f
				? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

			if ( cmd.m_move.y( ) == 0.f )
				return;

			cmd.m_buttons |=
				cmd.m_move.y( ) < 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( cmd.m_move.x( ) != 0.f )
			cmd.m_buttons |=
			cmd.m_move.x( ) > 0.f
			? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

		if ( cmd.m_move.y( ) == 0.f )
			return;

		cmd.m_buttons |=
			cmd.m_move.y( ) > 0.f
			? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
    }

	void c_move::auto_stop( valve::user_cmd_t& user_cmd ) {

		static sdk::qang_t wish_ang{};
		bool predict_available{};

		if ( const auto weapon = g_local_player->self( )->weapon( ) ) {
			if ( weapon->info( ) ) {
				if ( weapon->info( )->m_type == valve::e_weapon_type::grenade
					|| weapon->info( )->m_type == valve::e_weapon_type::c4
					|| weapon->info( )->m_type == valve::e_weapon_type::knife )
					return;
			}
		}

		if ( g_local_player->can_shoot( )
			&& pred_auto_stop( )
			&& hacks::g_move->allow_early_stop( )
			&& hacks::g_exploits->is_peeking( wish_ang, 2.f ) )
			predict_available = true;

		if ( !predict_available )
			if ( !hacks::g_aim_bot->stop_type( ) )
				return;

		auto stop_type = hacks::g_aim_bot->stop_type( );

		hacks::g_aim_bot->stop_type( ) = 0;

		if ( predict_available ) {
			stop_type = 1;
		}

		if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground )
			|| user_cmd.m_buttons & valve::e_buttons::in_jump )
			return;

		static auto sv_accelerate_use_weapon_speed = valve::g_cvar->find_var( ( "sv_accelerate_use_weapon_speed" ) );

		static auto sv_accelerate = valve::g_cvar->find_var( ( "sv_accelerate" ) );

		const auto weapon = g_local_player->self( )->weapon( );
		if ( !weapon )
			return;

		const auto wpn_data = weapon->info( );
		if ( !wpn_data )
			return;

		auto max_speed{ 260.f };

		if ( g_local_player->self( )->weapon( )
			&& g_local_player->self( )->weapon( )->info( ) ) {
			max_speed = g_local_player->self( )->scoped( ) ?
				g_local_player->self( )->weapon( )->info( )->m_max_speed_alt :
				g_local_player->self( )->weapon( )->info( )->m_max_speed;
		}

		float target_speed{};

		if ( stop_type == 1 )
			target_speed = 33.f;

		sdk::vec3_t cur_velocity{ g_local_player->self( )->velocity( ) };

		if ( predict_available )
			predict_move( user_cmd, cur_velocity );

		const auto speed_2d = cur_velocity.length( 2u );

		if ( stop_type == 2 ) {
			if ( speed_2d < 1.f ) {
				user_cmd.m_move.x( ) = user_cmd.m_move.y( ) = 0.f;

				return;
			}
		}
		else if ( target_speed >= speed_2d ) {
			m_max_player_speed = m_max_weapon_speed = target_speed;

			modify_move( user_cmd, cur_velocity );

			return;
		}

		user_cmd.m_buttons &= ~valve::e_buttons::in_speed;

		auto finalwishspeed = std::min( max_speed, 250.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| g_local_player->self( )->flags( ) & valve::e_ent_flags::ducking;

		bool slow_down_to_fast_nigga{};

		if ( sv_accelerate_use_weapon_speed->get_int( ) ) {
			const auto item_index = static_cast< std::uint16_t >( weapon->item_index( ) );
			if ( weapon->zoom_lvl( ) > 0
				&& ( item_index == 11 || item_index == 38 || item_index == 9 || item_index == 8 || item_index == 39 || item_index == 40 ) )
				slow_down_to_fast_nigga = ( max_speed * 0.52f ) < 110.f;

			if ( !ducking
				|| slow_down_to_fast_nigga )
				finalwishspeed *= std::min( 1.f, max_speed / 250.f );
		}

		if ( ducking
			&& !slow_down_to_fast_nigga )
			finalwishspeed *= 0.34f;

		static auto surf_friction = valve::find_in_datamap( g_local_player->self( )->pred_desc_map( ), "m_surfaceFriction" );

		finalwishspeed =
			( ( valve::g_global_vars.get( )->m_interval_per_tick * sv_accelerate->get_float( ) ) * finalwishspeed )
			* ( *( float* ) ( std::uintptr_t( g_local_player->self( ) ) + surf_friction ) );

		if ( stop_type == 1 ) {
			if ( max_speed * 0.43f <= speed_2d ) {
				sdk::qang_t dir{};
				sdk::vec_angs( cur_velocity *= -1.f, dir );

				dir.y( ) = user_cmd.m_view_angles.y( ) - dir.y( );

				sdk::vec3_t dir_ang_handler{};

				sdk::ang_vecs( dir, &dir_ang_handler, nullptr, nullptr );

				user_cmd.m_move.x( ) = dir_ang_handler.x( ) * finalwishspeed;
				user_cmd.m_move.y( ) = dir_ang_handler.y( ) * finalwishspeed;
			}
			else {
				m_max_player_speed = m_max_weapon_speed = target_speed;

				modify_move( user_cmd, cur_velocity );
			}
		}
		else {
			sdk::qang_t dir{};
			sdk::vec_angs( cur_velocity *= -1.f, dir );

			dir.y( ) = user_cmd.m_view_angles.y( ) - dir.y( );

			sdk::vec3_t dir_ang_handler{};

			sdk::ang_vecs( dir, &dir_ang_handler, nullptr, nullptr );

			user_cmd.m_move.x( ) = dir_ang_handler.x( ) * finalwishspeed;
			user_cmd.m_move.y( ) = dir_ang_handler.y( ) * finalwishspeed;
		}
	}


	void c_move::fake_duck( valve::user_cmd_t& cmd, bool& send_packet ) const {
		if ( !m_should_fake_duck )
			return;

		if ( g_exploits->type( ) ) {
			if ( g_local_player->self ( )->duck_amt( ) > 0.5f )
				cmd.m_buttons |= valve::e_buttons::in_duck;
			else
				cmd.m_buttons &= ~valve::e_buttons::in_duck;
		}
		else {
			cmd.m_buttons |= valve::e_buttons::in_bullrush;

			const auto pred_duck_amount = std::min( 1.f, g_local_player->self( )->duck_speed( ) * valve::to_time( 7 ) );
			if ( g_local_player->self( )->duck_amt( ) <= pred_duck_amount ) {			
				if ( valve::g_client_state->m_choked_cmds < 7 )
					cmd.m_buttons &= ~valve::e_buttons::in_duck;
				else
					cmd.m_buttons |= valve::e_buttons::in_duck;

				if ( g_exploits->allow_choke( ) )
					send_packet = false;
				else
					send_packet = true;
			}
			else {
				cmd.m_buttons &= ~valve::e_buttons::in_duck;

				send_packet = true;
			}
		}
	}

	void c_move::slow_walk( valve::user_cmd_t& cmd ) {
		if ( !g_key_binds->get_keybind_state( &g_menu->main( ).m_move_cfg.get( ).m_slow_walk ) )
			return;

		if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) )
			return;

		if ( cmd.m_move.x( ) == 0.f
			&& cmd.m_move.y( ) == 0.f )
			return;

		if ( 30.f > g_local_player->self( )->velocity( ).length( 2u ) )
			return;

		m_max_player_speed = m_max_weapon_speed = 30.f;

		auto velocity = g_local_player->self( )->velocity( );

		modify_move( cmd, velocity );
	}

	void c_move::fast_stop( valve::user_cmd_t& cmd ) const {
		if ( !g_menu->main( ).m_move_cfg.get( ).m_fast_stop )
			return;

		const auto move_type = g_local_player->self( )->move_type( );

		if ( move_type == valve::e_move_type::ladder
			|| move_type == valve::e_move_type::noclip )
			return;

		if ( cmd.m_buttons & ( ( 1 << 1 ) | ( 1 << 9 ) | ( 1 << 10 ) | ( 1 << 3 ) | ( 1 << 4 ) ) )
			return;

		if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) )
			return;

		if ( g_local_player->self( )->velocity( ).length( 2 ) <= 260 * 0.34f ) {
			cmd.m_move.x( ) = cmd.m_move.y( ) = 0.f;
			return;
		}

		sdk::qang_t resistance_ang{};

		sdk::vec_angs( g_local_player->self( )->velocity( ) * -1.f, resistance_ang );

		resistance_ang.y( ) = cmd.m_view_angles.y( ) - resistance_ang.y( );
		resistance_ang.x( ) = cmd.m_view_angles.x( ) - resistance_ang.x( );

		sdk::vec3_t resistance_vec{};

		sdk::ang_vecs( resistance_ang, &resistance_vec, nullptr, nullptr );

		cmd.m_move.x( ) = std::clamp( resistance_vec.x( ), -450.f, 450.0f );
		cmd.m_move.y( ) = std::clamp( resistance_vec.y( ), -450.f, 450.0f );
	}

    void c_move::auto_strafe( valve::user_cmd_t& user_cmd ) const {
		if ( !g_menu->main( ).m_move_cfg.get( ).m_auto_strafe )
			return;

		if ( g_local_player->self( )->move_type( ) == valve::e_move_type::ladder
			|| g_local_player->self( )->move_type( ) == valve::e_move_type::noclip )
			return;

        if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) ) {
			auto wish_angles = user_cmd.m_view_angles;

			const auto velocity = g_local_player->self( )->velocity( );

			static bool strafe_switch{};

			auto speed_2d = velocity.length( 2u );

			auto ideal_0 = ( speed_2d > 0.f ) ? sdk::to_deg( std::asin( 15.f / speed_2d ) ) : 90.f;

			ideal_0 = std::clamp( ideal_0, 0.f, 90.f );

			const auto mult = strafe_switch ? 1.f : -1.f;

			strafe_switch = !strafe_switch;

			enum e_dirs {
				e_forwards = 0,
				e_backwards = 180,
				e_left = 90,
				e_right = -90,
				e_back_left = 135,
				e_back_right = -135
			};

			float wish_dir{};

			bool hold_w = user_cmd.m_buttons & valve::e_buttons::in_fwd;
			bool hold_a = user_cmd.m_buttons & valve::e_buttons::in_move_left;
			bool hold_s = user_cmd.m_buttons & valve::e_buttons::in_back;
			bool hold_d = user_cmd.m_buttons & valve::e_buttons::in_move_right;

			if ( hold_w ) {
				if ( hold_a )
					wish_dir += ( e_dirs::e_left / 2 );
				else if ( hold_d )
					wish_dir += ( e_dirs::e_right / 2 );
				else
					wish_dir += e_dirs::e_forwards;
			}
			else if ( hold_s ) {
				if ( hold_a )
					wish_dir += e_dirs::e_back_left;
				else if ( hold_d )
					wish_dir += e_dirs::e_back_right;
				else
					wish_dir += e_dirs::e_backwards;

				user_cmd.m_move.x( ) = 0.f;
			}
			else if ( hold_a )
				wish_dir += e_dirs::e_left;
			else if ( hold_d )
				wish_dir += e_dirs::e_right;

			wish_angles.y( ) += sdk::normalize_yaw( wish_dir );

			user_cmd.m_move.x( ) = 0.f;

			auto delta = sdk::normalize_yaw( wish_angles.y( ) - m_prev_view_yaw );

			auto abs_delta = std::abs( delta );


			if ( abs_delta <= ideal_0
				|| abs_delta >= 30.f ) {
				sdk::qang_t vel_ang{};

				sdk::vec_angs( velocity, vel_ang );

				auto vel_delta = sdk::normalize_yaw( wish_angles.y( ) - vel_ang.y( ) );

				auto correct = ideal_0;

				if ( vel_delta <= correct ||
					speed_2d <= 15.f ) {
					if ( -correct <= vel_delta || speed_2d <= 15.f ) {
						wish_angles.y( ) += ( ideal_0 * mult );
						user_cmd.m_move.y( ) = 450.f * mult;
					}
					else {
						wish_angles.y( ) = vel_ang.y( ) - correct;
						user_cmd.m_move.y( ) = 450.f;
					}
				}
				else {
					wish_angles.y( ) = vel_ang.y( ) + correct;
					user_cmd.m_move.y( ) = -450.f;
				}
			}

			else if ( delta > 0.f )
				user_cmd.m_move.y( ) = -450.f;
			else if ( delta < 0.f )
				user_cmd.m_move.y( ) = 450.f;

			rotate(
				user_cmd, wish_angles
			);
        }
    }

	void c_move::accelerate(
		const valve::user_cmd_t& user_cmd, const sdk::vec3_t& wishdir,
		const float wishspeed, sdk::vec3_t& velocity, float acceleration
	) const {
		const auto cur_speed = velocity.dot( wishdir );

		static auto sv_accelerate_use_weapon_speed = valve::g_cvar->find_var( ( "sv_accelerate_use_weapon_speed" ) );

		const auto add_speed = wishspeed - cur_speed;
		if ( add_speed <= 0.f )
			return;

		const auto v57 = std::max( cur_speed, 0.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| g_local_player->self( )->flags( ) & valve::e_ent_flags::ducking;

		auto v20 = true;
		if ( ducking
			|| !( user_cmd.m_buttons & valve::e_buttons::in_speed ) )
			v20 = false;

		auto finalwishspeed = std::max( wishspeed, 250.f );
		auto abs_finalwishspeed = finalwishspeed;

		const auto weapon = g_local_player->self( )->weapon( );

		bool slow_down_to_fast_nigga{};

		if ( weapon
			&& sv_accelerate_use_weapon_speed->get_int( ) ) {
			const auto item_index = static_cast< std::uint16_t >( weapon->item_index( ) );
			if ( weapon->zoom_lvl( ) > 0
				&& ( item_index == 11 || item_index == 38 || item_index == 9 || item_index == 8 || item_index == 39 || item_index == 40 ) )
				slow_down_to_fast_nigga = ( m_max_weapon_speed * 0.52f ) < 110.f;

			const auto modifier = std::min( 1.f, m_max_weapon_speed / 250.f );

			abs_finalwishspeed *= modifier;

			if ( ( !ducking && !v20 )
				|| slow_down_to_fast_nigga )
				finalwishspeed *= modifier;
		}

		if ( ducking ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.34f;

			abs_finalwishspeed *= 0.34f;
		}

		if ( v20 ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.52f;

			abs_finalwishspeed *= 0.52f;

			const auto abs_finalwishspeed_minus5 = abs_finalwishspeed - 5.f;
			if ( v57 < abs_finalwishspeed_minus5 ) {
				const auto v30 =
					std::max( v57 - abs_finalwishspeed_minus5, 0.f )
					/ std::max( abs_finalwishspeed - abs_finalwishspeed_minus5, 0.f );

				const auto v27 = 1.f - v30;
				if ( v27 >= 0.f )
					acceleration = std::min( v27, 1.f ) * acceleration;
				else
					acceleration = 0.f;
			}
		}

		static auto surf_friction = valve::find_in_datamap( g_local_player->self( )->pred_desc_map( ), "m_surfaceFriction" );

		const auto v33 = std::min(
			add_speed,
			( ( valve::g_global_vars.get( )->m_interval_per_tick * acceleration ) * finalwishspeed )
			* ( *( float* ) ( std::uintptr_t( g_local_player->self( ) ) + surf_friction ) ) );

		velocity += wishdir * v33;

		const auto len = velocity.length( );
		if ( len
			&& len > m_max_weapon_speed )
			velocity *= m_max_weapon_speed / len;

	}

	void c_move::full_walk_move(
		const valve::user_cmd_t& user_cmd, sdk::vec3_t& move,
		sdk::vec3_t& fwd, sdk::vec3_t& right, sdk::vec3_t& velocity
	) const {
		static auto sv_maxvelocity = valve::g_cvar->find_var( ( "sv_maxvelocity" ) );
		static auto sv_friction = valve::g_cvar->find_var( ( "sv_friction" ) );
		static auto surf_friction = valve::find_in_datamap( g_local_player->self( )->pred_desc_map( ), "m_surfaceFriction" );

		if ( static_cast < sdk::ulong_t > ( g_local_player->self( )->ground_entity_handle( ) ) ) {
			velocity.z( ) = 0.f;

			const auto speed = velocity.length( );
			if ( speed >= 0.1f ) {
				const auto friction = sv_friction->get_float( ) * ( *( float* ) ( std::uintptr_t( g_local_player->self( ) ) + surf_friction ) );
				const auto sv_stopspeed = sv_friction->get_float( );
				const auto control = speed < sv_stopspeed ? sv_stopspeed : speed;

				const auto new_speed = std::max( 0.f, speed - ( ( control * friction ) * valve::g_global_vars.get( )->m_interval_per_tick ) );
				if ( speed != new_speed )
					velocity *= new_speed / speed;
			}

			walk_move( user_cmd, move, fwd, right, velocity );

			velocity.z( ) = 0.f;
		}

		const auto sv_maxvelocity_ = sv_maxvelocity->get_float( );
		for ( std::size_t i{}; i < 3u; ++i ) {
			auto& element = velocity.at( i );

			if ( element > sv_maxvelocity_ )
				element = sv_maxvelocity_;
			else if ( element < -sv_maxvelocity_ )
				element = -sv_maxvelocity_;
		}
	}

	void c_move::modify_move( valve::user_cmd_t& user_cmd, sdk::vec3_t& velocity ) const {
		sdk::vec3_t fwd{}, right{};

		sdk::ang_vecs( user_cmd.m_view_angles, &fwd, &right, nullptr );

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			user_cmd.m_move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, user_cmd.m_move, fwd, right, velocity );
	}

	void c_move::predict_move( const valve::user_cmd_t& user_cmd, sdk::vec3_t& velocity ) const {
		sdk::vec3_t fwd{}, right{};

		sdk::ang_vecs( user_cmd.m_view_angles, &fwd, &right, nullptr );

		auto move = user_cmd.m_move;

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, move, fwd, right, velocity );
	}


	void c_move::walk_move(
		const valve::user_cmd_t& user_cmd, sdk::vec3_t& move,
		sdk::vec3_t& fwd, sdk::vec3_t& right, sdk::vec3_t& velocity
	) const {
		if ( fwd.z( ) != 0.f )
			fwd.normalize( );

		if ( right.z( ) != 0.f )
			right.normalize( );

		sdk::vec3_t wishvel{
			fwd.x( ) * move.x( ) + right.x( ) * move.y( ),
			fwd.y( ) * move.x( ) + right.y( ) * move.y( ),
			0.f
		};
		static auto sv_accelerate = valve::g_cvar->find_var( ( "sv_accelerate" ) );
		auto wishdir = wishvel;

		auto wishspeed = wishdir.normalize( );
		if ( wishspeed
			&& wishspeed > m_max_player_speed ) {
			wishvel *= m_max_player_speed / wishspeed;

			wishspeed = m_max_player_speed;
		}

		velocity.z( ) = 0.f;
		accelerate( user_cmd, wishdir, wishspeed, velocity, sv_accelerate->get_float( ) );
		velocity.z( ) = 0.f;

		const auto speed_sqr = velocity.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			velocity *= m_max_player_speed / std::sqrt( speed_sqr );

		if ( velocity.length( ) < 1.f )
			velocity = {};
	}

	ALWAYS_INLINE bool compare_vec( sdk::vec3_t& first, sdk::vec3_t& second )
	{
		if ( first.x( ) == second.x( ) &&
			first.y( ) == second.y( ) &&
			first.z( ) == second.z( ) )
			return true;

		return false;
	}

	ALWAYS_INLINE float vector_normalize( sdk::vec3_t& vec )
	{
		float radius = sqrtf( vec.x( ) * vec.x( ) + vec.y( ) * vec.y( ) + vec.z( ) * vec.z( ) );
		float iradius = 1.f / ( radius + FLT_EPSILON );

		vec.x( ) *= iradius;
		vec.y( ) *= iradius;
		vec.z( ) *= iradius;

		return radius;
	}

	ALWAYS_INLINE bool is_zero( sdk::vec3_t vec ) {
		return ( vec.x( ) > -0.01f && vec.x( ) < 0.01f &&
			vec.y( ) > -0.01f && vec.y( ) < 0.01f &&
			vec.z( ) > -0.01f && vec.z( ) < 0.01f );
	}

	void c_move::auto_peek( sdk::qang_t& wish_angles, valve::user_cmd_t& user_cmd ) {
		if ( !g_local_player->self( ) )
			return;

		if ( !g_local_player->self( )->weapon( ) )
			return;

		if ( g_local_player->self( )->weapon( )->is_knife( ) )
			return;

		if ( g_local_player->self( )->weapon( )->item_index( ) == valve::e_item_index::taser )
			return;

		if ( !g_local_player->self( )->weapon( )->info( ) )
			return;

		if ( g_local_player->self( )->weapon( )->info( )->m_type == static_cast < valve::e_weapon_type > ( 9 ) )
			return;

		if ( g_key_binds->get_keybind_state( &g_menu->main( ).m_move_cfg.get( ).m_auto_peek ) ) {
			if ( is_zero( m_auto_peek.m_pos ) ) {
				m_auto_peek.m_pos = g_local_player->self( )->abs_origin( );

				if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) ) {
					valve::ray_t* ray = new valve::ray_t( m_auto_peek.m_pos, m_auto_peek.m_pos - sdk::vec3_t( 0.0f, 0.0f, 1000.0f ) );
					valve::trace_filter_world_only_t filter;
					valve::trace_t trace;

					valve::g_engine_trace->trace_ray( *ray, valve::e_mask::solid, &filter, &trace );

					if ( trace.m_frac < 1.f ) {
						m_auto_peek.m_pos = trace.m_end + sdk::vec3_t( 0.f, 0.f, 2.f );
					}
				}
			}
			else {
				if ( user_cmd.m_buttons & valve::e_buttons::in_attack && g_local_player->self( )->weapon( ) ) {
					if ( g_local_player->self( )->weapon( )->item_index( ) != valve::e_item_index::revolver )
						m_auto_peek.m_shot = true;
				}

				if ( m_auto_peek.m_shot ) {
					auto current_position = g_local_player->self( )->abs_origin( );
					auto difference = current_position - m_auto_peek.m_pos;

					if ( difference.length( 2u ) > 1.7f )
					{
						user_cmd.m_buttons &= ~valve::e_buttons::in_jump;
						const auto chocked_ticks = ( user_cmd.m_number % 2 ) != 1 ? ( 14 - valve::g_client_state->m_choked_cmds ) : valve::g_client_state->m_choked_cmds;
						const auto wish_ang_ = sdk::calc_ang( g_local_player->self( )->abs_origin( ), m_auto_peek.m_pos );

						wish_angles.x( ) = wish_ang_.x( );
						wish_angles.y( ) = wish_ang_.y( );
						wish_angles.z( ) = wish_ang_.z( );

						static auto cl_forwardspeed = valve::g_cvar->find_var( ( "cl_forwardspeed" ) );
						user_cmd.m_move.x( ) = cl_forwardspeed->get_float( ) - ( 1.2f * chocked_ticks );
						user_cmd.m_move.y( ) = 0.0f;
					}
					else
					{
						m_auto_peek.m_shot = false;
						m_auto_peek.m_pos = sdk::vec3_t( );
					}

					rotate(
						user_cmd, wish_angles
					);
				}
			}
		}
		else
		{
			m_auto_peek.m_shot = false;
			m_auto_peek.m_pos = sdk::vec3_t( );
		}
	}

    void c_move::rotate( valve::user_cmd_t& cmd, const sdk::qang_t& wish_angles ) const {
		if ( !g_menu->main( ).m_anti_aim_cfg.get( ).m_roll ) {
			const auto move_type = g_local_player->self( )->move_type( );
			if ( move_type == valve::e_move_type::noclip
				|| move_type == valve::e_move_type::ladder )
				return;

			sdk::vec3_t right{};
			auto fwd = cmd.m_view_angles.vectors( &right );

			if ( fwd.z( ) ) {
				fwd.z( ) = 0.f;
				fwd.normalize( );
			}

			if ( right.z( ) ) {
				right.z( ) = 0.f;
				right.normalize( );
			}

			if ( const auto div = right.y( ) * fwd.x( ) - right.x( ) * fwd.y( ) ) {
				sdk::vec3_t wish_right{};
				auto wish_fwd = wish_angles.vectors( &wish_right );

				if ( wish_fwd.z( ) ) {
					wish_fwd.z( ) = 0.f;
					wish_fwd.normalize( );
				}

				if ( wish_right.z( ) ) {
					wish_right.z( ) = 0.f;
					wish_right.normalize( );
				}

				const sdk::vec2_t wish_vel{
					wish_fwd.x( ) * cmd.m_move.x( ) + wish_right.x( ) * cmd.m_move.y( ),
					wish_fwd.y( ) * cmd.m_move.x( ) + wish_right.y( ) * cmd.m_move.y( )
				};

				cmd.m_move.y( ) = ( wish_vel.y( ) * fwd.x( ) - wish_vel.x( ) * fwd.y( ) ) / div;
				cmd.m_move.x( ) = ( wish_vel.x( ) * right.y( ) - wish_vel.y( ) * right.x( ) ) / div;
			}

			const auto& cvars = g_ctx->cvars( );

			const auto max_fwd_speed = cvars.cl_forwardspeed->get_float( );
			const auto max_back_speed = cvars.cl_backspeed->get_float( );
			const auto max_side_speed = cvars.cl_sidespeed->get_float( );

			cmd.m_buttons &= ~valve::e_buttons::in_move;

			if ( ( cmd.m_move.x( ) = std::clamp( cmd.m_move.x( ), -max_back_speed, max_fwd_speed ) ) )
				cmd.m_buttons |= cmd.m_move.x( ) > 0.f
				? valve::e_buttons::in_fwd : valve::e_buttons::in_back;

			if ( ( cmd.m_move.y( ) = std::clamp( cmd.m_move.y( ), -max_side_speed, max_side_speed ) ) )
				cmd.m_buttons |= cmd.m_move.y( ) > 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
		}
		else {
			sdk::vec3_t pure_fwd, pure_right, pure_up, curr_fwd, curr_right, curr_up;
			sdk::ang_vecs( wish_angles, &pure_fwd, &pure_right, &pure_up );
			sdk::ang_vecs( cmd.m_view_angles, &curr_fwd, &curr_right, &curr_up );

			pure_fwd.at ( 2 ) = pure_right.at ( 2 ) = curr_fwd.at ( 2 ) = curr_right.at ( 2 ) = 0.f;

			vector_normalize( pure_fwd );
			vector_normalize( pure_right );
			vector_normalize( curr_fwd );
			vector_normalize( curr_right );
			sdk::vec3_t pure_target_dir;
			for ( auto i = 0u; i < 2; i++ )
				pure_target_dir.at( i ) = pure_fwd.at( i ) * cmd.m_move.x( ) + pure_right.at( i ) * cmd.m_move.y( );
			pure_target_dir.at( 2 ) = 0.f;

			sdk::vec3_t curr_target_dir;
			for ( auto i = 0u; i < 2; i++ )
				curr_target_dir.at( i ) = curr_fwd.at( i ) * cmd.m_move.x( ) + curr_right.at( i ) * cmd.m_move.y( );
			curr_target_dir.at( 2 ) = 0.f;

			if ( !compare_vec( pure_target_dir, curr_target_dir ) ) {
				cmd.m_move.x( ) = ( pure_target_dir.x( ) * curr_right.y( ) - curr_right.x( ) * pure_target_dir.y( ) ) / ( curr_right.y( ) * curr_fwd.x( ) - curr_right.x( ) * curr_fwd.y( ) );
				cmd.m_move.y( ) = ( pure_target_dir.y( ) * curr_fwd.x( ) - curr_fwd.y( ) * pure_target_dir.x( ) ) / ( curr_right.y( ) * curr_fwd.x( ) - curr_right.x( ) * curr_fwd.y( ) );
			}
		}
    }
}