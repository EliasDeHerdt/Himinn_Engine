#pragma once
#include "Singleton.h"
#include <string>
#include <vector>

namespace Himinn
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void SetActiveScene(const std::string& name);
		
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();

		std::weak_ptr<Scene> GetActiveScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;

		std::weak_ptr<Scene> m_NewActiveScene;
		std::weak_ptr<Scene> m_ActiveScene;
	};
}
