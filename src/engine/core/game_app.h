#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource {
	class ResourceManager;
}


namespace engine::core {

class Time;

class GameApp final {

private:
    SDL_Window *window;
	SDL_Renderer *renderer;
    bool is_running = false;

    //Engine Components
    std::unique_ptr<engine::core::Time> time;
	std::unique_ptr<engine::resource::ResourceManager> resource_manager;

public:
	GameApp();
	~GameApp();

	void run();

	GameApp(const GameApp &) = delete;
	GameApp &operator=(const GameApp &) = delete;
	GameApp(GameApp &&) = delete;
	GameApp &operator=(GameApp &&) = delete;

private:
	[[nodiscard]] bool init();
	void handleEvents();
	void update(float deltaTime);
	void render();
	void cleanup();


	// Engine Component Initialization
	bool initSDL();
	bool initTime();
	bool initResourceManager();

	//Test functions
	void testResourceManager();
};
} // namespace engine::core