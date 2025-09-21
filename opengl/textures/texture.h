#pragma once

namespace vml
{
	namespace textures
	{

		////////////////////////////////////////////////////////////////
		// Texture class asset
		
		class Texture : public vml::utils::SharedResource
		{
			
			private:
				
				int			   Width;				// Texture width
				int			   Height;				// Texture height
				GLuint		   ID;					// Texture Id
				GLint		   BPP;					// Texture bytes per pixel
				GLenum		   TextureType;			// Texture Type ( GL_TEXTUTRE_2D,... )
				GLuint		   MipMapsGenerated;	// Texture MipMaps generated ?
				GLuint		   Minification;		// Texture Minification parameter
				GLuint		   Magnification;		// Texture Magnification parameter
				GLuint		   RepeatS;				// Texture repeat S direction flag
				GLuint		   RepeatT;				// Texture repeat T direction flag
				GLenum		   Format;				// Textrure format
				GLuint		   GammaCorrection;		// Gamma correction
				GLenum		   DataFormat;			// Texture data format
				GLuint		   Resident;			// check if texture is residnet in gpu memory
				GLfloat		   Anisotropy;			// anisotropy
				GLuint		   ReleaseTextureData;	// rlease data for this texture if set to 1 the data will be deleted after being uploaded to the gpu
				float		   Ratio;				// image ratio
				unsigned char* Data;				// actual image bytes
				
				// --------------------------------------------------------------------------
				// create texture according to specified parameters

				void CreateTexture2dFromData()
				{
					// set parameters for this texture
					
					Width         = 0;
					Height        = 0;
					BPP           = 0;
				
					// load image trhouhg stb

					stbi_set_flip_vertically_on_load(true);

					// fill data

					Data = stbi_load(ResourceFileName.c_str(), &Width, &Height, &BPP, 0);

					//vml::utils::Logger::GetInstance()->Info("Texture", "Creating : " + ResourceFileName);

					if (Data)
					{
						// compute aspect ratio

						Ratio = (float)Width / (float)Height;

						// bytes per pixel

						if (BPP != 1 &&	BPP != 3 &&	BPP != 4)
								vml::os::Message::Error("Texture : ","Invalid Bpp for texture ' ",ResourceFileName.c_str(), " ' is out of range");

						// repeat u parameter

						if (RepeatS != TEXTURE_REPEAT_S &&
							RepeatS != TEXTURE_MIRRORED_S &&
							RepeatS != TEXTURE_CLAMP_TO_EDGE_S &&
							RepeatS != TEXTURE_CLAMP_TO_BORDER_S)
								vml::os::Message::Error("Texture : ", "Invalid RepeatU for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						// repeat v parameter

						if (RepeatT != TEXTURE_REPEAT_T &&
							RepeatT != TEXTURE_MIRRORED_T &&
							RepeatT != TEXTURE_CLAMP_TO_EDGE_T &&
							RepeatT != TEXTURE_CLAMP_TO_BORDER_T)
								vml::os::Message::Error("Texture : ", "Invalid RepeatV for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						// magnification filter

						if (Magnification != TEXTURE_FILTER_MAG_NEAREST &&
							Magnification != TEXTURE_FILTER_MAG_LINEAR)
								vml::os::Message::Error("Texture : ", "Invalid MipMap Mag for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						// minificationfilter

						if (Minification != TEXTURE_FILTER_MIN_NEAREST &&
							Minification != TEXTURE_FILTER_MIN_LINEAR &&
							Minification != TEXTURE_FILTER_MIN_NEAREST_MIPMAP &&
							Minification != TEXTURE_FILTER_MIN_LINEAR_MIPMAP &&
							Minification != TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR)
								vml::os::Message::Error("Texture : ", "Invalid MipMap Min for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						// do we need to create mipmaps 

						if (MipMapsGenerated != TEXTURE_MIPMAP_TRUE &&
							MipMapsGenerated != TEXTURE_MIPMAP_FALSE)
								vml::os::Message::Error("Texture : ", "Invalid GenerateMipMap flag for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						if (Resident != TEXTURE_RESIDENT_TRUE &&
							Resident != TEXTURE_RESIDENT_FALSE)
								vml::os::Message::Error("Texture : ", "Invalid Resident flag for texture ' ", ResourceFileName.c_str(), " ' is out of range");

						// byte per pixel

						if (BPP == 1)
						{
							DataFormat = TEXTURE_RED;
							Format = DataFormat = TEXTURE_RED;
						}

						if (BPP == 3)
						{
							DataFormat = TEXTURE_RGB;
							Format = GammaCorrection ? TEXTURE_SRGB : TEXTURE_RGB;
						}

						if (BPP == 4)
						{
							DataFormat = TEXTURE_RGBA;
							Format = GammaCorrection ? TEXTURE_SRGB_ALPHA : TEXTURE_RGBA;
						}

						// generate texture

						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
						glGenTextures(1, &ID);
						glBindTexture(GL_TEXTURE_2D, ID);

						// sets max anisotropy

						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Anisotropy);

						// generate texture

						glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, DataFormat, GL_UNSIGNED_BYTE, Data);

						// generate mipmaps

						if (MipMapsGenerated == TEXTURE_MIPMAP_TRUE)
						{
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

							// create 2d mipmaps for the minifying function

							glGenerateMipmap(GL_TEXTURE_2D);

						}

						// checks if texture is resident

						if (Resident == TEXTURE_RESIDENT_TRUE)
						{
							GLboolean QueryList[1];
							GLuint TextureNames[] = { ID };
							glAreTexturesResident(1, TextureNames, QueryList);
							GLboolean QueryResidence = QueryList[0];
							if (QueryResidence == GL_TRUE) Resident = TEXTURE_RESIDENT_TRUE;
							else Resident = TEXTURE_RESIDENT_FALSE;
						}

						// clamping

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RepeatS);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RepeatT);

						// filtering

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Minification);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Magnification);

						// free data

						if (ReleaseTextureData == TEXTURE_RELEASE_DATA_TRUE )
						{
							stbi_image_free(Data);
							Data = nullptr;
						}

					}
					else
					{
						vml::os::Message::Error("Texture :","failed to load at path : ' ", ResourceFileName.c_str()," '");
						stbi_image_free(Data);
					}
				}
				
			public:

				// ------------------------------------------------------------
				// texture filtering flags
				// OpenGL has two parameters that can be set :
				//
				// GL_TEXTURE_MIN_FILTER
				// GL_TEXTURE_MAG_FILTER
				//
				// GL_TEXTURE_MIN_FILTER accepts the following options :
				//
				//	GL_NEAREST
				//	GL_LINEAR
				//	GL_NEAREST_MIPMAP_NEAREST
				//	GL_NEAREST_MIPMAP_LINEAR
				//	GL_LINEAR_MIPMAP_NEAREST
				//	GL_LINEAR_MIPMAP_LINEAR
				//
				// GL_TEXTURE_MAG_FILTER accepts the following options :
				//
				// GL_NEAREST
				// GL_LINEAR

				static const unsigned int TEXTURE_FILTER_MIN_NEAREST			  = GL_NEAREST;					// Nearest criterion for minification
				static const unsigned int TEXTURE_FILTER_MIN_LINEAR				  = GL_LINEAR;					// Bilinear criterion for minification
				static const unsigned int TEXTURE_FILTER_MIN_NEAREST_MIPMAP		  = GL_NEAREST_MIPMAP_NEAREST;	// Nearest criterion for minification, but on closest mipmap
				static const unsigned int TEXTURE_FILTER_MIN_LINEAR_MIPMAP		  = GL_LINEAR_MIPMAP_NEAREST;	// Bilinear criterion for minification, but on closest mipmap
				static const unsigned int TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;	// Bilinear criterion for minification on two closest mipmaps, then averaged
				static const unsigned int TEXTURE_FILTER_MAG_NEAREST			  = GL_NEAREST;					// Nearest criterion for magnification
				static const unsigned int TEXTURE_FILTER_MAG_LINEAR				  = GL_LINEAR;					// Bilinear criterion for magnification
				
				// ------------------------------------------------------------
				// color formats

				static const unsigned int TEXTURE_RED							  = GL_RED;
				static const unsigned int TEXTURE_RGB							  = GL_RGB;
				static const unsigned int TEXTURE_RGBA							  = GL_RGBA;
				static const unsigned int TEXTURE_SRGB							  = GL_SRGB;
				static const unsigned int TEXTURE_SRGB_ALPHA					  = GL_SRGB_ALPHA;

				// ------------------------------------------------------------
				// GL_REPEAT:			The integer part of the coordinate will be ignored and a repeating pattern is formed.
				// GL_MIRRORED_REPEAT : The texture will also be repeated, but it will be mirrored when the integer 
				//						part of the coordinate is odd.
				// GL_CLAMP_TO_EDGE :   The coordinate will simply be clamped between 0 and 1.
				// GL_CLAMP_TO_BORDER : The

				static const unsigned int TEXTURE_REPEAT_S			= GL_REPEAT;					// texture repeat for u coordinate
				static const unsigned int TEXTURE_REPEAT_T		    = GL_REPEAT;					// texture repeat for v coordinate
				static const unsigned int TEXTURE_MIRRORED_S        = GL_MIRRORED_REPEAT;			// texture mirrored for u coordinate
				static const unsigned int TEXTURE_MIRRORED_T        = GL_MIRRORED_REPEAT;			// texture mirrored for v coordinate
				static const unsigned int TEXTURE_CLAMP_TO_EDGE_S   = GL_CLAMP_TO_EDGE;			// texture clamp tp edge for u coordinate
				static const unsigned int TEXTURE_CLAMP_TO_EDGE_T   = GL_CLAMP_TO_EDGE;			// texture clamp tp edge for v coordinate
				static const unsigned int TEXTURE_CLAMP_TO_BORDER_S = GL_CLAMP_TO_BORDER;			// texture clamp to border for u coordinate
				static const unsigned int TEXTURE_CLAMP_TO_BORDER_T = GL_CLAMP_TO_BORDER;		// texture clamp to border for u coordinate

				// ------------------------------------------------------------
				// texture mipmap boolean flags

				static const unsigned int TEXTURE_MIPMAP_FALSE = 0;
				static const unsigned int TEXTURE_MIPMAP_TRUE  = 1;

				// ------------------------------------------------------------
				// texture residence query flags

				static const unsigned int TEXTURE_RESIDENT_FALSE = 0;
				static const unsigned int TEXTURE_RESIDENT_TRUE  = 1;

				// ------------------------------------------------------------
				// texture gamma correction flags

				static const unsigned int TEXTURE_GAMMA_FALSE = 0;
				static const unsigned int TEXTURE_GAMMA_TRUE  = 1;

				// ------------------------------------------------------------
				// texture memory flags , user might decide to keep data
				// this happens in cases when the user needs to further reuse
				// data not just to use as textures

				static const unsigned int TEXTURE_RELEASE_DATA_FALSE = 0;
				static const unsigned int TEXTURE_RELEASE_DATA_TRUE  = 1;

				// -----------------------------------------------------------------
				//	Binds texture to texture unit 

				void BindTexture(int TextureUnit = 0) const
				{
					glActiveTexture(GL_TEXTURE0 + TextureUnit);
					glBindTexture(GL_TEXTURE_2D, ID);
				}

				//----------------------------------------------------------------
					// dump info

				const std::string Dump()
				{
					std::string text = "FileName               : "+ ResourceFileName +"\n";
						text += "Width                  : " + std::to_string(Width) + "\n";
						text += "Height                 : " + std::to_string(Height) + "\n";
					if ( TextureType== GL_TEXTURE_2D) 
						text += "TextureType            : GL_TEXTURE_2D\n";
					text += "Id                     : " + std::to_string(ID) + "\n";
					text += "BPP                    : " + std::to_string(BPP) + "\n";
					if (MipMapsGenerated == TEXTURE_MIPMAP_TRUE) 
						text += "MipMapsGenerated       : Yes\n";
					if (MipMapsGenerated== TEXTURE_MIPMAP_FALSE) 
						text += "MipMapsGenerated       : No\n";
					text += "Anisotropy             : " + std::to_string(Anisotropy) + "\n";
					if (Minification == TEXTURE_FILTER_MIN_NEAREST )
						text += "Minification           : TEXTURE_FILTER_MIN_NEAREST\n";
					if (Minification == TEXTURE_FILTER_MIN_LINEAR )
						text += "Minification            : TEXTURE_FILTER_MIN_LINEAR\n";
					if (Minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP )
						text += "Minification            : TEXTURE_FILTER_MIN_NEAREST_MIPMAP\n";
					if (Minification == TEXTURE_FILTER_MIN_LINEAR_MIPMAP )
						text += "Minification            : TEXTURE_FILTER_MIN_LINEAR_MIPMAP\n";
					if (Minification == TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR)
						text += "Minification            : TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR\n";
					if (Magnification == TEXTURE_FILTER_MAG_NEAREST )
						text += "Magnification          : TEXTURE_FILTER_MAG_NEAREST\n";
					if (Magnification == TEXTURE_FILTER_MAG_LINEAR)
						text += "Magnification          : TEXTURE_FILTER_MAG_LINEAR\n";
					if (RepeatS == TEXTURE_REPEAT_S)
						text += "RepeatS                : TEXTURE_REPEAT_S\n";
					if (RepeatS == TEXTURE_MIRRORED_S)
						text += "RepeatS                : TEXTURE_MIRRORED_S\n";
					if (RepeatS == TEXTURE_CLAMP_TO_EDGE_S)
						text += "RepeatS                : TEXTURE_CLAMP_TO_EDGE_S\n";
					if (RepeatS == TEXTURE_CLAMP_TO_BORDER_S)
						text += "RepeatS                : TEXTURE_CLAMP_TO_BORDER_S\n";
					if (RepeatT == TEXTURE_REPEAT_S)
						text += "RepeatT                : TEXTURE_REPEAT_S\n";
					if (RepeatT == TEXTURE_MIRRORED_S)
						text += "RepeatT                : TEXTURE_MIRRORED_S\n";
					if (RepeatT == TEXTURE_CLAMP_TO_EDGE_S)
						text += "RepeatT                : TEXTURE_CLAMP_TO_EDGE_S\n";
					if (RepeatT == TEXTURE_CLAMP_TO_BORDER_S)
						text += "RepeatT                : TEXTURE_CLAMP_TO_BORDER_S\n";
					if (Format == GL_RED)		 
						text += "Format                 : GL_RED\n";
					if (Format == GL_RGB)		 
						text += "Format                 : GL_RGB\n";
					if (Format == GL_SRGB)		 
						text += "Format                 : GL_SRGB\n";
					if (Format == GL_RGBA)		 
						text += "Format                 : GL_RGBA\n";
					if (Format == GL_SRGB_ALPHA) 
						text += "Format                 : GL_SRGB_ALPHA\n";
					if (DataFormat == GL_RED)
						text += "DataFormat             : GL_RED\n";
					if (DataFormat == GL_RGB)
						text += "DataFormat             : GL_RGB\n";
					if (DataFormat == GL_RGBA)
						text += "DataFormat             : GL_RGBA\n";
					if (Resident == TEXTURE_RESIDENT_TRUE)
						text += "Texture Resident Check : Yes\n";
					if (Resident == TEXTURE_RESIDENT_FALSE)
						text += "Texture Resident Check : False\n";
					if ( GammaCorrection==TEXTURE_GAMMA_TRUE)
						text += "Gamma Correction       : True\n";
					if (GammaCorrection == TEXTURE_GAMMA_FALSE)
						text += "Gamma Correction       : False\n";
					if (ReleaseTextureData == TEXTURE_RELEASE_DATA_TRUE)
						text += "Texture Data Released  : Yes\n";
					if (ReleaseTextureData == TEXTURE_RELEASE_DATA_FALSE)
						text += "Texture Data Released  : False\n";
					text += "Instances              : "+ std::to_string(GetInstancesCount())+"\n";
					return text;
				}
				
				// -----------------------------------------------------------
				// query functions

				constexpr GLuint		GetMinificationFilter()  const { return Minification; }
				constexpr GLuint		GetMagnificationFilter() const { return Magnification; }
				constexpr unsigned int	GetWidth()               const { return Width; }
				constexpr unsigned int	GetHeight()              const { return Height; }
				constexpr unsigned int	GetBPP()                 const { return BPP; }
				constexpr unsigned int	AreMipMapsGenerated()    const { return MipMapsGenerated; }
				constexpr GLboolean     IsResident()             const { return Resident; }
				constexpr GLuint		GetID()                  const { return ID; }
				constexpr GLenum		GetFormat()              const { return Format; }
				constexpr GLfloat		GetAnisotropy()          const { return Anisotropy; }
				constexpr float		    GetAspectRatio()         const { return Ratio; }
				constexpr unsigned int 	GetSize()                const { return Width*Height; }
				constexpr GLenum		GetType()                const { return TextureType;  }
				constexpr GLuint		GetRepeatS()             const { return RepeatS; }
				constexpr GLuint		GetRepeatT()             const { return RepeatT; }
				const unsigned char*	GetData()                const { return Data; }
				constexpr bool		    IsValid()                const { return ( Data != nullptr) && (Width!=0) && (Height!=0); }
				constexpr bool		    IsDataPreserved()        const { return ReleaseTextureData; }
					
				// ------------------------------------------------------------
				// Texture uploading default parameters
				// follow this *strict* order , since each 
				// entry is correlated to the opengl texture parm(s)

				static inline std::initializer_list<uint32_t> DefaultTextureParms{ TEXTURE_REPEAT_S,
																				   TEXTURE_REPEAT_T,
																				   TEXTURE_FILTER_MAG_LINEAR,
																				   TEXTURE_FILTER_MIN_LINEAR,
																				   TEXTURE_MIPMAP_TRUE,
																				   TEXTURE_GAMMA_FALSE,
																				   TEXTURE_RELEASE_DATA_TRUE,
																				   TEXTURE_RESIDENT_FALSE };
				
				// ---------------------------------------------------------
				// ctor / dtor

				Texture( const std::string &resourcefilename, const std::initializer_list<uint32_t>& il) : vml::utils::SharedResource(resourcefilename,il)
				{

					TextureType			= GL_TEXTURE_2D;
					Width				= 0;
					Height				= 0;	
					ID					= 0;	
					BPP					= 0;		
					Format				= 0;
					DataFormat			= 0;
					Anisotropy			= 0;
					Data				= nullptr;						// image buffer
					RepeatS				= TEXTURE_REPEAT_S;				// repeats;
					RepeatT				= TEXTURE_REPEAT_T;				// repeatt;
					Magnification		= TEXTURE_FILTER_MAG_LINEAR;	// magnification;
					Minification		= TEXTURE_FILTER_MIN_LINEAR;	// minification;
					MipMapsGenerated	= TEXTURE_MIPMAP_TRUE;			// mipmapsgenerated;
					GammaCorrection		= TEXTURE_GAMMA_FALSE;			// gammacorrection;
					ReleaseTextureData	= TEXTURE_RELEASE_DATA_TRUE;	// preservedata;
					Resident			= TEXTURE_RESIDENT_FALSE;		// resident;

					// if extra pack is provided then extract pack data
					// and assign to each texture parm(s), follow strictly 
					// the assigned order
					
					if (il.size() != 0 )
					{
						RepeatS			   = *(il.begin() + 0);		// repeats;
						RepeatT			   = *(il.begin() + 1);		// repeatt;
						Magnification	   = *(il.begin() + 2);		// magnification;
						Minification	   = *(il.begin() + 3);		// minification;
						MipMapsGenerated   = *(il.begin() + 4);		// mipmapsgenerated;
						GammaCorrection    = *(il.begin() + 5);		// gammacorrection;
						ReleaseTextureData = *(il.begin() + 6);		// preservedata;
						Resident		   = *(il.begin() + 7);		// resident;
					}

					// create opengl texture

					CreateTexture2dFromData();
				}

				~Texture()
				{
					if (ID) glDeleteTextures(1, &ID);
					vml::os::SafeDelete(Data);
			//		vml::utils::Logger::GetInstance()->Info("Texture", "Deleting : " + ResourceFileName);
				}

		};
	
	}	// end of image namespace

} // end of vml namespace
