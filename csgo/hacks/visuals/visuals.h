#pragma once

namespace hacks {
	struct key_data_t {
		__forceinline key_data_t( ) = default;
		float m_alpha{};
		std::string m_status{};
	};

	struct dormant_data_t {
		float m_alpha{};
		bool m_was_unseen{};
		sdk::vec3_t m_origin{};
		float m_receive_time{};
		int m_weapon_id{};
		int m_weapon_type{ -1 };
		bool m_use_shared{};
		float m_last_shared_time{};
	};

	enum collision_group_t_ {
		_COLLISION_GROUP_NONE = 0,
		_COLLISION_GROUP_DEBRIS,
		_COLLISION_GROUP_DEBRIS_TRIGGER,
		_COLLISION_GROUP_INTERACTIVE_DEBRIS,
		_COLLISION_GROUP_INTERACTIVE,
		_COLLISION_GROUP_PLAYER,
		_COLLISION_GROUP_BREAKABLE_GLASS,
		_COLLISION_GROUP_VEHICLE,
		_COLLISION_GROUP_PLAYER_MOVEMENT,
		_COLLISION_GROUP_NPC,
		_COLLISION_GROUP_IN_VEHICLE,
		_COLLISION_GROUP_WEAPON,
		_COLLISION_GROUP_VEHICLE_CLIP,
		_COLLISION_GROUP_PROJECTILE,
		_COLLISION_GROUP_DOOR_BLOCKER,
		_COLLISION_GROUP_PASSABLE_DOOR,
		_COLLISION_GROUP_DISSOLVING,
		_COLLISION_GROUP_PUSHAWAY,
		_COLLISION_GROUP_NPC_ACTOR,
		_COLLISION_GROUP_NPC_SCRIPTED,
		_COLLISION_GROUP_PZ_CLIP,
		_COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE,
		_LAST_SHARED_COLLISION_GROUP
	};

	enum contents_t_ {
		_CONTENTS_EMPTY = 0,
		_CONTENTS_SOLID = 0x1,
		_CONTENTS_WINDOW = 0x2,
		_CONTENTS_AUX = 0x4,
		_CONTENTS_GRATE = 0x8,
		_CONTENTS_SLIME = 0x10,
		_CONTENTS_WATER = 0x20,
		_CONTENTS_BLOCKLOS = 0x40,
		_CONTENTS_OPAQUE = 0x80,
		_CONTENTS_TESTFOGVOLUME = 0x100,
		_CONTENTS_UNUSED = 0x200,
		_CONTENTS_BLOCKLIGHT = 0x400,
		_CONTENTS_TEAM_ct = 0x800,
		_CONTENTS_TEAM_t = 0x1000,
		_CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
		_CONTENTS_MOVEABLE = 0x4000,
		_CONTENTS_AREAPORTAL = 0x8000,
		_CONTENTS_PLAYERCLIP = 0x10000,
		_CONTENTS_MONSTERCLIP = 0x20000,
		_CONTENTS_CURRENT_0 = 0x40000,
		_CONTENTS_CURRENT_90 = 0x80000,
		_CONTENTS_CURRENT_180 = 0x100000,
		_CONTENTS_CURRENT_270 = 0x200000,
		_CONTENTS_CURRENT_UP = 0x400000,
		_CONTENTS_CURRENT_DOWN = 0x800000,
		_CONTENTS_ORIGIN = 0x1000000,
		_CONTENTS_MONSTER = 0x2000000,
		_CONTENTS_DEBRIS = 0x4000000,
		_CONTENTS_DETAIL = 0x8000000,
		_CONTENTS_TRANSLUCENT = 0x10000000,
		_CONTENTS_LADDER = 0x20000000,
		_CONTENTS_HITBOX = 0x40000000,
	};

	enum masks_t_ {
		_MASK_ALL = 0xFFFFFFFF,
		_MASK_SOLID = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_MONSTER | _CONTENTS_GRATE,
		_MASK_PLAYERSOLID = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_PLAYERCLIP | _CONTENTS_WINDOW | _CONTENTS_MONSTER | _CONTENTS_GRATE,
		_MASK_NPCSOLID = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_MONSTERCLIP | _CONTENTS_WINDOW | _CONTENTS_MONSTER | _CONTENTS_GRATE,
		_MASK_NPCFLUID = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_MONSTERCLIP | _CONTENTS_WINDOW | _CONTENTS_MONSTER | _CONTENTS_GRATE,
		_MASK_WATER = _CONTENTS_WATER | _CONTENTS_MOVEABLE | _CONTENTS_SLIME,
		_MASK_OPAQUE = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_OPAQUE,
		_MASK_OPAQUE_AND_NPCS = _MASK_OPAQUE | _CONTENTS_MONSTER,
		_MASK_BLOCKLOS = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_BLOCKLOS,
		_MASK_BLOCKLOS_AND_NPCS = _MASK_BLOCKLOS | _CONTENTS_MONSTER,
		_MASK_VISIBLE = _MASK_OPAQUE | _CONTENTS_IGNORE_NODRAW_OPAQUE,
		_MASK_VISIBLE_AND_NPCS = _MASK_OPAQUE_AND_NPCS | _CONTENTS_IGNORE_NODRAW_OPAQUE,
		_MASK_SHOT = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_MONSTER | _CONTENTS_WINDOW | _CONTENTS_DEBRIS | _CONTENTS_GRATE | _CONTENTS_HITBOX,
		_MASK_SHOT_BRUSHONLY = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_DEBRIS,
		_MASK_SHOT_HULL = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_MONSTER | _CONTENTS_WINDOW | _CONTENTS_DEBRIS | _CONTENTS_GRATE,
		_MASK_SHOT_PORTAL = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_MONSTER,
		_MASK_SOLID_BRUSHONLY = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_GRATE,
		_MASK_PLAYERSOLID_BRUSHONLY = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_PLAYERCLIP | _CONTENTS_GRATE,
		_MASK_NPCSOLID_BRUSHONLY = _CONTENTS_SOLID | _CONTENTS_MOVEABLE | _CONTENTS_WINDOW | _CONTENTS_MONSTERCLIP | _CONTENTS_GRATE,
		_MASK_NPCWORLDSTATIC = _CONTENTS_SOLID | _CONTENTS_WINDOW | _CONTENTS_MONSTERCLIP | _CONTENTS_GRATE,
		_MASK_NPCWORLDSTATIC_FLUID = _CONTENTS_SOLID | _CONTENTS_WINDOW | _CONTENTS_MONSTERCLIP,
		_MASK_SPLITAREPORTAL = _CONTENTS_WATER | _CONTENTS_SLIME,
		_MASK_CURRENT = _CONTENTS_CURRENT_0 | _CONTENTS_CURRENT_90 | _CONTENTS_CURRENT_180 | _CONTENTS_CURRENT_270 | _CONTENTS_CURRENT_UP | _CONTENTS_CURRENT_DOWN,
		_MASK_DEADSOLID = _CONTENTS_SOLID | _CONTENTS_PLAYERCLIP | _CONTENTS_WINDOW | _CONTENTS_GRATE,
	};

	class c_visuals {
	private:
		bool m_reset_kill_feed{};

		struct grenade_simulation_t {
			__forceinline grenade_simulation_t( ) = default;

			__forceinline grenade_simulation_t(
				valve::cs_player_t* const owner, const valve::e_item_index index,
				const sdk::vec3_t& origin, const sdk::vec3_t& velocity, const float throw_time, const int offset
			) : m_owner{ owner }, m_index{ index } {
				predict( origin, velocity, throw_time, offset );
			}

			void predict( const sdk::vec3_t& origin, const sdk::vec3_t& velocity, const float throw_time, const int offset );

			bool physics_simulate( );

			void physics_trace_entity(
				const sdk::vec3_t& src, const sdk::vec3_t& dst,
				const std::uint32_t mask, valve::trace_t& trace
			);

			void physics_push_entity( const sdk::vec3_t& push, valve::trace_t& trace );

			void perform_fly_collision_resolution( valve::trace_t& trace );

			void think( );

			void detonate( const bool bounced );

			void update_path( const bool bounced );

			__forceinline void push_broken_ent( valve::base_entity_t* ent )
			{
				m_broken_ents.emplace_back( ent );
			}

			__forceinline void clear_broken_ents( )
			{
				m_broken_ents.clear( );
			}

			__forceinline bool is_ent_broken( valve::base_entity_t* ent )
			{
				return find( m_broken_ents.begin( ), m_broken_ents.end( ), ent ) != m_broken_ents.end( );
			}

			bool										m_detonated{};
			valve::cs_player_t* m_owner{};
			sdk::vec3_t										m_origin{}, m_velocity{};
			valve::base_entity_t* m_last_hit_entity{}, * m_last_breakable{};
			float										m_detonate_time{}, m_expire_time{}, m_source_time{};
			valve::e_item_index							m_index{};
			int											m_tick{}, m_next_think_tick{},
				m_last_update_tick{}, m_bounces_count{}, m_collision_group{};
			std::vector< std::pair< sdk::vec3_t, bool > >	m_path{};
			std::vector < valve::base_entity_t* >            m_broken_ents{};
		};

		struct shot_mdl_t {
			int                         m_player_index{};
			unsigned int                m_hash{};
			float						m_time{};
			valve::bones_t				m_bones{};
			sdk::mat3x4_t					m_world_matrix{};


			valve::model_render_info_t	m_info{};
			valve::draw_model_state_t	m_state{};
		};

		std::vector< shot_mdl_t >		m_shot_mdls{};

	public:
		void handle( ) const;

		void skybox_changer( ) const;

		struct flags_data_t {
			std::string m_name{};
			float m_alpha{};
			sdk::col_t m_clr{};
		};

		struct bullet_impact_t {
			__forceinline bullet_impact_t( ) = default;

			__forceinline bullet_impact_t(
				const float time, const sdk::vec3_t& from, const sdk::vec3_t& pos
			) : m_time{ time }, m_from{ from }, m_pos{ pos } {}

			float	m_time{};
			bool	m_final{ true };

			sdk::vec3_t	m_from{}, m_pos{};
		};
		struct bullet_trace_data_t {
			sdk::vec3_t m_start_pos{}, m_end_pos{};
			float m_spawn_time{}, m_alpha_modifier{};
		};

		struct hit_marker_data_t {
			sdk::vec3_t m_pos{};
			float m_spawn_time{};
			float m_alpha{};
		};

		struct shared_t {
			ALWAYS_INLINE shared_t( ) = default;

			void send_net_data( valve::cs_player_t* const player );
		} m_shared{};

		void handle_player_drawings( );

		void draw_name( valve::cs_player_t* player, RECT& rect );

		void draw_box( valve::cs_player_t* player, RECT& rect );

		void draw_health( valve::cs_player_t* player, RECT& rect );

		void oof_indicators( valve::cs_player_t* );

		void draw_wpn( valve::cs_player_t* player, RECT& rect );

		void draw_ammo( valve::cs_player_t* player, RECT& rect );

		void draw_flags( valve::cs_player_t* player, RECT& rect );

		void tone_map_modulation( valve::base_entity_t* const entity ) const;

		void change_shadows( valve::base_entity_t* const entity ) const;

		void kill_feed( );

		void molotov_timer( valve::base_entity_t* const entity ) const;

		void draw_scope_lines( ) const;
		
		void water_mark( ) const;

		void spectators( ) const;

		void key_binds( ) const;

		void draw_hitmarkers( );

		void draw_enemy_bullet_tracers( );

		void draw_bullet_tracers( );

		void add_shot_mdl( valve::cs_player_t* player, const sdk::mat3x4_t* );

		void draw_shot_mdl( );

		void smoke_timer( valve::base_entity_t* const entity ) const;

		void draw_auto_peek( );

		void grenade_projectiles( valve::base_entity_t* const entity ) const;

		static bool is_entity_broken( valve::base_entity_t* entity );

		using throwed_grenades_t = std::unordered_map< valve::ent_handle_t, grenade_simulation_t >;

		bool add_grenade_simulation( const grenade_simulation_t& sim, const bool warning, const bool no_dmg ) const;

		void handle_warning_pred( valve::base_entity_t* const entity, const valve::e_class_id class_id );

		void on_create_move( const valve::user_cmd_t& cmd );

		throwed_grenades_t				m_throwed_grenades{};
		grenade_simulation_t			m_grenade_trajectory{};

		std::array<dormant_data_t, 65> m_dormant_data{};
		std::array < sdk::vec3_t, 65 > m_alive_origin{};
		std::array < sdk::vec3_t, 65 > m_alive_mins{};
		std::array < sdk::vec3_t, 65 > m_alive_maxs{};
		std::array < bool, 65 > m_change_offset_due_to_lby{};
		std::array < bool, 65 > m_change_offset_due_to_ammo{};

		ALWAYS_INLINE bool& reset_kill_feed( );

		std::vector< bullet_impact_t >	m_bullet_impacts{};
		std::deque < bullet_trace_data_t > m_bullet_tracers{};
		std::deque < bullet_trace_data_t > m_enemy_bullet_tracers{};
		std::deque < hit_marker_data_t > m_hit_markers{};

		bool m_rendering_shot_mdl{};
	};

	class c_dormant_esp {
	public:
		void start( );

		bool adjust_sound( valve::cs_player_t* player );
		void setup_adjust( valve::cs_player_t* player, valve::snd_info_t& sound );
		bool valid_sound( valve::snd_info_t& sound );

		struct sound_player_t {
			void reset( bool store_data = false, const sdk::vec3_t& origin = {}, int flags = 0 )
			{
				if ( store_data )
				{
					m_receive_time = valve::g_global_vars.get( )->m_real_time;
					m_origin = origin;
					m_flags = flags;
				}
				else
				{
					m_receive_time = 0.0f;
					m_origin = {};
					m_flags = 0;
				}
			}

			void override( valve::snd_info_t& sound )
			{
				m_receive_time = valve::g_global_vars.get( )->m_real_time;
				m_origin = *sound.m_origin;
			}

			float m_receive_time = 0.0f;
			sdk::vec3_t m_origin = {};
			int m_flags = {};
		} m_sound_players[ 65 ];

		valve::utl_vec_t <valve::snd_info_t> m_sound_buffer;
		valve::utl_vec_t <valve::snd_info_t> m_cur_sound_list;
	};

	inline std::unique_ptr < c_dormant_esp > g_dormant_esp = std::make_unique < c_dormant_esp >( );


	inline const auto g_visuals = std::make_unique < c_visuals >( );
}

#include "impl/visuals.inl"