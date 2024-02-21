#include "renderer.hpp"

void Renderer::draw_3d_world(const GameWorld& world)
{
}

void Renderer::set_camera(v2 position, v2 direction, float lens)
{
	m_camera = { position, direction, lens };
}

void Renderer::draw_map(const GameMap& map)
{
	const v2 pos = m_camera.position;
	const v2 dir = m_camera.direction;
	const v2 camplane = perp(dir) * m_camera.lens; // determines the fov

	//floor and ceiling
	const sg::Image& floor = m_images.get_texture(map.get_texture_id_floor());
	const sg::Image& ceiling = m_images.get_texture(map.get_texture_id_ceiling());
	const int winheight = m_framebuffer.get_height();
	const int winwidth = m_framebuffer.get_width();

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
			if (map.get_texture_id_block(mapX, mapY) > 0) hit = 1;
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
		const sg::Image& texture = m_images.get_texture(map.get_texture_id_block(mapX, mapY));
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
			sg::Color c = texture.get_pixel(texX, texY);
			if (side == 1) {
				c.r /= 2;
				c.g /= 2;
				c.b /= 2;
			}
			m_framebuffer.draw_pixel(x, y, c);
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


			m_framebuffer.draw_pixel(x, y, floor.get_pixel(floorTexX, floorTexY));
			m_framebuffer.draw_pixel(x, winheight - y, ceiling.get_pixel(floorTexX, floorTexY));
		}
		m_framebuffer.set_depth(x, 0, perpWallDist);
	}
}

void Renderer::draw_sprites()
{
}

void Renderer::draw_weapon()
{
}
