#pragma once
#include "Component.h"
#include "RenderComponent.h"

using namespace std;
namespace Himinn {

	class ImageComponent : public Component
	{
	public:
		ImageComponent(const string& filename);

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(const Transform&) override;

		void SetTexture(const string& filename);

	private:
		RenderComponent m_RenderComponent;
	};
}