#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

std::weak_ptr<Himinn::Scene> Himinn::SceneManager::CreateScene(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](std::shared_ptr<Scene> rhs)
		{
			return name == rhs->GetName();
		});
	
	if (it != m_Scenes.end())
	{
		std::cout << "SceneManager: A scene with name " << name << " does already exist, no scene added.";
		return *it;
	}
	
	const auto scene = std::shared_ptr<Scene>(new Scene(name));

	if (m_ActiveScene.expired())
		m_ActiveScene = scene;

	m_Scenes.push_back(scene);
	return scene;
}

void Himinn::SceneManager::SetActiveScene(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](std::shared_ptr<Scene> rhs)
		{
			return name == rhs->GetName();
		});

	if (it != m_Scenes.end())
		m_NewActiveScene = *it;
	else
		std::cout << "SceneManager: scene with " << name << " does not exist, not changing active scene.";
}

std::weak_ptr<Himinn::Scene> Himinn::SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}

void Himinn::SceneManager::FixedUpdate()
{
	m_ActiveScene.lock()->FixedUpdate();
}

void Himinn::SceneManager::Update()
{
	m_ActiveScene.lock()->Update();
}

void Himinn::SceneManager::LateUpdate()
{
	m_ActiveScene.lock()->LateUpdate();

	if (!m_NewActiveScene.expired()
		&& m_NewActiveScene.lock() != m_ActiveScene.lock())
		m_ActiveScene = m_NewActiveScene;

	m_Scenes.erase(std::remove_if(m_Scenes.begin(), m_Scenes.end(), [](std::shared_ptr<Scene> rhs)
		{
			return rhs->ShouldBeDestroyed();
		}), m_Scenes.end());
}

void Himinn::SceneManager::Render()
{
	m_ActiveScene.lock()->Render();
}