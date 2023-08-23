#pragma once

namespace hacks {
    struct shot_t {
    public:
        __forceinline constexpr shot_t( ) = default;

        __forceinline shot_t(
            const sdk::vec3_t& src, const aim_target_t* const target, const int next_shift_amount, const std::ptrdiff_t cmd_num, const float sent_time, const float latency
        ) : m_src{ src }, m_target{ target ? *target : aim_target_t{} }, m_shot_time{ valve::g_global_vars.get( )->m_real_time },
            m_target_index{ target && target->m_entry->m_player ? target->m_entry->m_player->networkable( )->index( ) : -1 }, m_next_shift_amount{ next_shift_amount },
            m_sent_time{ cmd_num != -1 ? sent_time : 0.f }, m_cmd_number{ cmd_num != -1 ? cmd_num : -1 }, m_latency{ cmd_num != -1 ? latency : 0.f } {}

        sdk::vec3_t							m_src{};

        aim_target_t				m_target{};
        std::string                 m_str{};

        float							m_shot_time{},
            m_sent_time{}, m_latency{};
        bool							m_processed{};
        int								m_dmg{},
            m_target_index{},
            m_next_shift_amount{},
            m_cmd_number{ -1 }, m_process_tick{};

        struct {
            sdk::vec3_t	m_impact_pos{};
            int		m_fire_tick{}, m_hurt_tick{}, m_hitgroup{}, m_dmg{};
        }							m_server_info{};
    };

	class c_shots {
	public:
        void on_net_update( );

        std::deque< shot_t > m_elements{};

        __forceinline std::deque< shot_t >& elements( ) { return m_elements; }

        __forceinline shot_t* last_unprocessed( ) {
            if ( m_elements.empty( ) )
                return nullptr;

            const auto shot = std::find_if(
                m_elements.begin( ), m_elements.end( ),
                [ ]( const shot_t& shot ) {
                    return !shot.m_processed
                        && shot.m_server_info.m_fire_tick
                        && shot.m_server_info.m_fire_tick == valve::g_client_state->m_server_tick;
                }
            );

            return shot == m_elements.end( ) ? nullptr : &*shot;
        }

        __forceinline void add(
            const sdk::vec3_t& src, const aim_target_t* const target, const int next_shift_amount, const std::ptrdiff_t cmd_num, const float sent_time, const float latency
        ) {
            m_elements.emplace_back( src, target, next_shift_amount, cmd_num, sent_time, latency );

            if ( m_elements.size( ) < 128 )
                return;

            m_elements.pop_front( );
        }
        

		void on_new_event( valve::game_event_t* const event );
	};

	inline const auto g_shots = std::make_unique < c_shots >( );

    struct log_data_t {
        float m_creation_time = 0.f;
        float m_text_alpha = 0.0f;
        float m_back_alpha = 50.0f;
        float m_spacing = 0.0f;

        std::string m_string = "";
        std::string m_icon = "";

        sdk::col_t m_color;

        bool m_printed = false;
    };

    class c_logs {
    public:
        void draw_data( );
        void push_log( std::string log, std::string icon, sdk::col_t color );

    private:
        std::deque < log_data_t > m_logs;
    };

    inline std::unique_ptr < c_logs > g_logs = std::make_unique < c_logs >( );

    class c_shot_record {
    public:
        __forceinline c_shot_record( ) : m_record{}, m_shot_time{}, m_lat{}, m_damage{}, m_pos{}, m_impacted{}, m_hurt{}, m_confirmed{}, m_hitbox{}, m_matrix{} {}

    public:
        std::shared_ptr < lag_record_t > m_record;
        float      m_shot_time, m_lat, m_damage;
        sdk::vec3_t     m_pos;
        sdk::vec3_t m_server_pos;
        int		   m_hitbox;
        bool       m_impacted;
        bool	   m_hurt;
        bool	   m_confirmed;
        float	   m_weapon_range;
        float      m_sent_time{};
        valve::bones_t m_matrix;
        int m_cmd_num{ -1 };
        aim_target_t* m_target{};
    };

    class c_shot_construct {
    public:
        void on_rage_bot( aim_target_t* target, float damage, int bullets, std::shared_ptr < lag_record_t > record, int hitbox, const sdk::vec3_t& shoot_pos, int cmd_number );

        void on_impact( valve::game_event_t* evt );

        void on_hurt( valve::game_event_t* evt );

        void on_render_start( );

        void on_fire( valve::game_event_t* evt );

    public:
        std::array< std::string, 11 > m_groups = {
       ( "body" ),
       ( "head" ),
       ( "chest" ),
       ( "stomach" ),
       ( "left arm" ),
       ( "right arm" ),
       ( "left leg" ),
       ( "right leg" ),
       ( "neck" ),
       ( "unknown" ),
       ( "gear" )
        };

        std::deque< c_shot_record >          m_shots;

    };

    inline const auto g_shot_construct = std::make_unique < c_shot_construct >( );
}

#include "impl/shots.inl"