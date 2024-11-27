#pragma once

#include "atlas.h"
#include "util.h"
#include "camera.h"

#include <graphics.h>
#include <functional>

class Animation {
public:
	Animation() = default;
	~Animation() = default;

	//重置动画（从头播放）
	void reset() {
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* new_atlas) {
		reset();
		atlas = new_atlas;
	}

	//设置是否循环播放
	void set_loop(bool flag) {
		is_loop = flag;
	}

	//设置帧间隔
	void set_interval(int ms) {
		interval = ms;
	}

	//获取帧索引
	int get_idx_frame() {
		return idx_frame;
	}

	//获取当前播放的帧动画
	IMAGE* get_frame() {
		return atlas->get_image(idx_frame);
	}

	//检查动画时候播放结束
	bool check_finished() {
		if (is_loop) return false;
		return (idx_frame == atlas->get_size() - 1);
	}

	//更新动画
	void on_update(int delta) {
		timer += delta;
		if (timer >= interval) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size()) {
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				if (!is_loop && callback) {
					callback();
				}
			}
		}
	}

	//绘制动画
	void on_drow(const Camera& camera,int x,int y) const {
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	//
	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}


private:
	int timer = 0;		//计时器
	int interval = 0;	//帧间隔
	int idx_frame = 0;	//帧索引
	bool is_loop = true;//是否循环
	Atlas* atlas = nullptr;
	std::function<void()> callback;

};
