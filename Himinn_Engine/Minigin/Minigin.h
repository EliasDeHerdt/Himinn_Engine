#pragma once
struct SDL_Window;
namespace Himinn
{
	class Minigin
	{
	public:
		Minigin() = default;
		virtual ~Minigin() = default;
		Minigin(const Minigin& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
		
		void Initialize();
		virtual void LoadGame();
		virtual void Cleanup();
		virtual void UIRender();
		void Run();
	
	protected:
		//static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		static const int MsPerUpdate = 2; // <- renamed for code clarity
		SDL_Window* m_Window{};
	};
}