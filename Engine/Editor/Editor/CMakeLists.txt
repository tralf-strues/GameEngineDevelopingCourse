# get all files
file(GLOB
	PRIVATE_FILES
	private/*.cpp
	private/*.h
)
file(GLOB
	PUBLIC_FILES
	public/*.h
)

# Add executable
add_executable(Editor WIN32)

set_target_properties(Editor PROPERTIES FOLDER "Editor")

target_compile_definitions(Editor
	PRIVATE
		UNICODE)

if (WINDOWS)
	find_library(IMGUI_DEBUG NAMES imgui
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(IMGUI_RELEASE NAMES imgui 
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(IMGUI_LIBRARIES debug ${IMGUI_DEBUG} optimized ${IMGUI_RELEASE})
endif()

# Set Sources
target_sources(Editor
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(Editor 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/imgui/include
)

target_link_libraries(Editor PRIVATE
	Core
	EntitySystem
	LevelEditor
	Math
	RenderCore
	RenderEngine
	RHI
	ScriptSystem
	${IMGUI_LIBRARIES}
	
	Direct3D12RHI
	d3dcompiler.lib
	D3D12.lib
	dxgi.lib
)

target_precompile_headers(Editor
	PRIVATE
		<unordered_map>
		<vector>
		<string>
		<cassert>
		<format>
		<functional>
		<thread>
		<mutex>
		<condition_variable>
		<tuple>
		<utility>
		<cstdint>
		<limits>
		<chrono>
		<filesystem>
		<bitset>
		<queue>
		<variant>
)