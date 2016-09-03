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
#include "sugarinstance.h"

#include "graphics/mesh.h"

#include "Base/Common.h"
#include "Base/Macro.h"


namespace Donut
{
	TSugarInstance::TSugarInstance()
	{

	}

	TSugarInstance::~TSugarInstance()
	{

	}

	void TSugarInstance::Draw(std::map<std::string, TUniformHandler>& _values, const TBufferOutput& _previousData)
	{
		foreach_macro(mesh, m_meshes)
		{
			(*mesh)->Draw(_values, _previousData);
		}
	}
	void TSugarInstance::AddMesh(TMesh* _mesh)
	{
		m_meshes.push_back(_mesh);
	}

	TMesh* TSugarInstance::GetMesh(int _meshIndex)
	{
		ASSERT_NO_RELEASE(_meshIndex < m_meshes.size());
		return m_meshes[_meshIndex];
	}
}