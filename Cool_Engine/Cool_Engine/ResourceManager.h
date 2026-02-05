#pragma once
#include <raylib.h>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>

class ResourceManager
{
public:
	static ResourceManager& instance()
	{
		static ResourceManager instance;
		return instance;
	}

	std::shared_ptr<Texture2D> load_texture(const std::string& file)
	{
		std::string filePath = filePathTex + file;
		auto it = map_textures.find(filePath);
		if (it != map_textures.end())
		{
			return it->second;
		}
		
		if (!FileExists(filePath.c_str()))
		{
			std::cerr << "Failed to load texture: " << filePath << std::endl;
			return nullptr;
		}
		auto texture = std::make_shared<Texture2D>(LoadTexture(filePath.c_str()));
		map_textures[filePath] = texture;
		return texture;
	}

	std::shared_ptr<Font> load_font(const std::string& file)
	{
		std::string filePath = filePathFonts + file;
		auto it = map_fonts.find(filePath);
		if (it != map_fonts.end())
		{
			return it->second;
		}

		if (!FileExists(filePath.c_str()))
		{
			std::cerr << "Failed to load font: " << filePath << std::endl;
			return nullptr;
		}
		auto font = std::make_shared<Font>(LoadFontEx(filePath.c_str(), 32, 0, 250));
		map_fonts[filePath] = font;
		return font;
	}

	std::shared_ptr<Sound> load_sounds(const std::string& file)
	{
		std::string filePath = filePathSounds + file;
		auto it = map_sounds.find(filePath);
		if (it != map_sounds.end())
		{
			return it->second;
		}

		if (!FileExists(filePath.c_str()))
		{
			std::cerr << "Failed to load sound: " << filePath << std::endl;
			return nullptr;
		}
		auto sound = std::make_shared<Sound>(LoadSound(filePath.c_str()));
		map_sounds[filePath] = sound;
		return sound;
	}

	std::shared_ptr<Music> load_music(const std::string& file)
	{
		std::string filePath = filePathMusic + file;
		auto it = map_music.find(filePath);
		if (it != map_music.end())
		{
			return it->second;
		}

		if (!FileExists(filePath.c_str()))
		{
			std::cerr << "Failed to load music: " << filePath << std::endl;
			return nullptr;
		}
		auto music = std::make_shared<Music>(LoadMusicStream(filePath.c_str()));
		map_music[filePath] = music;
		return music;
	}

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	std::unordered_map<std::string, std::shared_ptr<Texture2D>> map_textures;
	std::unordered_map<std::string, std::shared_ptr<Font>> map_fonts;
	std::unordered_map<std::string, std::shared_ptr<Sound>> map_sounds;
	std::unordered_map<std::string, std::shared_ptr<Music>> map_music;

	std::string filePathTex = "assets/textures/";
	std::string filePathFonts = "assets/fonts/";
	std::string filePathSounds = "assets/sounds/";
	std::string filePathMusic = "assets/music/";
};