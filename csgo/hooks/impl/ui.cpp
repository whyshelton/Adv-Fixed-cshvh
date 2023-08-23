#include "../../csgo.hpp"
#include "../csgo/security/Dependencies/Xor.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

namespace hooks {
    LRESULT __stdcall wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
        bool hold{};
        switch ( msg ) {
        case WM_CHAR:
        {
            wchar_t wch;
            MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, ( char* ) &wparam, 1, &wch, 1 );
            ImGui::GetIO( ).AddInputCharacter( wch );

            return o_wnd_proc( wnd, msg, wparam, lparam );
        }

        case WM_XBUTTONDOWN:

            switch ( GET_KEYSTATE_WPARAM( wparam ) )
            {
            case MK_XBUTTON1: g_key_binds->m_last_code = VK_XBUTTON1; break;
            case MK_XBUTTON2: g_key_binds->m_last_code = VK_XBUTTON2; break;
            }

            break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MOUSEWHEEL:
            hold = true;
            break;

        case WM_MBUTTONDOWN:

            g_key_binds->m_last_code = VK_MBUTTON;
            break;

        case WM_MOUSEMOVE:
            hold = true;
            break;

        case WM_RBUTTONDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        case WM_LBUTTONDOWN:
            hold = true;
            g_key_binds->m_last_code = wparam;
            break;
        }

        if ( msg == WM_KEYUP
            && wparam == VK_INSERT )
            g_menu->main( ).m_hidden ^= 1;

        if ( !g_menu->main( ).m_hidden && hold ) {
            ImGui_ImplWin32_WndProcHandler( wnd, msg, wparam, lparam );

            return 1;
        }

        return o_wnd_proc( wnd, msg, wparam, lparam );
    }

    long D3DAPI dx9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) {
        ImGui_ImplDX9_InvalidateDeviceObjects( );

        const auto ret = o_dx9_reset( device, params );

        ImGui_ImplDX9_CreateDeviceObjects( );

        return ret;
    }

    long D3DAPI dx9_present( IDirect3DDevice9* device,
        RECT* src_rect, RECT* dst_rect, HWND dst_wnd_override, RGNDATA* dirty_region
    ) {
        DWORD colorwrite, srgbwrite;
        IDirect3DVertexDeclaration9* vert_dec = nullptr;
        IDirect3DVertexShader9* vert_shader = nullptr;
        DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
        device->GetRenderState( D3DRS_COLORWRITEENABLE, &colorwrite );
        device->GetRenderState( D3DRS_SRGBWRITEENABLE, &srgbwrite );

        device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
        //removes the source engine color correction
        device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );

        device->GetRenderState( D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE );
        device->GetVertexDeclaration( &vert_dec );
        device->GetVertexShader( &vert_shader );
        device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
        device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
        device->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
        device->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
        device->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
        device->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );

        ImGui_ImplDX9_NewFrame( );

        ImGui_ImplWin32_NewFrame( );

        ImGui::NewFrame( );

        hacks::g_visuals->water_mark( );

        hacks::g_visuals->spectators( );

        hacks::g_visuals->key_binds( );

        g_menu->render ( );

        csgo::g_render->add_to_draw_list( );

        ImGui::EndFrame( );

        ImGui::Render( );

        ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

        device->SetRenderState( D3DRS_COLORWRITEENABLE, colorwrite );
        device->SetRenderState( D3DRS_SRGBWRITEENABLE, srgbwrite );
        device->SetRenderState( D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE );
        device->SetRenderState( D3DRS_SRGBWRITEENABLE, true );
        device->SetVertexDeclaration( vert_dec );
        device->SetVertexShader( vert_shader );


        return o_dx9_present( device, src_rect, dst_rect, dst_wnd_override, dirty_region );
    }

    void __fastcall lock_cursor( std::uintptr_t ecx, std::uintptr_t edx ) {
        using unlock_cursor_t = void( __thiscall* )( std::uintptr_t );

        if ( !g_menu->main( ).m_hidden )
            return ( *sdk::address_t{ ecx }.as< unlock_cursor_t** >( ) )[ 66u ]( ecx );

        o_lock_cursor( ecx, edx );
    }
}