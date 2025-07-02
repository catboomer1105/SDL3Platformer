#include "font_manager.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource {

FontManager::FontManager() {
	if (!TTF_WasInit() && !TTF_Init()) {
		throw std::runtime_error("FontManager error: TTF_Init failed: " + std::string(SDL_GetError()));
	}
	spdlog::trace("FontManager constructed successfully.");
}

FontManager::~FontManager() {
	if (!fonts.empty()) {
		spdlog::debug("FontManager is not empty, calling clearFonts to handle cleanup.");
		clearFonts();
	}
	TTF_Quit();
	spdlog::trace("FontManager destructed successfully.");
}

TTF_Font *FontManager::loadFont(std::string_view file_path, int point_size) {
	if (point_size <= 0) {
		spdlog::error("Failed to load font '{}': invalid point size {}.", file_path, point_size);
		return nullptr;
	}

	FontKey key = { std::string(file_path), point_size };

	auto it = fonts.find(key);
	if (it != fonts.end()) {
		return it->second.get();
	}

	spdlog::debug("Loading font: {} ({}pt)", file_path, point_size);
	TTF_Font *raw_font = TTF_OpenFont(file_path.data(), point_size);
	if (!raw_font) {
		spdlog::error("Failed to load font '{}' ({}pt): {}", file_path, point_size, SDL_GetError());
		return nullptr;
	}

	fonts.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(raw_font));
	spdlog::debug("Successfully loaded and cached font: {} ({}pt)", file_path, point_size);
	return raw_font;
}

TTF_Font *FontManager::getFont(std::string_view file_path, int point_size) {
	FontKey key = { std::string(file_path), point_size };
	auto it = fonts.find(key);
	if (it != fonts.end()) {
		return it->second.get();
	}

	spdlog::warn("Font '{}' ({}pt) is not in cache, attempting to load.", file_path, point_size);
	return loadFont(file_path, point_size);
}

void FontManager::unloadFont(std::string_view file_path, int point_size) {
	FontKey key = { std::string(file_path), point_size };
	auto it = fonts.find(key);
	if (it != fonts.end()) {
		spdlog::debug("Unloading font: {} ({}pt)", file_path, point_size);
		fonts.erase(it); // unique_ptr 会处理 TTF_CloseFont
	} else {
		spdlog::warn("Attempting to unload non-existent font: {} ({}pt)", file_path, point_size);
	}
}

void FontManager::clearFonts() {
	if (!fonts.empty()) {
		spdlog::debug("Clearing all {} cached fonts.", fonts.size());
		fonts.clear();
	}
}

} // namespace engine::resource