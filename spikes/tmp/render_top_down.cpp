#include "render_top_down.hpp"

#include "game_world.hpp"

#include "./ai/goal_follow_path.hpp"

#include <string>

static Matrix3 g_trans;

static void render_path(VideoBuffer* vb, std::list<PathEdge>& path, Matrix3 trans) {
	for (auto& e : path) {
		v2 p1 = transform(e.get_source(), trans);
		v2 p2 = transform(e.get_destination(), trans);
		vb->draw_line(p1.x, p1.y, p2.x, p2.y, Color::Blue);
	}
	if (path.size()) {
		v2 start = transform(path.front().get_source(), trans);
		v2 end = transform(path.back().get_destination(), trans);
		vb->draw_circle_solid(start.x, start.y, 5, Color::Green);
		vb->draw_circle_solid(end.x, end.y, 5, Color::Red);
	}
}

static void render_goals(VideoBuffer* vb, int depth, int x, int y, std::list<Goal*> goals) {
	for (auto g : goals) {
		vb->draw_text(depth*2 + x, y + depth * 8, g->type_to_string());
		if (g->get_type() == Goal::Type::FollowPath) {
			auto fp = static_cast<GoalFollowPath*>(g);
			render_path(vb, fp->get_path(), g_trans);
		}
		if (g->get_subgoals().size() > 0) {
			render_goals(vb, depth + 1, x, y, g->get_subgoals());
		}
	}
}

void render_top_down_view(VideoBuffer* vb, GameWorld* world, v2 invcampos, float scale)
{
	Matrix3 trans = Matrix3::create_translation(invcampos) * Matrix3::create_scale(scale);
	g_trans = trans;

	auto t = [&trans](v2 v) -> v2 {return transform(v, trans); };
	// render navgraph
	auto& nav = world->get_map().get_navgraph();

	// navgraph
	for (auto& n : nav.m_nodes) {
		v2 p = t(n.get_pos());
		vb->draw_circle(p.x, p.y, 0.1f * scale, Color::Green);
	}
	for (auto& es : nav.m_edges) {
		for (auto& e : es) {
			int from = e.get_from();
			int to = e.get_to();
			v2 p1 = t(nav.m_nodes[from].get_pos());
			v2 p2 = t(nav.m_nodes[to].get_pos());
			vb->draw_line(p1.x, p1.y, p2.x, p2.y, Color::Green);
		}
	}

	// map
	for (int i = 0; i < world->get_map().get_height(); i++) {
		for (int j = 0; j < world->get_map().get_width(); j++) {
			if (world->get_map().get_texture_id_block(j, i) != 0) {
				v2 p = t(v2{ (float)j, (float)i });
				vb->draw_rect(p.x, p.y, scale, scale);
				using namespace std::string_literals;
				auto text = std::to_string(j) + " "s + std::to_string(i);
				//vb->draw_text(p.x, p.y, text.c_str());
			}
		}
	}

	// bots
	for (auto& b : world->get_bots()) {
		const v2 pos = t(b->get_pos());
		vb->draw_circle(pos.x, pos.y, b->get_bounding_radius() * scale);
		const v2 dir = t(b->get_pos() + b->get_facing());
		vb->draw_line(pos.x, pos.y, dir.x, dir.y);
		// render feelers
		//auto feelers = b->get_steering().get_feelers();
		//feelers[0] = t(feelers[0]);
		//feelers[1] = t(feelers[1]);
		//feelers[2] = t(feelers[2]);
		//vb->draw_line(pos.x, pos.y, feelers[0].x, feelers[0].y, Color::Blue);
		//vb->draw_line(pos.x, pos.y, feelers[1].x, feelers[1].y, Color::Blue);
		//vb->draw_line(pos.x, pos.y, feelers[2].x, feelers[2].y, Color::Blue);
		// render goals
		auto& ai = b->get_ai();
		auto subgoals = ai.get_subgoals();
		render_goals(vb, 0, pos.x, pos.y, subgoals);
	}

	// projectiles
	for (auto& p : world->get_projectiles()) {
		const v2 pos = t(p->get_pos());
		vb->draw_circle(pos.x, pos.y, 0.1f * scale, Color::Red);
	}

	// spawnpoints
	for (auto& sp : world->get_map().get_spawnpoints()) {
		const v2 pos = t(sp);
		vb->draw_circle(pos.x, pos.y, 0.5f * scale, Color::Orange);
	}
}
