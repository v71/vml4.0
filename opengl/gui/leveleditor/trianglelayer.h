#pragma once

namespace fa2040
{
	namespace tools
	{

		class TriangleLayer
		{

			private:
				
				bool					   Initted;
				std::vector<float>		   VertexArray;
				std::vector<GLuint>		   IndicesArray;
				std::vector<GridTriangle>  TrianglesArray;
				glm::vec3				   BoundingBoxMin;
				glm::vec3				   BoundingBoxMax;
				glm::vec3				   BoundingBoxExtents;
				
				// triangle layer renderer

				fa2040::tools::TriangleLayerRender		  TriangleLayerRenderer;
				fa2040::tools::TriangleLayerRenderIndexed TriangleLayerRendererIndexed;
				int										  CurrentTriangleId;
				
				// ---------------------------------------------------------------
				//

				void CreateTriangleIndices()
				{

					IndicesArray.clear();

					for (size_t i = 0; i < TrianglesArray.size(); ++i)
					{
						IndicesArray.emplace_back(TrianglesArray[i].GetI0());
						IndicesArray.emplace_back(TrianglesArray[i].GetI1());
						IndicesArray.emplace_back(TrianglesArray[i].GetI2());
					}
				}
				
				// ---------------------------------------------------------------
				//

				void ComputeBoundingBox()
				{
					
					// compute bounding box

					BoundingBoxMin = glm::vec3( FLT_MAX,  FLT_MAX,  FLT_MAX);
					BoundingBoxMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < VertexArray.size() / 4; i++)
					{
						uint64_t offset = (uint64_t)(i * 4);

						float px = VertexArray[offset    ];
						float py = VertexArray[offset + 1];
						float pz = VertexArray[offset + 2];

						if (px <= BoundingBoxMin.x) BoundingBoxMin.x = px;
						if (py <= BoundingBoxMin.y) BoundingBoxMin.y = py;
						if (pz <= BoundingBoxMin.z) BoundingBoxMin.z = pz;
						if (px >= BoundingBoxMax.x) BoundingBoxMax.x = px;
						if (py >= BoundingBoxMax.y) BoundingBoxMax.y = py;
						if (pz >= BoundingBoxMax.z) BoundingBoxMax.z = pz;

					}

					// compute extents

					BoundingBoxExtents = BoundingBoxMax - BoundingBoxMin;
					
					// center bounding box

					glm::vec3 center = (BoundingBoxMax + BoundingBoxMin) * 0.5f;

					BoundingBoxMin -= center;
					BoundingBoxMax -= center;

			
				}
				
			public:
				
				// -----------------------------------------------------------------
				//

				void Begin()
				{
					VertexArray.clear();
					IndicesArray.clear();
					TrianglesArray.clear();

					Initted = false;

					// clear triangle layer renderer

					TriangleLayerRenderer.Clear();
					TriangleLayerRendererIndexed.Clear();

				}
				
				// --------------------------------------------------
				//

				void Clear()
				{
					VertexArray.clear();
					IndicesArray.clear();
					TrianglesArray.clear();

					Initted = false;

					// clear triangle layer renderer

					TriangleLayerRenderer.Clear();
					TriangleLayerRendererIndexed.Clear();

				}
				
				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col)
				{
					if (!Initted)
						return;

					TriangleLayerRenderer.Draw(MVP, sx, sy, col);
				}

				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col, const std::vector<GLuint> indices)
				{
					if (!Initted)
						return;

					std::vector<GLuint> triindices;

					for (size_t i = 0; i < indices.size(); i++)
					{
						int j = indices[i];

						triindices.emplace_back(TrianglesArray[j].I0);
						triindices.emplace_back(TrianglesArray[j].I1);
						triindices.emplace_back(TrianglesArray[j].I2);
					}

					TriangleLayerRendererIndexed.Draw(MVP, sx, sy, col, triindices, 1);
				}

				// -----------------------------------------------------------------
				//

				void DrawSingleTriangle(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col)
				{
					if (!Initted)
						return;

					std::vector<GLuint> triindices;
					GridTriangle& tri = TrianglesArray[CurrentTriangleId];

					triindices.emplace_back(tri.I0);
					triindices.emplace_back(tri.I1);
					triindices.emplace_back(tri.I2);

					TriangleLayerRendererIndexed.Draw(MVP, sx, sy, col, triindices, 1);

					triindices.clear();

					for (size_t i = 0; i < tri.Links.size(); ++i)
					{
						GridTriangle* stri = tri.Links[i];

						triindices.emplace_back(stri->I0);
						triindices.emplace_back(stri->I1);
						triindices.emplace_back(stri->I2);

						TriangleLayerRendererIndexed.Draw(MVP, sx, sy, vml::colors::Red, triindices, 1);
					}

				}
				
				// ---------------------------------------------------------------
				//

				void Finalize()
				{
					Initted = true;
		
					// compute bouding box

					ComputeBoundingBox();

					// create triangle indices for rendering

					CreateTriangleIndices();

					// finalize triangle layers renderers

					TriangleLayerRenderer.Finalize(VertexArray, IndicesArray);
					TriangleLayerRendererIndexed.Finalize(VertexArray, IndicesArray);

			//		std::cout << "Created indices " << IndicesArray.size() << std::endl;

				}
							
				// --------------------------------------------------
				//

				void AddTriangle(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, int type)
				{

					// add a vertex coordinate 

					VertexArray.emplace_back(p0.x);
					VertexArray.emplace_back(p0.y);
					VertexArray.emplace_back(0);
					VertexArray.emplace_back(1);

					// compute index 

					int i0 = ((int)VertexArray.size() - 4) / 4;

					// add a vertex coordinate 

					VertexArray.emplace_back(p1.x);
					VertexArray.emplace_back(p1.y);
					VertexArray.emplace_back(0);
					VertexArray.emplace_back(1);

					// compute index 

					int i1 = ((int)VertexArray.size() - 4) / 4;

					// add a vertex coordinate 

					VertexArray.emplace_back(p2.x);
					VertexArray.emplace_back(p2.y);
					VertexArray.emplace_back(0);
					VertexArray.emplace_back(1);

					// compute index 

					int i2 = ((int)VertexArray.size() - 4) / 4;

					// add triangle

					GridTriangle tri((int)TrianglesArray.size(), i0, i1, i2, p0, p1, p2, type);

					TrianglesArray.emplace_back(tri);

				}
				
				// -----------------------------------------------------------------

				float GetVertexXAt(int i) const { return VertexArray[(uint64_t)(i * 4    )]; }
				float GetVertexYAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 1)]; }
				float GetVertexZAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 2)]; }
				float GetVertexWAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 3)]; }
				
				const glm::vec3& GetBoundingBoxMin() const { return BoundingBoxMin; }
				const glm::vec3& GetBoundingBoxMax() const { return BoundingBoxMax; }
				const glm::vec3& GetBoundingBoxExtents() const { return BoundingBoxExtents; }

				const std::vector<float>& GetVertexArray() const { return VertexArray; }
				const std::vector<GLuint> &GetIndicesArray() const { return IndicesArray; }
				std::vector<GridTriangle>& GetTriangleList() { return TrianglesArray; }
				const GridTriangle& GetTriangleAt(int i) const { return TrianglesArray[i]; }

				size_t GetVerticesCount() const { return VertexArray.size() / 4; }
				size_t GetIndicesCount() const { return IndicesArray.size(); }
				GLuint GetIndexAt(int i) const { return IndicesArray[i]; }

				size_t GetTrianglesCount() const { return TrianglesArray.size(); }
				int GetCurrentTriangle() const { return CurrentTriangleId; }

				void GetNextTriangle()
				{
					CurrentTriangleId++;
					size_t n = TrianglesArray.size() - 1;
					if (CurrentTriangleId > n) CurrentTriangleId = (int)n;
				}

				void GetPrevTriangle()
				{
					CurrentTriangleId--;
					if (CurrentTriangleId < 0) CurrentTriangleId = 0;
				}
				
				// -----------------------------------------------------------------
				// ctor / dtor

				TriangleLayer()
				{
					Initted            = false;
					CurrentTriangleId  = 0;
					BoundingBoxMin     = glm::vec3(0, 0, 0);
					BoundingBoxMax     = glm::vec3(0, 0, 0);
					BoundingBoxExtents = glm::vec3(0, 0, 0);
				}

				TriangleLayer(const TriangleLayer&) = default;
				TriangleLayer& operator=(const TriangleLayer&) = default;

				~TriangleLayer()
				{
				}

		};
	}
}
