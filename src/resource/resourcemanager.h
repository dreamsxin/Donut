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



#ifndef RESOURCE_MANAGER_DONUT
#define RESOURCE_MANAGER_DONUT

// Library includes
#include "graphics/shaderManager.h"
#include "graphics/geometry.h"
#include "graphics/material.h"

#include "base/singleton.h"
#include "texture.h"
#include "ggxbrdf.h"
#include "geometrycontainer.h"

// STL includes
#include <string>
#include <vector>
#include <map>

 namespace Donut 
 {
 	class ResourceManager : public Singleton<ResourceManager>
 	{
 	public:
 		ResourceManager();
 		~ResourceManager();

 		void Init(const std::string& _assertFolder);
 	// Asset folder setting
 		inline const std::string& RootAssetsFolder() const
 		{
 			return m_rootAssetFolder;
 		}
 		inline std::string RelativePath(const std::string& _fileName)
 		{
 			return (m_rootAssetFolder + _fileName);
 		}

 	// Fetching methods
		// Returns a pointer to a given brdf file (using its filepath)
 		// If it has not been loaded yet, it loads the brdf data into the GPU RAM
 		// Else it returns a pointer to what has been loaded
 		TGGXBRDF* FetchBRDF(const std::string& _brdfFileName);
 		// Returns a pointer to a given texture (using its filepath)
 		// If it has not been loaded yet, it loads the texture into the CPU RAM and the GPU RAM
 		// Else it returns a pointer to what has been loaded
		TTexture* FetchTexture(const std::string&  parTextureName);
		// Returns a pointer to a given wavefrontFile (using its filepath)
 		// If it has not been loaded yet, it loads the geometry into the GPU RAM
 		// Else it returns a pointer to what has been loaded
		TGeometry* FetchGeometry(const TShader& parShader, const std::string&  parObjName);
		// Returns a pointer to a given skybox (using its filepath)
 		// If it has not been loaded yet, it loads the texture into the CPU RAM and the GPU RAM
 		// Else it returns a pointer to what has been loaded
		TSkyboxTexture* FetchSkybox(const std::string& _skyboxFolder, const std::string& _extension);


	// Should be taken a look at
		// Creates a geometry and registers it with a given name
		TGeometry* CreateGeometry(const std::string& _name, const TShader& parShader, float* _dataArray, int _numVert, unsigned* _indexArray, int num_faces);
		// Reads a given file and serializes its content as a geometry container
		TGeometryContainer* ReadWavefront(const std::string&  parObjName);
		// Binds into GPU memory a given material
		void BindMaterial(const TShader& _shader, const TMaterial& _material); // <<<< should probably be moved

		// Still to refactor
		std::vector<int> LoadObjToTexture(const std::string&  parFileName, std::vector<TTexture*>& parTexturetable);

	protected:
		// Asset folder path
		std::string m_rootAssetFolder;

		// Data records
		std::map<std::string, TGGXBRDF*> m_brdfs;
		std::map<std::string, TTexture*> FTextures;
		std::map<std::string, TGeometry*> FGeometries;
		std::map<std::string, TSkyboxTexture*> FSkyboxTextures;
	};
}

#endif // RESOURCE_MANAGER_DONUT
