/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 **/

// ----------------------------------------
//  Includes
// ----------------------------------------
// Donut includes
#include "Engine.h"
#include "engine/common.h"

#include <base/common.h>

// ----------------------------------------
//  Implementation
// ----------------------------------------

namespace Donut
{
 	TEngine::TEngine()
 	: FRenderingRunning(false)
 	, m_scene(nullptr)
 	{
 		FRenderer = new Donut::TRenderer();
 		ENGINE_INFO("Engine created");
 	}
 	
 	TEngine::~TEngine()
 	{	
 		delete FRenderer;
 	}

 	void TEngine::LaunchRendering(const TGraphicsSettings& parContext)
 	{
 		ASSERT_MSG_NO_RELEASE(!FRenderingRunning, "Rendering already launched, it is just paused.");
 		TPipeline* pipeline = GenerateGraphicPipeline(m_scene, parContext.width, parContext.lenght);
 		FRenderer->CreateRenderWindow(parContext);
 		FRenderer->SetPipeline(pipeline);
 		InitScene();
#if __posix__
 		FThreadData = CREATE_THREAD(FTRenderingThread,CreateRenderingThread,FRenderer);
#elif WIN32
 		CREATE_THREAD(FTRenderingThread,CreateRenderingThread,FRenderer);
#endif
 		ENGINE_INFO("Redering thread created");
 		FRenderingRunning = true;
 		
 	}

 	void TEngine::StopRendering()
 	{
 		FRenderer->SetRendering(false);
 		THREAD_JOIN(FTRenderingThread, FThreadData, NULL);
 		FRenderer->DestroyRenderWindow();
 		ENGINE_INFO("Rendering stoped");
 		FRenderingRunning = false;
 	}

 	void TEngine::PauseRendering()
 	{
 		FRenderer->SetRendering(false);
 		THREAD_JOIN(FTRenderingThread, FThreadData,NULL);
 		FRenderer->HideRenderWindow();
 		ENGINE_INFO("Rendering paused");

 	}

 	void TEngine::ResumeRendering()
 	{
 		ASSERT_MSG_NO_RELEASE(FRenderingRunning, "Rendering is not launched.")
 		FRenderer->ShowRenderWindow();
 		FRenderer->SetRendering(true);
 		FThreadData = CREATE_THREAD(FTRenderingThread, CreateRenderingThread, FRenderer);
 		ENGINE_INFO("Rendering resumed");
 	}

 	bool TEngine::IsRendering()
 	{
 		return FRenderer->IsRendering();
 	}

	void TEngine::Update(float dt)
	{
		FarmEvents();
	}
}