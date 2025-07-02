#include "texture_manager.h"

#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace engine::resource {

TextureManager::TextureManager(SDL_Renderer *renderer) : renderer_(renderer) {
    if (!renderer_) {
        spdlog::error("SDL_Renderer is null. TextureManager cannot be initialized.");
        throw std::runtime_error("SDL_Renderer is null. TextureManager cannot be initialized.");
    }
    spdlog::trace("TextureManager initialized successfully.");
}


SDL_Texture* TextureManager::loadTexture(std::string_view file_path) {
    auto it = textures.find(std::string(file_path));
    if (it != textures.end()) {
        return it->second.get();
    }

    // if the texture is not found, load it
    SDL_Texture* raw_texture = IMG_LoadTexture(renderer_, file_path.data());

    // When loading a texture, set the texture scaling mode to nearest neighbor (this is essential, otherwise there will be edge gaps/blurriness in TileLayer rendering)
    if (!SDL_SetTextureScaleMode(raw_texture, SDL_SCALEMODE_NEAREST)) {
        spdlog::warn("Cannot set texture scale mode for '{}': {}", file_path, SDL_GetError());
    }

    if (!raw_texture) {
        spdlog::error("Failed to load texture: '{}': {}", file_path, SDL_GetError());
        return nullptr;
    }

    textures.emplace(file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(raw_texture));
    spdlog::debug("Successfully loaded and cached texture: {}", file_path);

    return raw_texture;
}

SDL_Texture* TextureManager::getTexture(std::string_view file_path) {
    auto it = textures.find(std::string(file_path));
    if (it != textures.end()) {
        return it->second.get();
    }

    // if the texture is not found, try to load it
    spdlog::warn("Texture '{}' not found in cache, trying to load.", file_path);
    return loadTexture(file_path);
}

glm::vec2 TextureManager::getTextureSize(std::string_view file_path) {
    // get the texture from cache or load it
    SDL_Texture* texture = getTexture(file_path);
    if (!texture) {
        spdlog::error("Failed to get texture: {}", file_path);
        return glm::vec2(0);
    }

    // query the texture size
    glm::vec2 size;
    if (!SDL_GetTextureSize(texture, &size.x, &size.y)) {
        spdlog::error("Failed to query texture size: {}", file_path);
        return glm::vec2(0);
    }
    return size;
}

void TextureManager::unloadTexture(std::string_view file_path) {
    auto it = textures.find(std::string(file_path));
    if (it != textures.end()) {
        spdlog::debug("Unloading texture: {}", file_path);
        textures.erase(it);
    } else {
        spdlog::warn("Attempting to unload a non-existent texture: {}", file_path);
    }
}

void TextureManager::clearTextures() {
    if (!textures.empty()) {
        spdlog::debug("Clearing all {} cached textures.", textures.size());
        textures.clear();
    }
}



} // namespace engine::resource