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
 #include "scenenode.h"

 #include "Base/Common.h"
 #include "Base/Macro.h"
 #include "Render/Collector.h"


namespace Donut
{
	TSceneNode::TSceneNode()
	{

	}

	TSceneNode::~TSceneNode()
	{
		
	}

	// Parse this node and its subnodes
	void TSceneNode::Evaluate(TCollector& _requestCollector, const Matrix4& _parentTransform)
	{
		// For each drawable attached to this node
		const Matrix4& currentTransform = _parentTransform * m_transform;

		// Evaluate the drawables
		foreach_macro(drawable, m_drawables)
		{
            (*drawable)->Evaluate(_requestCollector, currentTransform);
		}

		foreach_macro(son, m_sons)
		{
            (*son)->Evaluate(_requestCollector, currentTransform);
		}
	}

	void TSceneNode::AppendDrawable(TDrawable* _drawable)
	{
		m_drawables.push_back(_drawable);
	}
}