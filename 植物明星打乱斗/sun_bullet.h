#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet :public Bullet {
public:
	SunBullet() {
		size.x = 96, size.y = 96;
		damage = 15;

		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {can_remove = true; });

		// 计算爆炸动画的位置偏移量
		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;

	}
	~SunBullet() = default;

	// 碰撞后的逻辑
	void on_collidle() {
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta) {
		if (valid) {
			velocity.y += gravity * delta;
			pos += velocity * (float)delta;
		}

		if (!valid) {
			animation_explode.on_update(delta);
		}
		else {
			animation_idle.on_update(delta);
		}

		if (check_if_exceeds_screen()) {
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera) const {
		if (valid) {
			animation_idle.on_drow(camera, (int)pos.x, (int)pos.y);
		}
		else {
			animation_explode.on_drow(camera, (int)(pos.x + explode_render_offset.x), (int)(pos.y + explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}

private:
	Animation animation_idle;				// 日光炸弹默认动画
	Animation animation_explode;			// 日光炸弹爆炸动画
	Vector2 explode_render_offset;			// 爆炸动画渲染偏移量

	const float gravity = 1e-3f;			// 日光炸弹重力

};

