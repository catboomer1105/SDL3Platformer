#include "resource_manager.h"

#include <spdlog/spdlog.h>

#include "font_manager.h"
#include "texture_manager.h"


namespace engine::resource {

ResourceManager::ResourceManager(SDL_Renderer *renderer) {
	texture_manager = std::make_unique<TextureManager>(renderer);
	font_manager = std::make_unique<FontManager>();
}

ResourceManager::~ResourceManager() = default;

void ResourceManager::clear() {
	texture_manager->clearTextures();
	font_manager->clearFonts();
	spdlog::trace("ResourceManager cleared all resources.");
}

SDL_Texture *ResourceManager::loadTexture(const std::string &filePath) {
	return texture_manager->loadTexture(filePath);
}
SDL_Texture *ResourceManager::getTexture(const std::string &filePath) {
	return texture_manager->getTexture(filePath);
}

void ResourceManager::unloadTexture(const std::string &filePath) {
	texture_manager->unloadTexture(filePath);
}

void ResourceManager::clearTextures() {
	texture_manager->clearTextures();
	spdlog::trace("ResourceManager cleared all textures.");
}

TTF_Font *ResourceManager::loadFont(std::string_view file_path, int point_size) {
	return font_manager->loadFont(file_path, point_size);
}
TTF_Font *ResourceManager::getFont(std::string_view file_path, int point_size) {
	return font_manager->getFont(file_path, point_size);
}
void ResourceManager::unloadFont(std::string_view file_path, int point_size) {
	font_manager->unloadFont(file_path, point_size);
}
void ResourceManager::clearFonts() {
	font_manager->clearFonts();
	spdlog::trace("ResourceManager cleared all fonts.");
}

} //namespace engine::resource