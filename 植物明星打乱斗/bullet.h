#pragma once
#include "vector2.h"
#include "player_id.h"
#include "camera.h"

#include <functional>
#include<graphics.h>

extern bool is_debug;

class Bullet {
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val) {
		damage = val;
	}

	int get_damage() {
		return damage;
	}

	void set_pos(float x, float y) {
		pos.x = x;
		pos.y = y;
	}

	const Vector2& get_pos()const {
		return pos;
	}

	const Vector2& get_size() {
		return size;
	}

	void set_velocity(float x, float y) {
		velocity.x = x;
		velocity.y = y;
	}

	void set_collide_target(PlayerID target) {
		target_id = target;
	}

	PlayerID get_collide_target()const {
		return target_id;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	void set_valid(bool flag) {
		valid = flag;
	}

	bool get_valid() {
		return valid;
	}

	bool check_can_remove()const {
		return can_remove;
	}

	virtual void on_collide() {
		if (callback) {
			callback();
		}
	}

	// ����Ƿ�����ײ
	virtual bool check_collison(const Vector2& pos, const Vector2& size) {
		return this->pos.x + this->size.x / 2 >= pos.x
			&& this->pos.x + this->size.x / 2 <= pos.x + size.x
			&& this->pos.y + this->size.y / 2 >= pos.y
			&& this->pos.y + this->size.y / 2 <= pos.y + size.y;
	}

	virtual void on_update(int delta) {}
	virtual void on_draw(const Camera& camera) const {
		if (is_debug) {
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)pos.x, (int)pos.y, (int)(pos.x + size.x), (int)(pos.y + size.y));
			solidcircle((int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2), 5);
		}
	}

protected:
	bool check_if_exceeds_screen() {
		return (pos.x + size.x <= -size.x || pos.x >= getwidth() + size.x || pos.y + size.y <= -size.y  || pos.y >= getheight() + size.y);
	}


protected:
	Vector2 size;						// �ӵ��ߴ�
	Vector2 pos;						// �ӵ�λ��
	Vector2 velocity;					// �ӵ��ٶ�
	int damage = 10;					// �ӵ��˺�
	bool valid = true;					// �ӵ��Ƿ���Ч
	bool can_remove = false;			// �ӵ��Ƿ���Ա�ɾ��
	std::function<void()> callback;		// �ӵ���ײ�ص�����
	PlayerID target_id = PlayerID::P1;	// �ӵ���ײ��� ID

};
