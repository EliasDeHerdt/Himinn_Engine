#include "MiniginPCH.h"
#include "SubjectComponent.h"
#include <algorithm>



Himinn::SubjectComponent::SubjectComponent(const std::weak_ptr<GameObject>& owner)
	: Component(owner)
{
}

void Himinn::SubjectComponent::FixedUpdate()
{
}

void Himinn::SubjectComponent::Update()
{
}

void Himinn::SubjectComponent::LateUpdate()
{
}

void Himinn::SubjectComponent::Render()
{
}

void Himinn::SubjectComponent::Notify(EventInfo eventInfo, unsigned int observerEvent) const
{
	std::for_each(m_Observers.begin(), m_Observers.end(), [eventInfo, observerEvent](std::shared_ptr<Observer> var)
		{
			var->OnNotify(eventInfo, observerEvent);
		});
}

bool Himinn::SubjectComponent::AddObserver(const std::shared_ptr<Observer>& observer)
{
	if (observer)
	{
		m_Observers.push_back(observer);
		return true;
	}
	
	return false;
}

bool Himinn::SubjectComponent::RemoveObserver(const std::shared_ptr<Observer>&)
{
	return false;
}
