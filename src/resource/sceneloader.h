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
#include "core/node.h"
#include "base/singleton.h"

// STL includes
#include <string.h>

namespace Donut
{

	namespace TSceneFile
	{
		enum Type
		{
			JSon,
			Xml,
			UNKNOWN
		};
	}
	
	class TSceneLoader : public Singleton<TSceneLoader>
	{
	public:
		TSceneLoader();
		~TSceneLoader();

		// This function reads the scene descriptor file
		// and loads into memory the scene structures
		TNode* LoadScene(const std::string& _sceneFileName);
	};
}