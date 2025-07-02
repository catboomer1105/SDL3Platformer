#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <SDL3_ttf/SDL_ttf.h>

namespace engine::resource {

using FontKey = std::pair<std::string, int>;

struct FontKeyHash {
	std::size_t operator()(const FontKey &key) const {
		std::hash<std::string> string_hasher;
		std::hash<int> int_hasher;
		return string_hasher(key.first) ^ int_hasher(key.second);
	}
};

class FontManager final {
	friend class ResourceManager;

private:
	// TTF_Font 的自定义删除器
	struct SDLFontDeleter {
		void operator()(TTF_Font *font) const {
			if (font) {
				TTF_CloseFont(font);
			}
		}
	};

	std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> fonts;

public:

	FontManager();

	~FontManager();

	FontManager(const FontManager &) = delete;
	FontManager &operator=(const FontManager &) = delete;
	FontManager(FontManager &&) = delete;
	FontManager &operator=(FontManager &&) = delete;

private:
	TTF_Font *loadFont(std::string_view file_path, int point_size);
	TTF_Font *getFont(std::string_view file_path, int point_size);
	void unloadFont(std::string_view file_path, int point_size);
	void clearFonts();
};

} //namespace engine::resource