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
		std::weak_ptr<Himinn::Scene> CreateScene(const std::string& name);
		void SetActiveScene(const std::string& name);
		std::weak_ptr<Scene> GetActiveScene() const;
		void RemoveScene(const std::string& name);
		
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;

		std::weak_ptr<Scene> m_NewActiveScene;
		std::weak_ptr<Scene> m_ActiveScene;
	};
}
