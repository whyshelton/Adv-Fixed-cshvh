#pragma once

namespace hacks {
	class c_misc {
	private:
		int m_buy_bot_count{};

	public:
		void buy_bot( );

		ALWAYS_INLINE c_misc( ) = default;

		ALWAYS_INLINE void third_person( );

		ALWAYS_INLINE void clan_tag( );

		ALWAYS_INLINE void removals( );

		ALWAYS_INLINE int& buy_bot_count( );
	};

	inline const auto g_misc = std::make_unique < c_misc >( );
}

#include "impl/misc.inl"