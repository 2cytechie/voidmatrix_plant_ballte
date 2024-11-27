#pragma once

#include "scene.h"
#include "util.h"
#include "player.h"
#include "platform.h"
#include "status_bar.h"
#include "scene_manager.h"

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;
extern IMAGE img_1P_winnner;
extern IMAGE img_2P_winnner;
extern IMAGE img_winnner_bar;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern Player* player_1;
extern Player* player_2;

extern Camera main_camera;

extern SceneManager scene_manager;

extern std::vector<Platform> platform_list;

class GameScene : public Scene {
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		is_game_over = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winnner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winnner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winnner_bar.getwidth()) / 2;

		pos_img_winner_text.x = -pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winnner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winnner.getwidth()) / 2;

		player_1->set_pos(200, 50);
		player_2->set_pos(975, 50);

		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_2P.set_avatar(img_player_2_avatar);

		status_bar_1P.set_pos(235, 625);
		status_bar_2P.set_pos(675, 625);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;
		pos_img_hill.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hill.y = (getheight() - img_hills.getheight()) / 2;

		timer_winnner_slide_in.restart();
		timer_winnner_slide_in.set_wait_time(2500);
		timer_winnner_slide_in.set_one_shot(true);
		timer_winnner_slide_in.set_callback([&]() {
			is_slide_out_started = true;
			});

		timer_winnner_slide_out.restart();
		timer_winnner_slide_out.set_wait_time(1000);
		timer_winnner_slide_out.set_one_shot(true);
		timer_winnner_slide_out.set_callback([&]() {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
			});

		platform_list.resize(4);

		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_pos.x = 122;
		large_platform.render_pos.y = 455;
		large_platform.shape.left = (float)large_platform.render_pos.x + 30;
		large_platform.shape.right = (float)large_platform.render_pos.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_pos.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_pos.x = 175;
		small_platform_1.render_pos.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_pos.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_pos.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_pos.y + img_platform_small.getheight()/2;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_pos.x = 855;
		small_platform_2.render_pos.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_pos.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_pos.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_pos.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_pos.x = 515;
		small_platform_3.render_pos.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_pos.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_pos.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_pos.y + img_platform_small.getheight() / 2;

		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
	}
	void on_update(int delta) {
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_undate(delta);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet) {
				bool deletable = bullet->check_can_remove();
				if (deletable) {
					delete bullet;
				}
				return deletable;
			}),
			bullet_list.end());

		for (Bullet* bullet : bullet_list) {
			bullet->on_update(delta);
		}

		const Vector2& pos_player_1 = player_1->get_pos();
		const Vector2& pos_player_2 = player_2->get_pos();
		if (pos_player_1.y >= getheight() + player_1->get_size().y) {
			player_1->set_hp(0);
		}
		if (pos_player_2.y >= getheight() + player_2->get_size().y) {
			player_2->set_hp(0);
		}

		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0) {
			if (!is_game_over) {
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}
			is_game_over = true;
		}

		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		if (is_game_over) {
			pos_img_winner_bar.x += (int)(speed_winnner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winnner_text * delta);
			if (!is_slide_out_started) {
				timer_winnner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst) {
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				}
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst) {
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
				}
			}
			else {
				timer_winnner_slide_out.on_update(delta);
			}
		}
	}
	void on_draw(const Camera& camera) {
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hill.x, pos_img_hill.y, &img_hills);

		for (const Platform& platform : platform_list) {
			platform.on_drow(camera);
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);


		if (is_debug) {
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("��������ģʽ������F1���ر�"));
		}

		for (const Bullet* bullet : bullet_list) {
			bullet->on_draw(camera);
		}

		if (is_game_over) {
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winnner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, player_1->get_hp() > 0 ? &img_1P_winnner : &img_2P_winnner);
		}
		else {
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}
	}
	void on_input(const ExMessage& msg) {
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// F1
			if (msg.vkcode == 0x70) {
				is_debug = !is_debug;
			}
			break;
		default:
			break;
		}
	}
	void on_exit() {
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();//û��
		main_camera.reset();
	}

private:
	POINT pos_img_sky = { 0 };				// ��ձ���ͼƬ
	POINT pos_img_hill = { 0 };				// ɽ������ͼƬ

	StatusBar status_bar_1P;				// ��� 1 ��״̬��
	StatusBar status_bar_2P;				// ��� 2 ��״̬��

	bool is_game_over = false;				// ��Ϸ�Ƿ����

	POINT pos_img_winner_bar = { 0 };		// ���㶯������λ��
	POINT pos_img_winner_text = { 0 };		// ���㶯���ı�λ��
	int pos_x_img_winner_bar_dst = 0;		// ���㶯�������ƶ���Ŀ��λ��
	int pos_x_img_winner_text_dst = 0;		// ���㶯���ı��ƶ���Ŀ��λ��
	Timer timer_winnner_slide_in;			// ���㶯�����붨ʱ��
	Timer timer_winnner_slide_out;			// ���㶯��������ʱ��
	bool is_slide_out_started = false;		// ���㶯���Ƿ�ʼ����
	const float speed_winnner_bar = 3.0f;	// ���㶯�������ƶ��ٶ�
	const float speed_winnner_text = 1.5f;	// ���㶯�������ƶ��ٶ�
};