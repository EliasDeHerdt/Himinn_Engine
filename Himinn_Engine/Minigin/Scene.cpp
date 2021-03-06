#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace Himinn;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Remove(const std::shared_ptr<GameObject>& object)
{
	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), [object](const std::shared_ptr<GameObject>& rhs)
		{
			return object == rhs;
		}), m_Objects.end());
}

void Himinn::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Himinn::Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		object->LateUpdate();
	}

	// Clean-Up
	for (auto& object : m_Objects)
	{
		if (object
			&& object->ShouldBeDestroyed())
			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

const std::string& Himinn::Scene::GetName() const
{
	return m_Name;
}

