#include "LevelManagerComponent.h"

#include <iostream>


#include "FPSComponent.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "GridComponent.h"
#include "GridObserver.h"
#include "ManagerObserver.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SubjectComponent.h"

LevelManagerComponent::LevelManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner)
	: Component(owner)
	, m_ActiveLevel()
	, m_pGridObserver(std::make_shared<GridObserver>())
	, m_LevelScenes()
{
}

void LevelManagerComponent::FixedUpdate()
{
}

void LevelManagerComponent::Update()
{
}

void LevelManagerComponent::LateUpdate()
{
}

void LevelManagerComponent::Render()
{
}

void LevelManagerComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

 	m_pGridObserver->SetLevelManager(m_Owner.lock()->GetComponent<LevelManagerComponent>());

	auto subjectComp = m_Owner.lock()->GetComponent<Himinn::SubjectComponent>();
	if (subjectComp.expired())
		std::cout << "LevelManagerComponent: No SubjectComponent was present, no observations will be made.\n";
	else
		m_SubjectComponent = subjectComp;
}

void LevelManagerComponent::StartGame()
{
	NotifyObserver();
}

void LevelManagerComponent::LevelCompleted()
{
	if (m_ActiveLevel + 1 < m_LevelScenes.size())
	{
		SetActiveLevel(m_ActiveLevel + 1);
		NotifyObserver();
	}
}

void LevelManagerComponent::AddLevel(std::string name, std::string settingsPath)
{
	m_LevelScenes.push_back(Himinn::SceneManager::GetInstance().CreateScene(name));
	std::weak_ptr<Himinn::Scene> scene = m_LevelScenes.back();
	if (scene.expired())
		return;
	
	// Font & color
	SDL_Color color = SDL_Color{ 0, 255, 0 };
	std::shared_ptr<Himinn::Font> font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	// Background
	auto go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(go, "background.jpg"));
	scene.lock()->Add(go);

	// Grid
	go = std::make_shared<Himinn::GameObject>();
	auto subjectComp = make_shared<Himinn::SubjectComponent>(go);
	subjectComp->AddObserver(m_pGridObserver);
	go->AddComponent<Himinn::SubjectComponent>(subjectComp);

	auto gridComp = make_shared<GridComponent>(go, scene.lock(), settingsPath);
	go->AddComponent<GridComponent>(gridComp);
	m_LevelGrids.push_back(gridComp);
	scene.lock()->Add(go);
	
	// FPS
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::FPSComponent>(make_shared<Himinn::FPSComponent>(go, font, color));
	go->SetPosition(0, 0);
	scene.lock()->Add(go);
}

std::weak_ptr<Himinn::Scene> LevelManagerComponent::GetLevel(int level)
{
	if (level > 0
		&& level < m_LevelScenes.size())
		return m_LevelScenes.at(level);

	std::cout << "LevelManagerComponent: Number " << std::to_string(level) << " doesn't exist in the list, returning empty pointer.";
	return std::weak_ptr<Himinn::Scene>();
}

std::weak_ptr<Himinn::Scene> LevelManagerComponent::GetLevel(std::string name)
{
	auto it = std::find_if(m_LevelScenes.begin(), m_LevelScenes.end(), [name](std::weak_ptr<Himinn::Scene> rhs)
		{
			return name == rhs.lock()->GetName();
		});

	if (it != m_LevelScenes.end())
		return *it;

	std::cout << "LevelManagerComponent: No level with name " << name << " found, returning empty pointer.";
	return std::weak_ptr<Himinn::Scene>();
}

void LevelManagerComponent::SetActiveLevel(unsigned level)
{
	if (level > 0
		&& level < m_LevelScenes.size())
	{
		if (m_ActiveLevel == level)
			return;
		
		m_ActiveLevel = level;
		TransferLevel();
	}
	else
		std::cout << "LevelManagerComponent: Number " << std::to_string(level) << " doesn't exist in the list, no changes where made.";
}

void LevelManagerComponent::SetActiveLevel(std::string name)
{
	int index{-1};
	auto it = std::find_if(m_LevelScenes.begin(), m_LevelScenes.end(), [name, &index](std::weak_ptr<Himinn::Scene> rhs)
		{
			++index;
			return name == rhs.lock()->GetName();
		});

	if (it != m_LevelScenes.end())
		SetActiveLevel(index);
	else
		std::cout << "LevelManagerComponent: No level with name " << name << " found, no changes where made.";
}

void LevelManagerComponent::TransferLevel()
{
	Himinn::SceneManager::GetInstance().SetActiveScene(m_LevelScenes.at(m_ActiveLevel).lock()->GetName());
}

void LevelManagerComponent::NotifyObserver()
{
	if (!m_SubjectComponent.expired())
	{
		Himinn::EventInfo info{};
		info.pointerInfo = { m_LevelScenes.at(m_ActiveLevel), m_LevelGrids.at(m_ActiveLevel) };
		m_SubjectComponent.lock()->Notify(info, (unsigned)ManagerObserverEvent::LevelLoaded);
	}
}
