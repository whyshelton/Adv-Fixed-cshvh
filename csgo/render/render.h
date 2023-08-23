#pragma once
#include <mutex>
namespace csgo {
	class c_render {
	public:

		sdk::vec2_t get_text_size ( std::string_view txt, ImFont* font );

		void text ( std::string_view txt, sdk::vec2_t pos, const sdk::col_t& clr, ImFont* font, bool should_outline, bool should_center_x, bool should_center_y, bool lower_alpha = false );

		void line ( const sdk::vec2_t& from, const sdk::vec2_t& to, const sdk::col_t& clr );
		void draw_line ( float x1, float y1, float x2, float y2, sdk::col_t clr, float thickness = 1.f );

		void rect ( const sdk::vec2_t& pos, const sdk::vec2_t& size, const sdk::col_t& clr, float rounding = 0.f, bool multiplied = false );

		void rect_filled ( const sdk::vec2_t& pos, const sdk::vec2_t& size, const sdk::col_t& clr, float rounding = 0.f, bool multiplied = false );

		void rect_filled_multi_clr ( const sdk::vec2_t& pos, const sdk::vec2_t& size, const sdk::col_t& clr_upr_left, const sdk::col_t& clr_upr_right, const sdk::col_t& clr_bot_left, const sdk::col_t& clr_bot_right );

		void polygon ( const std::vector<sdk::vec2_t>& points, const sdk::col_t& clr );

		void polygon_filled ( const std::vector<sdk::vec2_t>& points, const sdk::col_t& clr );

		void triangle ( float x1, float y1, float x2, float y2, float x3, float y3, sdk::col_t clr, float thickness );

		void triangle_filled ( float x1, float y1, float x2, float y2, float x3, float y3, sdk::col_t clr );

		void render_filled_3d_circle ( const sdk::vec3_t& origin, float radius, sdk::col_t color );

		void render_convex_poly_filled ( ImVec2* vec, int num_points, sdk::col_t col );

		void render_3d_circle ( const sdk::vec3_t& origin, float radius, sdk::col_t color );

		void arc ( float x, float y, float radius, float min_angle, float max_angle, sdk::col_t col, float thickness );

		bool world_to_screen ( const sdk::vec3_t& in, sdk::vec3_t& out );

		void draw_rect_filled ( float x, float y, float w, float h, sdk::col_t clr, float rounding, ImDrawCornerFlags rounding_corners = 15 );

		void draw_rect ( float x1, float y1, float x2, float y2, sdk::col_t color );

		__forceinline void add_to_draw_list( ) {
			const auto lock = std::unique_lock<std::mutex>( m_mutex, std::try_to_lock );
			if ( lock.owns_lock( ) ) {
				*m_replace_draw_list = *m_data_draw_list;
			}

			*ImGui::GetBackgroundDrawList( ) = *m_replace_draw_list;
		}

		std::mutex m_mutex;

		sdk::vec2_t m_screen_size;
		std::shared_ptr < ImDrawList > m_draw_list {};
		std::shared_ptr < ImDrawList > m_data_draw_list{};
		std::shared_ptr < ImDrawList > m_replace_draw_list{};

        struct fonts_t {
        private:
            ImFontConfig m_menu_font_cfg{};
            ImFontConfig m_visuals_font_cfg{};
        public:
            struct museo_sans_t {
                ImFont* m_300_26{};
                ImFont* m_300_30{};
                ImFont* m_700_39{};
                ImFont* m_300_16{};
            } m_museo_sans{};

            struct mukta_t {
                ImFont* m_mukta_44{}, * m_mukta_38{}, * m_mukta_30{}, * m_mukta_34{};
            } m_mukta{};

            struct icons_t {
                ImFont* m_icon_font{}, *m_warning_icon_font{};
            } m_icons{};

            ImFont* m_roboto_22_regular{};
            ImFont* m_verdana_15_regular{};
            ImFont* m_tahoma_14_regular{};
            ImFont* m_04b03_8{};
            ImFont* m_tahoma{};

            __forceinline void init( const ImGuiIO& io ) {
                static const ImWchar ranges[ ] =
                {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
                    0x2DE0, 0x2DFF, // Cyrillic Extended-A
                    0xA640, 0xA69F, // Cyrillic Extended-B
                    0xE000, 0xE226, // icons
                    0,
                };

                m_menu_font_cfg.RasterizerFlags = ImGuiFreeType::ForceAutoHint;
                m_menu_font_cfg.OversampleH = m_menu_font_cfg.OversampleV = 5;
                m_menu_font_cfg.PixelSnapH = false;
                m_menu_font_cfg.RasterizerMultiply = 1.2f;

                m_visuals_font_cfg.RasterizerFlags = ImGuiFreeType::MonoHinting | ImGuiFreeType::Monochrome;
                m_visuals_font_cfg.OversampleH = m_visuals_font_cfg.OversampleV = 5;
                m_visuals_font_cfg.PixelSnapH = false;
                m_visuals_font_cfg.RasterizerMultiply = 1.2f;

                m_verdana_15_regular = io.Fonts->AddFontFromFileTTF( ( "C:\\Windows\\Fonts\\Verdana.ttf" ), 15.f, &m_menu_font_cfg, ranges );
                m_tahoma_14_regular = io.Fonts->AddFontFromFileTTF( ( "C:\\Windows\\Fonts\\Tahoma.ttf" ), 14.f, &m_menu_font_cfg, ranges );
                m_museo_sans.m_300_30 = io.Fonts->AddFontFromMemoryTTF( ( void* ) museo_sans_300, sizeof( museo_sans_300 ), 30.f, &m_menu_font_cfg, ranges );
                m_museo_sans.m_700_39 = io.Fonts->AddFontFromMemoryTTF( ( void* ) museo_sans_700, sizeof( museo_sans_700 ), 39.f, &m_menu_font_cfg, ranges );
                m_museo_sans.m_300_26 = io.Fonts->AddFontFromMemoryTTF( ( void* ) museo_sans_300, sizeof( museo_sans_300 ), 26.f, &m_menu_font_cfg, ranges );
                m_museo_sans.m_300_16 = io.Fonts->AddFontFromMemoryTTF( ( void* ) museo_sans_300, sizeof( museo_sans_300 ), 16.f, &m_menu_font_cfg, ranges );
                m_roboto_22_regular = io.Fonts->AddFontFromMemoryTTF( ( void* ) roboto, sizeof( roboto ), 22.f, &m_menu_font_cfg, ranges );
                m_mukta.m_mukta_44 = io.Fonts->AddFontFromMemoryTTF( ( void* ) mukta, sizeof( mukta ), 44.f, &m_menu_font_cfg, ranges );
                m_mukta.m_mukta_38 = io.Fonts->AddFontFromMemoryTTF( ( void* ) mukta, sizeof( mukta ), 38.f, &m_menu_font_cfg, ranges );
                m_mukta.m_mukta_30 = io.Fonts->AddFontFromMemoryTTF( ( void* ) mukta, sizeof( mukta ), 30.f, &m_menu_font_cfg, ranges );
                m_mukta.m_mukta_34 = io.Fonts->AddFontFromMemoryTTF( ( void* ) mukta, sizeof( mukta ), 34.f, &m_menu_font_cfg, ranges );
                m_04b03_8 = io.Fonts->AddFontFromMemoryTTF( ( void* ) _04B03, sizeof( _04B03 ), 8.f, &m_visuals_font_cfg, ranges );
                m_icons.m_icon_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF( WeaponIcons_compressed_data_base85, 16.f, &m_menu_font_cfg, ranges );
                m_icons.m_warning_icon_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF( WeaponIcons_compressed_data_base85, 24.f, &m_menu_font_cfg, ranges );
                m_tahoma = io.Fonts->AddFontFromMemoryTTF( ( void* ) tahoma, sizeof( tahoma ), 11.f, &m_visuals_font_cfg, ranges );
            }
        } m_fonts{};
	};

	inline const std::unique_ptr < c_render > g_render = std::make_unique < c_render > ( );

}