#pragma once
#include "image_holder.hpp"
#include <corelib/simple_graphics/framebuffer.hpp>
#include <corelib/math/math.hpp>

class GameWorld;
class GameMap;

class Renderer
{
	struct Camera
	{
		v2 position;
		v2 direction;
		float lens;
	};

public:
	Renderer();

	void initialize(int width, int height);
	void shutdown();

	sg::Framebuffer& get_framebuffer() { return m_framebuffer; }
	ImageHolder& get_image_holder() { return m_images; }

	//void draw_2d_world(v2 campos, float zoom, const GameWorld& world);
	void draw_3d_world(const GameWorld& world);

private:
	v2 coord_to_pixel(v2 coord);
	v2 pixel_to_coord(v2 pixel);

private:
	void set_camera(v2 position, v2 direction, float lens);
	void draw_map(const GameMap& map);
	void draw_sprites();
	void draw_weapon();
private:
	sg::Framebuffer m_framebuffer;
	ImageHolder m_images;
	Camera m_camera;
};
