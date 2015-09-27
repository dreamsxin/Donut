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

#include <iostream>
#include <Render/Renderer.h>
#include <Input/InputHelper.h>
#include <Input/DefaultInputManager.h>
#include <Input/InputManager.h>
#include <resource/sugarloader.h>
#include <Render/Representations/3D/Mesh.h>
#include <Render/Representations/3D/CubeR.h>
#include <Render/Defines.h>
#include <Render/SceneNode.h>
#include <unistd.h>


int main()
{
	Donut::TSugarLoader::Instance().Init("data");	
	// Creating the rendering window
	Donut::TRenderer * window = new Donut::TRenderer();

	// Context info
	Donut::TContextDetail newContext;
	newContext.windowName = "testSSAO";
	window->CreateRenderWindow(newContext, 1);
	window->SetRenderType(Donut::FrameCanvasContent::GBUFFER);
	window->SetVertexShader("shaders/canvas/ssaoV.glsl");
	window->SetFragmentShader("shaders/canvas/ssaoF.glsl");
	window->Init();

	// Getting the camera
	Donut::TRenderPass* pass= window->GetPasses()[0];
	Donut::TNode* root= pass->GetRoot();
	Donut::Camera* camera = pass->GetCamera();
	Donut::TDefaultInputManager* inManager = static_cast<Donut::TDefaultInputManager*>(Donut::GetInputManager());
	inManager->FCamera = camera;
	camera->DefinePerspective(45.0,1280.0/720.0,1.0,500.0);
	Donut::TDrawableObject* rabbit = new Donut::TMesh(TVec3(0,0,-40),"Lego");

	Donut::TSceneNode* node = new Donut::TSceneNode();
	rabbit->GenerateShader();
	rabbit->Init();
	node->AddDrawable(rabbit);
	root->AddChild(node);
	window->RegisterToDraw(rabbit);
	
	while(window->IsRendering())
	{
		window->Draw();
		Donut::FarmEvents();
		inManager->Update(0.016);
	}
	window->UnRegisterToDraw(rabbit);
	delete rabbit;
	delete window;
	return 0;

}
