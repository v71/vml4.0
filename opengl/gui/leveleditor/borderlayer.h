#pragma once

namespace fa2040
{
	namespace tools
	{

		///////////////////////////////////////////////////////////////////////////////////
		// border layer 

		class BorderLayer
		{
			private:
				
				bool					   Initted;
				std::vector<float>		   VertexArray;
				std::vector<GLuint>		   IndicesArray;
				glm::vec3				   BoundingBoxMin;
				glm::vec3				   BoundingBoxMax;
				glm::vec3				   BoundingBoxExtents;

				fa2040::tools::BorderLayerRender BorderLayerRenderer;

				// ---------------------------------------------------------------
				//

				void ComputeBoundingBox()
				{
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

				void Begin()
				{
					VertexArray.clear();
					IndicesArray.clear();
					Initted = false;
				}

				// -----------------------------------------------------------------

				void Clear()
				{
					VertexArray.clear();
					IndicesArray.clear();

					Initted = false;

					// clear triangle layer renderer

					BorderLayerRenderer.Clear();

				}

				// ---------------------------------------------------------------

				void Finalize()
				{
					Initted = true;

					ComputeBoundingBox();

					// finalize triangle layers renderers

					BorderLayerRenderer.Finalize(VertexArray, IndicesArray);

				//	std::cout << "Created indices " << IndicesArray.size() << std::endl;

				}

				// ---------------------------------------------------------------

				void AddBorderLayerLine(const glm::vec2& c0, const glm::vec2& c1)
				{
					
					// add a vertex coordinate 

					VertexArray.emplace_back(c0.x);
					VertexArray.emplace_back(c0.y);
					VertexArray.emplace_back(0);
					VertexArray.emplace_back(1);

					// compute index 

					GLuint i0 = ((GLuint)VertexArray.size() - 4) / 4;

					// add a vertex coordinate 

					VertexArray.emplace_back(c1.x);
					VertexArray.emplace_back(c1.y);
					VertexArray.emplace_back(0);
					VertexArray.emplace_back(1);

					// compute index 

					GLuint i1 = ((GLuint)VertexArray.size() - 4) / 4;

					// add border

					IndicesArray.emplace_back(i0);
					IndicesArray.emplace_back(i1);
					
				}

				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col)
				{
					if (!Initted)
						return;

					BorderLayerRenderer.Draw(MVP, sx, sy, col);
				}

				// -----------------------------------------------------------------

				int GetVerticesCount() const { return (int)(VertexArray.size() / 4); }
				float GetVertexXAt(int i) const { return VertexArray[(uint64_t)(i * 4)]; }
				float GetVertexYAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 1)]; }
				float GetVertexZAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 2)]; }
				float GetVertexWAt(int i) const { return VertexArray[(uint64_t)(i * 4 + 3)]; }
				const std::vector<float> GetVertexArray() const { return VertexArray; }
				const glm::vec3& GetBoundingBoxMin() const { return BoundingBoxMin; }
				const glm::vec3& GetBoundingBoxMax() const { return BoundingBoxMax; }
				const glm::vec3& GetBoundingBoxExtents() const { return BoundingBoxExtents; }
				int GetIndicesCount() const { return (int)IndicesArray.size(); }
				const std::vector<GLuint> GetIndicesArray() const { return IndicesArray; }
				int GetEdgesCount() const { return (int)IndicesArray.size() / 2; }
				int GetI0(int i) const { return IndicesArray[(uint64_t)i * 2    ]; }
				int GetI1(int i) const { return IndicesArray[(uint64_t)i * 2 + 1]; }
				
				// -----------------------------------------------------------------
				// ctor / dtor

				BorderLayer()
				{
					
					Initted = false;
				
					BoundingBoxMin     = glm::vec3(0, 0, 0);
					BoundingBoxMax     = glm::vec3(0, 0, 0);
					BoundingBoxExtents = glm::vec3(0, 0, 0);
					
				}

				~BorderLayer()
				{
				}

		};

	}
}
