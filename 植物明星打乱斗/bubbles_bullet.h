#pragma once

#include "bullet.h"
#include "atlas.h"
#include "animation.h"

extern Atlas atlas_bubbles;									// ÆøÅİ¹¥»÷¶¯»­Í¼¼¯


class Bubbles :public Bullet {
public:
	Bubbles() {
		size.x = 192, size.y = 192;
		damage = 10;

		animation_idle.set_atlas(&atlas_bubbles);
		animation_idle.set_interval(50);
		animation_idle.set_loop(false);
		animation_idle.set_callback([&]() {can_remove = true; });

		collisioned_timer.set_one_shot(true);
		collisioned_timer.set_wait_time(100);
		collisioned_timer.set_callback([&]() {
			can_remove = true;
			});
		
	}
	~Bubbles() = default;

	void on_update(int delta) {
		if (valid) {
			animation_idle.on_update(delta);
		}
		else {
			collisioned_timer.on_update(delta);
		}
	}

	void on_draw(const Camera& camera) const {
		if (valid) {
			animation_idle.on_drow(camera, pos.x, pos.y);
		}

		Bullet::on_draw(camera);
	}

	// ¼ì²âÅö×²
	bool check_collison(const Vector2& pos, const Vector2& size) {
		return this->pos.x + this->size.x / 4 <= pos.x + size.x &&
			this->pos.x + this->size.x * 3 / 4>=pos.x &&
			this->pos.y + this->size.y / 4 <= pos.y + size.y &&
			this->pos.y + this->size.y * 3 / 4 >= pos.y;
	}

private:
	Animation animation_idle;					// ÅİÅİÆÕÍ¨¹¥»÷
	Timer collisioned_timer;					// Åö×²ºóÑÓ³ÙÏûÊ§
};
