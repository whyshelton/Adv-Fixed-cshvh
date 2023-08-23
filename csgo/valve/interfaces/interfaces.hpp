#pragma once
#include "random.h"
namespace valve {
    class c_client {
    public:
        VFUNC( client_class_t*( __thiscall* )( decltype( this ) ), all_classes( ), 8u );
    } inline* g_client{};

    class c_engine {
    public:
        VFUNC( bool( __thiscall* )( decltype( this ), int, player_info_t* ),
            get_player_info( int index, player_info_t* info ), 8u, index, info
        );

        VFUNC( int( __thiscall* )( decltype( this ), int ), index_for_uid( int uid ), 9u, uid );

        VFUNC( int( __thiscall* ) ( decltype ( this ) ), get_local_player( ), 12u );

        VFUNC( bool( __thiscall* )( decltype( this ) ), in_game( ), 26u );

        VFUNC( void( __thiscall* )( decltype ( this ) ), fire_events( ), 59u );

        VFUNC( sdk::qang_t( __thiscall* )( decltype( this ) ), view_angles( ), 18u );

        VFUNC( int( __thiscall* )( decltype( this ) ), server_tick( ), 208u );

        VFUNC( void( __thiscall* ) ( decltype ( this ), int&, int& ),
            screen_size( int& w, int& h ), 5u, w, h );

        ALWAYS_INLINE void exec_cmd( const char* cmd ) {
            using fn_t = void( __thiscall* )( decltype( this ), const char* );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 108u ]( this, cmd );
        }

        ALWAYS_INLINE net_chan_info_t* c_engine::net_channel_info( ) {
            using fn_t = net_chan_info_t * ( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 78u ]( this );
        }

        ALWAYS_INLINE std::uintptr_t* bsp_tree_query( ) {
            using fn_t = std::uintptr_t* ( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 43u ]( this );
        }

    };

    inline cc_def( c_engine* ) g_engine { nullptr };

    class c_mdl_cache {
    public:
        VFUNC( std::uintptr_t( __thiscall* ) ( decltype ( this ), std::uint16_t ), lookup_hw_data( std::uint16_t handle ), 15u, handle );
    } inline* g_mdl_cache{};

    struct debug_overlay_t {
        __forceinline void add_box_overlay(
            const sdk::vec3_t& pos, const sdk::vec3_t& min, const sdk::vec3_t& max, const sdk::qang_t& rotation,
            const int r, const int g, const int b, const int a, const float duration
        ) {
            using fn_t = void( __thiscall* )( decltype( this ),
                const sdk::vec3_t&, const sdk::vec3_t&, const sdk::vec3_t&, const sdk::qang_t&,
                const int, const int, const int, const int, const float
                );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ](
                this, pos, min, max, rotation, r, g, b, a, duration
                );
        }

    } inline* g_debug_overlay{};

    class c_model_info {
    public:
        VFUNC( int( __thiscall* ) ( decltype ( this ), const char* ), model_index( const char* name ), 2u, name );
    } inline* g_model_info{};

    class c_mdl_render {
    public:
        VFUNC( bool( __thiscall* ) ( decltype ( this ) ), is_forced_mat_override( ), 2u );
        VFUNC( void( __thiscall* ) ( decltype ( this ), std::uintptr_t, const draw_model_state_t&, const model_render_info_t&, sdk::mat3x4_t* ),
            draw_mdl_exec( std::uintptr_t ctx, const draw_model_state_t& mdl_state, const model_render_info_t& mdl_render, sdk::mat3x4_t* bones ),
            21u, ctx, mdl_state, mdl_render, bones );
    } inline* g_mdl_render{};

    struct render_view_t {
    public:
        virtual void draw_brush_mdl( void*, valve::model_t* mdl, const sdk::vec3_t& origin, const sdk::qang_t& angle, bool sort ) = 0;
        virtual void draw_identity_brush_model( void* who, valve::model_t* mdl ) = 0;
        virtual void touch_light( void* idi_naxui ) = 0;
        virtual void draw_3d_debug_overlays( void ) = 0;
        virtual void set_blend( float blend ) = 0;

    } inline* g_render_view{};

    typedef void* file_name_handle_t;
    struct snd_info_t {
        int			m_guid;
        file_name_handle_t m_file_name_handle;
        int			m_sound_src;
        int			m_channel;
        int			m_speaker_entity;
        float		m_volume;
        float		m_last_spatialized_volume;
        float		m_radius;
        int			m_pitch;
        sdk::vec3_t* m_origin;
        sdk::vec3_t* m_dir;
        bool		m_update_pos;
        bool		m_in_sentence;
        bool		m_dry_mix;
        bool		m_speaker;
        bool		m_from_server;
    };

    struct engine_sound_t {
        __forceinline void get_act_sounds( utl_vec_t < snd_info_t >& snd_list ) {
            using fn_t = void( __thiscall* )( decltype( this ), utl_vec_t < snd_info_t >& );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 19u ]( this, snd_list );
        };

    } inline* g_engine_sound{};

    class c_entity_list {
    public:
        VFUNC( base_entity_t*( __thiscall* )( decltype( this ), int ), get_entity( int index ), 3u, index );

        VFUNC( base_entity_t*( __thiscall* )( decltype( this ), ent_handle_t ), get_entity( ent_handle_t handle ), 4u, handle );

        VFUNC( std::ptrdiff_t( __thiscall* ) ( decltype ( this ) ), highest_index( ), 6u );
    };

    inline cc_def( c_entity_list* ) g_entity_list { nullptr };

    struct global_vars_base_t {
        float           m_real_time{};
        int             m_frame_count{};

        float           m_abs_frame_time{},
                        m_abs_frame_start_time_std_dev{},
                        m_cur_time{},
                        m_frame_time{};

        int             m_max_clients{}, m_tick_count{};
        float           m_interval_per_tick{}, m_interp_amt{};
        int             m_sim_ticks_this_frame{}, m_network_protocol{};

        sdk::address_t  m_save_data{};
        bool            m_client{}, m_remote_client{};
    };

    inline cc_def( global_vars_base_t* ) g_global_vars { nullptr };

    struct client_state_t {
        struct net_chan_t {
            VFUNC( float( __thiscall* )( decltype( this ), e_net_flow ), latency( const e_net_flow flow ), 9u, flow );

            VFUNC( int( __thiscall* )( decltype( this ), std::uintptr_t ), send_datagram( ), VARVAL( 48u, 46u ), 0u );
            VFUNC( bool( __thiscall* )( decltype ( this ), void*, bool, bool ), send_net_msg( void* msg, bool force_reliable = false, bool voice = false ), VARVAL( 42u, 42u ), msg, force_reliable, voice );

            std::uint8_t    pad0[ 20u ]{};
            bool            m_processing_messages{},
                            m_should_delete{};
            std::uint8_t    pad1[ 2u ]{};
            int             m_out_seq{},
                            m_in_seq{},
                            m_out_seq_ack{},
                            m_out_rel_state{},
                            m_in_rel_state{},
                            m_choked_packets{};
            std::uint8_t    pad2[ 1044u ]{};
        };

        std::uint8_t    pad0[ 156u ]{};
        net_chan_t*     m_net_chan{};
        int             m_challenge_number{};
        std::uint8_t    pad1[ 4u ]{};
        double          m_connect_time{};
        int             m_retry_number{};
        std::uint8_t    pad2[ 84u ]{};
        int             m_signon_state{};
        std::uint8_t    pad3[ 4u ]{};
        double          m_next_cmd_time{};
        int             m_server_count{},
                        m_cur_seq{};
        std::uint8_t    pad4[ 8u ]{};
        float           m_clock_offsets[ 16u ]{};
        int             m_cur_clock_offset{},
                        m_server_tick{},
                        m_client_tick{},
                        m_delta_tick{};
        std::uint8_t    pad5[ 4u ]{};
        int             m_view_entity{},
                        m_player_slot{};
        bool            m_paused{};
        std::uint8_t    pad6[ 3u ]{};
        char            m_level_name[ 260u ]{},
                        m_level_name_short[ 40u ]{};
        std::uint8_t    pad7[ VARVAL( 92u, 212u ) ]{};
        int             m_max_clients{};
        std::uint8_t    pad8[ VARVAL( 18828u, 18836u ) ]{};
        int             m_old_tick_count{};
        float           m_tick_remainder{},
                        m_frame_time{};
        int             m_last_cmd_out{},
                        m_choked_cmds{},
                        m_last_cmd_ack{},
                        m_last_server_tick{},
                        m_cmd_ack{},
                        m_sound_seq{},
                        m_last_progress_percent{};
        bool            m_is_hltv{};
        std::uint8_t    pad9[ 75u ]{};
        sdk::qang_t     m_view_angles{};
        std::uint8_t    pad10[ 208u ]{};
    } inline* g_client_state{};

    struct input_t {
        ALWAYS_INLINE user_cmd_t* user_cmd( const int slot, const int seq_number ) {
            using fn_t = user_cmd_t * ( __thiscall* )( decltype( this ), const int, const int );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 8u ]( this, slot, seq_number );
        }

        std::uint8_t        pad0[ VARVAL( 4u, 12u ) ]{};
        bool                m_track_ir_available{},
                            m_mouse_initialized{},
                            m_mouse_active{};
        std::uint8_t        pad1[ VARVAL( 158u, 154u ) ]{};
        bool                m_camera_in_third_person{};
        std::uint8_t        pad2[ VARVAL( 1u, 2u ) ]{};
        sdk::vec3_t         m_camera_offset{};
        std::uint8_t        pad3[ VARVAL( 54u, 56u ) ]{};
        user_cmd_t*         m_cmds{};
        vfyd_user_cmd_t*    m_vfyd_cmds{};
    } inline* g_input{};

    class c_cvar {
    public:
        VFUNC( cvar_t*( __thiscall* )( decltype( this ), const char* ), find_var( const char* name ), 16u, name );

        ALWAYS_INLINE void con_print( const bool notify, const std::uint8_t& clr, const char* str ) {
            using fn_t = void( __cdecl* )( decltype( this ), const std::uint8_t&, const char*, ... );

            using add_notify_t = void( __stdcall* )( const std::uint8_t&, const char* );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 25u ]( this, clr, str );
        }

    } inline* g_cvar{};

    struct base_player_t;

    class c_move_helper {
    public:
        VFUNC( void( __thiscall* )( decltype( this ), base_player_t* ), set_host( base_player_t* player ), 1u, player );
    } inline* g_move_helper{};

    struct prediction_t {
        VFUNC( void( __thiscall* )( decltype( this ), int, bool, int, int ),
            update( int start, bool valid, int in_ack, int out_cmd ), 3u, start, valid, in_ack, out_cmd
        );

        VFUNC( void( __thiscall* )( decltype( this ), base_player_t*, user_cmd_t*, c_move_helper*, move_data_t* ),
            setup_move( base_player_t* player, user_cmd_t* cmd, c_move_helper* move_helper, move_data_t* move_data ),
            20u, player, cmd, move_helper, move_data
        );
       
        VFUNC( void( __thiscall* )( decltype( this ), base_player_t*, user_cmd_t*, move_data_t* ),
            finish_move( base_player_t* player, user_cmd_t* cmd, move_data_t* move_data ), 21u, player, cmd, move_data
        );

        std::uint8_t    pad0[ 8u ]{};
        bool            m_in_prediction{};
        std::uint8_t    pad1{};
        bool            m_engine_paused{},
                        m_old_cl_predict_value{};
        int             m_prev_start_frame{},
                        m_incoming_packet_number{};
        float           m_last_server_world_time_stamp{};
        bool            m_first_time_predicted{};
        std::uint8_t    pad2[ 3u ]{};
        int             m_cmds_predicted{},
                        m_server_cmds_acked{};
        bool            m_prev_ack_had_errors{};
    };

    inline cc_def( prediction_t* ) g_prediction { nullptr };

    class c_movement {
    public:
        inline base_player_t*& player( ) {
            return *( base_player_t** ) ( ( DWORD ) ( this ) + 0x4 );
        };

        VFUNC( void( __thiscall* )( decltype( this ), base_player_t*, move_data_t* ),
            process_movement( base_player_t* player, move_data_t* move_data ), 1u, player, move_data
        );

        VFUNC( sdk::vec3_t& ( __thiscall* )( decltype( this ), bool ),
            player_view_offset( bool ducked ), 8u, ducked );
        
        VFUNC( void( __thiscall* )( decltype( this ), base_player_t* ),
            start_track_pred_errors( base_player_t* player ), 3u, player
        );

        VFUNC( void( __thiscall* )( decltype( this ), base_player_t* ),
            finish_track_pred_errors( base_player_t* player ), 4u, player
        );
    };

    inline cc_def( c_movement* ) g_movement { nullptr };

    struct studio_render_t {
        VFUNC( void( __thiscall* )( decltype( this ), float* ), set_clr_mod( float* value ), 27u, value );
        VFUNC( void( __thiscall* )( decltype( this ), float ), set_alpha_mod( float value ), 28u, value );

        VFUNC( void( __thiscall* )( decltype( this ), material_t*, e_override_type, int ),
            forced_material_override(
                material_t* material = nullptr,
                e_override_type type = e_override_type::normal,
                int material_index = 0
            ),
            33u, material, type, material_index
        );

        ALWAYS_INLINE bool is_forced_material_override( );

        std::uint8_t    pad0[ 592u ]{};
        material_t*     m_override_material{};
        std::uint8_t    pad1[ 12u ]{};
        e_override_type m_override_type{};
    } inline* g_studio_render{};

    class c_game_event_mgr {
    public:
        __forceinline void add_listener( base_event_listener_t* listener, const char* event_name, const bool server_side ) {
            using fn_t = void( __thiscall* )( decltype( this ), base_event_listener_t*, const char*, const bool );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ]( this, listener, event_name, server_side );
        }
    } inline* g_game_event_mgr{};

    class c_surface_data {
    public:
        VFUNC( surface_data_t* ( __thiscall* )( decltype( this ), int ), get( int index ), 5u, index );
    } inline* g_surface_data{};

    class c_material_system {
    public:
        VFUNC( material_t* ( __thiscall* )( decltype( this ), const char*, key_values_t* ),
            create_material( const char* name, key_values_t* key_values ), 83u, name, key_values
        );
        
        VFUNC( valve::material_t* ( __thiscall* ) ( decltype ( this ), const char*, const char*, bool, const char* ),
            find_mat( const char* name, const char* group, bool complain = true, const char* complain_prefix = nullptr ), 84u,
            name, group, complain, complain_prefix );

        ALWAYS_INLINE std::uintptr_t* render_context( ) {
            using fn_t = std::uintptr_t* ( __thiscall* )( decltype( this ) );

            return ( *reinterpret_cast< fn_t** >( this ) )[ 115u ]( this );
        }

    } inline* g_material_system{};

    class c_panel {
    public:
        VFUNC( const char* ( __thiscall* ) ( decltype ( this ), std::uint32_t ), get_name( std::uint32_t unk ), 36, unk );
    } inline* g_panel{};

    struct base_entity_t;

    struct view_render_t {
        char			pad0[ 4u ];
        view_setup_t	m_setup{};
    } inline* g_view_render{};

    class c_engine_trace {
    public:
        VFUNC( e_mask( __thiscall* )( decltype( this ), const sdk::vec3_t&, e_mask, base_entity_t** ),
            get_point_contents( const sdk::vec3_t& point, e_mask mask, base_entity_t** entity = nullptr ), 0u, point, mask, entity
        );

        VFUNC( void( __thiscall* )( decltype( this ), const ray_t&, e_mask, base_entity_t*, trace_t* ),
            clip_ray_to_entity( const ray_t& ray, e_mask mask, base_entity_t* entity, trace_t* trace ), 3u, ray, mask, entity, trace
        );

        VFUNC( void( __thiscall* )( decltype( this ), const ray_t&, e_mask, base_trace_filter_t*, trace_t* ),
            trace_ray( const ray_t& ray, e_mask mask, base_trace_filter_t* filter, trace_t* trace ), 5u, ray, mask, filter, trace
        );

        VFUNC( void( __thiscall* )( decltype( this ), const ray_t&, std::uint32_t, base_trace_filter_t*, trace_t* ),
            trace_ray_( const ray_t& ray, std::uint32_t mask, base_trace_filter_t* filter, trace_t* trace ), 5u, ray, mask, filter, trace );

    } inline* g_engine_trace{};

    class c_physics_surface_props {
    public:
        VFUNC( surface_data_t*( __thiscall* )( decltype( this ), int ), get_surface_data( int index ), 5u, index );
    } inline* g_phys_props{};

    struct game_rules_t {
        OFFSET( bool, warmup_period( ), g_ctx->offsets( ).m_game_rules.m_warmup_period );
        OFFSET( bool, freeze_period( ), g_ctx->offsets( ).m_game_rules.m_freeze_period );

        OFFSET( bool, valve_ds( ), g_ctx->offsets( ).m_game_rules.m_valve_ds );
        OFFSET( bool, bomb_planted( ), g_ctx->offsets( ).m_game_rules.m_bomb_planted );
    } inline** g_game_rules{};

    class c_game_types {
    public:
        VFUNC( e_game_type( __thiscall* )( decltype( this ) ), game_type( ), 8u );
    } inline* g_game_types{};

    ALWAYS_INLINE int to_ticks( const float time ) {
        return static_cast< int >(
            time / g_global_vars.get( )->m_interval_per_tick + 0.5f
            );
    }

    ALWAYS_INLINE float to_time( const int ticks ) {
        return ticks * g_global_vars.get( )->m_interval_per_tick;
    }
}

#include "impl/interfaces.inl"