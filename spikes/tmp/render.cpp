#include "render.hpp"
#include "video_buffer.hpp"
#include "game_world.hpp"
#include "image_holder.hpp"
#include "game_map.hpp"

#define MATH_IMPL
#include "math.hpp"
#undef MATH_IMPL

#include "./ai/goal_explore.hpp"

#include <algorithm>
#include <math.h>
#include <string>

RendererOld g_renderer;

void RendererOld::draw(VideoBuffer* vb, ImageHolder* ih)
{
	vb->clear();
	render_map(vb, ih);
	render_sprites(vb, ih);
	render_weapon(vb, ih);
	m_render_queue.clear();
}

void RendererOld::push_camera(v2 pos, v2 dir, float cameralens)
{
	m_pos = pos;
	m_dir = dir;
	m_cameralens = cameralens;
}

void RendererOld::push_map(MapRenderData & data)
{
	m_map = data;
}

void RendererOld::push_sprite(SpriteRenderData & data)
{
	m_render_queue.push_back(data);
}

void RendererOld::push_weapon(WeaponRenderData & data)
{
	m_weapon = data;
}

void RendererOld::push_path(PolygonRenderData& data) {

	//if (data.line.size() > 0) {
	//	v2 last = data.line.front();
	//	for (auto p : data.line) {
	//		g_video.draw
	//	}
	//}
}

void RendererOld::render_map(VideoBuffer* vb, ImageHolder* ih)
{
	const v2 pos = m_pos;
	const v2 dir = m_dir;
	const v2 camplane = perp(dir) * m_cameralens; // determines the fov

	//floor and ceiling
	const Image& floor = ih->get_texture(m_map.floor);
	const Image& ceiling = ih->get_texture(m_map.ceiling);
	const int winheight = vb->get_height();
	const int winwidth = vb->get_width();

	// render playerview
	for (int x = 0; x < winwidth; x++) {
		int mapX = static_cast<int>(pos.x);
		int mapY = static_cast<int>(pos.y);

		const float cameraX = 2 * x / static_cast<float>(winwidth) - 1;
		const float rayDirX = dir.x + camplane.x * cameraX;
		const float rayDirY = dir.y + camplane.y * cameraX;

		float deltaDistX = std::abs(1.f / rayDirX);
		float deltaDistY = std::abs(1.f / rayDirY);

		float sideDistX, sideDistY;
		int   stepX, stepY;
		float perpWallDist;
		int   hit = 0;
		int   side;

		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (pos.x - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.f - pos.x) * deltaDistX;
		}

		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (pos.y - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.f - pos.y) * deltaDistY;
		}

		//perform dda
		while (hit == 0) {
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (m_map.walls[mapY * m_map.width + mapX] > 0) hit = 1;
		}

		if (side == 0) perpWallDist = (mapX - pos.x + (1 - stepX) / 2) / rayDirX;
			else           perpWallDist = (mapY - pos.y + (1 - stepY) / 2) / rayDirY;

		int lineHeight = static_cast<int>(winheight / perpWallDist);

		// texture
		int drawStart = -lineHeight / 2 + winheight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + winheight / 2;
		if (drawEnd >= winheight) drawEnd = winheight - 1;

		//int texNum = m_map.walls[mapY * m_map.width + mapX];// -1; // a index of 1 means texture with index 0
		const Image& texture = ih->get_texture(m_map.walls[mapY * m_map.width + mapX]);
		int texWidth = texture.get_width();
		int texHeight = texture.get_height();

		float wallX;
		if (side == 0) wallX = pos.y + perpWallDist * rayDirY;
			else           wallX = pos.x + perpWallDist * rayDirX;
		wallX -= floorf(wallX);

		int texX = int(wallX * texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		for (int y = drawStart; y < drawEnd; y++) {
			int realDrawStart = (-lineHeight / 2 + winheight / 2);
			float yPosScreen = (y - realDrawStart) / (float(lineHeight));
			int texY = yPosScreen * texHeight /* texture height */;
			Color c = texture.get_pixel(texX, texY);
			if (side == 1) {
				c.r /= 2;
				c.g /= 2;
				c.b /= 2;
			}
			vb->draw_pixel(x, y, c);
		}

		//FLOOR CASTING
		float floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

		//4 different wall directions possible
		if (side == 0 && rayDirX > 0)
		{
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		}
		else if (side == 0 && rayDirX < 0)
		{
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		}
		else if (side == 1 && rayDirY > 0)
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		}
		else
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}

		float distWall, distPlayer, currentDist;

		distWall = perpWallDist;
		distPlayer = 0.0;

		if (drawEnd < 0) drawEnd = winheight; //becomes < 0 when the integer overflows

		//draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < winheight; y++)
		{
			currentDist = winheight / (2.0 * y - winheight); //you could make a small lookup table for this instead

			float weight = (currentDist - distPlayer) / (distWall - distPlayer);

			float currentFloorX = weight * floorXWall + (1.0 - weight) * pos.x;
			float currentFloorY = weight * floorYWall + (1.0 - weight) * pos.y;

			int floorTexX, floorTexY;
			floorTexX = int(currentFloorX * texWidth) % texWidth;
			floorTexY = int(currentFloorY * texHeight) % texHeight;


			vb->draw_pixel(x, y, floor.get_pixel(floorTexX, floorTexY));
			vb->draw_pixel(x,winheight - y, ceiling.get_pixel(floorTexX, floorTexY));
		}
		vb->get_depth(x, 0) = perpWallDist;// fill the z-buffer
	}
}

void RendererOld::render_sprites(VideoBuffer* vb, ImageHolder* ih) {
	// render sprites
	const v2 pos = m_pos;
	const v2 dir = m_dir;
	const v2 camplane = perp(dir) * m_cameralens; // determines the fov

	std::sort(m_render_queue.begin(), m_render_queue.end(), 
	   [pos](SpriteRenderData& l, SpriteRenderData& r) -> bool
	   {
	   float xdiffl = l.pos.x - pos.x;
	   float ydiffl = l.pos.y - pos.y;
	   float length_sql = xdiffl * xdiffl + ydiffl * ydiffl;

	   float xdiffr = r.pos.x - pos.x;
	   float ydiffr = r.pos.y - pos.y;
	   float length_sqr = xdiffr * xdiffr + ydiffr * ydiffr;

	   return length_sql > length_sqr; 
	});


	for (auto& s : m_render_queue) {
		float spriteX = s.pos.x - pos.x;
		float spriteY = s.pos.y - pos.y;

		float invDet = 1.f / (camplane.x * dir.y - dir.x * camplane.y);
		float transformX = invDet * (dir.y * spriteX - dir.x * spriteY);
		float transformY = invDet * (-camplane.y * spriteX + camplane.x * spriteY);

		int uDiv = s.uDiv; // scale x, 2 means half size(divide by 2)
		int vDiv = s.vDiv; // scale y
		float vMove = s.vMove; // move in screen coords
		int vMoveScreen = int(vMove / transformY);

		// height of sprite
		int spriteScreenX = int((vb->get_width() / 2) * (1 + transformX / transformY));
		int spriteHeight = std::abs((int(vb->get_height() / transformY))) / vDiv;
		int drawStartY = -spriteHeight / 2 + vb->get_height() / 2 + vMoveScreen;
		if (drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + vb->get_height() / 2 + vMoveScreen;
		if (drawEndY >= vb->get_height()) drawEndY = vb->get_height() - 1;

		// width of sprite
		int spriteWidth = std::abs(int(vb->get_height() / transformY)) / uDiv;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= vb->get_width())drawEndX = vb->get_width() - 1;

		Image& sprite_texture = ih->get_texture(s.textureId);
		const int texture_width = 64;

		for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
			int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texture_width / spriteWidth) / 256;
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if (transformY > 0 && stripe > 0 && stripe < vb->get_width() && transformY < vb->get_depth(stripe, 0)) {
				for (int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
				{
					int d = (y - vMoveScreen) * 256 - vb->get_height() * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
					int texY = ((d * sprite_texture.get_height()) / spriteHeight) / 256;
					//Color c = sprite_texture.buffer[texX + texture_width * s.col + texY * sprite_texture.get_width()];
					Color c = sprite_texture.get_pixel(texX + texture_width * s.col, texY);
					if (c != Color::Black) vb->draw_pixel(stripe, y, c);
				}
			}
		}
	}
}

void RendererOld::render_weapon(VideoBuffer * vb, ImageHolder * ih)
{
	const int texture_id = m_weapon.id;
	Image& texture = ih->get_texture(texture_id);
	const int cx = vb->get_width() / 2;
	const int cy = vb->get_height() / 2;
	const int ctx = texture.get_width() / 2;
	//const int cty = texture.get_height() / 2;

	static float bobbing = 0.f;
	bobbing += 0.1f;
	int boby = math::sin(bobbing) * 5;
	int bobx = math::cos(bobbing) * 4;

	// draw cross hair
	const int margin = 5;
	const int length = 10;
	vb->draw_line(cx, cy+margin, cx, cy + length); // bot
	vb->draw_line(cx, cy-margin, cx, cy - length); // top
	vb->draw_line(cx-margin, cy, cx - length, cy); // left
	vb->draw_line(cx+margin, cy, cx + length, cy); // right

	// draw weapon
	int draw_startY = vb->get_height() - texture.get_height() + 5 + boby;
	int draw_startX = cx - ctx + bobx;
	for (int i = 0; i < texture.get_height(); i++) {
		for (int j = 0; j < texture.get_width(); j++) {
			if (texture.get_pixel(j, i) == Color::Black) continue;
			if (draw_startY + i >= vb->get_height()) continue;
			vb->draw_pixel(draw_startX + j, draw_startY + i, texture.get_pixel(j, i));
		}
	}
}
