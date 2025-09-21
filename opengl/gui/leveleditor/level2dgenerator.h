#pragma once

#include <string>
#include <vml4.0/opengl/gui/leveleditor/gridtriangle.h>
#include <vml4.0/opengl/gui/leveleditor/trianglelayerrender.h>
#include <vml4.0/opengl/gui/leveleditor/borderlayer.h>
#include <vml4.0/opengl/gui/leveleditor/trianglelayer.h>
#include <vml4.0/opengl/gui/leveleditor/gridblock.h>

//////////////////////////////////////////////////////////////////
// vertex class holding vertex attributes

namespace vml
{
	namespace geo3d
	{

		class Vertex
		{
			public:

				glm::vec4 Pos;		// homogeneous point coordinates
				glm::vec3 Normal;	// normal coordinates
				glm::vec2 UV;		// texture coordinates

				// ------------------------------------------
				// ctor / dtor

				Vertex(const glm::vec3& pos,
					   const glm::vec3& normal,
					   const glm::vec2& uv) : Pos(glm::vec4(pos.x, pos.y, pos.z, 1)), Normal(normal), UV(uv)
				{
				}

				Vertex() :Pos(glm::vec4(0, 0, 0, 0)), Normal(glm::vec3(0, 0, 0)), UV(glm::vec2(0, 0))
				{
				}

				~Vertex()
				{
				}
			};

	}
}

////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box class

namespace vml
{
	namespace geo3d
	{

		class AABBox
		{

			private:

				glm::vec3 Min;
				glm::vec3 Max;
				glm::vec3 Center;
				glm::vec3 Extents;
				glm::vec3 HalfExtents;

			public:

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetMin()		  const { return Min; }
				const glm::vec3& GetMax()		  const { return Max; }
				const glm::vec3& GetCenter()	  const { return Center; }
				const glm::vec3& GetHalfExtents() const { return HalfExtents; }
				const glm::vec3& GetExtents()	  const { return Extents; }

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min = bmin;
					Max = bmax;
					Extents = Max - Min;
					Center = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				AABBox()
				{
					Min = glm::vec3(0, 0, 0);
					Max = glm::vec3(0, 0, 0);
					Extents = glm::vec3(0, 0, 0);
					Center = glm::vec3(0, 0, 0);
					HalfExtents = glm::vec3(0, 0, 0);
				}

				AABBox(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min = bmin;
					Max = bmax;
					Extents = Max - Min;
					Center = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				~AABBox()
				{
				}

		};

	}	// end of geo3d namespace

}	// end of vml namespace

//////////////////////////////////////////////////////////////////////////////
// TO DO :

namespace fa2040
{
	namespace tools
	{

		class Level2dMeshBuilder
		{
			private:

				// ---------------------------------------------------------------

				float								Radius;				// Mesh radius
				vml::geo3d::AABBox					BoundingBox;		// Mesh Bounding Box
				std::vector<vml::geo3d::Vertex>		VertexArray;		// array of vertices
				std::vector<glm::ivec3>				SurfaceArray;		// array of surfaces
				std::vector<int>					SurfaceIndices;		// vbo surface indices 	
				std::string							ResourceFileName;	// resource file name

			private:

				static float Max(const float a, const float b)
				{
					if (a > b) return a; return b;
				}

				static bool CloseEnough(const float f1, const float f2, const double eps = vml::math::EPSILON)
				{
					if (fabs(f1 - f2) < eps)
						return true;
					return fabs(f1 - f2) < eps * Max(fabs(f1), fabs(f2));
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// tranform object
				// this function transforms directly the mesh definition
				// often is usefull to modify the mesh' s geometry 
				// for example after importing , this is not
				// a rednering pipeline tranformation, this is a 
				// direct modification of mesh's geometry

				void Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
				{
					float sx = sin(rot.x * vml::math::DEGTORAD);
					float sy = sin(rot.y * vml::math::DEGTORAD);
					float sz = sin(rot.z * vml::math::DEGTORAD);
					float cx = cos(rot.x * vml::math::DEGTORAD);
					float cy = cos(rot.y * vml::math::DEGTORAD);
					float cz = cos(rot.z * vml::math::DEGTORAD);

					// get parent matrix pointer

					float M[16] = { 0 };

					// compute model matrix

					M[0] = (cy * cz) * scale.x;
					M[1] = (cy * sz) * scale.x;
					M[2] = (-sy) * scale.x;
					M[3] = 0;
					M[4] = (sx * sy * cz - cx * sz) * scale.y;
					M[5] = (sx * sy * sz + cx * cz) * scale.y;
					M[6] = (sx * cy) * scale.y;
					M[7] = 0;
					M[8] = (cx * sy * cz + sx * sz) * scale.z;
					M[9] = (cx * sy * sz - sx * cz) * scale.z;
					M[10] = (cx * cy) * scale.z;
					M[11] = 0;
					M[12] = pos.x;
					M[13] = pos.y;
					M[14] = pos.z;
					M[15] = 1;

					std::vector<vml::geo3d::Vertex> destarray=VertexArray;

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						destarray[i]=vml::geo3d::Vertex(glm::vec4(VertexArray[i].Pos.x * M[0] + VertexArray[i].Pos.y * M[4] + VertexArray[i].Pos.z * M[8] + M[12],
																  VertexArray[i].Pos.x * M[1] + VertexArray[i].Pos.y * M[5] + VertexArray[i].Pos.z * M[ 9] + M[13],
																  VertexArray[i].Pos.x * M[2] + VertexArray[i].Pos.y * M[6] + VertexArray[i].Pos.z * M[10] + M[14],
																  1.0f),
														glm::vec3(VertexArray[i].Normal.x * M[0] + VertexArray[i].Normal.y * M[4] + VertexArray[i].Normal.z * M[ 8] + M[12],
																  VertexArray[i].Normal.x * M[1] + VertexArray[i].Normal.y * M[5] + VertexArray[i].Normal.z * M[ 9] + M[13],
																  VertexArray[i].Normal.x * M[2] + VertexArray[i].Normal.y * M[6] + VertexArray[i].Normal.z * M[10] + M[14]),
														glm::vec2(VertexArray[i].UV.x, VertexArray[i].UV.y));
					}

					VertexArray=destarray;
				}

				// ---------------------------------------------------------------------
				// Compute mesh metrics, bounding box, boung spheres and
				// center model at local origin

				void ComputeVertexArrayRadius()
				{
					std::cout << "Computing radius for " << ResourceFileName << std::endl;

					// compute mesh bounding sphere

					Radius = -FLT_MAX;

					glm::vec3 center(0, 0, 0);

					for (size_t i = 0; i < VertexArray.size(); i++)
						center += glm::vec3(VertexArray[i].Pos.x, VertexArray[i].Pos.y, VertexArray[i].Pos.z);

					float n = (float)VertexArray.size();

					center.x /= n;
					center.y /= n;
					center.z /= n;

					for (size_t i = 0; i < VertexArray.size(); i++)
					{
						float dx = VertexArray[i].Pos.x - center.x;
						float dy = VertexArray[i].Pos.y - center.y;
						float dz = VertexArray[i].Pos.z - center.z;

						float d = dx * dx + dy * dy + dz * dz;

						if (d >= Radius)
							Radius = d;
					}

					Radius = sqrtf(Radius);

				}

				// ---------------------------------------------------------------------
				// Compute surface normals

				void ComputeSurfaceNormals()
				{
					std::cout << "Computing per face normals for " << ResourceFileName << std::endl;

					// zero out the entire array

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						VertexArray[i].Normal[0] = 0.0f;
						VertexArray[i].Normal[1] = 0.0f;
						VertexArray[i].Normal[2] = 0.0f;
					}

					// compute mesh normals

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{

						int p1 = SurfaceArray[i].x;
						int p2 = SurfaceArray[i].y;
						int p3 = SurfaceArray[i].z;

						glm::vec3 v0(VertexArray[p1].Pos.x, VertexArray[p1].Pos.y, VertexArray[p1].Pos.z);
						glm::vec3 v1(VertexArray[p2].Pos.x, VertexArray[p2].Pos.y, VertexArray[p2].Pos.z);
						glm::vec3 v2(VertexArray[p3].Pos.x, VertexArray[p3].Pos.y, VertexArray[p3].Pos.z);

						glm::vec3 n = glm::cross(v1 - v0, v2 - v0);

						VertexArray[p1].Normal[0] += n.x;
						VertexArray[p1].Normal[1] += n.y;
						VertexArray[p1].Normal[2] += n.z;

						VertexArray[p2].Normal[0] += n.x;
						VertexArray[p2].Normal[1] += n.y;
						VertexArray[p2].Normal[2] += n.z;

						VertexArray[p3].Normal[0] += n.x;
						VertexArray[p3].Normal[1] += n.y;
						VertexArray[p3].Normal[2] += n.z;

					}

					// normalize vertex normals

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{

						float d = sqrtf(VertexArray[i].Normal[0] * VertexArray[i].Normal[0] +
										VertexArray[i].Normal[1] * VertexArray[i].Normal[1] +
										VertexArray[i].Normal[2] * VertexArray[i].Normal[2]);

						if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
						else d = 1.0f / d;

						VertexArray[i].Normal[0] *= d;
						VertexArray[i].Normal[1] *= d;
						VertexArray[i].Normal[2] *= d;
					}

				}

				// ---------------------------------------------------------------
				// compute surface indices

				void ComputeSurfaceIndices()
				{
					std::cout << "Computing surface indices for " << ResourceFileName << std::endl;

					SurfaceIndices.resize(SurfaceArray.size() * 3);

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{
						SurfaceIndices[i * 3] = SurfaceArray[i].x;
						SurfaceIndices[i * 3 + 1] = SurfaceArray[i].y;
						SurfaceIndices[i * 3 + 2] = SurfaceArray[i].z;
					}

				}

				// ---------------------------------------------------------------
				// The bounding box is computed given a vertexarray

				void ComputeVertexArrayBoundingBox()
				{
					std::cout << "Computing bounding box for " << ResourceFileName << std::endl;

					glm::vec3 boundingboxmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
					glm::vec3 boundingboxmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < VertexArray.size(); i++)
					{
						if (VertexArray[i].Pos.x <= boundingboxmin.x) boundingboxmin.x = VertexArray[i].Pos.x;
						if (VertexArray[i].Pos.y <= boundingboxmin.y) boundingboxmin.y = VertexArray[i].Pos.y;
						if (VertexArray[i].Pos.z <= boundingboxmin.z) boundingboxmin.z = VertexArray[i].Pos.z;
						if (VertexArray[i].Pos.x >= boundingboxmax.x) boundingboxmax.x = VertexArray[i].Pos.x;
						if (VertexArray[i].Pos.y >= boundingboxmax.y) boundingboxmax.y = VertexArray[i].Pos.y;
						if (VertexArray[i].Pos.z >= boundingboxmax.z) boundingboxmax.z = VertexArray[i].Pos.z;
					}

					BoundingBox=vml::geo3d::AABBox(boundingboxmin, boundingboxmax);
				}

			public:
				
				// ---------------------------------------------------------------
				// Memory clearing :
				// Resets all data to initial values (0) and 
				// clears vertex, surface and surface indices arrays
				// releases also vao(s).

				void ReleaseAll()
				{
					ResourceFileName = "";
					Radius			 = 0;
					BoundingBox		 = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					VertexArray.clear();
					SurfaceArray.clear();
					SurfaceIndices.clear();
				}

				// ---------------------------------------------------------------
				// closes mesh definition and finalizes mesh data

				void Finalize(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
				{
					Transform(pos, rot, scale);
					ComputeVertexArrayBoundingBox();
					ComputeVertexArrayRadius();
					ComputeSurfaceNormals();
					ComputeSurfaceIndices();

					std::cout << std::endl;
				}
				
				// ---------------------------------------------------------------
				// add a surface to this mesh

				void AddSurface(const glm::ivec3& vertexindices) 
				{
					
					// retrieve vertex indices

					int I0 = vertexindices.x;
					int I1 = vertexindices.y;
					int I2 = vertexindices.z;

					bool skipface = false;

					// check for out of range index

					if (I0 < 0 || I0 >= VertexArray.size())
						skipface = true;

					if (I1 < 0 || I1 >= VertexArray.size())
						skipface = true;

					if (I2 < 0 || I2 >= VertexArray.size())
						skipface = true;

					//	checks for equal indicies 

					if (I0 == I1 || I1 == I2 || I0 == I2)
						skipface = true;

					// if we get here triangle is not degenerated
					// so we can create a new surface and insert into the
					// surface array 

					if (!skipface)
					{
						skipface = false;

						// retrieve point coordinates and transform into glm vectors
						
						glm::vec3 V0 = glm::vec3(VertexArray[I0].Pos.x, VertexArray[I0].Pos.y, VertexArray[I0].Pos.z);
						glm::vec3 V1 = glm::vec3(VertexArray[I1].Pos.x, VertexArray[I1].Pos.y, VertexArray[I1].Pos.z);
						glm::vec3 V2 = glm::vec3(VertexArray[I2].Pos.x, VertexArray[I2].Pos.y, VertexArray[I2].Pos.z);

						// check equal for points 

						if ((CloseEnough(V0.x, V1.x) &&
							 CloseEnough(V0.y, V1.y) &&
							 CloseEnough(V0.z, V1.z)))
								skipface = true;

						if ((CloseEnough(V1.x, V2.x) &&
							 CloseEnough(V1.y, V2.y) &&
							 CloseEnough(V1.z, V2.z)))
								skipface = true;

						if ((CloseEnough(V0.x, V2.x) &&
							 CloseEnough(V0.y, V2.y) &&
							 CloseEnough(V0.z, V2.z)))
								skipface = true;

						// add surface
		
						if (!skipface)
						{
							SurfaceArray.emplace_back(vertexindices);
						}
						else
						{
							std::cout << "Skipped face " << std::endl;
						}

					}
					else
					{
						std::cout << "Skipped face " << std::endl;
					}
				}
		
				// ---------------------------------------------------------------
				// add vertex

				void AddVertex(const glm::vec3& p)
				{
					VertexArray.emplace_back(p, glm::vec3(0, 0, 0), glm::vec2(0, 0));
				}

				// ---------------------------------------------------------------
				// begin mesh definition

				void Begin(const std::string& resourcefilename)
				{
					Radius = 0;
					BoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					ResourceFileName = resourcefilename;
					VertexArray.clear();
					SurfaceArray.clear();
					SurfaceIndices.clear();
				}
				
				// ---------------------------------------------------------------
				// save mesh given current path

				void Save()
				{
					
					std::cout << "MeshBuilder : Saving " << ResourceFileName << std::endl;
					
					FILE* stream;

					errno_t err = fopen_s(&stream, ResourceFileName.c_str(), "wb");

					if (err == 0)
					{
						// save vertices count

						unsigned int verticescount = (unsigned int)VertexArray.size();
						fwrite(&verticescount, sizeof(unsigned int), 1, stream);

						// save vertices

						for (size_t i = 0; i < verticescount; ++i)
						{
							fwrite(&VertexArray[i].Pos, sizeof(float), 3, stream);
							fwrite(&VertexArray[i].Normal, sizeof(float), 3, stream);
							fwrite(&VertexArray[i].UV, sizeof(float), 2, stream);
						}

					//	std::cout << "MeshBuilder : Saving  vertices " << verticescount << std::endl;

						// save surfaces indices count

						unsigned int indicescount = (int)SurfaceIndices.size();
						fwrite(&indicescount, sizeof(unsigned int), 1, stream);

						// save surfaces indices

						fwrite(&SurfaceIndices[0], sizeof(unsigned int), indicescount, stream);

					//	std::cout << "MeshBuilder : Saving " << indicescount << std::endl;

						// save metrics

						fwrite(&BoundingBox.GetMin(), sizeof(float), 3, stream);
						fwrite(&BoundingBox.GetMax(), sizeof(float), 3, stream);
						fwrite(&Radius, sizeof(float), 1, stream);

					//	std::cout << "Bmin : " << BoundingBox.GetMin().x << " " << BoundingBox.GetMin().y << " " << BoundingBox.GetMin().z << std::endl;
					//	std::cout << "Bmax : " << BoundingBox.GetMax().x << " " << BoundingBox.GetMax().y << " " << BoundingBox.GetMax().z << std::endl;
					//	std::cout << "Radius : " << Radius << std::endl;

						// Close stream if it is not NULL 

						if (fclose(stream) != 0)
							vml::os::Message::Error("MeshBuilder : ", "Cannot close mesh file ' ", ResourceFileName.c_str(), " '");

						std::cout << "MeshBuilder : Finished Saving " << ResourceFileName << std::endl;
						
					}
					else
					{
						vml::os::Message::Error("MeshBuilder : ", "Cannot save mesh ' ", ResourceFileName.c_str(), " ' (error code : ", err, " )");
					}
				
				}

				// ---------------------------------------------------------------
				//	query functions 

				size_t GetVertexCount()					 const { return VertexArray.size(); }
				glm::vec3 GetVertexPosAt(const size_t i) const { return glm::vec3(VertexArray[i].Pos.x, VertexArray[i].Pos.y, VertexArray[i].Pos.z); }

				// ---------------------------------------------------------------
				// constructors / destructors

				Level2dMeshBuilder()
				{
					Radius = 0;
					BoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					ResourceFileName = "";
				}

				// destructor

				~Level2dMeshBuilder()
				{
					ReleaseAll();
				}
		
		};

	} // end of meshes namespace

} // end of vml namespace


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fa2040
{
	namespace tools
	{

		class Level2dGenerator
		{
			private:
				
				unsigned char* LumaTexture;			// Texture holding only luminance values 1 for pixel being occupied , 0 if not			 
				unsigned int   LumaTextureW;
				unsigned int   LumaTextureH;
				unsigned int   LumaTextureSize;
				
				unsigned char* NavTexture;			// Texture holding only luminance values 1 for pixel being occupied , 0 if not			 
				unsigned int   NavTextureW;
				unsigned int   NavTextureH;
				unsigned int   NavTextureSize;

				int			   NavMeshScalingFactor;
				int			   NavMeshErosionFactor;
				bool		   Compiled;

				std::string	   FileName;
				std::string	   BitmapFileName;
				std::string	   MeshFileName;
				std::string	   CollisionMeshFileName;
				std::string	   NavMeshFileName;
				std::string	   NavMeshBitmapMaskFileName;
				
				// ------------------------------------------------------------------------------------------
				// Groups horizontal blocks into a larger horizontal block

				bool GrowHorzTopBlocks(fa2040::tools::TriangleLayer& trilayer, const unsigned char* data)
				{

					glm::vec2 Cp[8] = {};		// control points

					float pi, pj;

					bool On = false;

					glm::vec2 StartEdge1, StartEdge2, EndEdge1, EndEdge2;

					// start at center of pixel cell

					pi = 0.5f;
					pj = 0.5f;

					for (int j = 0; j < LumaTextureH; ++j)
					{

						for (int i = 0; i < LumaTextureW; ++i)
						{

							// setup control points

						//	Cp[0] = glm::vec2(pi, pj - 0.5f);			// top
						//	Cp[1] = glm::vec2(pi + 0.5f, pj);			// right
						//	Cp[2] = glm::vec2(pi, pj + 0.5f);			// bottom
							Cp[3] = glm::vec2(pi - 0.5f, pj);			// left
						//	Cp[4] = glm::vec2(pi - 0.5f, pj - 0.5f);	// upper left
						//	Cp[5] = glm::vec2(pi + 0.5f, pj - 0.5f);	// upper right
						//	Cp[6] = glm::vec2(pi + 0.5f, pj + 0.5f);	// lower left
							Cp[7] = glm::vec2(pi - 0.5f, pj + 0.5f);	// lower right

							if (data[i + j * LumaTextureW] == 1)
							{
								if (On == false)
								{
									StartEdge1 = Cp[3];
									EndEdge1 = Cp[7];
									On = true;
								}
							}
							else
							{
								if (On == true)
								{
									StartEdge2 = Cp[3];
									EndEdge2 = Cp[7];
									On = false;

									TriangleLayer.AddTriangle(StartEdge1, StartEdge2, EndEdge1, GridTriangle::TOP_TRIANGLE);
									TriangleLayer.AddTriangle(StartEdge2, EndEdge2, EndEdge1, GridTriangle::BORDER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(StartEdge1, StartEdge2);

								}
							}

							// increase u step

							pi += 1.0f;

						}

						// reset u to start and increases v step

						pi = 0.5f;
						pj += 1.0f;

					}

					return true;
				}
				
				// ---------------------------------------------------------
				// Groups horizontal blocks into a larger horizontal block

				bool GrowHorzBottomBlocks(fa2040::tools::TriangleLayer& trilayer, const unsigned char* data)
				{
					glm::vec2 Cp[8] = {};		// control points

					float pi, pj;

					bool On = false;

					glm::vec2 StartEdge1, StartEdge2, EndEdge1, EndEdge2;

					// start at center of pixel cell

					pi = 0.5f;
					pj = 0.5f;

					for (int j = 0; j < LumaTextureH; ++j)
					{
						for (int i = 0; i < LumaTextureW; ++i)
						{

							// setup control points

						//	Cp[0] = glm::vec2(pi, pj - 0.5f);			// top
						//	Cp[1] = glm::vec2(pi + 0.5f, pj);			// right
						//	Cp[2] = glm::vec2(pi, pj + 0.5f);			// bottom
							Cp[3] = glm::vec2(pi - 0.5f, pj);			// left
							Cp[4] = glm::vec2(pi - 0.5f, pj - 0.5f);	// upper left
						//	Cp[5] = glm::vec2(pi + 0.5f, pj - 0.5f);	// upper right
						//	Cp[6] = glm::vec2(pi + 0.5f, pj + 0.5f);	// lower left
						//	Cp[7] = glm::vec2(pi - 0.5f, pj + 0.5f);	// lower right

							if (data[i + j * LumaTextureW] == 1)
							{
								if (On == false)
								{
									StartEdge1 = Cp[4];
									EndEdge1 = Cp[3];
									On = true;
								}
							}
							else
							{
								if (On == true)
								{
									StartEdge2 = Cp[4];
									EndEdge2 = Cp[3];
									On = false;

									TriangleLayer.AddTriangle(StartEdge1, StartEdge2, EndEdge1, GridTriangle::BOTTOM_TRIANGLE);
									TriangleLayer.AddTriangle(StartEdge2, EndEdge2, EndEdge1, GridTriangle::BORDER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(EndEdge2, EndEdge1);

								}
							}

							// increase u step

							pi += 1.0f;

						}

						// reset u to start and increases v step

						pi = 0.5f;
						pj += 1.0f;

					}

					return true;
				}
				
				// ---------------------------------------------------------
				//

				bool GrowVertLeftBlocks(fa2040::tools::TriangleLayer& trilayer, const unsigned char* data)
				{
					glm::vec2 Cp[8] = {};		// control points

					float pi, pj;

					bool On = false;

					glm::vec2 StartEdge1, StartEdge2, EndEdge1, EndEdge2;

					// start at center of pixel cell

					pi = 0.5f;
					pj = 0.5f;

					for (int i = 0; i < LumaTextureW; ++i)
					{
						for (int j = 0; j < LumaTextureH; ++j)
						{

							// setup control points

						//	Cp[0] = glm::vec2(pi, pj - 0.5f);		  // top
						//	Cp[1] = glm::vec2(pi + 0.5f, pj);		  // right
							Cp[2] = glm::vec2(pi, pj + 0.5f);		  // bottom
						//	Cp[3] = glm::vec2(pi - 0.5f, pj);		  // left
						//	Cp[4] = glm::vec2(pi - 0.5f, pj - 0.5f);  // upper lieft
						//	Cp[5] = glm::vec2(pi + 0.5f, pj - 0.5f);  // upper right
							Cp[6] = glm::vec2(pi + 0.5f, pj + 0.5f);  // lower left
						//	Cp[7] = glm::vec2(pi - 0.5f, pj + 0.5f);  // lower right

							if (data[i + j * LumaTextureW] == 1)
							{
								if (On == false)
								{
									StartEdge1 = Cp[2];
									EndEdge1 = Cp[6];

									StartEdge1.y -= 1.0f;
									EndEdge1.y -= 1.0f;

									On = true;
								}
							}
							else
							{
								if (On == true)
								{
									StartEdge2 = Cp[2];
									EndEdge2 = Cp[6];

									StartEdge2.y -= 1.0f;
									EndEdge2.y -= 1.0f;

									On = false;

									TriangleLayer.AddTriangle(StartEdge2, StartEdge1, EndEdge1, GridTriangle::BORDER_TRIANGLE);
									TriangleLayer.AddTriangle(StartEdge2, EndEdge1, EndEdge2, GridTriangle::LEFT_TRIANGLE);

									BorderLayer.AddBorderLayerLine(StartEdge2, StartEdge1);

								}
							}

							// increase u step

							pj += 1.0f;

						}

						// reset u to start and increases v step

						pj = 0.5f;
						pi += 1.0f;

					}

					return true;
				}

				// ---------------------------------------------------------
				//

				bool GrowVertRightBlocks(fa2040::tools::TriangleLayer& trilayer, const unsigned char* data)
				{

					glm::vec2 Cp[8] = {};		// control points

					float pi, pj;

					bool On = false;

					glm::vec2 StartEdge1, StartEdge2, EndEdge1, EndEdge2;

					// start at center of pixel cell

					pi = 0.5f;
					pj = 0.5f;

					for (int i = 0; i < LumaTextureW; ++i)
					{
						for (int j = 0; j < LumaTextureH; ++j)
						{

							// setup control points

						//	Cp[0] = glm::vec2(pi, pj - 0.5f);		  // top
						//	Cp[1] = glm::vec2(pi + 0.5f, pj);		  // right
							Cp[2] = glm::vec2(pi, pj + 0.5f);		  // bottom
						//	Cp[3] = glm::vec2(pi - 0.5f, pj);		  // left
						//	Cp[4] = glm::vec2(pi - 0.5f, pj - 0.5f);  // upper lieft
						//	Cp[5] = glm::vec2(pi + 0.5f, pj - 0.5f);  // upper right
						//	Cp[6] = glm::vec2(pi + 0.5f, pj + 0.5f);  // lower left
							Cp[7] = glm::vec2(pi - 0.5f, pj + 0.5f);  // lower right

							if (data[i + j * LumaTextureW] == 1)
							{
								if (On == false)
								{
									StartEdge1 = Cp[7];
									EndEdge1 = Cp[2];

									StartEdge1.y -= 1.0f;
									EndEdge1.y -= 1.0f;

									On = true;
								}
							}
							else
							{
								if (On == true)
								{
									StartEdge2 = Cp[7];
									EndEdge2 = Cp[2];

									StartEdge2.y -= 1.0f;
									EndEdge2.y -= 1.0f;

									On = false;

									TriangleLayer.AddTriangle(StartEdge2, EndEdge1, EndEdge2, GridTriangle::RIGHT_TRIANGLE);
									TriangleLayer.AddTriangle(StartEdge2, StartEdge1, EndEdge1, GridTriangle::BORDER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(EndEdge1, EndEdge2);

								}
							}

							// increase u step

							pj += 1.0f;

						}

						// reset u to start and increases v step

						pj = 0.5f;
						pi += 1.0f;

					}

					return true;
				}

				// --------------------------------------------------------------------------
				// Checks if a cell is totally composed of skip values

				bool CheckCell(unsigned char* data, int i, int j, int ii, int jj, int keep) const
				{
					for (int b = j; b <= jj; b++)
					{
						for (int a = i; a <= ii; a++)
						{
							if (data[a + b * LumaTextureW] != keep)
								return false;
						}
					}
					return true;
				}

				// --------------------------------------------------------------------------
				// Fills block with skip value to allow the search
				// for the next free block

				void CleanCell(unsigned char* data, int i, int j, int ii, int jj, int skip)
				{
					for (int b = j; b <= jj; b++)
					{
						for (int a = i; a <= ii; a++)
						{
							data[a + b * LumaTextureW] = skip;
						}
					}
				}

				// --------------------------------------------------------------------------
				// Find the first unused pixel which from there
				// will be used to grow a new block

				bool FindFreeCell(unsigned char* data, int* i, int* j, int keep) const
				{
					for (int b = 0; b < LumaTextureH; b++)
					{
						for (int a = 0; a < LumaTextureW; a++)
						{
							if (data[a + b * LumaTextureW] == keep)
							{
								*i = a;
								*j = b;

								return true;
							}
						}
					}
					return false;
				}
				
				// --------------------------------------------------------------------------
				//

				bool GrowAll(unsigned char* data, GridBlock& source, int keep)
				{
					int x1 = source.GetI1();
					int y1 = source.GetJ1();
					int x2 = source.GetI2();
					int y2 = source.GetJ2();

					x2++;
					y2++;

					if (x2 >= LumaTextureW) return false;
					if (y2 >= LumaTextureH) return false;

					if (CheckCell(data, x1, y1, x2, y2, keep))
					{
						source = GridBlock(0, x1, y1, x2, y2);
						return true;
					}

					return false;
				}
				
				// --------------------------------------------------
				//

				bool GrowRight(unsigned char* data, GridBlock& source, int keep)
				{
					int x1 = source.GetI1();
					int y1 = source.GetJ1();
					int x2 = source.GetI2();
					int y2 = source.GetJ2();

					x2++;

					if (x2 >= LumaTextureW) return false;

					if (CheckCell(data, x1, y1, x2, y2, keep))
					{
						source = GridBlock(0, x1, y1, x2, y2);
						return true;
					}

					return false;
				}
				
				// --------------------------------------------------
				//

				bool GrowDown(unsigned char* data, GridBlock& source, int keep)
				{
					int x1 = source.GetI1();
					int y1 = source.GetJ1();
					int x2 = source.GetI2();
					int y2 = source.GetJ2();

					y2++;

					if (y2 >= LumaTextureH) return false;

					if (CheckCell(data, x1, y1, x2, y2, keep))
					{
						source = GridBlock(0, x1, y1, x2, y2);
						return true;
					}

					return false;
				}

				// --------------------------------------------------
				//

				bool GrowBlock(unsigned char* data, GridBlock& dest, int x1, int y1, int keep)
				{
					int x2 = x1;
					int y2 = y1;

					GridBlock source = GridBlock(0, x1, y1, x2, y2);

					while (GrowAll(data, source, keep)) {};

					GridBlock right = source;
					GridBlock down = source;

					while (GrowRight(data, right, keep)) {};
					while (GrowDown(data, down, keep)) {};

					// chose the largest block

					if (right.GetArea() > down.GetArea())
						dest = right;
					else dest = down;

					return true;
				}
				
				// --------------------------------------------------
				// main growth function

				bool GrowRectangularBlocksAndFinalize(fa2040::tools::TriangleLayer& trilayer,
													  const unsigned char* data,
													  const int keep = 1, const int skip = 0,
													  const int state = GridTriangle::INTERNAL_TRIANGLE)
				{

					// allocate cache map matrix

					unsigned char* map = new unsigned char[LumaTextureSize];

					// copy source matrix in cache matrix

					memcpy(map, data, LumaTextureSize);

					int celli = -1, cellj = -1;

					while (FindFreeCell(map, &celli, &cellj, keep))
					{
						GridBlock block;

						if (GrowBlock(map, block, celli, cellj, keep))
						{

							// block has reached max area

							int i1 = block.GetI1();
							int j1 = block.GetJ1();
							int i2 = block.GetI2();
							int j2 = block.GetJ2();

							glm::vec2 p = glm::vec2(i1, j1);
							glm::vec2 q = glm::vec2(i2 + 1, j2 + 1);
							glm::vec2 t = glm::vec2(p.x, q.y);
							glm::vec2 w = glm::vec2(q.x, p.y);

							trilayer.AddTriangle(p, q, t, state);
							trilayer.AddTriangle(q, p, w, state);

							// set this block as visited

							CleanCell(map, block.GetI1(), block.GetJ1(), block.GetI2(), block.GetJ2(), skip);

						}

					}

					// free memory

					vml::os::SafeDelete(map);

					// optmize layer

					return true;
				}
				
				// --------------------------------------------------
				// main growth function

				bool CreateNavMesh(fa2040::tools::TriangleLayer& trilayer)
				{
					glm::vec2 a,b,c,d;

					float scaling = (float)NavMeshScalingFactor;
					
					glm::vec2 o(-scaling * 0.5f, -scaling * 0.5f);

					for (int j = 0; j < NavTextureH; ++j)
					{
						for (int i = 0; i < NavTextureW; ++i)
						{
							if (NavTexture[i + j * NavTextureW] == 1)
							{
								float pi = (float)i;
								float pj = (float)j;

								a = glm::vec2(pi        , pj        ) * scaling + o;  // upper left
								b = glm::vec2(pi + 1.0  , pj        ) * scaling + o;  // upper right
								c = glm::vec2(pi        , pj + 1.0f ) * scaling + o;  // lower left
								d = glm::vec2(pi + 1.0f , pj + 1.0f ) * scaling + o;  // lower right

								ExternalTriangleLayer2.AddTriangle(a, b, c, GridTriangle::INTERNAL_TRIANGLE);
								ExternalTriangleLayer2.AddTriangle(b, d, c, GridTriangle::INTERNAL_TRIANGLE);
							}

						}

					}

					return true;
				}

				// --------------------------------------------------------------------------

				void DownScaleNavMesh(const unsigned char *data )
				{

					if (NavMeshScalingFactor <= 1) return;
					
					NavTextureH = LumaTextureH / NavMeshScalingFactor;
					NavTextureW = LumaTextureW / NavMeshScalingFactor;
					NavTextureSize = NavTextureH * NavTextureW;

					delete NavTexture;

					NavTexture = new unsigned char[NavTextureSize];

					for ( int y = 0; y < NavTextureH; y++ )
					{
						int y2 = NavMeshScalingFactor * y;

						for ( int x = 0; x < NavTextureW; x++ )
						{
							int x2 = NavMeshScalingFactor * x;

							NavTexture[ x + y * NavTextureW ] = data[ x2 + y2 * LumaTextureW ];
						}

					}
				}

				// --------------------------------------------------------------------------
				// erode bitmap to make it thiner 

				void ErodeNavMesh()
				{
					if (NavMeshErosionFactor <= 0) return;
					
					for ( size_t i=0; i< NavMeshErosionFactor; ++i)
					{
						unsigned char* out = new unsigned char[NavTextureSize];
						unsigned char* itr = NavTexture;
						unsigned char* otr = out;

						memcpy(out, NavTexture, NavTextureSize);

						// for true pixels. kill pixel if there is at least one false neighbor 

						for (int row = 0; row < NavTextureH; row++)
						{
							for (int col = 0; col < NavTextureW; col++)
							{
								if (*itr)      // erode only operates on true pixels 
								{
									// more efficient with C's left to right evaluation of     
									// conjuctions. E N S functions not executed if W is false 

									unsigned char *ch = (unsigned char*)itr;

									unsigned char getwest  = 0;
									unsigned char geteast  = 0;
									unsigned char getnorth = 0;
									unsigned char getsouth = 0;

									if (col < 1) getwest = 1;
									 else getwest=*(ch - 1);

									if (col >= NavTextureW - 1)	geteast = 1;
									else geteast= *(ch + 1);

									if (row < 1) getnorth = 1;
									else	getnorth = *(ch - NavTextureW);

									if (row >= NavTextureH - 1)	getsouth = 1;
									else getsouth = *(ch + NavTextureW);

									if (!getwest || !geteast || !getnorth || !getsouth)
										*otr = 0;

								}

								itr++;
								otr++;
							}
						}

						delete NavTexture;
						NavTexture = out;
					}

				}

				// --------------------------------------------------------------------------
				// 0 is visited , 1 is unvisited, grows a larger boundary around visited pixels
				// so the path polygon mesh does not collide with poygon borders

				void GrowVisited(unsigned char* data)
				{
					int index[9] = { 0 };

					for (int j = 1; j < LumaTextureH - 1; ++j)
					{
						for (int i = 1; i < LumaTextureW - 1; ++i)
						{
							int offset = i + j * LumaTextureW;

							if (data[offset] == 0)
							{

								index[0] = i - 1 + (j - 1) * LumaTextureW;
								index[1] = i     + (j - 1) * LumaTextureW;
								index[2] = i + 1 + (j - 1) * LumaTextureW;

								index[3] = i - 1 + (j)     * LumaTextureW;
								index[4] = i     + (j)     * LumaTextureW;
								index[5] = i + 1 + (j)     * LumaTextureW;

								index[6] = i - 1 + (j + 1) * LumaTextureW;
								index[7] = i     + (j + 1) * LumaTextureW;
								index[8] = i + 1 + (j + 1) * LumaTextureW;

								if (data[index[0]] == 1) { data[index[0]] = 2; }
								if (data[index[1]] == 1) { data[index[1]] = 2; }
								if (data[index[2]] == 1) { data[index[2]] = 2; }
								if (data[index[3]] == 1) { data[index[3]] = 2; }
								if (data[index[4]] == 1) { data[index[4]] = 2; }
								if (data[index[5]] == 1) { data[index[5]] = 2; }
								if (data[index[6]] == 1) { data[index[6]] = 2; }
								if (data[index[7]] == 1) { data[index[7]] = 2; }
								if (data[index[8]] == 1) { data[index[8]] = 2; }

							}
						}
					}
					
					// border pixels set as visited, note that eve if the operation is
					// executed on all the bitmap, only outer borders are rset to zero
					// becasue '2' value indicates that the border mesh is not triangularized

					for (int i = 0; i < LumaTextureW * LumaTextureH; ++i)
					{
						if ( data[i] == 2 )
						{
							data[i] = 0;
						}
					}
					
					// copy data popinter to navtextue

					NavTextureW = LumaTextureW;
					NavTextureH = LumaTextureH;
					NavTextureSize = NavTextureW * NavTextureH;

					memcpy(NavTexture, data, NavTextureSize);
									
				}
				
				// -------------------------------------------------------
				// polygonize a bitmap given the bitmap itself and level id

				void PolygonizeLayer()
				{
					
					glm::ivec2 N[9]  = {};		// address of adjacent pixels
					glm::vec2  Cp[8] = {};	    // control points

					// clean visited map

					unsigned char* VisitedBitmap = new unsigned char[LumaTextureSize];	// bitmap used to stare visited pixels
					unsigned char* HorzBitmap1 = new unsigned char[LumaTextureSize];	// bitmap used to compuite long top horizontal pixels
					unsigned char* HorzBitmap2 = new unsigned char[LumaTextureSize];	// bitmap used to compuite long top horizontal pixels
					unsigned char* VertBitmap1 = new unsigned char[LumaTextureSize];  // bitmap used to compuite long left vertical pixels
					unsigned char* VertBitmap2 = new unsigned char[LumaTextureSize];  // bitmap used to compuite long right vertical pixels

					memset(VisitedBitmap, 0, LumaTextureSize);
					memset(HorzBitmap1, 0, LumaTextureSize);
					memset(HorzBitmap2, 0, LumaTextureSize);
					memset(VertBitmap1, 0, LumaTextureSize);
					memset(VertBitmap2, 0, LumaTextureSize);

					// marching squares algorithm
					// each cell is identified with a bit in the mask if the bit is on then its nearest pixel
					// to the center cell is on
					// example : if c0 which is the upper entry in the data array is set to 1 ( white pixel ),
					// then the '0' at the top left corner of the c4 ( center cell is set to 1 )
					// if c1 is true, then both '0' and'1' will be set to true as well , and so forth

					// ------------------------------------------
					// | c0			| c1			| c2		|
					// |			|				|			|
					// |			|				|			|
					// |			|				|			|
					// -------------0---------------1------------
					// | c3			| c4			| c5		|
					// |			|				|			|
					// |			|	center		|			|
					// |			|				|			|
					// -------------3---------------2------------
					// | c6			| c7			| c8		|
					// |			|				|			|
					// |			|				|			|
					// |			|				|			|
					// ------------------------------------------

					// start at center of pixel cell

					float pi = 0.5f;
					float pj = 0.5f;
					
					for (int j = 0; j < LumaTextureH; ++j)
					{

						for (int i = 0; i < LumaTextureW; ++i)
						{
							
							// get adjacent cells to the current cell
							// they are in total 9 , center cell is
							// current cell located at 4
							// N is 3x3 matrix holding offset
							// i and j are the current indices in the [ (ax,ay) , (bx,by) ] ranges
							// defining a rectangular grid starting at (ax,ay) and ending at (bx,by)

							unsigned short int mask = 0;	    // mask for case handling

							if (i == 0) mask |= 1;
						//	if (i < 0 ) mask |= 2;
							if (i > 0 ) mask |= 4;

							if (j == 0) mask |= 8;
						//	if (j < 0 ) mask |= 16;
							if (j > 0 ) mask |= 32;

							if (i == LumaTextureW - 1) mask |= 64;
							if (i <  LumaTextureW - 1) mask |= 128;
						//	if (i >  LumaTextureW - 1) mask |= 256;

							if (j == LumaTextureH - 1) mask |= 512;
							if (j <  LumaTextureH - 1) mask |= 1024;
						//	if (j >  LumaTextureH - 1) mask |= 2048;

							glm::ivec2 noaddr(-1, -1);
							
							// handle cases

							switch (mask)
							{

								case 0x489:

									// cell is in the upper left corner of the map (dec code 1161)

									N[0] = noaddr;
									N[1] = noaddr;
									N[2] = noaddr;
									N[3] = noaddr;
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = noaddr;
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = glm::ivec2(i + 1, j + 1);

								break;

								case 0x48C:

									// cell is at the top of the map ( dec code 1164 )

									N[0] = noaddr;
									N[1] = noaddr;
									N[2] = noaddr;
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = glm::ivec2(i - 1, j + 1);
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = glm::ivec2(i + 1, j + 1);

								break;

								case 0x44C:

									// cell is in the upper right corner of the map ( dec code 1100 )

									N[0] = noaddr;
									N[1] = noaddr;
									N[2] = noaddr;
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = noaddr;
									N[6] = glm::ivec2(i - 1, j + 1);
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = noaddr;

								break;

								case 0x464:

									// cell is at the right of the map ( dec code 1124 )

									N[0] = glm::ivec2(i - 1, j - 1);
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = noaddr;
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = noaddr;
									N[6] = glm::ivec2(i - 1, j + 1);
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = noaddr;

								break;

								case 0x264:

									// cell is at the bottom right corner of the map ( dec code 612 )

									N[0] = glm::ivec2(i - 1, j - 1);
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = noaddr;
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = noaddr;
									N[6] = noaddr;
									N[7] = noaddr;
									N[8] = noaddr;

								break;

								case 0x2A4:

									// cell is at the bottom of the map ( dec code 676 )

									N[0] = glm::ivec2(i - 1, j - 1);
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = glm::ivec2(i + 1, j - 1);
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = noaddr;
									N[7] = noaddr;
									N[8] = noaddr;

								break;

								case 0x2A1:

									// cell is at the bottom left corner of the map ( dec code 673 )

									N[0] = noaddr;
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = glm::ivec2(i + 1, j - 1);
									N[3] = noaddr;
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = noaddr;
									N[7] = noaddr;
									N[8] = noaddr;

								break;

								case 0x4A1:

									// cell is at the left of the map ( dec code 1185 )

									N[0] = noaddr;
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = glm::ivec2(i + 1, j - 1);
									N[3] = noaddr;
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = noaddr;
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = glm::ivec2(i + 1, j + 1);

								break;

								default:

									// cell is fully inside the map

									N[0] = glm::ivec2(i - 1, j - 1);
									N[1] = glm::ivec2(i    , j - 1);
									N[2] = glm::ivec2(i + 1, j - 1);
									N[3] = glm::ivec2(i - 1, j);
									N[4] = glm::ivec2(i    , j);
									N[5] = glm::ivec2(i + 1, j);
									N[6] = glm::ivec2(i - 1, j + 1);
									N[7] = glm::ivec2(i    , j + 1);
									N[8] = glm::ivec2(i + 1, j + 1);

								break;

							}

							// according to active cell addresses, the function computes which case
							// the current pixel falls in see diagram above for further explanation these
							// hex values are hard coded from bit masks
							//
							//		bit 0		bit 1		bit 2		bit 3		value		hex
							// --------------------------------------------------------------------------
							// |	1		|	0		|	0		|	0		|	1		|	0x1		|
							// --------------------------------------------------------------------------
							// |	1		|	1		|	0		|	0		|	3		|	0x3		|
							// --------------------------------------------------------------------------
							// |	0		|	1		|	0		|	0		|	2		|	0x2		|
							// --------------------------------------------------------------------------
							// |	1		|	0		|	0		|	1		|	9		|	0x9		|
							// --------------------------------------------------------------------------
							// |	1		|	1		|	1		|	1		|	15		|	0xF		|
							// --------------------------------------------------------------------------
							// |	0		|	1		|	1		|	0		|	6		|	0x6		|
							// --------------------------------------------------------------------------
							// |	0		|	0		|	0		|	1		|	8		|	0x8		|
							// --------------------------------------------------------------------------
							// |	0		|	0		|	1		|	1		|	12		|	0xC		|
							// --------------------------------------------------------------------------
							// |	0		|	0		|	1		|	0		|	4		|	0x4		|
							// --------------------------------------------------------------------------

							mask = 0;

							if (N[0].x != -1 && N[0].y != -1) if (LumaTexture[N[0].x + N[0].y * LumaTextureW] == 1) mask |= 0x1;
							if (N[1].x != -1 && N[1].y != -1) if (LumaTexture[N[1].x + N[1].y * LumaTextureW] == 1) mask |= 0x3;
							if (N[2].x != -1 && N[2].y != -1) if (LumaTexture[N[2].x + N[2].y * LumaTextureW] == 1) mask |= 0x2;
							if (N[3].x != -1 && N[3].y != -1) if (LumaTexture[N[3].x + N[3].y * LumaTextureW] == 1) mask |= 0x9;
							if (N[4].x != -1 && N[4].y != -1) if (LumaTexture[N[4].x + N[4].y * LumaTextureW] == 1) mask |= 0xF;
							if (N[5].x != -1 && N[5].y != -1) if (LumaTexture[N[5].x + N[5].y * LumaTextureW] == 1) mask |= 0x6;
							if (N[6].x != -1 && N[6].y != -1) if (LumaTexture[N[6].x + N[6].y * LumaTextureW] == 1) mask |= 0x8;
							if (N[7].x != -1 && N[7].y != -1) if (LumaTexture[N[7].x + N[7].y * LumaTextureW] == 1) mask |= 0xC;
							if (N[8].x != -1 && N[8].y != -1) if (LumaTexture[N[8].x + N[8].y * LumaTextureW] == 1) mask |= 0x4;

							// Handle the 16 cases manually for the  marching square algorithm.
							// Case 0xF (all inside of image area) produces no outlines.
							// Case 0x0 (all outside of image area) produces no geometry.
							//
							// case 0x1		case 0x2	case 0x4	case 0x8
							//
							//	10			00			00			01
							//	00			10			01			00
							//
							// case 0x3		case 0x6	case 0x9	case 0xC
							//
							// 11			01			10			00
							// 00			01			10			11
							//
							// case 0x5		case 0xA	case 0x7	case 0xB
							//
							// 10			01			11			11
							// 01			10			01			10
							//
							// case 0xD		case 0xE	case 0xF	case 0x0
							//
							// 10			01			11			00
							// 11			11			11			00

							// setup control points

							Cp[0] = glm::vec2(pi       , pj - 0.5f);  // top
							Cp[1] = glm::vec2(pi + 0.5f, pj);		  // right
							Cp[2] = glm::vec2(pi       , pj + 0.5f);  // bottom
							Cp[3] = glm::vec2(pi - 0.5f, pj);		  // left
							Cp[4] = glm::vec2(pi - 0.5f, pj - 0.5f);  // upper left
							Cp[5] = glm::vec2(pi + 0.5f, pj - 0.5f);  // upper right
							Cp[6] = glm::vec2(pi + 0.5f, pj + 0.5f);  // lower right
							Cp[7] = glm::vec2(pi - 0.5f, pj + 0.5f);  // lower left

							// triangles compising the boredr layer , moved below
							// 
							//		BorderLayer.AddBorderLayerLine(Cp[0], Cp[4]);
							//		BorderLayer.AddBorderLayerLine(Cp[4], Cp[3]);
							//		BorderLayer.AddBorderLayerLine(Cp[3], Cp[0]);

							//		BorderLayer.AddBorderLayerLine(Cp[0], Cp[5]);
							//		BorderLayer.AddBorderLayerLine(Cp[5], Cp[1]);
							//		BorderLayer.AddBorderLayerLine(Cp[1], Cp[0]);

							//		BorderLayer.AddBorderLayerLine(Cp[1], Cp[6]);
							//		BorderLayer.AddBorderLayerLine(Cp[6], Cp[2]);
							//		BorderLayer.AddBorderLayerLine(Cp[2], Cp[1]);

							//		BorderLayer.AddBorderLayerLine(Cp[2], Cp[7]);
							//		BorderLayer.AddBorderLayerLine(Cp[7], Cp[3]);
							//		BorderLayer.AddBorderLayerLine(Cp[3], Cp[2]);
							
							// handle cases and generate segment soup
							
							switch (mask)
							{

								case 0x1:

									// bottom right corner

									VisitedBitmap[i + j * LumaTextureW] = 1;
									TriangleLayer.AddTriangle(Cp[0], Cp[3], Cp[4], GridTriangle::BOTTOM_RIGHT_CORNER_TRIANGLE);
							
									BorderLayer.AddBorderLayerLine(Cp[0], Cp[3]);

								break;

								case 0x2:

									// bottom left corner

									VisitedBitmap[i + j * LumaTextureW] = 1;
									TriangleLayer.AddTriangle(Cp[1], Cp[0], Cp[5], GridTriangle::BOTTOM_LEFT_CORNER_TRIANGLE);
							
									BorderLayer.AddBorderLayerLine(Cp[1], Cp[0]);

								break;

								case 0x4:

									// top left corner

									VisitedBitmap[i + j * LumaTextureW] = 1;
									TriangleLayer.AddTriangle(Cp[2], Cp[1], Cp[6], GridTriangle::TOP_LEFT_CORNER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(Cp[2], Cp[1]);

								break;

								case 0x8:

									// top right corner

									VisitedBitmap[i + j * LumaTextureW] = 1;
									TriangleLayer.AddTriangle(Cp[3], Cp[2], Cp[7], GridTriangle::TOP_RIGHT_CORNER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(Cp[3], Cp[2]);
						
								break;

								case 0x3:

									// bottom

									VisitedBitmap[i + j * LumaTextureW] = 1;
									HorzBitmap2[i + j * LumaTextureW] = 1;
								
								break;

								case 0x6:

									// left

									VisitedBitmap[i + j * LumaTextureW] = 1;
									VertBitmap1[i + j * LumaTextureW] = 1;
							
								break;

								case 0x9:

									// right

									VisitedBitmap[i + j * LumaTextureW] = 1;
									VertBitmap2[i + j * LumaTextureW] = 1;

								break;

								case 0xC:

									// top

									VisitedBitmap[i + j * LumaTextureW] = 1;
									HorzBitmap1[i + j * LumaTextureW] = 1;

								break;

								case 0x5:

									// right - left corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[0], Cp[3], Cp[4], GridTriangle::RIGHT_LEFT_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[2], Cp[1], Cp[6], GridTriangle::RIGHT_LEFT_CORNER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(Cp[0], Cp[3]);
									BorderLayer.AddBorderLayerLine(Cp[2], Cp[1]);

								break;

								case 0xA:

									// left - right corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[1], Cp[0], Cp[5], GridTriangle::LEFT_RIGHT_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[3], Cp[2], Cp[7], GridTriangle::LEFT_RIGHT_CORNER_TRIANGLE);
				
									BorderLayer.AddBorderLayerLine(Cp[1], Cp[0]);
									BorderLayer.AddBorderLayerLine(Cp[3], Cp[2]);

								break;

								case 0x7:

									// right upper corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[3], Cp[5], Cp[2], GridTriangle::RIGHT_UPPER_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[2], Cp[5], Cp[6], GridTriangle::BORDER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[3], Cp[4], Cp[5], GridTriangle::BORDER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(Cp[2], Cp[3]);

								break;

								case 0xB:

									// left upper corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[2], Cp[4], Cp[1], GridTriangle::LEFT_UPPER_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[2], Cp[7], Cp[4], GridTriangle::BORDER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[1], Cp[4], Cp[5], GridTriangle::BORDER_TRIANGLE);
							
									BorderLayer.AddBorderLayerLine(Cp[1], Cp[2]);

								break;

								case 0xD:

									// left bottom corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[1], Cp[7], Cp[0], GridTriangle::LEFT_LOWER_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[1], Cp[6], Cp[7], GridTriangle::BORDER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[0], Cp[7], Cp[4], GridTriangle::BORDER_TRIANGLE);

									BorderLayer.AddBorderLayerLine(Cp[0], Cp[1]);

								break;

								case 0xE:

									// right bottom corner

									VisitedBitmap[i + j * LumaTextureW] = 1;

									TriangleLayer.AddTriangle(Cp[0], Cp[6], Cp[3], GridTriangle::RIGHT_LOWER_CORNER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[0], Cp[5], Cp[6], GridTriangle::BORDER_TRIANGLE);
									TriangleLayer.AddTriangle(Cp[3], Cp[6], Cp[7], GridTriangle::BORDER_TRIANGLE);
															
									BorderLayer.AddBorderLayerLine(Cp[3], Cp[0]);

								break;

								case 0xF:

									// Case 0xF (all inside of image area) produces no outlines.

								break;

								case 0x0:

									// Case 0x0 (all outside of image area) produces no geometry.

									VisitedBitmap[i + j * LumaTextureW] = 1;

								break;

								default:

									// No default case

								break;
							}
							
							// increase u step

							pi += 1.0f;
						}

						// reset u to start and increases v step

						pi = 0.5f;
						pj += 1.0f;
					}
					
					
				//	std::cout << "1st step" << std::endl;
					
					// rectangularize horizontal and vertical long pixel runs

					GrowHorzTopBlocks(TriangleLayer    , HorzBitmap1);
					GrowHorzBottomBlocks(TriangleLayer , HorzBitmap2);
					GrowVertLeftBlocks(TriangleLayer   , VertBitmap1);
					GrowVertRightBlocks(TriangleLayer  , VertBitmap2);
					
				//	std::cout << "2nd step" << std::endl;

					// rectangularize the interior pixels

					GrowRectangularBlocksAndFinalize(TriangleLayer, VisitedBitmap, 0, 1, GridTriangle::EXTERNAL_TRIANGLE);
					
				//	std::cout << "3rd step" << std::endl;

					// rectangularize the outer pixels which will be used as navigation map

					GrowVisited(VisitedBitmap);

				//	std::cout << "4th step" << std::endl;

					// scales map down 
					
					DownScaleNavMesh(VisitedBitmap);

				//	std::cout << "5th step" << std::endl;

					ErodeNavMesh();

				//	std::cout << "6th step" << std::endl;

					// compute navmesh

					CreateNavMesh(ExternalTriangleLayer2);
					
				//	std::cout << "7th step" << std::endl;

					// free memory

					vml::os::SafeDelete(VisitedBitmap);
					vml::os::SafeDelete(HorzBitmap1);
					vml::os::SafeDelete(HorzBitmap2);
					vml::os::SafeDelete(VertBitmap1);
					vml::os::SafeDelete(VertBitmap2);
					
				}
							
				// ---------------------------------------------------------------
				//

				void SaveNavMeshMask2(const std::string& filename, const glm::vec3& center, const float scale)
				{

					std::cout << "saving navmesh (2)" << filename << std::endl;
					
					FILE* stream;

					errno_t err = fopen_s(&stream, filename.c_str(), "wb");

					if (err == 0)
					{
					
						unsigned int maskw = NavTextureW;
						unsigned int maskh = NavTextureH;

						fwrite(&maskw, sizeof(unsigned int), 1, stream);
						fwrite(&maskh, sizeof(unsigned int), 1, stream);
					
						// start at center of pixel cell

						float pi = 0.5f;
						float pj = 0.5f;

						float scaling = (float)NavMeshScalingFactor;

						glm::vec2 o(-scaling * 0.5f, -scaling * 0.5f);

						float dpx = scaling * scale;
						float dpy = scaling * scale;

						fwrite(&dpx, sizeof(float), 1, stream);
						fwrite(&dpy, sizeof(float), 1, stream);

					//	std::cout << "Mask Width " << maskw << std::endl;
					//	std::cout << "Mask Height " << maskh << std::endl;
					//	std::cout << "Mask dW " << dpx << std::endl;
					//	std::cout << "Mask dh " << dpy << std::endl;

						for (unsigned int j = 0; j < NavTextureH; ++j)
						{
							for (unsigned int i = 0; i < NavTextureW; ++i)
							{

								glm::vec2 a = glm::vec2(pi - 0.5f, pj - 0.5f) * scaling + o;  // upper left
								glm::vec2 b = glm::vec2(pi + 0.5f, pj - 0.5f) * scaling + o;  // upper right
								glm::vec2 c = glm::vec2(pi - 0.5f, pj + 0.5f) * scaling + o;  // lower left
								glm::vec2 d = glm::vec2(pi + 0.5f, pj + 0.5f) * scaling + o;  // lower right

								glm::vec3 ra(a.x * scale + center.x, a.y * scale + center.y, center.z);
								glm::vec3 rb(b.x * scale + center.x, b.y * scale + center.y, center.z);
								glm::vec3 rc(c.x * scale + center.x, c.y * scale + center.y, center.z);
								glm::vec3 rd(d.x * scale + center.x, d.y * scale + center.y, center.z);

								int offset = i + j * NavTextureW;
								unsigned int val = NavTexture[offset];
								
								fwrite(&i, sizeof(unsigned int), 1, stream);
								fwrite(&j, sizeof(unsigned int), 1, stream);
								
								fwrite(&ra.x, sizeof(float), 1, stream);
								fwrite(&ra.y, sizeof(float), 1, stream);
								fwrite(&ra.z, sizeof(float), 1, stream);
								
								fwrite(&rb.x, sizeof(float), 1, stream);
								fwrite(&rb.y, sizeof(float), 1, stream);
								fwrite(&rb.z, sizeof(float), 1, stream);
								
								fwrite(&rc.x, sizeof(float), 1, stream);
								fwrite(&rc.y, sizeof(float), 1, stream);
								fwrite(&rc.z, sizeof(float), 1, stream);
								
								fwrite(&rd.x, sizeof(float), 1, stream);
								fwrite(&rd.y, sizeof(float), 1, stream);
								fwrite(&rd.z, sizeof(float), 1, stream);

								fwrite(&val, sizeof(unsigned int), 1, stream);
										
								// increse pi

								pi += 1.0f;

							}

							// reset u to start and increases v step

							pi = 0.5f;
							pj += 1.0f;

						}
						
						// close stream

						if (fclose(stream) != 0)
							vml::os::Message::Error("MeshBuilder : ", "Cannot close mesh file ' ", filename.c_str(), " '");

					//	std::cout << "saving navmesh (2) done" << std::endl;

					}
					else
					{
						vml::os::Message::Error("MeshBuilder : ", "Cannot save navmask ' ", filename.c_str(), " ' (error code : ", err, " )");
					}
					
				}

				// ---------------------------------------------------------------
				//

				void ReleaseAll()
				{

					vml::os::SafeDelete(LumaTexture);
					vml::os::SafeDelete(NavTexture);

					LumaTextureW = 0;
					LumaTextureH = 0;
					LumaTextureSize = 0;

					NavTextureW = 0;
					NavTextureH = 0;
					NavTextureSize = 0;

					FileName = "";
					MeshFileName = "";
					CollisionMeshFileName = "";
					NavMeshFileName = "";
					NavMeshBitmapMaskFileName = "";

					Compiled = false;

					NavMeshScalingFactor = 1;
					NavMeshErosionFactor = 0;

					TriangleLayer.Clear();
					ExternalTriangleLayer2.Clear();
					BorderLayer.Clear();

					vml::utils::Logger::GetInstance()->Info("LevelGenerator : Resetting LevelGenerator");

//					std::cout << "Levelgeneratro2d : reset" << std::endl;

				}

			public:
				
				fa2040::tools::TriangleLayer TriangleLayer;
				fa2040::tools::TriangleLayer ExternalTriangleLayer2;
				fa2040::tools::BorderLayer   BorderLayer;
								
				fa2040::tools::TriangleLayer TriangleIslandLayer;

				// ------------------------------------------------------------------------------------
				//

				void Convert2dMapTo3dFileAndSave(const std::string &mainpath)
				{
					
					const fa2040::tools::TriangleLayer& trilayer = TriangleLayer;
					const fa2040::tools::BorderLayer& borderlayer = BorderLayer;
					
					Level2dMeshBuilder mapmb;
					Level2dMeshBuilder navmb;
					Level2dMeshBuilder colmb;

					// filenames
					
					std::string levelpath = mainpath + "\\" + "levels\\" + FileName;

					MeshFileName			  = levelpath + "\\" + FileName + ".3df";
					CollisionMeshFileName	  = levelpath + "\\" + FileName + "_col.3df";
					NavMeshFileName			  = levelpath + "\\" + FileName + "_nav.3df";
					NavMeshBitmapMaskFileName = levelpath + "\\" + FileName + "_nav_mask.nvm";

//					std::cout << "LevelGenerator : Saving In Progress..." << std::endl;

					vml::utils::Logger::GetInstance()->Info("LevelGenerator : Saving In Progress...");

					std::cout << "LevelGenerator : Saving " << MeshFileName << std::endl;
					std::cout << "LevelGenerator : Saving " << CollisionMeshFileName << std::endl;
					std::cout << "LevelGenerator : Saving " << NavMeshFileName << std::endl;
					std::cout << "LevelGenerator : Saving " << NavMeshBitmapMaskFileName << std::endl;
					
					// create physiucal dir

					std::filesystem::create_directories(levelpath);
				
					// save meshes

					mapmb.Begin(MeshFileName);
					navmb.Begin(NavMeshFileName);
					colmb.Begin(CollisionMeshFileName);

					std::cout << "Dumping data" << std::endl;
					std::cout << "Main Vertices : " << (int)trilayer.GetVerticesCount() << std::endl;
					
					for (int i = 0; i < (int)trilayer.GetVerticesCount(); ++i)
					{

						glm::vec3 pos = glm::vec3(trilayer.GetVertexXAt(i), trilayer.GetVertexYAt(i), trilayer.GetVertexZAt(i));

						mapmb.AddVertex(pos);

					//	vml::os::Message::Trace("Vertex :", i, px, py, pz);
						
				//		std::cout << "Vertex : " << i << " : , " << px << " , " << py << " , " << pz << std::endl;
					}
			
					std::cout << "Main Surfaces : " << (int)trilayer.GetTrianglesCount() << std::endl;

					for (int i = 0; i < (int)trilayer.GetTrianglesCount(); ++i)
					{
						const fa2040::tools::GridTriangle& tri = trilayer.GetTriangleAt(i);

						int i0 = tri.GetI0();
						int i1 = tri.GetI1();
						int i2 = tri.GetI2();

						//		vml::os::Trace("Surface %d : %d %d %d\n", i, i0, i1, i2);

						mapmb.AddSurface(glm::ivec3(i0, i1, i2));
					}

					// extrude border layer

					std::cout << "Border Vertices : " << (int)borderlayer.GetVerticesCount() << std::endl;
					
					float zdepth       = -100;
					float navmeshdepth = -50;

					glm::vec3 posP, posQ;

					for (int i = 0; i < borderlayer.GetVerticesCount(); i += 2)
					{
						
						posP = glm::vec3(borderlayer.GetVertexXAt(i), borderlayer.GetVertexYAt(i), borderlayer.GetVertexZAt(i));
						
						mapmb.AddVertex(posP);

						int i0 = (int)mapmb.GetVertexCount() - 1;
						
						int j = i + 1;

						posQ = glm::vec3(borderlayer.GetVertexXAt(j), borderlayer.GetVertexYAt(j), borderlayer.GetVertexZAt(j));

						mapmb.AddVertex(posQ);

						int i1 = (int)mapmb.GetVertexCount() - 1;

						mapmb.AddVertex(posP + glm::vec3(0, 0, zdepth));

						int i2 = (int)mapmb.GetVertexCount() - 1;

						mapmb.AddVertex(posQ + glm::vec3(0, 0, zdepth));

						int i3 = (int)mapmb.GetVertexCount() - 1;

						mapmb.AddSurface(glm::ivec3(i0, i2, i3));
						mapmb.AddSurface(glm::ivec3(i0, i3, i1));
						
					}

					// create collision mesh

					for (int i = 0; i < borderlayer.GetVerticesCount(); i += 2)
					{
						
						posP = glm::vec3(borderlayer.GetVertexXAt(i), borderlayer.GetVertexYAt(i), borderlayer.GetVertexZAt(i));

						colmb.AddVertex(posP);

						int i0 = (int)colmb.GetVertexCount() - 1;

//						vml::os::Trace("Vertex %d : %f %f %f\n", i0, posP.x, posP.y, posP.z);

						int j = i + 1;

						posQ = glm::vec3(borderlayer.GetVertexXAt(j), borderlayer.GetVertexYAt(j), borderlayer.GetVertexZAt(j));

						colmb.AddVertex(posQ);

	//					vml::os::Trace("Vertex %d : %f %f %f\n", j, posQ.x, posQ.y, posQ.z);

						int i1 = (int)colmb.GetVertexCount() - 1;

						colmb.AddVertex(posP + glm::vec3(0, 0, zdepth));

		//				vml::os::Trace("Vertex %d : %f %f %f\n", i1, posP.x, posP.y, posP.z+zdepth);

						int i2 = (int)colmb.GetVertexCount() - 1;

						colmb.AddVertex(posQ + glm::vec3(0, 0, zdepth));

		//				vml::os::Trace("Vertex %d : %f %f %f\n", i2, posQ.x, posQ.y, posQ.z + zdepth);

						int i3 = (int)colmb.GetVertexCount() - 1;

						colmb.AddSurface(glm::ivec3(i0, i2, i3));
						colmb.AddSurface(glm::ivec3(i0, i3, i1));
						
					}
					
					// create navmesh file

					// navmesh 2 

					const fa2040::tools::TriangleLayer& navlayer2 = ExternalTriangleLayer2;

					std::cout << "NavMesh Vertices : " << (int)navlayer2.GetVerticesCount() << std::endl;
					
					for (int i = 0; i < (int)navlayer2.GetVerticesCount(); ++i)
					{
						float px = navlayer2.GetVertexXAt(i);
						float py = navlayer2.GetVertexYAt(i);
						float pz = navlayer2.GetVertexZAt(i);

						glm::vec3 pos = glm::vec3(px, py, pz + navmeshdepth);

						navmb.AddVertex(pos);

				//		vml::os::Trace("Vertex %d : %f %f %f\n", i, px, py, pz);
						
					}

					std::cout << "NavMesh Surfaces : " << (int)navlayer2.GetTrianglesCount() << std::endl;

					for (int i = 0; i < (int)navlayer2.GetTrianglesCount(); ++i)
					{
						
						const fa2040::tools::GridTriangle& tri = navlayer2.GetTriangleAt(i);

						int i0 = tri.GetI0();
						int i1 = tri.GetI1();
						int i2 = tri.GetI2();

						//		vml::os::Trace("Surface %d : %d %d %d\n", i, i0, i1, i2);

						navmb.AddSurface(glm::ivec3(i0, i1, i2));
					}
										
					// compute geometric center of mass 
					// centre both navmesh and level mesh 
					// at the same center

					glm::vec3 boundingboxmin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					glm::vec3 boundingboxmax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < mapmb.GetVertexCount(); i++)
					{
						glm::vec3 pos = mapmb.GetVertexPosAt(i);

						if (pos.x <= boundingboxmin.x) boundingboxmin.x = pos.x;
						if (pos.y <= boundingboxmin.y) boundingboxmin.y = pos.y;
						if (pos.z <= boundingboxmin.z) boundingboxmin.z = pos.z;
						if (pos.x >= boundingboxmax.x) boundingboxmax.x = pos.x;
						if (pos.y >= boundingboxmax.y) boundingboxmax.y = pos.y;
						if (pos.z >= boundingboxmax.z) boundingboxmax.z = pos.z;
					}

					float scale = 0.1f;

					glm::vec3 center = (boundingboxmax + boundingboxmin) * 0.5f * scale;

					// finalize

					mapmb.Finalize( -center, glm::vec3(0, 0, 0), glm::vec3(scale, scale, scale));
					navmb.Finalize( -center, glm::vec3(0, 0, 0), glm::vec3(scale, scale, scale));
					colmb.Finalize( -center, glm::vec3(0, 0, 0), glm::vec3(scale, scale, scale));
										
					// save meshes

					mapmb.Save();
					navmb.Save();
					colmb.Save();

					// save navmesh bitmap mask
					
					SaveNavMeshMask2(NavMeshBitmapMaskFileName, -center, scale);

//					std::cout << "LevelGenerator : Saving Done" << std::endl;

					vml::utils::Logger::GetInstance()->Info("LevelGenerator : Saving Done");

				}
				
				// ---------------------------------------------------------------
				//

				void Go(const vml::textures::Texture* Texture, int navmeshscalingfactorf, int erosionfact)
				{
					if (Texture)
					{
						
						// check if texture is valid

						if (!Texture->IsValid())
							vml::os::Message::Error("LevelGenerator : ","Texture is not valid");
					
						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Initting Map Generator");

						// reset 

						ReleaseAll();
						
						// get filename

						FileName = vml::strings::SplitPath::GetTitle(Texture->GetResourceFileName());

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Bitmap filename : " + FileName);
						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Allocating luma");

						// alloctae LumaTexture

						BitmapFileName = Texture->GetResourceFileName();

						int texturew = Texture->GetWidth();
						int textureh = Texture->GetHeight();
						const unsigned char* TextureData = Texture->GetData();

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Bitmap Width : "+ std::to_string(texturew));
						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Bitmap Height : " + std::to_string(textureh));

						if (!TextureData)
							vml::os::Message::Error("LevelGenerator2d : ","Texture data is null");
						
						LumaTextureW    = texturew + 2;
						LumaTextureH    = textureh + 2;
						LumaTextureSize = LumaTextureW * LumaTextureH;
						LumaTexture     = new unsigned char[LumaTextureSize];

						NavTextureW    = LumaTextureW;
						NavTextureH    = LumaTextureH;
						NavTextureSize = NavTextureW * NavTextureH;
						NavTexture     = new unsigned char[NavTextureSize];

						// clear memory

						memset(LumaTexture, 0, LumaTextureSize*sizeof(unsigned char));
						memset(NavTexture, 0, NavTextureSize * sizeof(unsigned char));

						// create luma texture
						// read original texture , chacke if we have a valid pixel
						// a valid pixel is a non black pixel
						// if found, it is written in the LumaTexture
						// which is a 1 channell texture enlarged by 1 in all
						// direction to avoid artifcat during countouring process

						unsigned int bytePerPixel = Texture->GetBPP();

						for (unsigned int j = 0; j < textureh; ++j)
						{
							for (unsigned int i = 0; i < texturew; ++i)
							{
								unsigned int offset = (i + texturew * j) * bytePerPixel;

								unsigned char r = TextureData[offset    ];
								unsigned char g = TextureData[offset + 1];
								unsigned char b = TextureData[offset + 2];

								//	unsigned char a = bytePerPixel >= 4 ? TextureData[offset+3] : 0xff;

								if (r != 0 && g != 0 && b != 0)
								{
									// write pixel in the lumaTexture
									// at i+1 and j+1 coordinate
									// becasue LUmaTexture has a 0 boarder
									// around texture read from TextureData

									int offset = (i + 1) + (j + 1) * LumaTextureW;

									LumaTexture[ offset ] = 1;
								}
							}
						}

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Created Luma Texture");

						// create level map

						TriangleLayer.Begin();
						ExternalTriangleLayer2.Begin();
						BorderLayer.Begin();

						// polygonizing

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Polygonize TRiangle Layers");

						NavMeshScalingFactor = navmeshscalingfactorf;
						NavMeshErosionFactor = erosionfact;

						// polygonize layers

						PolygonizeLayer();

						// finalizing

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Finalizing Triangle Layers");

						TriangleLayer.Finalize();
						ExternalTriangleLayer2.Finalize();
						BorderLayer.Finalize();

						vml::utils::Logger::GetInstance()->Info("LevelGenerator : Done");

						// set compiled flag as true

						Compiled = true;
						
					}
					else
					{
						vml::os::Message::Error("LevelGenerator : ","Texture pointer is null");
					}
				}
				
				// -----------------------------------------------------------------

				const std::string& GetFileNameNoExt() const 
				{
					return FileName; 
				}

				// -----------------------------------------------------------------

				constexpr bool IsCompiled() const 
				{
					return Compiled; 
				}
				
				// -----------------------------------------------------------------
				// ctor / dtor

				Level2dGenerator()
				{
					
					LumaTexture				 = nullptr;
					NavTexture				 = nullptr;
					LumaTextureW			 = 0;
					LumaTextureH			 = 0;
					LumaTextureSize			 = 0;
					NavTextureW				 = 0;
					NavTextureH				 = 0;
					NavTextureSize			 = 0;
					NavMeshScalingFactor     = 1;				// scaling factor for the namvmesh is 1 if downscaling is not touched
					NavMeshErosionFactor     = 0;				// erosion factor for navmesh
					Compiled				 = false;

				//	std::cout << "Levelgeneratro2d ctor" << std::endl;
					
				}

				~Level2dGenerator()
				{
					vml::os::SafeDelete(LumaTexture);
					vml::os::SafeDelete(NavTexture);

					TriangleLayer.Clear();
					ExternalTriangleLayer2.Clear();
					BorderLayer.Clear();
					
				//	std::cout << "Levelgeneratro2d dtor" << std::endl;
					
				}
		};
	}
}

