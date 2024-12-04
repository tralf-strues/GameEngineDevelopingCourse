#pragma once

#include <Timer.h>

namespace GameEngine
{
	class Editor final
	{
	public:
		Editor() = delete;
		Editor(
			std::function<bool()> PlatformLoopFunc
		);

	public:
		void Run();
		//void Update(float dt);

	private:
		Core::Timer m_Timer;

	private:
		Render::RenderThread::Ptr m_renderThread = nullptr;

		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;
	};
}