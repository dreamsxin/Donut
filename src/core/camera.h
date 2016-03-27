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

#ifndef CAMERA_CORE_DONUT
#define CAMERA_CORE_DONUT

// Library includes
#include "butter/types.h"

#include "graphics/uniformhandler.h"
#include "multithread/usualthreadsafeobjects.h"

// STL includes
#include <vector>
#include <map>

namespace Donut
{
	class Camera
	{
		public:
			Camera();
			~Camera();

			// Utility functions
			void Yaw(double parAngle);
			void Pitch(double parAngle);
			void Roll(double parAngle);
			void Translate(const Vector3& parDir);

			// Accessors
			const Matrix4& GetViewMatrix() const { return FViewMatrix;}
			const Matrix4& GetProjectionMatrix() const{ return FProjection;}
			const Matrix4& GetProjectionViewMatrix() const{ return FProjection;}
			// Setting the projection matrix
			void DefinePerspective(double parFovy, double parAspect, double parNear, double parFar);
			
			// Modification flags
			void ChangeNoticed();
			bool HasChanged() {return FHasChanged.GetValue();}
			double GetFCoeff() {return m_fcoeff;}
			void AppendUniforms(std::map<std::string, TUniformHandler>& _uniforms);

		protected:
			Matrix4 FViewMatrix;
			Matrix3 FViewMatrix_inverse;
			Matrix4 FProjection;
			Matrix4 FProjectionView;
			TThreadSafeBolean FHasChanged;
			double m_near;
			double m_far;
			double m_fcoeff;
			double m_focus;
	};
}


#endif //CAMERA_CORE_DONUT