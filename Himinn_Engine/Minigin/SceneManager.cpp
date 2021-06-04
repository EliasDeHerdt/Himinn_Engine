#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

Himinn::Scene& Himinn::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));

	if (m_ActiveScene.expired())
		m_ActiveScene = scene;

	m_Scenes.push_back(scene);
	return *scene;
}

void Himinn::SceneManager::SetActiveScene(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](std::shared_ptr<Scene> rhs)
		{
			return name == rhs->GetName();
		});

	if (it != m_Scenes.end())
		m_NewActiveScene = *it;
}

void Himinn::SceneManager::FixedUpdate()
{
	if (!m_NewActiveScene.expired()
		&& m_NewActiveScene.lock() != m_ActiveScene.lock())
		m_ActiveScene = m_NewActiveScene;
	
	m_ActiveScene.lock()->FixedUpdate();
}

void Himinn::SceneManager::Update()
{
	m_ActiveScene.lock()->Update();
}

void Himinn::SceneManager::LateUpdate()
{
	m_ActiveScene.lock()->LateUpdate();
}

void Himinn::SceneManager::Render()
{
	m_ActiveScene.lock()->Render();
}

std::weak_ptr<Himinn::Scene> Himinn::SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}
