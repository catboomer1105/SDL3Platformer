#include "game_app.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include "../render/camera.h"
#include "../render/renderer.h"
#include "../render/sprite.h"
#include "../resource/resource_manager.h"
#include "time.h"


namespace engine::core {

GameApp::GameApp() = default;

GameApp::~GameApp() {
	if (is_running) {
		spdlog::trace("Game application is being destroyed, cleaning up resources...");
		cleanup();
	}
}

void GameApp::run() {
	if (!init()) {
		spdlog::error("Failed to initialize the game application.");
		return;
	}

	//testResourceManager();

	time->setTargetFPS(60);

	while (is_running) {
		time->update();
		float deltaTime = time->getDeltaTime();

		handleEvents();
		update(deltaTime);
		render();

		//spdlog::info("Frame rendered. Delta Time: {:.3f} seconds", deltaTime);
	}

	cleanup();
}

bool GameApp::init() {
	spdlog::trace("Initializing game application...");

	if (!initSDL()) {
		return false;
	}
	if (!initTime()) {
		return false;
	}
	if (!initResourceManager()) {
		return false;
	}

	if (!initRenderer()) {
		return false;
	}

	if (!initCamera()) {
		return false;
	}

	is_running = true;

	spdlog::trace("Game application initialized successfully.");

	return true;
}

void GameApp::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			is_running = false;
		}
	}
}
void GameApp::update(float deltaTime) {
	testCamera();
}

void GameApp::render() {
	// 1. 清除屏幕
	renderer->clearScreen();

	// 2. 具体渲染代码
	testRenderer();

	// 3. 更新屏幕显示
	renderer->present();
}
void GameApp::cleanup() {
	spdlog::trace("Close game application...");

	if (sdl_renderer) {
		SDL_DestroyRenderer(sdl_renderer);
		sdl_renderer = nullptr;
	}
	if (sdl_window) {
		SDL_DestroyWindow(sdl_window);
		sdl_window = nullptr;
	}
	SDL_Quit();
	is_running = false;
}

bool GameApp::initSDL() {
	spdlog::trace("Initializing SDL...");
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		spdlog::error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
		return false;
	}

	sdl_window = SDL_CreateWindow("Platformer", 1280, 720, SDL_WINDOW_RESIZABLE);
	if (!sdl_window) {
		spdlog::error("Window could not be created! SDL_Error: {}", SDL_GetError());
		return false;
	}

	sdl_renderer = SDL_CreateRenderer(sdl_window, nullptr);
	if (!sdl_renderer) {
		spdlog::error("Renderer could not be created! SDL_Error: {}", SDL_GetError());
		return false;
	}

	

	spdlog::trace("SDL initialized successfully.");
	return true;
}

bool GameApp::initTime() {
	spdlog::trace("Initializing Time ...");
	try {
		time = std::make_unique<Time>();
	} catch (const std::exception &e) {
		spdlog::error("Failed to initialize Time: {}", e.what());
		return false;
	}
	spdlog::trace("Time initialized successfully.");
	return true;
}

bool GameApp::initResourceManager() {
	spdlog::trace("Initializing ResourceManager...");

	try {
		resource_manager = std::make_unique<engine::resource::ResourceManager>(sdl_renderer);
	} catch (const std::exception &e) {
		spdlog::error("Failed to initialize Resource Manager: {}", e.what());
		return false;
	}

	spdlog::trace("ResourceManager initialized successfully.");
	return true;
}

bool GameApp::initRenderer() {
	spdlog::trace("Initializing Renderer...");
	try {
		renderer = std::make_unique<engine::render::Renderer>(sdl_renderer, resource_manager.get());
	} catch (const std::exception &e) {
		spdlog::error("Failed to initialize Renderer: {}", e.what());
		return false;
	}
	spdlog::trace("Renderer initialized successfully.");
	return true;
}

bool GameApp::initCamera() {
	spdlog::trace("Initializing Camera...");
	try {
		camera = std::make_unique<engine::render::Camera>(glm::vec2(640, 360));
	} catch (const std::exception &e) {
		spdlog::error("Failed to initialize Camera: {}", e.what());
		return false;
	}
	spdlog::trace("Camera initialized successfully.");
	return true;
}

// --- Test Functions ---

void GameApp::testResourceManager() {
	resource_manager->getTexture("assets/textures/Actors/eagle-attack.png");
	resource_manager->getFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);

	resource_manager->unloadTexture("assets/textures/Actors/eagle-attack.png");
	resource_manager->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
}

void GameApp::testRenderer() {
	engine::render::Sprite sprite_world("assets/textures/Actors/frog.png");
	engine::render::Sprite sprite_ui("assets/textures/UI/buttons/Start1.png");
	engine::render::Sprite sprite_parallax("assets/textures/Layers/back.png");

	static float rotation = 0.0f;
	rotation += 0.1f;

	// 注意渲染顺序
	renderer->drawParallax(*camera, sprite_parallax, glm::vec2(100, 100), glm::vec2(0.5f, 0.5f), glm::bvec2(true, false));
	renderer->drawSprite(*camera, sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), rotation);
	renderer->drawUISprite(sprite_ui, glm::vec2(100, 100));
}

void GameApp::testCamera() {
	auto key_state = SDL_GetKeyboardState(nullptr);
	if (key_state[SDL_SCANCODE_UP]) {
		camera->move(glm::vec2(0, -1));
	}
	if (key_state[SDL_SCANCODE_DOWN]) {
		camera->move(glm::vec2(0, 1));
	}
	if (key_state[SDL_SCANCODE_LEFT]) {
		camera->move(glm::vec2(-1, 0));
	}
	if (key_state[SDL_SCANCODE_RIGHT]) {
		camera->move(glm::vec2(1, 0));
	}
}

} //namespace engine::core
