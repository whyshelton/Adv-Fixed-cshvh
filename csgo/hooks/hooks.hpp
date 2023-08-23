#pragma once

namespace hooks {
#pragma region ui
    LRESULT __stdcall wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
    inline decltype( &wnd_proc ) o_wnd_proc{};

    long D3DAPI dx9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params );
    inline decltype( &dx9_reset ) o_dx9_reset{};

    long D3DAPI dx9_present( IDirect3DDevice9* device,
        RECT* src_rect, RECT* dst_rect, HWND dst_wnd_override, RGNDATA* dirty_region
    );
    inline decltype( &dx9_present ) o_dx9_present{};

    void __fastcall lock_cursor( std::uintptr_t ecx, std::uintptr_t edx );
    inline decltype( &lock_cursor ) o_lock_cursor{};
#pragma endregion

#pragma region client
    void __stdcall create_move_proxy( int seq_number, float input_sample_frame_time, bool active );
    void __stdcall create_move( int seq_number, float input_sample_frame_time, bool active, bool& send_packet );

    void __fastcall physics_simulate( valve::cs_player_t* const ecx, const std::uintptr_t edx );
    inline decltype( &physics_simulate ) o_physics_simulate{};

    void __cdecl velocity_modifier( valve::recv_proxy_data_t* const data, valve::base_entity_t* const entity, void* const out );
    inline decltype( &velocity_modifier ) o_velocity_modifier{};

    bool __fastcall svc_msg_voice_data( std::uintptr_t ecx, std::uintptr_t edx, void* msg );
    inline decltype( &svc_msg_voice_data ) o_svc_msg_voice_data;

    bool __fastcall write_user_cmd_delta_to_buffer(
        const std::uintptr_t ecx, const std::uintptr_t edx,
        const int slot, valve::bf_write_t* const buffer, int from, int to, const bool is_new_cmd
    );
    inline decltype( &write_user_cmd_delta_to_buffer ) o_write_user_cmd_delta_to_buffer{};

    void __fastcall clamp_bones_in_box( 
        valve::cs_player_t* ecx, void* edx, sdk::mat3x4_t* matrix, int mask 
    );
    inline decltype ( &clamp_bones_in_box ) o_clamp_bones_in_box;

    void __fastcall on_latch_interpolated_vars(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, const int flags
    );
    inline decltype ( &on_latch_interpolated_vars ) o_on_latch_interpolated_vars{};

    void __stdcall frame_stage_notify( const valve::e_frame_stage stage );
    inline decltype( &frame_stage_notify ) o_frame_stage_notify{};

    void __fastcall player_move( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype( &player_move ) o_player_move{};

    void __fastcall do_extra_bones_processing(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, int a0, int a1, int a2, int a3, int a4, int a5
    );
    inline decltype( &do_extra_bones_processing ) o_extra_bones_processing{};

    bool __cdecl glow_effect_spectator(
        valve::cs_player_t* player, valve::cs_player_t* local,
        int& style, sdk::vec3_t& clr, float& alpha_from, float& alpha_to,
        float& time_from, float& time_to, bool& animate
    );
    inline decltype( &glow_effect_spectator ) o_glow_effect_spectator{};

    void __fastcall accumulate_layers(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, int a0, int a1, float a2, int a3
    );
    inline decltype ( &accumulate_layers ) o_accumulate_layers{};

    void __fastcall standard_blending_rules(
        valve::cs_player_t* const ecx, const std::uintptr_t edx, valve::model_data_t* const mdl_data, int a1, int a2, float a3, int mask
    );
    inline decltype ( &standard_blending_rules ) o_standard_blending_rules{};

    void __fastcall update_client_side_anim( valve::cs_player_t* const ecx, const std::uintptr_t edx );
    inline decltype ( &update_client_side_anim ) o_update_client_side_anim{};

    bool __fastcall setup_bones(
        const std::uintptr_t ecx, const std::uintptr_t edx, sdk::mat3x4_t* const bones, int max_bones, int mask, float time
    );
    inline decltype ( &setup_bones ) o_setup_bones{};

    void interpolate_server_entities( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype ( &interpolate_server_entities ) o_interpolate_server_entities{};

    void __fastcall calc_view( 
        void* ecx, const std::uintptr_t edx, sdk::vec3_t& eye_origin, const sdk::qang_t& eye_ang, float& z_near, float& z_far, float& fov 
    );
    inline decltype( &calc_view ) o_calc_view{};

    void __fastcall calc_viewmodel_bob( valve::cs_player_t* ecx, const std::uintptr_t edx, sdk::vec3_t& view_bob );
    inline decltype ( &calc_viewmodel_bob ) o_calc_viewmodel_bob{};

    void __fastcall modify_eye_pos( valve::anim_state_t* ecx, std::uintptr_t edx, sdk::vec3_t& pos );
    inline decltype ( &modify_eye_pos ) o_modify_eye_pos{};

    bool __fastcall should_draw_view_model( std::uintptr_t ecx, std::uintptr_t edx );
    inline decltype ( &should_draw_view_model ) o_should_draw_view_model{};

    struct event_listener_t : public valve::base_event_listener_t {
        void fire_game_event( valve::game_event_t* const event ) override;
    } inline g_event_listener{};

    int process_interpolated_list( );
    inline decltype ( &process_interpolated_list ) o_process_interp_list{};

    using o_create_move_t = void( __thiscall* )( valve::c_client* const, int, float, bool );
    inline o_create_move_t o_create_move{};
#pragma endregion

#pragma region engine
    void __fastcall packet_start(
        const std::uintptr_t ecx, const std::uintptr_t edx, const int in_seq, int out_acked
    );
    inline decltype ( &packet_start ) o_packet_start{};

    void __fastcall packet_end( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype ( &packet_end ) o_packet_end{};

    bool __fastcall process_temp_entities( const std::uintptr_t ecx, const std::uintptr_t edx, const std::uintptr_t msg
    );
    inline decltype ( &process_temp_entities ) o_process_temp_entities{};

    bool __cdecl is_using_debug_static_props( );
    inline decltype ( &is_using_debug_static_props ) o_is_using_debug_static_props;

    float __stdcall aspect_ratio( int width, int height );
    inline decltype ( &aspect_ratio ) o_aspect_ratio{ };

    bool __fastcall is_paused( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype( &is_paused ) o_is_paused{};

    bool __fastcall is_hltv( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype ( &is_hltv ) o_is_hltv{ };

    int __fastcall list_leaves_in_box(
        const std::uintptr_t ecx, const std::uintptr_t edx,
        const sdk::vec3_t& mins, const sdk::vec3_t& maxs, const uint16_t* const list, const int max
    );
    inline decltype( &list_leaves_in_box ) o_list_leaves_in_box{};

    void __cdecl cl_move( float accumulate_extra_samples, bool final_tick );
    inline decltype ( &cl_move ) o_cl_move{};
#pragma endregion

#pragma region movement
    void __fastcall process_movement( std::uintptr_t ecx, std::uintptr_t edx, valve::cs_player_t* player, valve::move_data_t* move_data
    );
    inline decltype ( &process_movement ) o_process_movement{};
#pragma endregion

#pragma region client_mode
    void __fastcall override_view(
        const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
    );
    inline decltype ( &override_view ) o_override_view{};

    int __fastcall do_post_screen_space_effects(
        const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
    );
    inline decltype( &do_post_screen_space_effects ) o_do_post_screen_space_effects{};
#pragma endregion

#pragma region render
    void __fastcall draw_model( valve::studio_render_t* ecx, std::uintptr_t edx, std::uintptr_t results, const valve::draw_model_info_t& info,
        sdk::mat3x4_t* bones, float* flex_weights, float* flex_delayed_weights, const sdk::vec3_t& origin, int flags
    );
    inline decltype( &draw_model ) o_draw_model{};
#pragma endregion

#pragma region panel
    void __fastcall paint_traverse( const std::uintptr_t ecx, const std::uintptr_t edx,
        const std::uint32_t id, bool force_repaint, bool allow_force
    );
    inline decltype ( &paint_traverse ) o_paint_traverse;
#pragma endregion

#pragma region material_system 
    void __fastcall get_color_modulation( void* ecx, void* edx, float* red, float* green, float* blue );
    inline decltype ( &get_color_modulation ) o_get_clr_modulation{};

    float __fastcall get_alpha_modulation( void* ecx, void* edx );
    inline decltype ( &get_alpha_modulation ) o_get_alpha_modulation{};
#pragma endregion 

#pragma region mdl_render
    void __stdcall draw_mdl_exec(
        uintptr_t ctx, const valve::draw_model_state_t& state, const valve::model_render_info_t&, sdk::mat3x4_t* bones
    );
    using o_draw_mdl_exec_t = void( __thiscall* )( void*, uintptr_t, const valve::draw_model_state_t&, const valve::model_render_info_t&, sdk::mat3x4_t* );
    inline o_draw_mdl_exec_t o_draw_mdl_exec{};
#pragma endregion

#pragma region prediction

    bool __stdcall in_prediction( );

    using fn_t = bool( __thiscall* ) ( void* );
    inline fn_t o_in_prediction{};

#pragma endregion
}