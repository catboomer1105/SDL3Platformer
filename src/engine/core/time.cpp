#include "time.h"
#include "SDL3/SDL_stdinc.h"

#include <SDL3/SDL_timer.h>
#include <spdlog/spdlog.h>

namespace engine::core {

Time::Time() {
	last_time = SDL_GetTicksNS();
	frame_start_time = last_time;
	spdlog::trace("Time system initialized. Last time: {}", last_time);
}

void Time::update() {
	frame_start_time = SDL_GetTicksNS();
	auto current_delta_time = static_cast<double>(frame_start_time - last_time) * 0.000000001; // Convert nanoseconds to seconds

	if (target_frame_time > 0.0) {
		limitFrameRate(current_delta_time);
	} else {
		delta_time = current_delta_time;
	}

	last_time = SDL_GetTicksNS();
}

void Time::limitFrameRate(float current_delta_time) {
    if (current_delta_time < target_frame_time) {
        double time_to_wait = target_frame_time - current_delta_time;
        Uint64 target_wait_time = static_cast<Uint64>(time_to_wait * 1000000000.0); // Convert seconds to nanoseconds
        SDL_DelayNS(target_wait_time);
        delta_time = static_cast<double>(SDL_GetTicksNS() - last_time) * 0.000000001; // Update delta_time after waiting
    }
}
} //namespace engine::core