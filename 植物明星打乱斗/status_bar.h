#pragma once
#include "util.h"

class StatusBar {
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img) {
		img_avatar = img;
	}

	void set_pos(int x, int y) {
		pos.x = x;
		pos.y = y;
	}

	void set_hp(int val) {
		hp = val;
	}

	void set_mp(int val) {
		mp = val;
	}

	void on_draw() {
		putimage_alpha(pos.x, pos.y, img_avatar);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(pos.x + 100, pos.y + 10, pos.x + 100 + width + 3 * 2, pos.y + 36, 8, 8);
		solidroundrect(pos.x + 100, pos.y + 45, pos.x + 100 + width + 3 * 2, pos.y + 71, 8, 8);
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(pos.x + 100, pos.y + 10, pos.x + 100 + width + 3 * 2, pos.y + 33, 8, 8);
		solidroundrect(pos.x + 100, pos.y + 45, pos.x + 100 + width + 3 * 2, pos.y + 68, 8, 8);

		float hp_bar_width = width * max(0, hp) / 100.0f;
		float mp_bar_width = width * min(100, mp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(pos.x + 100, pos.y + 10, pos.x + 100 + (int)hp_bar_width + 3, pos.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(pos.x + 100, pos.y + 45, pos.x + 100 + (int)mp_bar_width + 3, pos.y + 68, 8, 8);
	}

private:
	int hp = 0;							// ��ʾ��Ѫ��
	int mp = 0;							// ��ʾ������
	POINT pos = { 0 };					// ��ʾλ��
	IMAGE* img_avatar;					// ��ɫͷ��

	const int width = 275;				// ״̬�����
	
};
