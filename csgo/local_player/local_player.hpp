#pragma once

class c_local_player {
private:
    struct net_data_t {
        std::ptrdiff_t m_cmd_number{}, m_prev_cmd_number{};
        bool m_is_outgoing{}, m_is_used{};

        ALWAYS_INLINE void add_net_cmd( const std::ptrdiff_t cmd_number, const bool& send_packet );

        ALWAYS_INLINE const void process_net( const bool& send_packet ) const;
    } m_net_data{};

    struct prediction_t {
    private:
        struct prediction_backup_t {
            __forceinline prediction_backup_t( ) = default;

            float m_cur_time{}, m_frame_time{};
            bool m_in_prediction{}, m_first_time_predicted{};
        } m_prediction_backup{};

        struct r8_t {
            ALWAYS_INLINE r8_t( ) = default;

            ALWAYS_INLINE r8_t( const int tick, const bool in_attack, const bool can_shoot ) {
                m_tick = tick;
                m_in_attack = in_attack;
                m_can_shoot = can_shoot;
            }

            int m_tick{};
            bool m_in_attack{};
            bool m_can_shoot{};
        };

        struct view_model_t {
            int m_view_model_index, m_animation_parity;
            float m_view_cycle, m_anim_time;
            int m_view_sequence;

            ALWAYS_INLINE const void store_view_model( );
            ALWAYS_INLINE const void restore_view_model( );
        } m_view_model{};

        struct data_t {
            __forceinline data_t( ) = default;
            struct local_t {
                ALWAYS_INLINE void init( const valve::user_cmd_t& user_cmd );

                ALWAYS_INLINE void save( const int cmd_number );

                ALWAYS_INLINE void store_anim( );

                ALWAYS_INLINE void restore_anim( const bool layers ) const;

                float m_spawn_time{}, m_abs_yaw{};

                int m_adjusted_tick_base{}, m_tick_base{}, m_shift_amount{};

                bool m_override_tick_base{}, m_restore_tick_base{};

                sdk::vec3_t m_move{};

                valve::user_cmd_t m_user_cmd{};

                sdk::qang_t                m_choke_angle{};
                valve::anim_state_t		m_anim_state{};

                valve::anim_layers_t	m_anim_layers{};
                valve::pose_params_t	m_pose_params{};

                struct {
                    sdk::qang_t 			m_eye_angles{},
                        m_aim_punch{}, m_view_punch{}, m_aim_punch_vel{};

                    sdk::vec3_t				m_origin{}, m_abs_origin{},
                        m_obb_min{}, m_obb_max{},
                        m_velocity{}, m_view_offset{}, m_abs_velocity{};

                    bool				m_scoped{}, m_walking{};

                    int					m_tick_base{};

                    float				m_duck_amount{}, m_duck_speed{},
                        m_third_person_recoil{}, m_lby{}, m_fall_velocity{}, m_velocity_modifier{};

                    valve::e_ent_flags	m_flags{};
                    valve::e_move_type	m_move_type{};
                }						m_net_vars{}, m_pred_net_vars{};
            };
        private:
            struct net_vars_t {
                ALWAYS_INLINE const void store( const std::ptrdiff_t number );
                ALWAYS_INLINE const void restore( const std::ptrdiff_t number ) const;

                int m_cmd_number{};
                float		m_view_offset_z{}, m_velocity_modifier{}, m_fall_velocity{};
                sdk::qang_t	m_aim_punch{}, m_aim_punch_vel{}, m_view_punch{};
                sdk::vec3_t      m_velocity{}, m_origin{};

                r8_t        m_r8{};
            };

            float m_net_velocity_modifier{ 1.f }, m_spread{}, m_inaccuracy{}, m_min_inaccuracy{},
                m_velocity_modifier{}, m_recoil_index{}, m_accuracy_penalty{}, m_last_shot_time{},
                m_postpone_fire_ready_time{};

            bool m_is_out_of_epsilon{};

            valve::move_data_t m_move_data{};

            std::array < local_t, 150u > m_local_data{};

            std::array < net_vars_t, 150u > m_net_vars{};

            valve::bones_t m_shoot_bones{};

            sdk::qang_t m_shot_angles{};

            bool m_shot_valid_wpn{}, m_shot{}, m_old_shot{}, m_r8_can_shoot{}, m_shot_this_cycle{};

            std::ptrdiff_t m_shot_cmd_number{};

        public:

            ALWAYS_INLINE std::array < local_t, 150u >& local_data( );

            ALWAYS_INLINE std::array < net_vars_t, 150u >& net_vars( );

            ALWAYS_INLINE float& spread( );

            ALWAYS_INLINE float& inaccuracy( );

            ALWAYS_INLINE float& min_inaccuracy( );

            ALWAYS_INLINE sdk::qang_t& shot_angles( );

            ALWAYS_INLINE bool& shot_this_cycle( );

            ALWAYS_INLINE bool& is_out_of_epsilon( );

            ALWAYS_INLINE valve::move_data_t& move_data( );

            ALWAYS_INLINE float& net_velocity_modifier( );

            ALWAYS_INLINE float& recoil_index( );

            ALWAYS_INLINE float& accuracy_penalty( );

            ALWAYS_INLINE float& velocity_modifier( );

            ALWAYS_INLINE valve::bones_t& shoot_bones( );

            ALWAYS_INLINE bool& shot_valid_wpn( );

            ALWAYS_INLINE std::ptrdiff_t& shot_cmd_number( );

            ALWAYS_INLINE float& last_shot_time( );

            ALWAYS_INLINE bool& shot( );

            ALWAYS_INLINE bool& old_shot( );

            ALWAYS_INLINE bool& r8_can_shoot( );

            ALWAYS_INLINE float& postpone_fire_ready_time( );
        } m_data{};
    public:

        void start_prediction( );

        void process_prediction( valve::user_cmd_t* const user_cmd );

        void end_prediction( );

        ALWAYS_INLINE void correct_view_offset( );

        ALWAYS_INLINE void velocity_modifier_to_data_map( );

        ALWAYS_INLINE void third_person_recoil_to_data_map( );

        ALWAYS_INLINE data_t& data( );

        ALWAYS_INLINE view_model_t& view_model( );

        ALWAYS_INLINE void reset( );

        void update_shoot_pos( const valve::user_cmd_t& user_cmd );
    } m_prediction{};

    struct anim_sync_t {
        struct anim_data_t {
            struct fake_t {
                float m_spawn_time{};

                valve::bones_t m_bones{};

                valve::anim_layers_t m_anim_layers{};

                valve::pose_params_t m_pose_params{};

                float m_abs_yaw{};

                valve::anim_state_t m_anim_state{};

            } m_fake{};

            struct real_t {
                float m_spawn_time{};

                valve::bones_t m_bones{};

                valve::anim_layers_t m_anim_layers{};

                valve::pose_params_t m_pose_params{};

                float m_abs_yaw{};

                valve::anim_state_t m_anim_state{};
            } m_real{};

            ALWAYS_INLINE void reset( ) {
                m_fake.m_spawn_time = m_real.m_spawn_time = {};
                m_fake.m_bones = m_real.m_bones = {};
                m_fake.m_anim_layers = m_real.m_anim_layers = {};
                m_fake.m_pose_params = m_real.m_pose_params = {};
                m_fake.m_abs_yaw = m_real.m_abs_yaw = {};
                m_fake.m_anim_state = m_real.m_anim_state = {};

                m_move_type[ 0 ] = m_move_type[ 1 ] = {};
                m_flags[ 0 ] = m_flags[ 1 ] = {};
            }

            valve::e_move_type m_move_type[ 2 ];
            valve::e_ent_flags m_flags[ 2 ];

        } m_anim_data{};

        ALWAYS_INLINE bool setup_bones(
            valve::bones_t& bones, const int time, const int flags
        );

        ALWAYS_INLINE void update_local( const sdk::qang_t& view_angles, const bool no_view_model, const float time );

        void update_local_real( valve::user_cmd_t& user_cmd, bool send_packet );

        void setup_local_bones( );

        void do_animation_event( const std::ptrdiff_t const type );

    } m_anim_sync{};

    struct anti_aim_t {
        ALWAYS_INLINE anti_aim_t( ) = default;

        ALWAYS_INLINE void handle( valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds );

        ALWAYS_INLINE void select_yaw( float& yaw, const int side, valve::user_cmd_t& user_cmd );

        ALWAYS_INLINE int select_side( );

        ALWAYS_INLINE bool freestanding( float& yaw );

        ALWAYS_INLINE void at_target( float& yaw ) const;

        ALWAYS_INLINE float manual_yaw( );

        bool should_run( valve::user_cmd_t& user_cmd );

        ALWAYS_INLINE void fake_move( valve::user_cmd_t& user_cmd );

        void choke( bool& send_packet );
    private:
        int m_auto_dir_side{};

    } m_anti_aim{};

    struct net_info_t {
        float	m_lerp{};

        struct {
            float m_in{}, m_out{};
        }		m_latency{};
    }						m_net_info{};


    valve::weapon_cs_base_gun_t*    m_weapon{};
    valve::weapon_info_t*           m_weapon_info{};

    std::vector< int > m_net_cmds{};

    std::ptrdiff_t m_cur_seq{};

    sdk::vec3_t m_shoot_pos{};

    bool m_can_shoot{}, m_left_create_move{}, m_will_shoot{};

    valve::e_frame_stage m_last_frame_stage{};

    valve::user_cmd_t m_last_user_cmd{};

    int m_last_sent_cmd_number{}, m_last_cmd_number{};
  
public:
    void create_move( bool& send_packet,
        valve::user_cmd_t& cmd, valve::vfyd_user_cmd_t& vfyd_cmd
    );

    ALWAYS_INLINE std::ptrdiff_t& cur_seq( );

    ALWAYS_INLINE prediction_t& prediction( );

    ALWAYS_INLINE bool& can_shoot( );

    ALWAYS_INLINE bool can_shoot (
        bool skip_r8, const int shift_amt, const bool what
    );

    ALWAYS_INLINE anim_sync_t& anim_sync( );

    ALWAYS_INLINE net_info_t& net_info( );

    ALWAYS_INLINE sdk::vec3_t& shoot_pos( );

    ALWAYS_INLINE std::vector< int >& net_cmds( );

    ALWAYS_INLINE anti_aim_t& anti_aim( );

    ALWAYS_INLINE net_data_t& net_data( );

    ALWAYS_INLINE valve::cs_player_t* self( ) const;

    ALWAYS_INLINE valve::weapon_cs_base_gun_t* weapon( ) const;

    ALWAYS_INLINE valve::weapon_info_t* weapon_info( ) const;

    ALWAYS_INLINE valve::e_frame_stage& last_frame_stage( );

    ALWAYS_INLINE valve::user_cmd_t& last_user_cmd( );

    ALWAYS_INLINE bool& left_create_move( );

    ALWAYS_INLINE int& last_sent_cmd_number( );

    ALWAYS_INLINE int& last_cmd_number( );

    ALWAYS_INLINE bool will_shoot(
        valve::base_weapon_t* const weapon, const valve::user_cmd_t& user_cmd 
    ) const;
};

inline const auto g_local_player = std::make_unique< c_local_player >( );

#include "impl/local_player.inl"