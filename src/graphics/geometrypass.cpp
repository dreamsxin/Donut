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

// Libary includes
#include "geometrypass.h"
#include "graphics/common.h"
#include "base/Common.h"
#include "Base/Macro.h"
#include "Base/log.h"
#include "resource/resourcemanager.h"

// External includes
#include <map>

 namespace Donut
 {

 	//CLASS IMPLEMENTATION
	TGeometryPass::TGeometryPass(TCanvas* _canvas, TNode* _root)
	: m_canvas(_canvas)
	, m_root(_root)
	, m_reference()
	, m_camera(nullptr)
	{
		SetIdentity(m_reference);
	}
	TGeometryPass::~TGeometryPass()
	{
		delete m_canvas;
		delete m_root;
	}
	
	void TGeometryPass::Init()
	{
		m_canvas->Init();
	}

	void TGeometryPass::Draw(const TBufferOutput& _previousData)
	{
		// Reset the dispatcher
		m_collector.Clear();

		// Collect the requests
		m_root->Evaluate(m_collector, m_reference);

		// Fetch the requests
		auto& requests = m_collector.Requests();

		// Do all the processings on the requests
		m_dipatcher.ProcessRequests(requests, m_camera->GetViewMatrix(), m_camera->FrusumDescriptor());

		// Fetch the uniform values from the camera
		std::map<std::string, TUniformHandler> values;
		m_camera->AppendUniforms(values);

		GENERAL_INFO(requests.size());

		// Bind the canvas
		Bind();

		// Process eache render request 
		foreach_macro(request, requests)
		{
			// Process the render request
			ProcessRenderRequest(*request, values);
		}

		// Unbind the canvas
		Unbind();
	}

	void TGeometryPass::Bind()
	{
		m_canvas->Enable();
	}
	
	const TBufferOutput* TGeometryPass::GetOutput()
	{
		return &(m_canvas->Result());
	}

	void TGeometryPass::Unbind()
	{
		m_canvas->Disable();
	}

	// END CLASS IMPLEMENTATION
 }
