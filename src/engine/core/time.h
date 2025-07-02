#pragma once

#include <SDL3/SDL_stdinc.h>

namespace engine::core {

class Time final {
private:
	Uint64 last_time;
	Uint64 frame_start_time;
	double delta_time = 0.0;
	double time_scale = 1.0; // Default time scale is 1.0 (normal speed)

	int target_fps = 0; // Default target FPS
	double target_frame_time = 0.0; // Target frame time in seconds
public:
	Time();
    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;
    Time(Time &&) = delete;
    Time &operator=(Time &&) = delete;

	void update();

	float getDeltaTime() const {
        return delta_time * time_scale;
    }

	float getUnscaledDeltaTime() const {
        return delta_time;
    }

	int getTargetFPS() const {
        return target_fps;
    }

	void setTargetFPS(int fps) {
        target_fps = fps;
        target_frame_time = target_fps > 0 ? 1.0 / target_fps : -1.0;
    }

private:
	void limitFrameRate(float current_delta_time);
};

} // namespace engine::core
