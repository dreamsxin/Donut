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
	newContext.windowName = "testGbuffer";
	newContext.width = 1280;
	newContext.lenght = 720;
	newContext.major = 4;
	newContext.minor = 1;
	window->CreateRenderWindow(newContext, 1);
	window->SetFragmentShader("shaders/canvas/vertex.glsl");
	window->SetFragmentShader("shaders/canvas/lightout.glsl");
	window->SetRenderType(Donut::FrameCanvasContent::DEFFERED);
	window->Init();

	Donut::TLight* lightSource = new Donut::TLight();
	lightSource->SetPosition(TVec3(0,0,0));
	lightSource->SetColor(TVec4(0,0,1,1.0));
	window->AddLight(lightSource);

	Donut::TLight* lightSource2 = new Donut::TLight();
	lightSource2->SetPosition(TVec3(0.0,20,-60));
	lightSource2->SetColor(TVec4(1.0,0,0,1.0));
	window->AddLight(lightSource2);


	Donut::TLight* lightSource3 = new Donut::TLight();
	lightSource3->SetPosition(TVec3(0.0,-20,-50));
	lightSource3->SetColor(TVec4(0.0,1,0,1.0));
	window->AddLight(lightSource3);



	Donut::TLight* lightSource4 = new Donut::TLight();
	lightSource4->SetPosition(TVec3(0.0,30,-50));
	lightSource4->SetColor(TVec4(1,1,1,1.0));
	window->AddLight(lightSource4);

	Donut::TNode* root= window->GetRoot();
	Donut::Camera* camera = window->GetCamera();
	Donut::TDefaultInputManager* inManager = static_cast<Donut::TDefaultInputManager*>(Donut::GetInputManager());
	inManager->FCamera = camera;
	camera->DefinePerspective(45.0,1280.0/720.0,1.0,500.0);
	Donut::TDrawableObject* teapot = new Donut::TMesh(TVec3(0,0,-40),"Teapot");
	Donut::TDrawableObject* cube = new Donut::TCubeR(TVec3(0,0,0),0.5);


	Donut::TSceneNode* node = new Donut::TSceneNode();
	cube->GenerateShader();
	cube->Init();
	teapot->GenerateShader();
	teapot->Init();
	node->AddDrawable(cube);
	node->AddDrawable(teapot);
	root->AddChild(node);
	window->RegisterToDraw(cube);
	window->RegisterToDraw(teapot);
	
	for (int i = 0; i< 10; i++)
	{
		for (int j = 0; j< 10; j++)
		{
			Donut::TDrawableObject* teapot2 = new Donut::TMesh(TVec3(20*j,0,-40*i),"Teapot");
			teapot2->GenerateShader();
			teapot2->Init();
			node->AddDrawable(teapot2);
			window->RegisterToDraw(teapot2);
		}
	}
	while(window->IsRendering())
	{
		window->Draw();
		Donut::FarmEvents();
		inManager->Update(0.016);
	}
	window->UnRegisterToDraw(cube);
	window->UnRegisterToDraw(teapot);
	delete teapot;
	delete cube;

	delete window;
	return 0;

}
