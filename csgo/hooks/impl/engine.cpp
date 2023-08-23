#include "../../csgo.hpp"
namespace hooks {
	void __fastcall packet_start(
		const std::uintptr_t ecx, const std::uintptr_t edx, const int in_seq, const int out_acked
	) {
		if ( !valve::g_engine.get( )->in_game( ) 
			|| !g_local_player->self ( )
			|| !g_local_player->self ( )->alive( ) )
			return o_packet_start( ecx, edx, in_seq, out_acked );

		auto& net_cmds = g_local_player->net_cmds( );
		if ( net_cmds.empty( )
			|| std::find( net_cmds.rbegin( ), net_cmds.rend( ), out_acked ) == net_cmds.rend( ) )
			return;

		net_cmds.erase(
			std::remove_if(
				net_cmds.begin( ), net_cmds.end( ),
				[ & ]( const int cmd_number ) {
					return cmd_number < out_acked;
				}
			),
			net_cmds.end( )
					);

		return o_packet_start( ecx, edx, in_seq, out_acked );
	}

	void __cdecl cl_move( float samples, bool final_tick ) {
		if ( !valve::g_engine.get( )->in_game( )
			|| !g_local_player->self( )
			|| !g_local_player->self( )->alive( ) ) {
			return o_cl_move( samples, final_tick );
		}

		valve::global_vars_base_t backup = *valve::g_global_vars.get( );
		auto cl_tick_count = valve::g_client_state->m_client_tick;
		auto server_tick = valve::g_client_state->m_server_tick;

		g_ctx->offsets( ).m_addresses.m_cl_read_packets( final_tick );

		valve::g_client_state->m_client_tick = cl_tick_count;
		valve::g_client_state->m_server_tick = server_tick;
		*valve::g_global_vars.get( ) = backup;

		o_cl_move( samples, final_tick );
	}

	void __fastcall packet_end( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( !g_local_player->self( )
			|| valve::g_client_state->m_server_tick != valve::g_client_state->m_delta_tick )
			return o_packet_end( ecx, edx );

		const auto& local_data = g_local_player->prediction ( ).data( ).local_data( ).at( valve::g_client_state->m_cmd_ack % 150 );
		if ( local_data.m_spawn_time == g_local_player->self ( )->spawn_time( )
			&& local_data.m_shift_amount > 0
			&& local_data.m_tick_base > g_local_player->self ( )->tick_base( )
			&& ( local_data.m_tick_base - g_local_player->self( )->tick_base( ) ) <= 17 )
			g_local_player->self( )->tick_base( ) = local_data.m_tick_base + 1;

		o_packet_end( ecx, edx );
	}

	float __stdcall aspect_ratio( int width, int height ) {
		if ( !valve::g_engine.get( )->in_game( )
			|| !g_local_player->self( ) )
			return o_aspect_ratio( width, height );

		if ( !g_menu->main( ).m_misc_cfg.get( ).m_aspect_ratio )
			return o_aspect_ratio( width, height );
		else
			return g_menu->main( ).m_misc_cfg.get( ).m_aspect_ratio_val;
	}

	bool __fastcall process_temp_entities( const std::uintptr_t ecx, const std::uintptr_t edx, const std::uintptr_t msg 
	) {
		const auto backup_max_clients = valve::g_client_state->m_max_clients;

		valve::g_client_state->m_max_clients = 1;

		const auto ret = o_process_temp_entities( ecx, edx, msg );

		valve::g_client_state->m_max_clients = backup_max_clients;

		valve::g_engine.get( )->fire_events( );

		return ret;
	}

	bool __cdecl is_using_debug_static_props( ) {
		return true;
	}

	bool __fastcall is_paused( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->offsets( ).m_addresses.m_ret_to_extrapolation )
			return true;

		return o_is_paused( ecx, edx );
	}

	bool __fastcall is_hltv( const std::uintptr_t ecx, const std::uintptr_t edx ) {

		if ( g_ctx->allow_setup_bones( ) )
			return true;

		if ( g_ctx->allow_update_anim( ) )
			return true;

		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->offsets( ).m_addresses.m_ret_to_accumulate_layers ||
			*reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->offsets( ).m_addresses.m_ret_to_setup_velocity )
			return true;

		return o_is_hltv( ecx, edx );
	}

	int __fastcall list_leaves_in_box(
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const sdk::vec3_t& mins, const sdk::vec3_t& maxs, const uint16_t* const list, const int max
	) {

		if ( !g_local_player->self( ) )
			return o_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		if ( *( uint32_t* ) _AddressOfReturnAddress( ) != g_ctx->offsets( ).m_addresses.m_ret_insert_into_tree )
			return o_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		struct renderable_info_t {
			valve::i_client_renderable* m_renderable{};
			std::uintptr_t	m_alpha_property{};
			int				m_enum_count{};
			int				m_render_frame{};
			std::uint16_t	m_first_shadow{};
			std::uint16_t	m_leaf_list{};
			short			m_area{};
			std::uint16_t	m_flags0{};
			std::uint16_t	m_flags1{};
			sdk::vec3_t			m_bloated_abs_min{};
			sdk::vec3_t			m_bloated_abs_max{};
			sdk::vec3_t			m_abs_min{};
			sdk::vec3_t			m_abs_max{};
			char			pad0[ 4u ]{};
		};

		const auto info = *reinterpret_cast< renderable_info_t** >(
			reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14u
			);
		if ( !info
			|| !info->m_renderable )
			return o_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		const auto entity = info->m_renderable->get_client_unknown( )->get_base_entity( );
		if ( !entity
			|| !entity->is_player( ) )
			return o_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		info->m_flags0 &= ~0x100;
		info->m_flags1 |= 0xC0;

		return o_list_leaves_in_box(
			ecx, edx,
			{ -16384.f, -16384.f, -16384.f },
			{ 16384.f, 16384.f, 16384.f },
			list, max
		);
	}

}