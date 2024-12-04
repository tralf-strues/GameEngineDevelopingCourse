#include <Editor.h>
#include <Camera.h>
#include <DefaultGeometry.h>
#include <EntityManager.h>
#include <Input/InputHandler.h>

#include <Window/IWindow.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx12.h>

#include <RHIContext.h>
#include <D3D12RHICore.h>
#include <D3D12RHIContext.h>

namespace GameEngine
{
	Editor::Editor(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 12.0f, -10.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 12.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		//m_EntityManager = std::make_unique<EntitySystem::EntityManager>(m_renderThread.get());

		/*
		* HUGE TEST
		*/

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(GameEngine::Core::g_MainWindowsApplication->GetWindowHandle());
		ImGui_ImplDX12_Init((ID3D12Device*)m_renderThread->GetRHI()->GetDevice()->GetNativeObject(), RenderCore::g_FrameBufferCount, m_renderThread->GetRHI()->GetSwapChain()->GetCurrentBackBuffer()->GetFormat(),
			YOUR_SRV_DESC_HEAP,
			// You'll need to designate a descriptor from your descriptor heap for Dear ImGui to use internally for its font texture's SRV
			YOUR_CPU_DESCRIPTOR_HANDLE_FOR_FONT_SRV,
			YOUR_GPU_DESCRIPTOR_HANDLE_FOR_FONT_SRV);
	}

	void Editor::Run()
	{
		assert(PlatformLoop != nullptr);

		m_Timer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_Timer.Tick();
			float dt = m_Timer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}
}