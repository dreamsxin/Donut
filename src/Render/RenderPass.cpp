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

#include "RenderPass.h"

#include "Base/Common.h"
#include "MultiThread/Defines.h"
 namespace Donut
 {

 	//CLASS IMPLEMENTATION
	TRenderPass::TRenderPass()
	{
		FRenderToTexture = true;
		FFrameCanvas = new TFrameCanvas();
		FCamera = new Camera();
	}
	TRenderPass::~TRenderPass()
	{
		delete FCamera;
		delete FFrameCanvas;
		FDrawables.clear();
	}

	void TRenderPass::Draw()
	{
		PreparePass();
		if(FRenderToTexture)
		{
			
			Bind();
			foreach(drawable,FDrawables)
			{
 				//RENDER_DEBUG("Drawable");
				ShaderManager& sm = ShaderManager::Instance();
				TDrawableObject & drw = *(*drawable);
				drw.Bind();
				drw.InjectModelMatrix();
				ShaderManager::Instance().InjectMat4(drw.GetShader(),FCamera->GetViewMatrix(),"view");
				ShaderManager::Instance().InjectMat4(drw.GetShader(),FCamera->GetProjectionMatrix(),"projection");
				//ShaderManager::Instance().InjectMat4(drw.GetShader(),drw.GetModelMatrix()*FCamera->GetViewMatrix()*FCamera->GetProjectionMatrix(),"modelviewprojection");
				drw.Draw();
				drw.Unbind();
			}
			Unbind();
			
			FFrameCanvas->Draw();
		}
		else
		{
			foreach(drawable,FDrawables)
			{
 				//RENDER_DEBUG("Drawable");
				ShaderManager& sm = ShaderManager::Instance();
				TDrawableObject & drw = *(*drawable);
				drw.Bind();
				drw.InjectModelMatrix();
				ShaderManager::Instance().InjectMat4(drw.GetShader(),FCamera->GetViewMatrix(),"view");
				ShaderManager::Instance().InjectMat4(drw.GetShader(),FCamera->GetProjectionMatrix(),"projection");
				//ShaderManager::Instance().InjectMat4(drw.GetShader(),drw.GetModelMatrix()*FCamera->GetViewMatrix()*FCamera->GetProjectionMatrix(),"modelviewprojection");
				drw.Draw();
				drw.Unbind();
			}
		}
	}
	void TRenderPass::Init()
	{
 		//RENDER_DEBUG("Initing the canvas");
		
		if(FRenderToTexture)
		{
			FFrameCanvas->Init();
		}
		foreach(drawable,FDrawables)
		{
			(*drawable)->Init();
			(*drawable)->GenerateShader();
		}
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
 		//RENDER_DEBUG("Canvas created");
	}

	void TRenderPass::PreparePass()
	{

	}

	void TRenderPass::AddDrawable(TDrawableObject* parDrawable)
	{
		FDrawables.push_back(parDrawable);
 		RENDER_DEBUG("Drawable added "<< FDrawables.size());
	}

	void TRenderPass::Bind()
	{
		FFrameCanvas->Enable();
	}

	void TRenderPass::SetFragmentShader(const std::string& parFragShader)
	{
		FFrameCanvas->SetFragmentShader(parFragShader);
		FRenderToTexture = true;
	}

	void TRenderPass::SetVertexShader(const std::string& parFragShader)
	{
		FFrameCanvas->SetVertexShader(parFragShader);
		FRenderToTexture = true;
	}

	void TRenderPass::Unbind()
	{
		FFrameCanvas->Disable();
	}

	void TRenderPass::RemoveDrawable(TDrawableObject* parDrawable)
	{
 		RENDER_DEBUG("Removing drawable");
		FDrawables.remove(parDrawable);
	}

	void TRenderPass::Clear()
	{
		CRITICAL_SECTION_BEGIN();
		FDrawables.clear();
		CRITICAL_SECTION_END();
	}
	// END CLASS IMPLEMENTATION


 }
