#pragma once
#include <memory>

namespace Himinn {

	class GameObject;
	class Component
	{
	public:
		Component(const std::weak_ptr<GameObject>& owner)
			: m_Owner{ owner }
		{};
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void Render() = 0;
		
		virtual void OnAddedToObject(){};
	
	protected:
		bool m_NeedsUpdate = false;
		std::weak_ptr<GameObject> m_Owner;
	};
}
