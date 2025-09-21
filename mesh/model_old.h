#pragma once

//////////////////////////////////////////////////////////////////////////////////////
// TO DO:
// 
// Actually Visible flag is not used
// decouple rendering from logic
// nicer rotation mode selection

namespace vml
{
	namespace models
	{
		
		class Model3d_2
		{

			private:
		
				Model3d_2								   *Parent;							// Parent object
				std::vector<Model3d_2*>						Child;							// children objects
				std::string									ScreenName;						// Object ScreenName
				std::string									FullPathScreenName;				// Screenname based on model hierarchy
				uint32_t									InternalFlags;					// Bitfield used to store internal flags
				uint32_t									PreferencesFlags;				// Bitfield used to store preferences flags
				uint32_t	 								CullingFlags;					// Bitfield used to store internal flags
				glm::vec3									Position;						// position
				glm::vec3									Rotation;						// euler rotation angles
				glm::vec3									Scaling;						// scaling factor
				glm::vec3									OriginalRotation;				// old rotation position , used for deltas
				glm::vec3									OriginalScaling;				// old scaling position , used for deltas
				glm::vec3									OriginalPosition;				// old vector position , used for deltas
				std::vector<vml::meshes::Mesh3d*>			Meshes;							// Mesh pointer
				int											CurrentMesh;					// Currentmesh indx
				vml::geo3d::AABBox							AABoundingBox;					// Bounding box
				vml::geo3d::AOBBox							AOBoundingBox;					// Bounding box
				float										Radius;							// Radius
				glm::quat									Quaternion;						// Quaternion
				glm::vec3									Right;							// Right direction vector
				glm::vec3									Forward;						// Forward direction vector
				glm::vec3									Up;								// Up direction vector
				glm::mat4									M;								// model matrix
				glm::mat4									MV;								// model * view matrix
				glm::mat4									MVP;							// model * view * projection matrix
				glm::mat3									NV;								// viewnormal matrix
				glm::mat2									TM;								// texture matrix
				vml::textures::Texture*						DiffuseTexture;

				// ---------------------------------------------------------------
				// remove mesh if uniqe , if mesh is still in use, the
				// resource manager waits until no more models use it.

				void ReleaseAll()
				{
					// release attached mesh(es) trhough the meshstore

					for (size_t i = 0; i < Meshes.size(); ++i)
						vml::stores:: MeshStore->UnLoad(Meshes[i]->GetResourceFileName());

					if ( DiffuseTexture)
						vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());

					// clear meshes array

					Meshes.clear();

					// sets data members to initial values

					ScreenName		 = "";
					Parent			 = nullptr;
					InternalFlags    = 0;
					PreferencesFlags = 0;
					CullingFlags	 = 0;
					CurrentMesh		 = 0;
					Position		 = glm::vec3(0,0,0);
					Rotation		 = glm::vec3(0,0,0) * vml::math::DEGTORAD;
					Scaling			 = glm::vec3(1,1,1);
					OriginalPosition = Position;
					OriginalRotation = Rotation;
					OriginalScaling  = Scaling;
					Quaternion	     = glm::quat(0, 0, 0, 0);
					M				 = glm::mat4(0);
					MV				 = glm::mat4(0);
					MVP				 = glm::mat4(0);
					NV				 = glm::mat3(0);
					TM				 = glm::mat2(1, 0, 0, 1);
					Right			 = glm::vec3(1, 0, 0);
					Up				 = glm::vec3(0, 1, 0);
					Forward			 = glm::vec3(0, 0, 1);
					AOBoundingBox	 = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AABoundingBox	 = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius			 = 0.0f;
				}

				// ------------------------------------------------------------------
				// Compute quaternion matrix

				void ComputePolarMatrix()
				{
					// cache scaling factors

					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					// get angles

					float alpha = Rotation.x;
					float phi = Rotation.y;
					float theta = Rotation.z;

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					float cx = cosf(phi);
					float sx = sinf(phi);
					float cy = cosf(theta);
					float sy = sinf(theta);

					// polar coordinates
					// direction axis in spherical coordinates

					glm::vec3 axis(cx * cy, sx, cx * sy);

					// create quaternion around this axis

					glm::quat quaternion = glm::angleAxis(alpha, axis);

					glm::mat4 Q = glm::toMat4(quaternion);

					float* QMatrix = glm::value_ptr(Q);

					float f0  = QMatrix[ 0];
					float f1  = QMatrix[ 1];
					float f2  = QMatrix[ 2];
				//  float f3  = QMatrix[ 3];	// f3 is 0
					float f4  = QMatrix[ 4];
					float f5  = QMatrix[ 5];
					float f6  = QMatrix[ 6];
				//  float f7  = QMatrix[ 7];	// f7 is 0
					float f8  = QMatrix[ 8];
					float f9  = QMatrix[ 9];
					float f10 = QMatrix[10];
				//  float f11 = QMatrix[11];	// f11 is 0
				//  float f12 = QMatrix[12];	// f12 is 0
				//  float f13 = QMatrix[13];	// f13 is 0
				//  float f14 = QMatrix[14];	// f14 is 0
				//  float f15 = QMatrix[15];	// f15 is 0

					// assumes axis is already normalised

				//	glm::vec3 up(0, 1, 0);
				//	glm::vec3 p(cx * cy, sx, cx * sy);
				//	glm::vec3 s(glm::normalize(glm::cross(p, up)));
				//	glm::vec3 u(glm::cross(s, p));
				//	glm::mat4 O( p.x , p.y , p.z , 0.0f ,
				//			 	 u.x , u.y , u.z , 0.0f ,
				//		 		 s.x , s.y , s.z , 0.0f ,
				//				 0   ,   0 ,   0 , 1.0f );

					// hardcoded version of the above code

					float r0  =  cx * cy;
					float r1  =  sx;
					float r2  =  cx * sy;
					float r4  = -cy * sx;
					float r5  =  cx;
					float r6  = -sx * sy;
					float r8  = -sy;
					float r10 =  cy;

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[ 0] = -(r8 * f0 + r10 *  f8) * scx;
					matrix[ 1] = -(r8 * f1 + r10 *  f9) * scx;
					matrix[ 2] = -(r8 * f2 + r10 * f10) * scx;
					matrix[ 3] =  0;
					matrix[ 4] =  (r4 * f0 + r5 * f4 + r6 *  f8) * scy;
					matrix[ 5] =  (r4 * f1 + r5 * f5 + r6 *  f9) * scy;
					matrix[ 6] =  (r4 * f2 + r5 * f6 + r6 * f10) * scy;
					matrix[ 7] =  0;
					matrix[ 8] =  (r0 * f0 + r1 * f4 + r2 *  f8) * scz;
					matrix[ 9] =  (r0 * f1 + r1 * f5 + r2 *  f9) * scz;
					matrix[10] =  (r0 * f2 + r1 * f6 + r2 * f10) * scz;
					matrix[11] =  0;
					matrix[12] =  Position.x;
					matrix[13] =  Position.y;
					matrix[14] =  Position.z;
					matrix[15] =  1;
				}

				// -----------------------------------------------------------------------
				// Compute euler matrix

				void ComputeEulerMatrix()
				{
					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					// get angles

					float alpha = Rotation.x;
					float phi   = Rotation.y;
					float theta = Rotation.z;

					// clamp angles

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(alpha);
					float sy = sin(phi);
					float sz = sin(theta);
					float cx = cos(alpha);
					float cy = cos(phi);
					float cz = cos(theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[ 0] = ( cy * cz ) * scx;
					matrix[ 1] = ( sx * sy * cz + cx * sz ) * scx;
					matrix[ 2] = (-cx * sy * cz + sx * sz ) * scx;
					matrix[ 3] = 0;
					matrix[ 4] = (-cy * sz) * scy;
					matrix[ 5] = (-sx * sy * sz + cx * cz ) * scy;
					matrix[ 6] = ( cx * sy * sz + sx * cz ) * scy;
					matrix[ 7] = 0;
					matrix[ 8] = ( sy) * scz;
					matrix[ 9] = (-sx * cy) * scz;
					matrix[10] = ( cx * cy) * scz;
					matrix[11] = 0;
					matrix[12] = Position.x;
					matrix[13] = Position.y;
					matrix[14] = Position.z;
					matrix[15] = 1;
				}

				// -----------------------------------------------------------------------
				// compute quaternion matrix

				void ComputeQuaternionMatrix()
				{
					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					glm::mat4 R = glm::toMat4(Quaternion);

					//	M = T * R * S ;

					float* matrix = glm::value_ptr(M);

					float* r = glm::value_ptr(R);

					matrix[ 0] = r[ 0] * scx;
					matrix[ 1] = r[ 1] * scx;
					matrix[ 2] = r[ 2] * scx;
					matrix[ 3] = 0;
					matrix[ 4] = r[ 4] * scy;
					matrix[ 5] = r[ 5] * scy;
					matrix[ 6] = r[ 6] * scy;
					matrix[ 7] = 0;
					matrix[ 8] = r[ 8] * scz;
					matrix[ 9] = r[ 9] * scz;
					matrix[10] = r[10] * scz;
					matrix[11] = 0;
					matrix[12] = Position.x;
					matrix[13] = Position.y;
					matrix[14] = Position.z;
					matrix[15] = 1;
				}

			public:

				// ---------------------------------------------------------------
				// this flags are used when the mesh is attached to a model

				// ---------------------------------------------------------------
				// specify rotation transformation

				static constexpr uint32_t EULER		  = vml::utils::bits32::BIT0;
				static constexpr uint32_t QUATERNIONS = vml::utils::bits32::BIT1;
				static constexpr uint32_t POLAR		  = vml::utils::bits32::BIT2;

				// ---------------------------------------------------------------
				// visiblity flag

				static constexpr uint32_t VISIBLE = vml::utils::bits32::BIT8;

				// ---------------------------------------------------------------
				// specify if vertex buffers need to be release after creating vbo(s)

				static constexpr uint32_t DO_NOT_RELEASE_DATA = vml::utils::bits32::BIT10;

				// ---------------------------------------------------------------
				// specify rendering modes

				static constexpr unsigned int WIREFRAME = vml::utils::bits32::BIT16;

				// -------------------------------------------------------------
				// transform objet to view

				void TransformToView(vml::views::View* View)
				{
					// get View and projection matrices

					const glm::mat4& p = View->GetProjection();
					const glm::mat4& v = View->GetView();

					// concatenate v and p matrices

					MV  = v * M;
					MVP = p * MV;

					// floowing code is this transformation sequence
					// using glm  matrix lib

					//model->NV  = glm::mat3(glm::transpose(glm::inverse(model->MV)));

					// get normal matrix pointer

					float *normalviewmatrix = glm::value_ptr(NV);

					// get model * view matrix

					float *modelviewmatrix = glm::value_ptr(MV);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the
					// first 3x3 order for this matrix is meant to be divided
					// by each scaling factor along correspondig axes, so
					// we need to unskew normal matrix

					float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
										- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
										+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[5] * modelviewmatrix[8]);

					if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

					normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
					normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
					normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
					normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
					normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[ 6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[ 6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[ 5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

				}

				// -----------------------------------------------------------------------
				// transform model bounding box

				void TransformBoundingBoxes()
				{
					// compute oriented bounding box
					// the bounding box is computed transforming
					// models' mesh's bounding box by the model's
					// M matrix , which is the T * R * S matrix
					// see aabbox.h
					//
					// cache model's M matrix

					const float* m = glm::value_ptr(M);

					// get's model's mesh bounding box coordinates

					AOBoundingBox.Set(GetCurrentMesh()->GetBoundingBox().GetMin(), GetCurrentMesh()->GetBoundingBox().GetMax(), m);

					// compute axis aligned bounding box

					AABoundingBox.Set(AOBoundingBox.GetMin(), AOBoundingBox.GetMax());

					// compute bounding sphere

					Radius = sqrtf(AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
								   AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
								   AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z)*0.5f;

					// compute direction vectors
					// the oriented bounding box is centered at root's center

					Right   = glm::normalize(glm::vec3(m[0], m[1], m[ 2]));
					Up      = glm::normalize(glm::vec3(m[4], m[5], m[ 6]));
					Forward = glm::normalize(glm::vec3(m[8], m[9], m[10]));
				}

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Model3d_2(Model3d_2& model) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				Model3d_2 operator=(const Model3d_2& model) = delete;
				
				// --------------------------------------------------------------------------------
				// getters

				const std::string		  &GetScreenName()			const { return ScreenName; }
				const std::string		  &GetFullPathScreenName()	const { return FullPathScreenName; }
				unsigned int			   GetInternalFlags()		const { return InternalFlags; }
				unsigned int			   GetPreferencesFlags()	const { return PreferencesFlags; }
				Model3d_2				  *GetParent()				const { return Parent; }
				size_t					   GetChildCount()			const { return Child.size(); }

				// --------------------------------------------------------------------------
				// metrics getters

				const vml::geo3d::AABBox &GetAABoundingBox()		const { return AABoundingBox; }
				const vml::geo3d::AOBBox &GetAOBoundingBox()		const {	return AOBoundingBox; }
				float					  GetRadius()				const {	return Radius; }
				const glm::vec3			 &GetRightVector()			const { return Right; }
				const glm::vec3			 &GetForwardVector()		const {	return Forward; }
				const glm::vec3			 &GetUpVector()				const { return Up; }
				glm::vec3				  GetTransformedPosition()  const { const float* m = glm::value_ptr(M); return glm::vec3(m[12], m[13], m[14]); }
				bool					  IsVisbile()				const { return vml::utils::bits32::Get(PreferencesFlags, VISIBLE); }
				unsigned int			  GetCullingFlags()			const { return CullingFlags; }
				unsigned int			  IsInFrustum()				const { return CullingFlags != vml::views::frustum::OUTSIDE; }
				bool					  IsWire()					const { return vml::utils::bits32::Get(PreferencesFlags, WIREFRAME); }
				bool					  IsSolid()					const { return !vml::utils::bits32::Get(PreferencesFlags, WIREFRAME); }
				bool					  IsDataReleased()			const { return !vml::utils::bits32::Get(PreferencesFlags, DO_NOT_RELEASE_DATA); }
				bool					  IsDataRetained()			const { return  vml::utils::bits32::Get(PreferencesFlags, DO_NOT_RELEASE_DATA); }

				// --------------------------------------------------------------------------
				// hierarchy getters

				Model3d_2				*GetChild(size_t pos)		 const { return *(Child.begin() + pos); }
				vml::meshes::Mesh3d	    *GetCurrentMesh()			 const { return Meshes[CurrentMesh]; }
				vml::meshes::Mesh3d		*GetMeshAt(const size_t pos) const { return Meshes[pos]; }
				size_t					 GetMeshesCount()			 const { return Meshes.size(); }
				
				Model3d_2* GetChild(const std::string& childname) const
				{
					if (childname.empty())
						vml::os::Message::Error("ObjectManager : ", "Model3d : Null name is not allowed");
					for (size_t i = 0; i < Child.size(); ++i)
						if (Child[i]->GetScreenName() == childname) return Child[i];
					return nullptr;
				}
				
				// --------------------------------------------------------------------------
				// matrix getters

				float* GetMptr()		   { return glm::value_ptr(M); }
				float* GetNVptr()		   { return glm::value_ptr(NV); }
				float* GetMVptr()		   { return glm::value_ptr(MV); }
				float* GetMVPptr()		   { return glm::value_ptr(MVP); }
				float* GetTMptr()		   { return glm::value_ptr(TM); }
				glm::quat& GetQuaternion() { return Quaternion; }

				const glm::mat4& GetM()   const { return M; }
				const glm::mat4& GetMV()  const { return MV; }
				const glm::mat4& GetMVP() const { return MVP; }
				const glm::mat3& GetNV()  const { return NV; }

				// rotation / postion / scaling getters

				const glm::vec3& GetPosition() const { return Position; }
				const glm::vec3& GetAngles()   const { return Rotation; }
				const glm::vec3& GetScaling()  const { return Scaling; }

				float GetPosX() const { return Position.x; }
				float GetPosY() const { return Position.y; }
				float GetPosZ() const { return Position.z; }

				// -----------------------------------------------------------------------
				// Get rotation mode

				int GetRotationMode() const
				{
					if (vml::utils::bits32::Get(PreferencesFlags, QUATERNIONS)) return QUATERNIONS;
					if (vml::utils::bits32::Get(PreferencesFlags, EULER))		return EULER;
					if (vml::utils::bits32::Get(PreferencesFlags, POLAR))		return POLAR;
					return -1;
				}

				// --------------------------------------------------------------------------
				// texturing

				vml::textures::Texture* GetDiffuseTexture() const { return DiffuseTexture; }

				// ----------------------------------------------------------------------------------------
				// Setters

				void SetParent(Model3d_2* parent)								  { Parent = parent; }
				void ClearChild(Model3d_2* child)								  { Child.clear(); }
				void AddChild(Model3d_2* child)									  { Child.emplace_back(child); }
				void SetFullPathScreenName(const std::string& fullpathscreenname) { FullPathScreenName = fullpathscreenname; }
				void SetVisible()												  {	vml::utils::bits32::SetToTrue(PreferencesFlags, VISIBLE); }
				void SetInvisible()												  { vml::utils::bits32::SetToFalse(PreferencesFlags, VISIBLE); }
				void SetCullingFlags(unsigned int cullingflags)					  { CullingFlags = cullingflags; }
				void SetCullingFlagToOutside()									  { CullingFlags = vml::views::frustum::OUTSIDE; }
				void SetCullingFlagToIntersected()								  { CullingFlags = vml::views::frustum::INTERSECTED; }
				void SetCullingFlagToInside()									  { CullingFlags = vml::views::frustum::INSIDE; }

				void SetRotationMode(int mode)
				{
					if (mode != EULER && mode != QUATERNIONS && mode != POLAR )
						vml::os::Message::Error("ObjectManager : ", "unacceptable rotation mode");
					vml::utils::bits32::SetToTrue(PreferencesFlags, mode);
				}
				
				void SetPosition(const glm::vec3& pos)		    { Position  = pos; }									// Set model position given a vector
				void SetOriginalPosition(const glm::vec3& pos)  { OriginalPosition = pos; }								// Set model position given a vector
				void Move(const glm::vec3& disp)			    { Position += disp; }									// Move object to position
				void MoveX(const float px)					    { Position.x += px; }									// Move object to position
				void MoveY(const float py)					    { Position.y += py; }									// Move object to position
				void MoveZ(const float pz)					    { Position.z += pz; }									// Move object to position
				void SetPosX(const float px)				    { Position.x = px; }									// Set model position given a vector
				void SetPosY(const float py)				    { Position.y = py; }									// Set model position given a vector
				void SetPosZ(const float pz)				    { Position.z = pz; }									// Set model position given a vector

				void SetAngles(const glm::vec3& angles)			{ Rotation  = angles * vml::math::DEGTORAD; }			// Rotation angles
				void AddAngles(const glm::vec3& angles)			{ Rotation += angles * vml::math::DEGTORAD; }			// Add rotation angles
				void SetOriginalAngles(const glm::vec3& angles) { OriginalRotation = angles * vml::math::DEGTORAD; }	// Rotation angles
				void SetAngleX(const float ax)					{ Rotation.x = ax; }
				void SetAngleY(const float ay)					{ Rotation.y = ay; }
				void SetAngleZ(const float az)					{ Rotation.z = az; }
				void AddAngleX(const float ax)				    { Rotation.x += ax; }
				void AddAngleY(const float ay)				    { Rotation.y += ay; }
				void AddAngleZ(const float az)				    { Rotation.z += az; }

				void SetScale(const glm::vec3& scale)		    { Scaling = scale; }										// Scale model by a scaling vector
				void SetOriginalScale(const glm::vec3& scale)   { OriginalScaling = scale; }								// Scale model by a scaling vector
				void SetScaleX(const float sx)				    { Scaling.x = sx; }
				void SetScaleY(const float sy)				    { Scaling.y = sy; }
				void SetScaleZ(const float sz)				    { Scaling.z = sz; }
				void AddScaleX(const float sx)				    { Scaling.x *= sx; }
				void AddScaleY(const float sy)					{ Scaling.y *= sy; }
				void AddScaleZ(const float sz)					{ Scaling.z *= sz; }

				void SetTextureMatrixScale(const glm::vec2& scale) { TM[0][0] = scale.x;	TM[0][1] = 0; TM[1][0] = 0; TM[1][1] = scale.y; }
				
				// --------------------------------------------------------------------------------

				void ResetScaling()	  {	Scaling  = OriginalScaling;  }													// Resets model scale
				void ResetPosition()  { Position = OriginalPosition; }													// Reset model position
				void ResetRotation()  {	Rotation = OriginalRotation; }													// Reset angle rotation with initial values

				// -------------------------------------------------------------
				// reset all transofrmations

				void ResetTransformations()
				{
					Position = OriginalPosition;
					Rotation = OriginalRotation;
					Scaling  = OriginalScaling;
				}

				// -------------------------------------------------------------
				// compute transform matrix

				void ComputeMatrix()
				{
					if (vml::utils::bits32::Get(PreferencesFlags, QUATERNIONS)) ComputeQuaternionMatrix();
					if (vml::utils::bits32::Get(PreferencesFlags, EULER))		ComputeEulerMatrix();
					if (vml::utils::bits32::Get(PreferencesFlags, POLAR))		ComputePolarMatrix();
				}

				// -------------------------------------------------------------
				// add diffuse texture

				vml::textures::Texture *AddDiffuseTexture(const std::string& filename, std::initializer_list<uint32_t> &il=vml::textures::Texture::DefaultTextureParms)
				{
					if ( DiffuseTexture)
						vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());
					DiffuseTexture = vml::stores::TextureStore->Load<vml::textures::Texture>(filename,il);
					return DiffuseTexture;
				}

				// -------------------------------------------------------------

				void RemoveDiffuseTexture()
				{
					if (DiffuseTexture)
					{
						vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());
						DiffuseTexture = nullptr;
					}
				}

				// ------------------------------------------------------------------
				// ctor / dtor
				// A base class destructor may be responsible for cleaning 
				// up resources that were allocated by the base class constructor.
				// If your base class has a default constructor
				// (one that doesn't take parameters or has defaults
				// for all its parameters) that constructor is 
				// automatically called upon construction of a derived instance.
				// If your base class has a constructor that requires parameters, 
				// you must call it manually in the initializer list of 
				// the derived class constructor.

				Model3d_2( const std::string& screenname,
						   const std::string& filename,
						   const glm::vec3& pos = vml::geo3d::NullVec,
						   const glm::vec3& angles = vml::geo3d::NullVec,
						   const glm::vec3& scaling = vml::geo3d::UnaryVec,
						   const uint32_t preferencesflags = EULER )
				{
					if (screenname.empty())
						vml::os::Message::Error("ObjectManager : ", "Cannot assign empty screenname");
					if (filename.empty())
						vml::os::Message::Error("ObjectManager : ", "NUll mesh resource filename is not allowed fro model ' ", ScreenName.c_str()," '");
					
					// init member data

					ScreenName		  = vml::strings::StringUtils::TrimAndLower(screenname);
					Parent            = nullptr;
					InternalFlags     = vml::utils::InternalFlags::INITTED | vml::utils::InternalFlags::FINALIZED;
					PreferencesFlags  = preferencesflags;
					CullingFlags	  = 0;
					CurrentMesh       = 0;
					DiffuseTexture	  = nullptr;
					
					// set model transform data
					
					Position	     = pos;
					Rotation		 = angles * vml::math::DEGTORAD;
					Scaling		     = scaling;
					OriginalPosition = Position;
					OriginalRotation = Rotation;
					OriginalScaling  = Scaling;
					Quaternion		 = glm::quat(0,0,0,0);

					// set scaling

					if (fabs(Scaling.x) < vml::math::EPSILON ||
						fabs(Scaling.y) < vml::math::EPSILON ||
						fabs(Scaling.z) < vml::math::EPSILON)
							vml::os::Message::Error("ObjectManager : ", "Scale cannot bu null for model ' ", ScreenName.c_str(), " '");

					// if rotation flags are not set, provide a way
					// to use euler rotation computation for matrix computations

					bool euler       = vml::utils::bits32::Get(PreferencesFlags, EULER);
					bool quaternions = vml::utils::bits32::Get(PreferencesFlags, QUATERNIONS);
					bool polar       = vml::utils::bits32::Get(PreferencesFlags, POLAR);

					// rendering flags

					bool wire = vml::utils::bits32::Get(PreferencesFlags, WIREFRAME);

					// if angles flag is not euler, quaternion or polar,
					// then default to euler

					if (!((euler && !quaternions && !polar ) ||
						 (!euler &&  quaternions && !polar ) ||
						 (!euler && !quaternions &&  polar ) ||
						 (!euler && !quaternions && !polar )))
								vml::os::Message::Error("ObjectManager : ", "Choose only one rotation mode for model ' ", ScreenName.c_str()," '");

					// uploads the requested mesh
					// as default loading a mesh within a model
					// releases vertex data *see mesh class*

					Meshes.emplace_back(vml::stores::MeshStore->Load<vml::meshes::Mesh3d>(filename, {}));
					
					// init matrices

					M   = glm::mat4(0);
					MV  = glm::mat4(0);
					MVP = glm::mat4(0);
					NV  = glm::mat3(0);
					TM  = glm::mat2(1, 0, 0, 1);

					// init direction vector

					Right   = glm::vec3(1, 0, 0);
					Up      = glm::vec3(0, 1, 0);
					Forward = glm::vec3(0, 0, 1);

					// initialise metrics

					AOBoundingBox = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AABoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius		  = 0.0f;
					
					// flags

					vml::utils::bits32::SetToTrue(PreferencesFlags, VISIBLE);

				}

				// Your base class destructor will always be automatically 
				// called upon deletion of the derived instance since 
				// destructors don't take parameters.
				// If you're using polymorphism and your derived
				// instance is pointed to by a base class pointer, 
				// then the derived class destructor is only
				// called if the base destructor is virtual.

			   ~Model3d_2()
				{
					ReleaseAll();
				}

		};
		
	}
}


