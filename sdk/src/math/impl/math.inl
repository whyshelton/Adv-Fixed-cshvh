#pragma once

#include "../math.hpp"

namespace sdk {
    template < typename _value_t >
        requires std::is_arithmetic_v< _value_t >
    ALWAYS_INLINE constexpr auto to_deg( const _value_t rad ) {
        using ret_t = detail::enough_float_t< _value_t >;

        return static_cast< ret_t >( rad * k_rad_pi< ret_t > );
    }

    ALWAYS_INLINE void sin_cos( float rad, float& sin_, float& cos_ )
    {
        sin_ = sin( rad );
        cos_ = cos( rad );
    }

    ALWAYS_INLINE bool intersect_bb( vec3_t& start, vec3_t& delta, vec3_t& min, vec3_t& max )
    {
        float d1, d2, f;
        auto start_solid = true;
        auto t1 = -1.0, t2 = 1.0;

        const float _start[ 3 ] = { start.x( ), start.y( ), start.z( ) };
        const float _delta[ 3 ] = { delta.x( ), delta.y( ), delta.z( ) };
        const float mins[ 3 ] = { min.x( ), min.y( ), min.z( ) };
        const float maxs[ 3 ] = { max.x( ), max.y( ), max.z( ) };

        for ( auto i = 0; i < 6; ++i ) {
            if ( i >= 3 ) {
                const auto j = ( i - 3 );

                d1 = _start[ j ] - maxs[ j ];
                d2 = d1 + _delta[ j ];
            }
            else {
                d1 = -_start[ i ] + mins[ i ];
                d2 = d1 - _delta[ i ];
            }

            if ( d1 > 0 && d2 > 0 ) {
                start_solid = false;
                return false;
            }

            if ( d1 <= 0 && d2 <= 0 )
                continue;

            if ( d1 > 0 )
                start_solid = false;

            if ( d1 > d2 ) {
                f = d1;
                if ( f < 0 )
                    f = 0;

                f /= d1 - d2;
                if ( f > t1 )
                    t1 = f;
            }
            else {
                f = d1 / ( d1 - d2 );
                if ( f < t2 )
                    t2 = f;
            }
        }

        return start_solid || ( t1 < t2 && t1 >= 0.0f );
    }

    ALWAYS_INLINE void ang_vecs( const qang_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up ) {
        vec3_t cos, sin;

        const auto rad_x = angles.x( ) * k_deg_pi< float >;
        const auto rad_y = angles.y( ) * k_deg_pi< float >;
        const auto rad_z = angles.z( ) * k_deg_pi< float >;

        sin_cos( rad_x, sin.x( ), cos.x( ) );
        sin_cos( rad_y, sin.y( ), cos.y( ) );
        sin_cos( rad_z, sin.z( ), cos.z( ) );

        if ( forward ) {
            forward->x( ) = cos.x( ) * cos.y( );
            forward->y( ) = cos.x( ) * sin.y( );
            forward->z( ) = -sin.x( );
        }

        if ( right ) {
            right->x( ) = -sin.z( ) * sin.x( ) * cos.y( ) + -cos.z( ) * -sin.y( );
            right->y( ) = -sin.z( ) * sin.x( ) * sin.y( ) + -cos.z( ) * cos.y( );
            right->z( ) = -sin.z( ) * cos.x( );
        }

        if ( up ) {
            up->x( ) = cos.z( ) * sin.x( ) * cos.y( ) + -sin.z( ) * -sin.y( );
            up->y( ) = cos.z( ) * sin.x( ) * sin.y( ) + -sin.z( ) * cos.y( );
            up->z( ) = cos.z( ) * cos.x( );
        }
    }

    ALWAYS_INLINE float normalize_yaw( float f ) {
        while ( f < -180.0f )
            f += 360.0f;

        while ( f > 180.0f )
            f -= 360.0f;

        return f;
    }

    template <typename value_t >
    ALWAYS_INLINE value_t get_average_value( const std::vector< value_t >& v ) {
        if ( v.empty( ) )
            return 0.f;

        return std::reduce( v.begin( ), v.end( ), 0.0 ) / v.size( );
    }

    ALWAYS_INLINE float segment_to_segment( const vec3_t& s1, const vec3_t& s2, const vec3_t& k1, const vec3_t& k2 ) {
        const auto u = s2 - s1, v = k2 - k1, w = s1 - k1;

        const auto a = u.dot( u );
        const auto b = u.dot( v );
        const auto c = v.dot( v );
        const auto d = u.dot( w );
        const auto e = v.dot( w );
        const auto unk = a * c - b * b;

        auto sn = 0.f, tn = 0.f, sd = unk, td = unk;

        if ( unk < 0.00000001f ) {
            sn = 0.f;
            sd = 1.f;
            tn = e;
            td = c;
        }
        else {
            sn = b * e - c * d;
            tn = a * e - b * d;

            if ( sn < 0.f ) {
                sn = 0.f;
                tn = e;
                td = c;
            }
            else if ( sn > sd ) {
                sn = sd;
                tn = e + b;
                td = c;
            }
        }

        if ( tn < 0.f ) {
            tn = 0.f;

            if ( -d < 0.f ) {
                sn = 0.f;
            }
            else if ( -d > a ) {
                sn = sd;
            }
            else {
                sn = -d;
                sd = a;
            }
        }
        else if ( tn > td ) {
            tn = td;

            if ( -d + b < 0.f ) {
                sn = 0.f;
            }
            else if ( -d + b > a ) {
                sn = sd;
            }
            else {
                sn = -d + b;
                sd = a;
            }
        }

        const auto sc = std::abs( sn ) < 0.00000001f ? 0.f : sn / sd;
        const auto tc = std::abs( tn ) < 0.00000001f ? 0.f : tn / td;

        return ( w + u * sc - v * tc ).length( );
    }

    ALWAYS_INLINE bool intersect( vec3_t start, vec3_t end, vec3_t a, vec3_t b, float radius )
    {
        const auto dist = segment_to_segment( start, end, a, b );
        return ( dist < radius );
    }

    ALWAYS_INLINE mat3x4_t vector_matrix( const vec3_t& in ) {
        sdk::vec3_t right{}, up{};

        if ( in.x( ) == 0.f
            && in.y( ) == 0.f ) {
            right = { 0.f, -1.f, 0.f };
            up = { -in.z( ), 0.f, 0.f };
        }
        else {
            right = in.cross( { 0.f, 0.f, 1.f } ).normalized( );
            up = right.cross( in ).normalized( );
        }

        mat3x4_t ret{};

        ret[ 0 ][ 0 ] = in.x( );
        ret[ 1 ][ 0 ] = in.y( );
        ret[ 2 ][ 0 ] = in.z( );

        ret[ 0 ][ 1 ] = -right.x( );
        ret[ 1 ][ 1 ] = -right.y( );
        ret[ 2 ][ 1 ] = -right.z( );

        ret[ 0 ][ 2 ] = up.x( );
        ret[ 1 ][ 2 ] = up.y( );
        ret[ 2 ][ 2 ] = up.z( );

        return ret;
    }

    ALWAYS_INLINE vec3_t vector_rotate( const vec3_t& in1, const sdk::qang_t& in2 )
    {
        const auto matrix = in2.matrix( );
        
        return in1.rotate( matrix );
    }

    ALWAYS_INLINE bool line_vs_bb( const vec3_t& src, const vec3_t& dst, const vec3_t& min, const vec3_t& max ) {
        float d1{}, d2{}, f{};
        auto t1 = -1.f, t2 = 1.f;

        auto start_solid = true;

        for ( std::size_t i{}; i < 6u; ++i ) {
            if ( i >= 3 ) {
                const auto j = i - 3u;

                d1 = src.at( j ) - max.at( j );
                d2 = d1 + dst.at( j );
            }
            else {
                d1 = -src.at ( i ) + min.at( i );
                d2 = d1 - dst.at( i );
            }

            if ( d1 > 0.0f
                && d2 > 0.0f )
                return false;

            if ( d1 <= 0.0f
                && d2 <= 0.0f )
                continue;

            if ( d1 > 0.f )
                start_solid = false;

            if ( d1 > d2 ) {
                f = d1;

                if ( f < 0.f )
                    f = 0.f;

                f /= d1 - d2;

                if ( f > t1 )
                    t1 = f;
            }
            else {
                f = d1 / ( d1 - d2 );

                if ( f < t2 )
                    t2 = f;
            }
        }

        return start_solid || ( t1 < t2 && t1 >= 0.f );
    }

    ALWAYS_INLINE void vec_angs( const vec3_t& in, qang_t& out ) {
        if ( in.x( ) == 0.f
            && in.y( ) == 0.f ) {
            out.y( ) = 0.f;
            out.x( ) = in.z( ) > 0.f ? 270.f : 90.f;
        }
        else {
            out.y( ) = to_deg( std::atan2( in.y( ), in.x( ) ) );
            if ( out.y( ) < 0.f )
                out.y( ) += 360.f;

            out.x( ) = to_deg( std::atan2( -in.z( ), in.length( 2u ) ) );
            if ( out.x( ) < 0.f )
                out.x( ) += 360.f;
        }

        out.x( ) = std::remainder( out.x( ), 360.f );
        out.y( ) = std::remainder( out.y( ), 360.f );
        out.z( ) = std::remainder( out.z( ), 360.f );
    }

    ALWAYS_INLINE float angle_diff( float f, float w ) {
        float delta;

        delta = fmodf( f - w, 360.0f );
        if ( f > w ) {
            if ( delta >= 180 )
                delta -= 360;
        }
        else {
            if ( delta <= -180 )
                delta += 360;
        }
        return delta;
    }


    ALWAYS_INLINE float calc_fov( const sdk::qang_t view_angles, const vec3_t& src, const vec3_t& dst ) {
        const auto dir = ( dst - src ).normalized( );

        vec3_t fwd{};

        ang_vecs( view_angles, &fwd, nullptr, nullptr );

        return std::max( to_deg( std::acos( fwd.dot( dir ) ) ), 0.f );
    }

    ALWAYS_INLINE qang_t calc_ang( const vec3_t& src, const vec3_t& dst ) {
        qang_t angles;

        vec3_t delta = src - dst;
        float hyp = delta.length( 2u );

        angles.y( ) = std::atanf( delta.y( ) / delta.x( ) ) * k_rad_pi< double >;
        angles.x( ) = std::atanf( -delta.z( ) / hyp ) * -k_rad_pi< double >;
        angles.z( ) = 0.0f;

        if ( delta.x( ) >= 0.0f )
            angles.y( ) += 180.0f;

        return angles;
    }

    template < typename _value_t >
        requires std::is_arithmetic_v< _value_t >
    ALWAYS_INLINE constexpr auto to_rad( const _value_t deg ) {
        using ret_t = detail::enough_float_t< _value_t >;

        return static_cast< ret_t >( deg * k_deg_pi< ret_t > );
    }

    template < typename _value_t >
        requires is_addable< _value_t, _value_t > && is_multipliable< _value_t, float >
    ALWAYS_INLINE constexpr _value_t lerp( const _value_t& from, const _value_t& to, const float amt ) {
        return from * ( 1.f - amt ) + to * amt;
    }

    template < typename _value_t >
        requires std::is_floating_point_v< _value_t >
    ALWAYS_INLINE constexpr _value_t normalize_angle( const _value_t angle ) {
        return std::remainder( angle, static_cast< _value_t >( 360.0 ) );
    }
}