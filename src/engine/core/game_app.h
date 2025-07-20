#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource {
	class ResourceManager;
}

namespace engine::render{
class Renderer;
class Camera;
}

namespace engine::core {

class Time;

class GameApp final {

private:
    SDL_Window *sdl_window;
	SDL_Renderer *sdl_renderer;
    bool is_running = false;

    //Engine Components
    std::unique_ptr<engine::core::Time> time;
	std::unique_ptr<engine::resource::ResourceManager> resource_manager;
	std::unique_ptr<engine::render::Renderer> renderer;
	std::unique_ptr<engine::render::Camera> camera;

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
	[[nodiscard]] bool initSDL();
	[[nodiscard]] bool initTime();
	[[nodiscard]] bool initResourceManager();
	[[nodiscard]] bool initRenderer();
	[[nodiscard]] bool initCamera();

	//Test functions
	void testResourceManager();
	void testRenderer();
	void testCamera();
};
} // namespace engine::core