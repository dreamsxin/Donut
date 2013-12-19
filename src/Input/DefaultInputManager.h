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
 #ifndef DEFAULT_INPUT_MANAGER_DONUT 
 #define DEFAULT_INPUT_MANAGER_DONUT



 #include "Defines.h"
 #include "InputManager.h"
 #include "base/singleton.h"
 #include <Render/Camera.h>
 #include <map>

 namespace Donut
 {
 	class DefaultInputManager : public InputManager
 	{

 	public:
 		DefaultInputManager();
 		~DefaultInputManager();

 		void KeyPressed(TKeyCode::Type parKey);
 		void KeyReleased(TKeyCode::Type parKey);

 		void MouseMoved(float parX, float parY);
 		void MousePressed(TMouseCode::Type parButton);
 		void MouseReleased(TMouseCode::Type parButton);
 		void Update(float parTime);

 	public:
 		Camera* FCamera;
 	protected:
 		bool FInitDone;
 		float oldX;
 		float oldY;
 		std::map<TKeyCode::Type, bool> FKeys;
 	};
 }

 #endif //DEFAULT_INPUT_MANAGER_DONUT