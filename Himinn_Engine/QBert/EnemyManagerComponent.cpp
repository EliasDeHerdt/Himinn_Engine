#include "EnemyManagerComponent.h"

#include <iostream>

#include "CoilyComponent.h"
#include "ControllerComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SubjectComponent.h"
#include "SlickAndSamComponent.h"
#include "UggAndWrongwayComponent.h"

EnemyManagerComponent::EnemyManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner)
	: Component(owner)
	, m_Active(false)
	, m_MinSpawnDelaySS(6.f)
	, m_MinSpawnDelayUW(1.f)
	, m_MaxSpawnDelay(10.f)
	, m_SpawnDelaySS(RandFloat(m_MinSpawnDelaySS, m_MaxSpawnDelay))
	, m_SpawnDelayUW(RandFloat(m_MinSpawnDelayUW, m_MaxSpawnDelay))
	, m_SpawnDelayCoily(4.f)
	, m_TimerSS(0.f)
	, m_TimerUW(0.f)
	, m_TimerCoily(0.f)
	, m_StepDelay(0.8f)
	, m_Coily()
	, m_Enemies()
	, m_pScene()
	, m_pGridComponent()
	, m_SubjectComponent()
{
}

void EnemyManagerComponent::FixedUpdate()
{
}

void EnemyManagerComponent::Update()
{
	if (!m_Active)
		return;

	if (!m_Enemies.empty())
		CheckDeadEnemies();
	
	float dt = Himinn::GameTime::GetInstance().GetDeltaTime();
	
	m_TimerSS += dt;
	m_TimerUW += dt;
	if (m_Coily.expired())
		m_TimerCoily += dt;

	if (m_TimerSS >= m_SpawnDelaySS)
	{
		m_TimerSS = 0.f;
		m_SpawnDelaySS = RandFloat(m_MinSpawnDelaySS, m_MaxSpawnDelay);
		SpawnSlickOrSam();
	}

	if (m_TimerUW >= m_SpawnDelayUW)
	{
		m_TimerUW = 0.f;
		m_SpawnDelayUW = RandFloat(m_MinSpawnDelayUW, m_MaxSpawnDelay);
		SpawnUggOrWrongway();
	}

	if (m_TimerCoily >= m_SpawnDelayCoily)
	{
		m_TimerCoily = 0.f;
		SpawnCoily();
	}
}

void EnemyManagerComponent::LateUpdate()
{
}

void EnemyManagerComponent::Render()
{
}

void EnemyManagerComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

	auto subjectComp = m_Owner.lock()->GetComponent<Himinn::SubjectComponent>();
	if (subjectComp.expired())
		std::cout << "EnemyManagerComponent: No SubjectComponent was present, no observations will be made.\n";
	else
		m_SubjectComponent = subjectComp;
}

void EnemyManagerComponent::SetupManagerForLevel(const std::weak_ptr<Himinn::Scene>& scene, const std::weak_ptr<GridComponent>& grid)
{
	if (scene.expired()
		|| grid.expired())
		return;

	m_pScene = scene;
	m_pGridComponent = grid;

	m_Active = true;
	ClearEnemies();
}

void EnemyManagerComponent::SpawnSlickOrSam()
{
	if (m_pScene.expired()
		|| m_pGridComponent.expired())
		return;

	auto go = std::make_shared<Himinn::GameObject>();

	auto ssComp = std::make_shared<SlickAndSamComponent>(go, m_pGridComponent, m_StepDelay);
	go->AddComponent<SlickAndSamComponent>(ssComp);

	m_Enemies.push_back(go);
	m_pScene.lock()->Add(go);
	ssComp->Spawn();
}

void EnemyManagerComponent::SpawnUggOrWrongway()
{
	if (m_pScene.expired()
		|| m_pGridComponent.expired())
		return;

	auto go = std::make_shared<Himinn::GameObject>();
	
	auto uwComp = std::make_shared<UggAndWrongwayComponent>(go, m_pGridComponent, m_StepDelay);
	go->AddComponent<UggAndWrongwayComponent>(uwComp);

	m_Enemies.push_back(go);
	m_pScene.lock()->Add(go);
	uwComp->Spawn();
}

void EnemyManagerComponent::SpawnCoily()
{
	if (m_pScene.expired()
		|| m_pGridComponent.expired())
		return;

	auto go = std::make_shared<Himinn::GameObject>();
	
	auto coilyComp = std::make_shared<CoilyComponent>(go, m_pGridComponent, m_Players, m_StepDelay);
	go->AddComponent<CoilyComponent>(coilyComp);
	
	m_Coily = go;
	m_pScene.lock()->Add(go);
	coilyComp->Spawn();
}

void EnemyManagerComponent::ClearEnemies()
{
	if (!m_Coily.expired())
		m_Coily.lock()->MarkForDestruction();

	for (auto enemy : m_Enemies)
		if (!enemy.expired())
			enemy.lock()->MarkForDestruction();
}

void EnemyManagerComponent::SetPlayers(const std::vector<std::weak_ptr<ControllerComponent>>& players)
{
	m_Players = players;
}

float EnemyManagerComponent::RandFloat(float min, float max)
{
	return min + float(rand()) / float(RAND_MAX / (max - min));
}

void EnemyManagerComponent::CheckDeadEnemies()
{
	m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](const std::weak_ptr<Himinn::GameObject>& rhs)
		{
			return rhs.expired();
		}), m_Enemies.end());
}
