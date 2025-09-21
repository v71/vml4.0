#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace shaders
	{

		////////////////////////////////////////////////////////
		// opengl shader class 2.0

		class GlShader
		{

		private:

			GLenum Type;					// type of shader , vertex or fragment
			GLuint Id;						// the shader program identifier
			std::string ResourceFileName;	// shader filename

			// -----------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed since classes
			// are referenced

			GlShader(const GlShader& shader) = delete;

			// -----------------------------------------------------------------------
			// overload operator = is private
			// no copies allowed since classes
			// are referenced

			GlShader& operator = (const GlShader& shader) = delete;

			// -----------------------------------------------------------------------
			// compile shader

			void Compile(const std::string& source)
			{

				if (source.empty())
					vml::os::Message::Error("CGlShaderProgram : ", ResourceFileName.c_str(), "shader buffer is empty");

				const char* code = reinterpret_cast<const GLchar*>(source.c_str());

				int compiled;

				glShaderSource(Id, 1, &code, NULL);
				glCompileShader(Id);
				glGetShaderiv(Id, GL_COMPILE_STATUS, &compiled);

				if (!compiled)
				{
					vml::os::Message::Error("CGlShaderProgram : ", ResourceFileName.c_str(), "cannot compile file ", GetInfoLog().c_str());
					glDeleteShader(Id);
				}

			}

		public:

			// -----------------------------------------------------------------------
			// get info log for this shader

			const std::string GetInfoLog() const
			{
				GLsizei infoLogSize = 0;
				std::string infoLog;
				glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &infoLogSize);
				infoLog.resize(infoLogSize);
				glGetShaderInfoLog(Id, infoLogSize, &infoLogSize, &infoLog[0]);
				return infoLog;
			}

			// -----------------------------------------------------------------------
			// load shader program,no args needed

			bool LoadShader(const std::string& filename)
			{
				if (filename.empty())
					vml::os::Message::Error("CGlShaderProgram : ", "filename is empty");

				std::ifstream file(filename.c_str(), std::ios::binary);

				if (!file.is_open())
					return false;

				// load source file

				std::string source;
				file.seekg(0, std::ios::end);
				unsigned int fileSize = static_cast<unsigned int>(file.tellg());
				source.resize(fileSize);
				file.seekg(0, std::ios::beg);
				file.read(&source[0], fileSize);
				file.close();

				// compile

				Compile(source);

				return true;
			}

			// -----------------------------------------------------------------------
			// getters functions

			GLuint GetID() const
			{
				return Id;
			}

			// -----------------------------------------------------------------------
			// gets shader type , fragment or vertex shader

			const std::string GetTypeString() const
			{
				switch (Type)
				{
				case GL_VERTEX_SHADER: return "Vertex Shader";
				case GL_FRAGMENT_SHADER: return "Pixel Shader";
				case GL_GEOMETRY_SHADER: return "Geometry Shader";
				}

				return "type not recognized";
			}

			// -----------------------------------------------------------------------
			// gets filename

			const std::string& GetResourceFileName() const
			{
				return ResourceFileName;
			}

			// -----------------------------------------------------------------------
			// ctor / dtor

			GlShader(GLenum shaderType)
			{
				Type = shaderType;
				Id = glCreateShader(shaderType);
			}

			~GlShader()
			{
				glDeleteShader(Id);
			}

		};

		////////////////////////////////////////////////////////////
		// shader resource manager node class

		class GlShaderProgram
		{

		private:

			// -----------------------------------------------------------------------

			GLuint		Id;											// shader id
			GLint		ViewMatrixLocation;							// view matrix location
			GLint		NormalMatrixLocation;						// normal matrix location
			GLint		ProjectionMatrixLocation;					// projection matrix location
			GLint		ModelMatrixLocation;						// model matrix location
			GLint		ModelViewMatrixLocation;					// model * view matrix location
			GLint		ModelViewProjectionMatrixLocation;			// model * view * projection matrix location
			GLint		TextureMatrixLocation;						// texture matrix
			std::string ResourceFileName;

		public:

			// -----------------------------------------------------------------------
			// Attributes layouts.

			static const int ATTRIBUTE_POSITION = 0;
			static const int ATTRIBUTE_NORMAL = 1;
			static const int ATTRIBUTE_COLOR = 2;
			static const int ATTRIBUTE_TEXCOORD = 3;
			static const int ATTRIBUTE_TANGENT = 4;
			static const int ATTRIBUTE_BI_TANGENT = 5;

			// -----------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed since classes
			// are referenced

			GlShaderProgram(const GlShaderProgram& shaderprogram) = delete;

			// -----------------------------------------------------------------------
			// overload operator = is private
			// no copies allowed since classes
			// are referenced

			GlShaderProgram& operator = (const GlShaderProgram& shaderprogram) = delete;

			// -----------------------------------------------------------------------
			// use this glprogram

			void Use() const { glUseProgram(Id); }

			// -----------------------------------------------------------------------
			// unuse this glprogram

			void UnUse() const { glUseProgram(0); }

			// -----------------------------------------------------------------------
			// getters

			// -----------------------------------------------------------------------
			// get resource identification parms

			GLuint GetID() const { return Id; }

			// -----------------------------------------------------------------------
			// get transformation locations

			GLint GetViewMatrixLocation()				 const { return ViewMatrixLocation; }
			GLint GetModelMatrixLocation()				 const { return ModelMatrixLocation; }
			GLint GetProjectionMatrixLocation()			 const { return ProjectionMatrixLocation; }
			GLint GetNormalMatrixLocation()				 const { return NormalMatrixLocation; }
			GLint GetModelViewMatrixLocation()			 const { return ModelViewMatrixLocation; }
			GLint GetModelViewProjectionMatrixLocation() const { return ModelViewProjectionMatrixLocation; }
			GLint GetTextureMatrixLocation()			 const { return TextureMatrixLocation; }

			// -----------------------------------------------------------------------
			// utility uniform functions

			void SetBoolUniform(const std::string& name, bool value)			 const { glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value); }
			void SetIntUniform(const std::string& name, int value)				 const { glUniform1i(glGetUniformLocation(Id, name.c_str()), value); }
			void SetFloatUniform(const std::string& name, float value)			 const { glUniform1f(glGetUniformLocation(Id, name.c_str()), value); }
			void SetVec2Uniform(const std::string& name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
			void SetVec2Uniform(const std::string& name, float x, float y)		 const { glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y); }

			void SetVec3Uniform(const std::string& name, const glm::vec3& value)			 const { glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
			void SetVec3Uniform(const std::string& name, float x, float y, float z)			 const { glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z); }
			void SetVec4Uniform(const std::string& name, const glm::vec4& value)			 const { glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
			void SetVec4Uniform(const std::string& name, float x, float y, float z, float w) const { glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w); }
			void SetMat2Uniform(const std::string& name, const glm::mat2& mat)				 const { glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
			void SetMat3Uniform(const std::string& name, const glm::mat3& mat)				 const { glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
			void SetMat4Uniform(const std::string& name, const glm::mat4& mat)				 const { glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

			// -----------------------------------------------------------------------
			// ctor / dtor
			// no extrapack data for shaders

			GlShaderProgram(const std::string& shaderfilename)
			{

				ResourceFileName = shaderfilename;

				// uses raii paradigm to load shader resource
				// ResourceFileName is initted in base class

				Id = 0;

				// mandatory locations

				ViewMatrixLocation = 0;
				NormalMatrixLocation = 0;
				ProjectionMatrixLocation = 0;
				ModelViewMatrixLocation = 0;
				ModelMatrixLocation = 0;
				ModelViewProjectionMatrixLocation = 0;

				// check extension

				if (!ResourceFileName.ends_with(".shd"))
					vml::os::Message::Error("GlProgram :", ResourceFileName.c_str(), "bad extension");

				// log out 

			//	vml::utils::Logger::GetInstance()->Info("Shader", "Loading : " + ResourceFileName);

				// remove extension substring

				std::string noextfilename = vml::strings::SplitPath::RemoveExtensionFromPath(ResourceFileName);

				// path for vertex and fragment shaders

				std::string vertexshadername = noextfilename + ".vert";
				std::string fragmentshadername = noextfilename + ".frag";

				// initialize and create shaders

				GlShader* shVertex = new GlShader(GL_VERTEX_SHADER);
				GlShader* shFragment = new GlShader(GL_FRAGMENT_SHADER);

				// load shaders from file path

				if (!shVertex->LoadShader(vertexshadername))
					vml::os::Message::Error("CGlShaderProgram : ", vertexshadername.c_str(), " : ", ResourceFileName.c_str(), " cannot open file");

				if (!shFragment->LoadShader(fragmentshadername))
					vml::os::Message::Error("CGlShaderProgram : ", fragmentshadername.c_str(), " : ", ResourceFileName.c_str(), " cannot open file");

				// geometry shader is not mandatory , but we look
				// for its presence and if found will be loaded as well

				std::string geometryshadername = noextfilename + ".geom";

				// once shaders are loaded , attach them to the shader program 

				Id = glCreateProgram();

				if (Id == 0)
					vml::os::Message::Error("GlProgram : ", "error creating gl program");

				glAttachShader(Id, shVertex->GetID());
				glAttachShader(Id, shFragment->GetID());

				GlShader* shGeometry = new GlShader(GL_GEOMETRY_SHADER);

				bool hasgeometryshader = shGeometry->LoadShader(geometryshadername);

				if (hasgeometryshader)
					glAttachShader(Id, shGeometry->GetID());

				// link 

				int linked;

				glLinkProgram(Id);

				glGetProgramiv(Id, GL_LINK_STATUS, &linked);

				if (!linked)
				{
					glDeleteProgram(Id);

					GLsizei infoLogSize = 0;
					std::string infoLog;
					glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &infoLogSize);
					infoLog.resize(infoLogSize);
					glGetProgramInfoLog(Id, infoLogSize, &infoLogSize, &infoLog[0]);

					vml::os::Message::Error("GlProgram : ", "Program linking error ", infoLog.c_str());
				}

				// detach shaders after linking

				glDetachShader(Id, shVertex->GetID());
				glDetachShader(Id, shFragment->GetID());

				// check if a geometry shader is present

				if (hasgeometryshader)
					glDetachShader(Id, shGeometry->GetID());

				// delete shaders

				vml::os::SafeDelete(shVertex);
				vml::os::SafeDelete(shFragment);
				vml::os::SafeDelete(shGeometry);

				// get uniform locations

				glUseProgram(Id);

				NormalMatrixLocation = glGetUniformLocation(Id, "NormalMatrix");
				ViewMatrixLocation = glGetUniformLocation(Id, "ViewMatrix");
				ModelMatrixLocation = glGetUniformLocation(Id, "ModelMatrix");
				ModelViewMatrixLocation = glGetUniformLocation(Id, "ModelViewMatrix");
				ProjectionMatrixLocation = glGetUniformLocation(Id, "ProjectionMatrix");
				ModelViewProjectionMatrixLocation = glGetUniformLocation(Id, "ModelViewProjectionMatrix");
				TextureMatrixLocation = glGetUniformLocation(Id, "TextureMatrix");

				//  unuse this shader once it is loaded

				glUseProgram(0);

			}

			~GlShaderProgram()
			{
				glDeleteProgram(Id);
				glUseProgram(0);
				//	vml::utils::Logger::GetInstance()->Info("Shader", "Deleting : " + ResourceFileName);
			}

		};

	}	// end of shaders namespace

}	// end of vml namespace

