#pragma once
#include "util.h"
#include "atlas.h"
#include "vector2.h"

class Particle {
public:
	Particle() = default;
	Particle(const Vector2& pos, Atlas* atlas, int lifespan)
		:pos(pos), atlas(atlas), lifespan(lifespan) {}
	~Particle() = default;

	void set_atlas(Atlas* new_atlas) {
		atlas = new_atlas;
	}

	void set_pos(const Vector2& new_pos) {
		pos = new_pos;
	}

	void set_lifespan(int ms) {
		lifespan = ms;
	}

	bool check_valid()const {
		return valid;
	}

	void on_update(int delta) {
		timer += delta;
		if (timer >= lifespan) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size()) {
				idx_frame = atlas->get_size() - 1;
				valid = false;
			}
		}
	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, (int)pos.x, (int)pos.y, atlas->get_image(idx_frame));
	}


private:
	int timer = 0;					// 粒子动画播放定时器
	int lifespan = 0;				// 单帧粒子动画时长
	int idx_frame = 0;				// 当前粒子动画帧索引
	Vector2 pos;					// 粒子的世界坐标位置
	bool valid = true;				// 粒子对象是否有效
	Atlas* atlas = nullptr;			// 粒子动画图集
};
