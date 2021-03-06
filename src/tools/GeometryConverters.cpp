// Library include
#include "GeometryConverters.h"
#include "base/stringhelper.h"
#include "butter/vector3.h"
#include "resource/egg.h"

// External includes
#include <vector>

namespace Donut
{

	struct TShape
	{
		std::vector<std::string> info;
	};

	TEgg* CreateEggFromWavefront(const STRING_TYPE& _wavefrontFIle)
	{
		GENERAL_INFO("Trying to load Wavefront: " << _wavefrontFIle);
		// Liste des vertices
		std::vector<Vector3> listePoints;
		// Liste des infos par point
		std::vector<Vector3> normales;
		std::vector<Vector2> uvList;

		// Gestion des infos
		std::vector<TShape> shapes;

		std::fstream in;
		in.open(_wavefrontFIle.c_str(), std::fstream::in);
		if (!in)
		{
			ASSERT_FAIL_MSG("Cannot find _model obj: " << _wavefrontFIle);
			return NULL;
		}
		std::string line;
		while (getline(in, line))
		{
			if (line.substr(0, 2) == "o ")
			{
				while (getline(in, line) && line.substr(0, 2) != "o ")
				{
					if (line.substr(0, 2) == "v ")
					{
						// INPUT_DEBUG("Nouveau vertice.");
						std::stringstream s(line.substr(2));
						Vector3 v;
						s >> v.x;
						s >> v.y;
						s >> v.z;
						listePoints.push_back(v);
					}
					else if (line.substr(0, 2) == "f ")
					{
						TShape newShape;
						while (line.substr(0, 2) == "f ")
						{
							// INPUT_DEBUG("new primitive "<< line);
							newShape.info.push_back(line.substr(2));
							if (!getline(in, line))
								break;
						}

						shapes.push_back(newShape);
					}
					else if (line[0] == 'v' && line[1] == 't')
					{
						// INPUT_DEBUG("UV mapping.");
						std::istringstream s(line.substr(2));
						float u, v;
						s >> u;
						s >> v;
						Vector2 map;
						map.x = u;
						map.y = v;
						uvList.push_back(map);
					}
					else if (line[0] == 'v' && line[1] == 'n')
					{
						//INPUT_DEBUG("Normal.");
						std::istringstream s(line.substr(2));
						Vector3 normal;
						s >> normal.x;
						s >> normal.y;
						s >> normal.z;
						normales.push_back(normal);
					}
				}
			}
			else if (line[0] == '#')
			{
				// Commentaire
			}
		}
		TEgg * newModel = new TEgg();
		unsigned int* indices;

		foreach_macro(shape, shapes)
		{
			TShape & currentShape = *shape;
			int nbShape = (int)currentShape.info.size();
			ASSERT_MSG_NO_RELEASE(currentShape.info.size()>0, "Dans le fichier de modele, une ligne commencant par f error");
			std::vector<std::string> sample;
			split(currentShape.info[0], ' ', sample);
			int dimShape = (int)sample.size();
			ASSERT_MSG_NO_RELEASE((dimShape == 3 || dimShape == 4), "Shape de dimension autre que 3 ou 4");
			std::vector<std::string> sample2;
			split(sample[2], '/', sample2);
			int nbInfo = (int)sample2.size();

			// We only get the pos buffer from the obj
			if (nbInfo == 1)
			{
				newModel->vertsNormalsUVs = new float[8 * dimShape*nbShape];
				float* vertexArray = newModel->vertsNormalsUVs;
				float* normalArray = vertexArray + 3 * dimShape*nbShape;
				float* texCoord = normalArray + 3 * dimShape*nbShape;

				int verticeCounter = 0;
				foreach_macro(prim, currentShape.info)
				{
					std::vector<std::string> vertices;
					split(*prim, ' ', vertices);
					int primSize = (int)vertices.size();
					if (vertices.size() == 3)
					{
						Vector3 points[3];
						points[0] = listePoints[convertFromString<int>(vertices[0]) - 1];
						points[1] = listePoints[convertFromString<int>(vertices[1]) - 1];
						points[2] = listePoints[convertFromString<int>(vertices[2]) - 1];

						const Vector3& v0 = points[1] - points[0];
						const Vector3& v1 = points[2] - points[0];
						const Vector3& normal = normalize(crossProd(v0, v1));

						for (int i = 0; i < 3; ++i)
						{
							vertexArray[(verticeCounter + i) * 3] = (float)points[i].x;
							vertexArray[(verticeCounter + i) * 3 + 1] = (float)points[i].y;
							vertexArray[(verticeCounter + i) * 3 + 2] = (float)points[i].z;

							normalArray[(verticeCounter + i) * 3] = (float)normal.x;
							normalArray[(verticeCounter + i) * 3 + 1] = (float)normal.y;
							normalArray[(verticeCounter + i) * 3 + 2] = (float)normal.z;
						}
						verticeCounter += primSize;
					}
					else // 4
					{
						/*
						const Vector3& point0 = listePoints[convertFromString<int>(vertices[0]-1];
						const Vector3& point1 = listePoints[convertFromString<int>(vertices[1]-1];
						const Vector3& point2 = listePoints[convertFromString<int>(vertices[2]-1];
						const Vector3& point3 = listePoints[convertFromString<int>(vertices[3]-1];
						*/
						ASSERT_NOT_IMPLEMENTED();
					}
				}
				// Creating the IBO
				newModel->nbVertices = verticeCounter;
				newModel->faces = new unsigned int[verticeCounter];
				for (int i = 0; i < verticeCounter; i++)
				{
					newModel->faces[i] = i;
				}
				newModel->nbFaces = verticeCounter / 3;
			}
			else if (nbInfo == 2)
			{
				newModel->vertsNormalsUVs = new float[8 * dimShape*nbShape];
				float* vertexArray = newModel->vertsNormalsUVs;
				float* normalArray = vertexArray + 3 * dimShape*nbShape;
				float* texCoordArray = normalArray + 3 * dimShape*nbShape;

				int verticeCounter = 0;
				foreach_macro(prim, currentShape.info)
				{
					std::vector<std::string> vertices;
					split(*prim, ' ', vertices);
					int primSize = (int)vertices.size();
					if (primSize == 3)
					{
						Vector3 points[3];
						std::vector<std::string> dataVert0, dataVert1, dataVert2;
						split(vertices[0], '/', dataVert0);
						split(vertices[1], '/', dataVert1);
						split(vertices[2], '/', dataVert2);
						points[0] = listePoints[convertFromString<int>(dataVert0[0]) - 1];
						points[1] = listePoints[convertFromString<int>(dataVert1[0]) - 1];
						points[2] = listePoints[convertFromString<int>(dataVert2[0]) - 1];

						Vector2 texCoord[3];
						texCoord[0] = uvList[convertFromString<int>(dataVert0[1]) - 1];
						texCoord[1] = uvList[convertFromString<int>(dataVert1[1]) - 1];
						texCoord[2] = uvList[convertFromString<int>(dataVert2[1]) - 1];

						const Vector3& v0 = points[1] - points[0];
						const Vector3& v1 = points[2] - points[0];
						const Vector3& normal = normalize(crossProd(v0, v1));

						for (int i = 0; i < 3; ++i)
						{
							vertexArray[(verticeCounter + i) * 3] = (float)points[i].x;
							vertexArray[(verticeCounter + i) * 3 + 1] = (float)points[i].y;
							vertexArray[(verticeCounter + i) * 3 + 2] = (float)points[i].z;

							normalArray[(verticeCounter + i) * 3] = (float)normal.x;
							normalArray[(verticeCounter + i) * 3 + 1] = (float)normal.y;
							normalArray[(verticeCounter + i) * 3 + 2] = (float)normal.z;

							texCoordArray[(verticeCounter + i) * 2] = (float)texCoord[i].x;
							texCoordArray[(verticeCounter + i) * 2 + 1] = (float)texCoord[i].y;
						}
						verticeCounter += primSize;
					}
					else // 4
					{
						ASSERT_NOT_IMPLEMENTED();
					}
				}
				newModel->nbVertices = verticeCounter;
				newModel->faces = new unsigned int[verticeCounter];
				for (int i = 0; i < verticeCounter; i++)
				{
					newModel->faces[i] = i;
				}
				newModel->nbFaces = verticeCounter / 3;

			}
			else if (nbInfo == 3)
			{
				newModel->vertsNormalsUVs = new float[8 * dimShape*nbShape];
				float* vertexArray = newModel->vertsNormalsUVs;
				float* normalArray = vertexArray + 3 * dimShape*nbShape;
				float* texCoordArray = normalArray + 3 * dimShape*nbShape;

				int verticeCounter = 0;
				foreach_macro(prim, currentShape.info)
				{
					std::vector<std::string> vertices;
					split(*prim, ' ', vertices);
					int primSize = (int)vertices.size();
					if (primSize == 3)
					{
						Vector3 points[3];
						std::vector<std::string> dataVert0, dataVert1, dataVert2;
						split(vertices[0], '/', dataVert0);
						split(vertices[1], '/', dataVert1);
						split(vertices[2], '/', dataVert2);
						points[0] = listePoints[convertFromString<int>(dataVert0[0]) - 1];
						points[1] = listePoints[convertFromString<int>(dataVert1[0]) - 1];
						points[2] = listePoints[convertFromString<int>(dataVert2[0]) - 1];

						Vector2 texCoord[3];
						texCoord[0] = uvList[convertFromString<int>(dataVert0[1]) - 1];
						texCoord[1] = uvList[convertFromString<int>(dataVert1[1]) - 1];
						texCoord[2] = uvList[convertFromString<int>(dataVert2[1]) - 1];

						Vector3 normals[3];
						normals[0] = normales[convertFromString<int>(dataVert0[2]) - 1];
						normals[1] = normales[convertFromString<int>(dataVert1[2]) - 1];
						normals[2] = normales[convertFromString<int>(dataVert2[2]) - 1];

						for (int i = 0; i < 3; ++i)
						{
							vertexArray[(verticeCounter + i) * 3] = (float)points[i].x;
							vertexArray[(verticeCounter + i) * 3 + 1] = (float)points[i].y;
							vertexArray[(verticeCounter + i) * 3 + 2] = (float)points[i].z;

							normalArray[(verticeCounter + i) * 3] = (float)normals[i].x;
							normalArray[(verticeCounter + i) * 3 + 1] = (float)normals[i].y;
							normalArray[(verticeCounter + i) * 3 + 2] = (float)normals[i].z;

							texCoordArray[(verticeCounter + i) * 2] = (float)texCoord[i].x;
							texCoordArray[(verticeCounter + i) * 2 + 1] = (float)texCoord[i].y;
						}
						verticeCounter += primSize;
					}
					else // 4
					{
						ASSERT_NOT_IMPLEMENTED();
					}
				}
				newModel->nbVertices = verticeCounter;
				newModel->faces = new unsigned int[verticeCounter];
				for (int i = 0; i < verticeCounter; i++)
				{
					newModel->faces[i] = i;
				}
				newModel->nbFaces = verticeCounter / 3;
			}
		}
		return newModel;
	}

	void ExportEggFile(TEgg* eggInstance, const STRING_TYPE& _outputEggFile)
	{
		std::fstream out;
		out.open(_outputEggFile.c_str(), std::fstream::out | std::ios::binary);
		out << (*eggInstance);
		out.close();
	}

	// This file takes a wavefront (.obj) and converts it to an egg file
	void ConvertWavefrontToEgg(const STRING_TYPE& _wavefront, const STRING_TYPE& _outputEggFile)
	{
		TEgg* eggInstance = CreateEggFromWavefront(_wavefront);
		ExportEggFile(eggInstance, _outputEggFile);
	}
}