#include "MiniginPCH.h"
#include "PlayerComponent.h"

#include "SubjectComponent.h"
#include "GameObject.h"
#include "Observer.h"

Himinn::PlayerComponent::PlayerComponent(const std::weak_ptr<GameObject>& owner, int)
	: Component(owner)
{
	if (!owner.lock()->AddComponent<SubjectComponent>(make_shared<SubjectComponent>(owner)))
		std::cout << "PlayerComponent: A SubjectComponent was already present, so no new one was added.\n";
	m_pSubjectComponent = owner.lock()->GetComponent<SubjectComponent>();
}

void Himinn::PlayerComponent::FixedUpdate()
{
}

void Himinn::PlayerComponent::Update()
{
}

void Himinn::PlayerComponent::LateUpdate()
{
}

void Himinn::PlayerComponent::Render()
{
}
