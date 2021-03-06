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

#ifndef GEOMETRY_GRAPHICS_DONUT
#define GEOMETRY_GRAPHICS_DONUT

#include "graphics/common.h"
#include "core/box3.h"

namespace Donut
{
	struct TGeometry
	{
		// Runtime rendering data
	    GLuint vertexArray;
	    GLuint vertexBuffer;
	    GLuint indexBuffer;
	    int nbVertices;

		// Useful data
		TBox3 os_bb;

		// Draw the geometry
	    void Draw(bool _isTess) const;
	};	
}


#endif // GEOMETRY_GRAPHICS_DONUT
 