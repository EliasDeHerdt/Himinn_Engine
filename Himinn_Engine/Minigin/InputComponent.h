#pragma once
#include "Component.h"

namespace Himinn
{
	
	class InputComponent : public Component
	{
	public:
		InputComponent(const std::weak_ptr<GameObject>& owner, bool useController);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	private:
		bool m_UseController;
	};
}