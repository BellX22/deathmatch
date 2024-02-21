#pragma once

struct ImageHolder;
struct TheBot;
struct GameMap;
struct GameWorld;

struct SpriteRenderData {
	v2 pos = { 0, 0 };
	v2 dir = {1.f, 0.f};
	int uDiv = 1; // size x
	int vDiv = 1; // size y
	float vMove = 0.f; // above ground
	int textureId = 0;
	int row = 0;
	int col = 0;
};

struct MapRenderData {
	int width = 0, height = 0;
	std::vector<int> walls;
	int floor;
	int ceiling;
};

struct WeaponRenderData {
	int id = 0;
};

struct PolygonRenderData {
	std::vector<v2> line;
};

class RendererOld {
public:
	void draw(VideoBuffer* vb, ImageHolder* ih);
	void push_camera(v2 pos, v2 dir, float cameralens);
	void push_map(MapRenderData& data);
	void push_sprite(SpriteRenderData& data);
	void push_weapon(WeaponRenderData& data);
	void push_path(PolygonRenderData& data);

private:
	void render_map(VideoBuffer* vb, ImageHolder* ih);
	void render_sprites(VideoBuffer* vb, ImageHolder* ih);
	void render_weapon(VideoBuffer* vb, ImageHolder* ih);

private:
	v2 m_pos, m_dir;
	float m_cameralens;
	WeaponRenderData m_weapon;
	MapRenderData m_map;
	std::vector<SpriteRenderData> m_render_queue;
};

extern Renderer g_renderer;
