#pragma once
struct SDL_Window;
namespace Himinn
{
	class Minigin
	{
	public:
		void Initialize();
		virtual void LoadGame() const;
		virtual void Cleanup();
		void Run();
	private:
		//static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		static const int MsPerUpdate = 2; // <- renamed for code clarity
		SDL_Window* m_Window{};
	};
}