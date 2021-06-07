#include "EndMenuComponent.h"

#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"
#include "InputManager.h"
#include "PlayerManagerComponent.h"
#include "Renderer.h"

EndMenuComponent::EndMenuComponent(const std::weak_ptr<Himinn::GameObject>& owner)
	: Component(owner)
{
}

void EndMenuComponent::FixedUpdate()
{
}

void EndMenuComponent::Update()
{
}

void EndMenuComponent::LateUpdate()
{
}

void EndMenuComponent::Render()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(Himinn::Renderer::GetInstance().GetWindow());

	ImVec2 buttonSize = { 120, 20 };
	ImGui::NewFrame(); // creates new debug window
	ImGui::Begin("End Menu", &m_ShowHud);
	ImGui::Text("You either won or you died...");
	ImGui::Text("Thanks for playing!");
	if (ImGui::Button("ExitGame", buttonSize))
	{
		Himinn::InputManager::GetInstance().StopGame();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void EndMenuComponent::OnAddedToObject()
{
}
