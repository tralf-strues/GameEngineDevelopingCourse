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

# Add library
include(GenerateExportHeader)
add_library(LevelEditor SHARED)
generate_export_header(LevelEditor EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME LEVEL_EDITOR_API)

set_target_properties(LevelEditor PROPERTIES FOLDER "Editor")

target_compile_definitions(LevelEditor
	PRIVATE
		LevelEditor_EXPORTS
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
target_sources(LevelEditor
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(LevelEditor 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/imgui/include
)

target_link_libraries(LevelEditor PRIVATE
	Core
	EntitySystem
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

target_precompile_headers(LevelEditor
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