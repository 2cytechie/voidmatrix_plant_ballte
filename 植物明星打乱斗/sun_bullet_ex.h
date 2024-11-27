#pragma once
#include "bullet.h"
#include "animation.h"

#include <iostream>

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

extern Camera main_camera;

class SunBulletEx : public Bullet {
public:
	SunBulletEx() {
		size.x = 288, size.y = 288;
		damage = 20;

		animation_idle.set_atlas(&atlas_sun_ex);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_ex_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;

	}
	~SunBulletEx() = default;

	void on_collide() {
		Bullet::on_collide();

		main_camera.shake(20, 350);

		mciSendString(_T("play sun_explode_ex from 0"),NULL,0,NULL);
	}

	bool check_collision(const Vector2& pos, const Vector2& size) {
		bool is_collide_x = (max(this->pos.x + this->size.x, pos.x + size.x) - min(this->pos.x, pos.x) <= this->size.x + size.x);
		bool is_collide_y = (max(this->pos.y + this->size.y, pos.y + size.y) - min(this->pos.y, pos.y) <= this->size.y + size.y);

		return is_collide_x && is_collide_y;
	}

	void on_update(int delta) {
		if (valid) {
			pos += velocity * (float)delta;
		}

		if(!valid) {
			animation_explode.on_update(delta);
		}
		else {
			animation_idle.on_update(delta);
		}

		if (check_if_exceeds_screen()) {
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera)const {
		if (valid) {
			animation_idle.on_drow(camera, (int)pos.x, (int)pos.y);
		}
		else {
			animation_explode.on_drow(camera, (int)(pos.x + explode_render_offset.x), (int)(pos.y + explode_render_offset.y));
		}
		Bullet::on_draw(camera);
	}

private:
	Animation animation_idle;			// æﬁ»’π‚’®µØƒ¨»œ∂Øª≠
	Animation animation_explode;		// æﬁ»’π‚’®µØ±¨’®∂Øª≠
	Vector2 explode_render_offset;		// ’®µØ∂Øª≠‰÷»æ∆´“∆¡ø

};
