#pragma once

#include <LevelEditor/export.h>

namespace GameEngine
{
	class LEVEL_EDITOR_API LevelEditor final
	{
	public:
		LevelEditor() = default;

	public:
		void Run();
		//void Update(float dt);
	};
}