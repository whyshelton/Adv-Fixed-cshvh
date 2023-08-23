#include "../../../csgo.hpp"

namespace hacks {
	void c_misc::buy_bot( ) {
		const auto& cfg = g_menu->main( ).m_misc_cfg.get( );
		if ( cfg.m_buy_bot && m_buy_bot_count ) {
			--m_buy_bot_count;

			if ( !m_buy_bot_count ) {
				std::string buy{};

				switch ( cfg.m_buy_bot_snipers ) {
				case 1:
					buy += ( "buy g3sg1; " );
					break;
				case 2:
					buy += ( "buy awp; " );
					break;
				case 3:
					buy += ( "buy ssg08; " );
					break;
				}

				switch ( cfg.m_buy_bot_pistols ) {
				case 1:
					buy += ( "buy elite; " );
					break;
				case 2:
					buy += ( "buy deagle; buy revolver; " );
					break;
				}

				if ( cfg.m_buy_bot_additional & 2 )
					buy += ( "buy vesthelm; buy vest; " );

				if ( cfg.m_buy_bot_additional & 4 )
					buy += ( "buy taser; " );

				if ( cfg.m_buy_bot_additional & 1 )
					buy += ( "buy molotov; buy hegrenade; buy smokegrenade; " );

				if ( cfg.m_buy_bot_additional & 8 )
					buy += ( "buy defuser; " );

				valve::g_engine.get( )->exec_cmd( buy.data( ) );
			}
		}
	}
}