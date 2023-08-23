#include "../../../sdk.hpp"

namespace sdk {
    void c_cfg::save( const std::string_view name ) const {
        const auto path = std::filesystem::path{ "advance.tech" } /= name;

        std::filesystem::create_directory( "advance.tech" );

        nlohmann::json json{};

        /* use a new object to verify our cfg on the load */
        auto& object = json[ ( "advance.tech" ) ];

        for ( const auto& var : m_vars )
            var->save( object );

        auto str = json.dump( );

        if ( std::ofstream file{ path, std::ios::out | std::ios::trunc } )
            file << str;
    }

    void c_cfg::load( const std::string_view name ) {
        const auto path = std::filesystem::path{ "advance.tech" } /= name;

        std::filesystem::create_directory( "advance.tech" );

        std::string str{};
        if ( std::ifstream file{ path, std::ios::in } )
            file >> str;
        else
            return;

        if ( str.empty( ) )
            return;

        const auto json = nlohmann::json::parse( str );
        if ( !json.is_object( ) )
            return;

        /* verify cfg file via trying to find our identity object */
        const auto object = json.find( ( "advance.tech" ) );
        if ( object == json.end( ) )
            return;

        const auto& value = object.value( );

        for ( auto& var : m_vars )
            var->load( value );
    }
}