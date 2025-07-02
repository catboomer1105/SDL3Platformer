#pragma once

#include <memory>
#include <unordered_map>
#include <string_view>

#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>

namespace engine::resource {

class TextureManager final {

friend class ResourceManager;

private:
	struct SDLTextureDeleter {
		void operator()(SDL_Texture *texture) const {
			if (texture) {
				SDL_DestroyTexture(texture);
			}
		}
	};

	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> textures;

	SDL_Renderer *renderer_ = nullptr;

public:
	explicit TextureManager(SDL_Renderer *renderer);

	TextureManager(const TextureManager &) = delete;
	TextureManager &operator=(const TextureManager &) = delete;
	TextureManager(TextureManager &&) = delete;
	TextureManager &operator=(TextureManager &&) = delete;

private:
	SDL_Texture *loadTexture(std::string_view file_path);
	SDL_Texture *getTexture(std::string_view file_path);
	glm::vec2 getTextureSize(std::string_view file_path);
	void unloadTexture(std::string_view file_path);
	void clearTextures();
};

} //namespace engine::resource