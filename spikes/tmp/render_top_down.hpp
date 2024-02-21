#pragma once

#include "video_buffer.hpp"
#include "math.hpp"

class GameWorld;

void render_top_down_view(VideoBuffer* vb, GameWorld* world, v2 invcampos, float scale);
//void render_top_down_view(VideoBuffer* vb, GameWorld* world, Matrix3 transform);

