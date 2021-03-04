#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"

void Himinn::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
	ImGui::GetStyle().WindowRounding = 5.0f;
}

void Himinn::Renderer::Render()
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);

	ImVec2 buttonSize = { 120, 20 };
	ImGui::NewFrame(); // creates new debug window
	ImGui::Begin("Select Mode", &m_ShowHud/*, ImGuiWindowFlags_MenuBar*/);
	ImGui::Button("Single player", buttonSize);
	ImGui::Button("Co-op", buttonSize);
	ImGui::Button("Versus", buttonSize);
	if (ImGui::Button("How To Play", buttonSize))
		m_ShowTutorial = !m_ShowTutorial;
	ImGui::End();

	if (m_ShowTutorial)
	{
		ImGui::Begin("How To Play", &m_ShowHud);
		ImGui::Text("Currently, both players are controlled by 1 controller.");
		ImGui::Text("Although, multiple players are supported! (I own 1 controller)");
		ImGui::Text("A: Player 1 gains 25 points");
		ImGui::Text("B: Player 1 dies");
		ImGui::Text("X: Player 1 gains 500 points");
		ImGui::Text("D-Pad Down: Player 2 gains 25 points");
		ImGui::Text("D-Pad Right: Player 2 dies");
		ImGui::Text("D-Pad Left: Player 2 gains 500 points");
		ImGui::End();
	}
	
	//if (m_ShowHud)
	//	ImGui::ShowDemoWindow(&m_ShowHud);
	
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_Renderer);
}

void Himinn::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Himinn::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Himinn::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

// Function made with information found on forum
int Himinn::Renderer::GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}
