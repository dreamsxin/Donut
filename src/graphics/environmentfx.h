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


#ifndef ENVIRONMENT_FX_GRAPHICS_DONUT
#define ENVIRONMENT_FX_GRAPHICS_DONUT

// Library includes
#include "graphics/visualeffect.h"
#include "graphics/sphericalharmonics.h"

namespace Donut
{
	class TEnvironmentFX : public TVFX
	{
	public:
		// Creation/Destruction
		TEnvironmentFX();
		TEnvironmentFX(const TShader& _shader);
		virtual ~TEnvironmentFX();
		// Init
		virtual void Init();
		// Drawing it
		void SetSH(TSphericalHarmonics* _SH) {m_SH = _SH;}
		void Draw(std::map<std::string, TUniformHandler>& _values, const TBufferOutput& _previousData);

	protected:
		TSphericalHarmonics* m_SH;
	};
}

#endif // ENVIRONMENT_FX_GRAPHICS_DONUT
