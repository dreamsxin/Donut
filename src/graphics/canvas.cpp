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

// Library includes
#include "canvas.h"
#include "graphics/shaderdata.h"
#include "graphics/shaderdata.h"

// STL includes

namespace Donut
{
	// Init and delete
	TCanvas::TCanvas(int _width, int _height)
	: m_width(_width)
	, m_height(_height)
	{
	}

	TCanvas::~TCanvas()
	{

	}

	const TBufferOutput& TCanvas::Result()
	{
		return m_output;
	}
}