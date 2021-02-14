#pragma once
#include "Component.h"

using namespace std;
namespace Himinn {

	class Texture2D;
	class RenderComponent : public Component
	{
		public:
			RenderComponent();

			virtual void FixedUpdate() override;
			virtual void Update() override;
			virtual void LateUpdate() override;
			virtual void Render(const Transform& transform) override;

			virtual void SetTexture(const shared_ptr<Texture2D>& texture);

		private:
			shared_ptr<Texture2D> m_Texture;
	};
}

