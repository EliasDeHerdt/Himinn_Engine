#pragma once
#include "Transform.h"

using namespace std;
namespace Himinn {
	
	class GameObject;
	class Component
	{
	public:
		Component() {};
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void Render(const Transform& transform) = 0;
	protected:
		bool m_NeedsUpdate = false;
	};
}
