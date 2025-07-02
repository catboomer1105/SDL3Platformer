#pragma once

#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct TTF_Font;

namespace engine::resource {

class TextureManager;
class FontManager;

class ResourceManager {
private:
	std::unique_ptr<TextureManager> texture_manager;
	std::unique_ptr<FontManager> font_manager;

public:
	explicit ResourceManager(SDL_Renderer *renderer);
	~ResourceManager();

	void clear();

	ResourceManager(const ResourceManager &) = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&) = delete;
	ResourceManager &operator=(ResourceManager &&) = delete;

	// Unified interface for resource management
	SDL_Texture *loadTexture(const std::string &filePath);
	SDL_Texture *getTexture(const std::string &filePath);
	void unloadTexture(const std::string &filePath);
	void clearTextures();

	TTF_Font *loadFont(std::string_view file_path, int point_size);
	TTF_Font *getFont(std::string_view file_path, int point_size);
	void unloadFont(std::string_view file_path, int point_size);
	void clearFonts();
};
} //namespace engine::resource