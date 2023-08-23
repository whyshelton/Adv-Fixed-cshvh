#pragma once

namespace hacks {
    class c_move {
    private:
        float m_max_player_speed{}, m_max_weapon_speed{};
        float m_prev_view_yaw{};
        bool m_should_fake_duck{}, m_allow_early_stop{};

        struct auto_peek_t {
            sdk::vec3_t m_pos{};
            bool m_shot{};
        } m_auto_peek{};

    public:
        void handle( valve::user_cmd_t& cmd, bool& send_packet );

        void auto_strafe( valve::user_cmd_t& cmd ) const;

        void fast_stop( valve::user_cmd_t& cmd ) const;

        void slow_walk( valve::user_cmd_t& cmd );

        void fake_duck( valve::user_cmd_t& cmd, bool& send_packet ) const;

        void rotate( valve::user_cmd_t& cmd, const sdk::qang_t& wish_angles ) const;

        void accelerate(
            const valve::user_cmd_t& user_cmd, const sdk::vec3_t& wishdir,
            const float wishspeed, sdk::vec3_t& velocity, float acceleration
        ) const;

        void walk_move(
            const valve::user_cmd_t& user_cmd, sdk::vec3_t& move,
            sdk::vec3_t& fwd, sdk::vec3_t& right, sdk::vec3_t& velocity
        ) const;

        void full_walk_move(
            const valve::user_cmd_t& user_cmd, sdk::vec3_t& move,
            sdk::vec3_t& fwd, sdk::vec3_t& right, sdk::vec3_t& velocity
        ) const;

        void correct_mouse( valve::user_cmd_t& user_cmd );

        void modify_move( valve::user_cmd_t& user_cmd, sdk::vec3_t& velocity ) const;

        void predict_move( const valve::user_cmd_t& user_cmd, sdk::vec3_t& velocity ) const;

        void auto_stop( valve::user_cmd_t& user_cmd );

        void auto_peek( sdk::qang_t& wish_ang, valve::user_cmd_t& user_cmd );

        ALWAYS_INLINE float& prev_view_yaw( );

        ALWAYS_INLINE bool& should_fake_duck( );

        ALWAYS_INLINE bool& allow_early_stop( );

        ALWAYS_INLINE auto_peek_t& auto_peek_data( );
    };

    inline const auto g_move = std::make_unique< c_move >( );
}

#include "impl/move.inl"