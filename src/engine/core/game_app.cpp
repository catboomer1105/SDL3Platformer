#include "game_app.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include <spdlog/spdlog.h>

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
void GameApp::update(float deltaTime) {}
void GameApp::render() {}
void GameApp::cleanup() {
	spdlog::trace("Close game application...");

	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
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

	window = SDL_CreateWindow("Platformer", 1280, 720, SDL_WINDOW_RESIZABLE);
	if (!window) {
		spdlog::error("Window could not be created! SDL_Error: {}", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
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
		resource_manager = std::make_unique<engine::resource::ResourceManager>(renderer);
	} catch (const std::exception &e) {
		spdlog::error("Failed to initialize Resource Manager: {}", e.what());
		return false;
	}

	spdlog::trace("ResourceManager initialized successfully.");
	return true;
}

void GameApp::testResourceManager() {
    resource_manager->getTexture("assets/textures/Actors/eagle-attack.png");
    resource_manager->getFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);

    resource_manager->unloadTexture("assets/textures/Actors/eagle-attack.png");
    resource_manager->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
}
} //namespace engine::core
