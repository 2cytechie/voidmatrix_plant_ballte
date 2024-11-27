#pragma once

#include "player.h"
#include "bubbles_bullet.h"
#include "bubbles_bullet_ex.h"

extern Atlas atlas_gloomshroom_idle_left;						// ���繽����Ĭ�϶���ͼ��
extern Atlas atlas_gloomshroom_idle_right;						// ���繽����Ĭ�϶���ͼ��
extern Atlas atlas_gloomshroom_run_left;						// ���繽�����ܶ���ͼ��
extern Atlas atlas_gloomshroom_run_right;						// ���繽���ұ��ܶ���ͼ��
extern Atlas atlas_gloomshroom_attack_ex_left;					// ���繽�������⹥������ͼ��
extern Atlas atlas_gloomshroom_attack_ex_right;					// ���繽�������⹥������ͼ��
extern Atlas atlas_gloomshroom_die_left;						// ���繽������������ͼ��
extern Atlas atlas_gloomshroom_die_right;						// ���繽������������ͼ��

extern std::vector<Bullet*> bullet_list;

Bullet* bubble_bullet = nullptr;

class GloomshroomPlayer : public Player {
public:
	GloomshroomPlayer(bool facing_right = true) : Player(facing_right) {
		animation_idle_left.set_atlas(&atlas_gloomshroom_idle_left);
		animation_idle_right.set_atlas(&atlas_gloomshroom_idle_right);
		animation_run_left.set_atlas(&atlas_gloomshroom_run_left);
		animation_run_right.set_atlas(&atlas_gloomshroom_run_right);
		animation_attack_ex_left.set_atlas(&atlas_gloomshroom_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_gloomshroom_attack_ex_right);
		animation_die_left.set_atlas(&atlas_gloomshroom_die_left);
		animation_die_right.set_atlas(&atlas_gloomshroom_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		size.x = 96;
		size.y = 96;


		attack_cd = 300;
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {can_attack = true; });
	}
	~GloomshroomPlayer() = default;

	void on_attack() {
		Bullet* bullet = new Bubbles();

		Vector2 bullet_pos;
		const Vector2& bullet_size = bullet->get_size();
		bullet_pos.x = pos.x + (size.x - bullet_size.x) / 2.0f;
		bullet_pos.y = pos.y + (size.y - bullet_size.y) / 2.0f;

		bullet->set_pos(bullet_pos.x, bullet_pos.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		bullet->set_callback([&]() {mp += 35; });

		bullet_list.push_back(bullet);

		bubble_bullet = bullet_list.back();

		mciSendString(_T("play bubble_shot from 0"), NULL, 0, NULL);
	}

	void on_update(int delta) {
		Player::on_update(delta);

		if (bubble_bullet != nullptr) {
			Vector2 bullet_pos;
			const Vector2& bullet_size = bubble_bullet->get_size();
			bullet_pos.x = pos.x + (size.x - bullet_size.x) / 2.0f;
			bullet_pos.y = pos.y + (size.y - bullet_size.y) / 2.0f;

			bubble_bullet->set_pos(bullet_pos.x, bullet_pos.y);
		}
	}

	void on_draw(const Camera& camera) {
		Player::on_draw(camera);


	}

private:

};
