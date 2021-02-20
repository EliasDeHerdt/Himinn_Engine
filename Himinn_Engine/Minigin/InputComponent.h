#pragma once
#include "Component.h"
#include "GameObject.h"

namespace Himinn
{
	
	class InputComponent : public Component
	{
	public:
		InputComponent(weak_ptr<GameObject> owner, bool useController);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(const Transform&);
	private:
		weak_ptr<GameObject> m_Owner;
		bool m_UseController;
	};
}