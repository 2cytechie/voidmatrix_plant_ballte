#pragma once

#include "vector2.h"
#include "timer.h"

class Camera {
public:
	Camera() {
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			}
		);
	}

	~Camera() = default;

	const Vector2& get_pos() const {
		return pos;
	}

	void reset() {
		pos.x = 0;
		pos.y = 0;
	}

	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);//���ö�������ʱ��
		timer_shake.restart();

	}

	void on_undate(int delta) {
		timer_shake.on_update(delta);

		// ���������ʵ�ֶ���
		if (is_shaking) {
			pos.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			pos.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}


private:
	Vector2 pos;				//�����λ��
	Timer timer_shake;			//�����������ʱ��
	bool is_shaking = false;	//������Ƿ񶶶�
	float shaking_strength = 0; //�������������
};
