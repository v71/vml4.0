#pragma once

namespace fa2040
{
	namespace tools
	{

		class TriangleLayerRender
		{
			private:
				
				GLuint						   Vertices;
				GLuint						   Indices;
				GLuint						   VAOid;								// vertex array object id
				GLuint						   BufferObjects[2];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ColorLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;
				
				const std::string FullDebugPath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
				const std::string SingleColorShaderFilename = FullDebugPath + "/shaders/debug_single_color.shd";

			public:
				
				// -----------------------------------------------------------------

				bool WireMode;				// enables wire or filled drawing mode

				// --------------------------------------------------

				void Clear()
				{
					Vertices = 0;
					Indices = 0;
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
				}
				
				// ---------------------------------------------------------------

				void Finalize(const std::vector<float>& srcVertexArray, const std::vector<GLuint>& srcIndicesArray)
				{
					
					if (srcIndicesArray.size() == 0)
						return;
					if (srcVertexArray.size() == 0)
						return;

					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					IndexBufferObject = 0;

					Vertices = (int)srcVertexArray.size();
					Indices = (int)srcIndicesArray.size();

					if (Vertices == 0 || Indices == 0 )
						vml::os::Message::Error("trianglelayerrenderer : ", "Vertices or Indices buffers are empity");

					// Create the VAO for axis aligned bounding box

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, Vertices * sizeof(float), &srcVertexArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &srcIndicesArray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					
				}

				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col) const
				{
					
					glm::mat4 m(0);

					float* mptr = glm::value_ptr(m);

					float cx =  sx + 1;
					float cy =  sy + 1;
					float tx =  2 / cx;
					float ty = -2 / cy;

					// scale parent matrix

					mptr[ 0] =  2 / sx;
					mptr[ 1] =  0;
					mptr[ 2] =  0;
					mptr[ 3] =  0;
					mptr[ 4] =  0;
					mptr[ 5] = -2 / sy;
					mptr[ 6] =  0;
					mptr[ 7] =  0;
					mptr[ 8] =  0;
					mptr[ 9] =  0;
					mptr[10] =  1;
					mptr[11] =  0;
					mptr[12] = -1 - tx;
					mptr[13] =  1 - ty;
					mptr[14] =  0;
					mptr[15] =  1;

					glm::mat4 mvp = MVP * m;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
					glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

					glBindVertexArray(VAOid);

					glDisable(GL_DEPTH);
					glDisable(GL_CULL_FACE);
					if (WireMode)
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					else
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH);

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
					
				}
				
				// -----------------------------------------------------------------
				// ctor / dtor

				TriangleLayerRender()
				{
					
					VAOid							  =  0;
					BufferObjects[0]				  =  0;
					BufferObjects[1]				  =  0;
					IndexBufferObject				  =  0;
					Vertices				          =  0;
					Indices						      =  0;
					Shader						      =  nullptr;
					ShaderProgram					  =  0;
					ColorLocation				      =  0;
					ModelViewProjectionMatrixLocation =  0;
					WireMode                          =  1;

					// attach shader

					Shader = new vml::shaders::GlShaderProgram(SingleColorShaderFilename);

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
					if (ColorLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'Color' uniform, check shader source code");
					if (ModelViewProjectionMatrixLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'ModelViewProjectionMatrix' uniform, check shader source code");
				}

				TriangleLayerRender(const TriangleLayerRender&) = default;
				TriangleLayerRender& operator=(const TriangleLayerRender&) = default;

				~TriangleLayerRender()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::os::SafeDelete(Shader);
				}

		};
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fa2040
{
	namespace tools
	{

		class TriangleLayerRenderIndexed
		{
			private:
				
				GLuint						   Vertices;
				GLuint						   Indices;
				GLuint						   VAOid;								// vertex array object id
				GLuint						   BufferObjects[2];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ColorLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;

				const std::string FullDebugPath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
				const std::string SingleColorShaderFilename = FullDebugPath + "/shaders/debug_single_color.shd";

			public:
				
				// -----------------------------------------------------------------

				bool WireMode;				// enables wire or filled drawing mode

				// --------------------------------------------------

				void Clear()
				{
					Vertices = 0;
					Indices = 0;
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
				}

				// ---------------------------------------------------------------

				void Finalize(const std::vector<float>& srcVertexArray, const std::vector<GLuint>& srcIndicesArray)
				{
					if (srcIndicesArray.size() == 0)
						return;
					if (srcVertexArray.size() == 0)
						return;

					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					IndexBufferObject = 0;

					Vertices = (int)srcVertexArray.size();
					Indices = (int)srcIndicesArray.size();

					if (Vertices == 0 || Indices == 0)
						vml::os::Message::Error("trianglelayerrenderer : ", "Vertices or Indices buffers are empity");

					// Create the VAO for axis aligned bounding box

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, Vertices * sizeof(float), &srcVertexArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &srcIndicesArray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col, const std::vector<GLuint>& srcIndicesArray, bool fill) const
				{
			
					glm::mat4 m(0);

					float* mptr = glm::value_ptr(m);

					float cx = sx + 1;
					float cy = sy + 1;
					float tx = 2 / cx;
					float ty = -2 / cy;

					// scale parent matrix

					mptr[0] = 2 / sx;
					mptr[1] = 0;
					mptr[2] = 0;
					mptr[3] = 0;
					mptr[4] = 0;
					mptr[5] = -2 / sy;
					mptr[6] = 0;
					mptr[7] = 0;
					mptr[8] = 0;
					mptr[9] = 0;
					mptr[10] = 1;
					mptr[11] = 0;
					mptr[12] = -1 - tx;
					mptr[13] = 1 - ty;
					mptr[14] = 0;
					mptr[15] = 1;

					glm::mat4 mvp = MVP * m;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
					glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

					glBindVertexArray(VAOid);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, srcIndicesArray.size() * sizeof(GLuint), &srcIndicesArray[0], GL_STATIC_DRAW);

					glDisable(GL_DEPTH);
					glDisable(GL_CULL_FACE);
					if (!fill)
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					else
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, (GLsizei)srcIndicesArray.size(), GL_UNSIGNED_INT, 0);
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH);

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}
				
				// -----------------------------------------------------------------
				// ctor / dtor

				TriangleLayerRenderIndexed()
				{
					
					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					IndexBufferObject = 0;
					Vertices = 0;
					Indices = 0;
					Shader = nullptr;
					ShaderProgram = 0;
					ColorLocation = 0;
					ModelViewProjectionMatrixLocation = 0;
					WireMode = 1;

					// attach shader

					Shader = new vml::shaders::GlShaderProgram(SingleColorShaderFilename);

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
					if (ColorLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'Color' uniform, check shader source code");
					if (ModelViewProjectionMatrixLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'ModelViewProjectionMatrix' uniform, check shader source code");
				}

				TriangleLayerRenderIndexed(const TriangleLayerRenderIndexed&) = default;
				TriangleLayerRenderIndexed& operator=(const TriangleLayerRenderIndexed&) = default;

				~TriangleLayerRenderIndexed()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::os::SafeDelete(Shader);
				}

		};
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace fa2040
{
	namespace tools
	{

		///////////////////////////////////////////////////////////////////////////////////
		// border layer 

		class BorderLayerRender
		{
			private:
				
				GLuint						   Vertices;
				GLuint						   Indices;
				GLuint						   VAOid;								// vertex array object id
				GLuint						   BufferObjects[2];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ColorLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;
		
				const std::string FullDebugPath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
				const std::string SingleColorShaderFilename = FullDebugPath + "/shaders/debug_single_color.shd";

			public:
				
				// -----------------------------------------------------------------

				void Clear()
				{
					Vertices = 0;
					Indices = 0;
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
				}

				// ---------------------------------------------------------------

				void Finalize(const std::vector<float>& srcVertexArray, const std::vector<GLuint>& srcIndicesArray)
				{
					if (srcIndicesArray.size() == 0)
						return;
					if (srcVertexArray.size() == 0)
						return;

					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					IndexBufferObject = 0;

					Vertices = (int)srcVertexArray.size();
					Indices = (int)srcIndicesArray.size();

					if (Vertices == 0 || Indices == 0)
						vml::os::Message::Error("trianglelayerrenderer : ","Vertices or Indices buffers are empity");

					// Create the VAO for axis aligned bounding box

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, Vertices * sizeof(float), &srcVertexArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &srcIndicesArray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				// -----------------------------------------------------------------
				//

				void Draw(const glm::mat4& MVP, const float sx, const float sy, const glm::vec4& col) const
				{
					glm::mat4 m(0);

					float* mptr = glm::value_ptr(m);

					float cx = sx + 1;
					float cy = sy + 1;
					float tx = 2 / cx;
					float ty = -2 / cy;

					// scale parent matrix

					mptr[0] = 2 / sx;
					mptr[1] = 0;
					mptr[2] = 0;
					mptr[3] = 0;
					mptr[4] = 0;
					mptr[5] = -2 / sy;
					mptr[6] = 0;
					mptr[7] = 0;
					mptr[8] = 0;
					mptr[9] = 0;
					mptr[10] = 1;
					mptr[11] = 0;
					mptr[12] = -1 - tx;
					mptr[13] = 1 - ty;
					mptr[14] = 0;
					mptr[15] = 1;

					glm::mat4 mvp = MVP * m;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
					glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

					glBindVertexArray(VAOid);

					glDisable(GL_DEPTH);
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//	glPointSize(5.0f);
				//	glDrawElements(GL_POINTS, Indices, GL_UNSIGNED_INT, 0);

					glDrawElements(GL_LINES, Indices, GL_UNSIGNED_INT, 0);

					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH);

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}
				
				// -----------------------------------------------------------------
				// ctor / dtor

				BorderLayerRender()
				{
					
					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					IndexBufferObject = 0;
					Vertices = 0;
					Indices = 0;
					Shader = nullptr;
					ShaderProgram = 0;
					ColorLocation = 0;
					ModelViewProjectionMatrixLocation = 0;

					// attach shader

					Shader = new vml::shaders::GlShaderProgram(SingleColorShaderFilename);

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
					if (ColorLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'Color' uniform, check shader source code");
					if (ModelViewProjectionMatrixLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'ModelViewProjectionMatrix' uniform, check shader source code");
				}

				BorderLayerRender(const BorderLayerRender&) = default;
				BorderLayerRender& operator=(const BorderLayerRender&) = default;

				~BorderLayerRender()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::os::SafeDelete(Shader);
				}

		};

	}
}
