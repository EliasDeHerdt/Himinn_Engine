#pragma once
#include "SceneManager.h"

namespace Himinn
{
	class GameObject;
	class Scene
	{
		friend std::weak_ptr<Himinn::Scene> SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);
		void Remove(const std::shared_ptr<GameObject>& object);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		const std::string& GetName() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void MarkForDestruction() { m_Delete = true; }
		bool ShouldBeDestroyed() const { return m_Delete; }

	private: 
		explicit Scene(const std::string& name);

		bool m_Delete;
		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter; 
	};

}
