#pragma once
#include <string>

class LevelManager
{
public:
	LevelManager();
	virtual ~LevelManager() = default;
	LevelManager(const LevelManager& other) = delete;
	LevelManager& operator=(const LevelManager& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	LevelManager& operator=(LevelManager&& other) = delete;

	void SetUpLevel(std::string sceneName, std::string settingsPath);

private:
	
};
