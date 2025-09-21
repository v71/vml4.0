#pragma once

namespace vml
{
	namespace objects
	{

		class Object3d_2
		{
			
			private:
				
				//------------------------------------------------------------------
				// private data
				
				std::string						     FileName;				// filename for this object includes full path
				std::string						     ScreenName;			// screenname for this object
				vml::models::Model3d_2			   **Stack;					// stacked breadth-first hierarchyical transformation
				int								     StackCounter;			// Stack counter
				vml::geo3d::AABBox				     AABoundingBox;			// Bounding box
				vml::geo3d::AOBBox					 AOBoundingBox;			// Bounding box
				float								 Radius;				// radius for this object
				glm::vec3							 Right;					// Right direction vector
				glm::vec3							 Forward;				// Forward direction vector
				glm::vec3							 Up;					// Up direction vector
				int									 CurrentModel;			// current model
				unsigned int						 InternalFlags;			// bitflag used for flagging
				unsigned int	 					 CullingFlags;			// Bitfield used to store internal flags
				std::vector<vml::models::Model3d_2*> Models;				// models building up the object
				
				//------------------------------------------------------------------
				// release all items
				
				void ReleaseAll()
				{
					
					// meshes are released through the resource manager

					 for ( size_t i=0; i<Models.size(); ++i ) 
						vml::os::SafeDelete ( Models[i] );

					// delete models array

					 Models.clear();

					// delete stack

					 vml::os::SafeDelete(Stack);
					
					 // sets data members to initial values

					 FileName      =  "";
					 ScreenName    =  "";
					 InternalFlags =  0;
					 CullingFlags  =  0;
					 StackCounter  =  0;
					 CurrentModel  = -1;
					 Right		   =  glm::vec3(1, 0, 0);
					 Up			   =  glm::vec3(0, 1, 0);
					 Forward	   =  glm::vec3(0, 0, 1);
					 AABoundingBox =  vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					 AOBoundingBox =  vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					 Radius		   =  0;

				}
				
				// -------------------------------------------------------------
				// compute stack base on hierarchy between object's models
				
				void ComputeStack()
				{
					
					// allocate space for stack, the stack is used for 
					// hierarchical matrix transformation
			
					vml::os::SafeDelete(Stack);
						
					size_t n = Models.size();

					Stack= new vml::models::Model3d_2 *[ n ];
						
					// initial stack counter position

					StackCounter = 1;

					// root model is always at 0 position

					Stack[0] = Models[0];
						
					// breadth first stack

					if ( n>1 )
					{
						for (size_t sp = 0; sp < StackCounter; ++sp)
						{
							for (size_t j = 0; j < Stack[sp]->GetChildCount(); ++j)
							{
								Stack[StackCounter++] = Stack[sp]->GetChild(j);
							}
						}
					}

					//Log.Out("Object", "Connected", ScreenName, vml::strings::StringFormat::Text("Connected {0} models", sc));

					if (StackCounter !=Models.size() )
						vml::os::Message::Error("Object Instance ' ", ScreenName.c_str()," ' : All models must be connected");

					// create full path string for each model
			
					for (size_t i = 0; i < StackCounter; ++i)
					{
						if (i == 0)
						{
							Stack[0]->SetFullPathScreenName("root\\" + Stack[0]->GetScreenName());
						}
						else
						{
							Stack[i]->SetFullPathScreenName(Stack[i]->GetParent()->GetFullPathScreenName() + "\\" + vml::strings::SplitPath::GetTitle(Stack[i]->GetScreenName()));
						}
					}

					//	for (size_t i = 0; i < StackCounter; ++i)
					//	{
					//		if ( Stack[i]->GetParent())
					//			std::cout << Stack[i]->GetFullPathScreenName() << " Parent : " << Stack[i]->GetParent()->GetFullPathScreenName() << std::endl;
					//		else
					//			std::cout << Stack[i]->GetFullPathScreenName() << " Parent : Root" << std::endl;
					//	}
					
				}

				// -----------------------------------------------------------------------
				// transform compound bounding box

				void TransformBoundingBoxes()
				{
					
					// compute bounding box for the entire object
					// basically iterate trhough all the object's models
					// and finds the lowest and farthest vertices for
					// each models' axis aligned bounding box

					glm::vec3 alignedboundingboxmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
					glm::vec3 alignedboundingboxmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < Models.size(); ++i)
					{

						const vml::geo3d::AABBox& bbox = Stack[i]->GetAABoundingBox();

						if (bbox.GetMin().x < alignedboundingboxmin.x) alignedboundingboxmin.x = bbox.GetMin().x;
						if (bbox.GetMin().y < alignedboundingboxmin.y) alignedboundingboxmin.y = bbox.GetMin().y;
						if (bbox.GetMin().z < alignedboundingboxmin.z) alignedboundingboxmin.z = bbox.GetMin().z;

						if (bbox.GetMax().x > alignedboundingboxmax.x) alignedboundingboxmax.x = bbox.GetMax().x;
						if (bbox.GetMax().y > alignedboundingboxmax.y) alignedboundingboxmax.y = bbox.GetMax().y;
						if (bbox.GetMax().z > alignedboundingboxmax.z) alignedboundingboxmax.z = bbox.GetMax().z;

					}

					// compute axis aligned bounding box

					AABoundingBox.Set(alignedboundingboxmin, alignedboundingboxmax);

					// compute bounding sphere

					Radius = sqrtf( AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
									AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
									AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z)*0.5f;

					// direction vectors are taken from the root model
					// the oriented bounding box is centered at root's center
					// 0-1 x axis
					// 0-3 y axis
					// 0-4 z axis

					Right   = Models[0]->GetRightVector();
					Up      = Models[0]->GetUpVector();
					Forward = Models[0]->GetForwardVector();

					// gets oriented bounding box

					float dist;
					glm::vec3 p0, p1, p2, p3, p4, p5, p6, p7;

					float sizemx =  FLT_MAX;
					float sizeMx = -FLT_MAX;
					float sizemy =  FLT_MAX;
					float sizeMy = -FLT_MAX;
					float sizemz =  FLT_MAX;
					float sizeMz = -FLT_MAX;

					const glm::vec3 &center = AABoundingBox.GetCenter();

					for (size_t i = 0; i < Models.size(); ++i)
					{
							
						const vml::geo3d::AOBBox& aobbox = Models[i]->GetAOBoundingBox();
							
						p0 = aobbox[0] - center;
						p1 = aobbox[1] - center;
						p2 = aobbox[2] - center;
						p3 = aobbox[3] - center;
						p4 = aobbox[4] - center;
						p5 = aobbox[5] - center;
						p6 = aobbox[6] - center;
						p7 = aobbox[7] - center;
							
						dist = glm::dot(Right, p0); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p1); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p2); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p3); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p4); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p5); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p6); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p7); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;

						dist = glm::dot(Up, p0); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p1); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p2); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p3); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p4); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p5); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p6); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p7); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;

						dist = glm::dot(Forward, p0); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p1); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p2); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p3); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p4); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p5); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p6); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p7); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
							
					}
						
					// create oriented bounding box
					// minimum is p0 + p1 + p2 + center, 
					// maximum is p3 + p4 + p5 + center
					// corners are aligned to right, up 
					// and forward reference vectors
					
					p0 = sizemx * Right;
					p1 = sizemy * Up;
					p2 = sizemz * Forward;
					p3 = sizeMx * Right;
					p4 = sizeMy * Up;
					p5 = sizeMz * Forward;

					AOBoundingBox.Set(glm::vec3(p0 + p1 + p2 + center),
									  glm::vec3(p3 + p1 + p2 + center),
									  glm::vec3(p3 + p4 + p2 + center),
									  glm::vec3(p0 + p4 + p2 + center),
									  glm::vec3(p0 + p1 + p5 + center),
									  glm::vec3(p3 + p1 + p5 + center),
									  glm::vec3(p3 + p4 + p5 + center),
									  glm::vec3(p0 + p4 + p5 + center));
					
				}

				// -------------------------------------------------------------
				// trasnform model

				void TransformModel(vml::models::Model3d_2* model)
				{
					// transform model

					model->ComputeMatrix();

					// get parent model

					vml::models::Model3d_2* parent = model->GetParent();

					// if parent is null, it means the model is the root
					// model, so we need only to compute the transformed
					// bounding boxes, if model has parent, compute 
					// the rotation matrix concatenation according to 
					// parent's rotation matrix

					if (!parent)
					{
						model->TransformBoundingBoxes();
					}
					else
					{
						// if we get here, we need to perform the 
						// full matrix concatenazion

						float  parentscale[3]    = { 0 };
						float  invparentscale[3] = { 0 };
						float  tempmatrix[16]    = { 0 };
						float* childmatrix       = nullptr;
						float* parentmatrix      = nullptr;

						// rescale parent matrix , this is necessary
						// becasue of child-parent matrix multiplication
						// actually this performs a multiplicatiuon 
						// between the parent model matrix and the
						// inverse of its own scaling matrix

						// cache scaling factors

						parentscale[0] = parent->GetScaling().x;
						parentscale[1] = parent->GetScaling().y;
						parentscale[2] = parent->GetScaling().z;

						// compute inverse scaling

						invparentscale[0] = 1.0f / parentscale[0];
						invparentscale[1] = 1.0f / parentscale[1];
						invparentscale[2] = 1.0f / parentscale[2];

						// get parent matrix pointer

						parentmatrix = parent->GetMptr();

						// scale parent matrix by its inverse scaling vector

						parentmatrix[ 0] *= invparentscale[0];
						parentmatrix[ 1] *= invparentscale[0];
						parentmatrix[ 2] *= invparentscale[0];
						parentmatrix[ 4] *= invparentscale[1];
						parentmatrix[ 5] *= invparentscale[1];
						parentmatrix[ 6] *= invparentscale[1];
						parentmatrix[ 8] *= invparentscale[2];
						parentmatrix[ 9] *= invparentscale[2];
						parentmatrix[10] *= invparentscale[2];

						// concatenate parent and child matrices

						childmatrix = model->GetMptr();

						// save child matrix to a temporary matrix

						memcpy(tempmatrix, childmatrix, sizeof(float) * 16);

						// compute matrix multiplication this is 
						// the equvalent of this equation 
						// child->M = parent->M * child->M;

						childmatrix[ 0] = tempmatrix[ 0] * parentmatrix[0] + tempmatrix[ 1] * parentmatrix[4] + tempmatrix[ 2] * parentmatrix[ 8] + tempmatrix[ 3] * parentmatrix[12];
						childmatrix[ 1] = tempmatrix[ 0] * parentmatrix[1] + tempmatrix[ 1] * parentmatrix[5] + tempmatrix[ 2] * parentmatrix[ 9] + tempmatrix[ 3] * parentmatrix[13];
						childmatrix[ 2] = tempmatrix[ 0] * parentmatrix[2] + tempmatrix[ 1] * parentmatrix[6] + tempmatrix[ 2] * parentmatrix[10] + tempmatrix[ 3] * parentmatrix[14];
						childmatrix[ 3] = tempmatrix[ 0] * parentmatrix[3] + tempmatrix[ 1] * parentmatrix[7] + tempmatrix[ 2] * parentmatrix[11] + tempmatrix[ 3] * parentmatrix[15];
						childmatrix[ 4] = tempmatrix[ 4] * parentmatrix[0] + tempmatrix[ 5] * parentmatrix[4] + tempmatrix[ 6] * parentmatrix[ 8] + tempmatrix[ 7] * parentmatrix[12];
						childmatrix[ 5] = tempmatrix[ 4] * parentmatrix[1] + tempmatrix[ 5] * parentmatrix[5] + tempmatrix[ 6] * parentmatrix[ 9] + tempmatrix[ 7] * parentmatrix[13];
						childmatrix[ 6] = tempmatrix[ 4] * parentmatrix[2] + tempmatrix[ 5] * parentmatrix[6] + tempmatrix[ 6] * parentmatrix[10] + tempmatrix[ 7] * parentmatrix[14];
						childmatrix[ 7] = tempmatrix[ 4] * parentmatrix[3] + tempmatrix[ 5] * parentmatrix[7] + tempmatrix[ 6] * parentmatrix[11] + tempmatrix[ 7] * parentmatrix[15];
						childmatrix[ 8] = tempmatrix[ 8] * parentmatrix[0] + tempmatrix[ 9] * parentmatrix[4] + tempmatrix[10] * parentmatrix[ 8] + tempmatrix[11] * parentmatrix[12];
						childmatrix[ 9] = tempmatrix[ 8] * parentmatrix[1] + tempmatrix[ 9] * parentmatrix[5] + tempmatrix[10] * parentmatrix[ 9] + tempmatrix[11] * parentmatrix[13];
						childmatrix[10] = tempmatrix[ 8] * parentmatrix[2] + tempmatrix[ 9] * parentmatrix[6] + tempmatrix[10] * parentmatrix[10] + tempmatrix[11] * parentmatrix[14];
						childmatrix[11] = tempmatrix[ 8] * parentmatrix[3] + tempmatrix[ 9] * parentmatrix[7] + tempmatrix[10] * parentmatrix[11] + tempmatrix[11] * parentmatrix[15];
						childmatrix[12] = tempmatrix[12] * parentmatrix[0] + tempmatrix[13] * parentmatrix[4] + tempmatrix[14] * parentmatrix[ 8] + tempmatrix[15] * parentmatrix[12];
						childmatrix[13] = tempmatrix[12] * parentmatrix[1] + tempmatrix[13] * parentmatrix[5] + tempmatrix[14] * parentmatrix[ 9] + tempmatrix[15] * parentmatrix[13];
						childmatrix[14] = tempmatrix[12] * parentmatrix[2] + tempmatrix[13] * parentmatrix[6] + tempmatrix[14] * parentmatrix[10] + tempmatrix[15] * parentmatrix[14];
						childmatrix[15] = tempmatrix[12] * parentmatrix[3] + tempmatrix[13] * parentmatrix[7] + tempmatrix[14] * parentmatrix[11] + tempmatrix[15] * parentmatrix[15];

						// rescale parent matrix by its own scaling matrix

						parentmatrix[ 0] *= parentscale[0];
						parentmatrix[ 1] *= parentscale[0];
						parentmatrix[ 2] *= parentscale[0];
						parentmatrix[ 4] *= parentscale[1];
						parentmatrix[ 5] *= parentscale[1];
						parentmatrix[ 6] *= parentscale[1];
						parentmatrix[ 8] *= parentscale[2];
						parentmatrix[ 9] *= parentscale[2];
						parentmatrix[10] *= parentscale[2];

						// compute bounding boxes and oriented vectors

						model->TransformBoundingBoxes();
					}
				}

			public :
				
				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Object3d_2(Object3d_2& object) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				Object3d_2 operator=(const Object3d_2& model) = delete;

				//-------------------------------------------------------------------------
				// adds a model to the current object
				
				vml::models::Model3d_2 *AddModel(const std::string &screenname,
											     const std::string &filename,
											     const glm::vec3 &pos,
											     const glm::vec3 &rot,
											     const glm::vec3 &scale,
											     const unsigned int flags)
				{

					//create resource screenname string as :  Object\Model
					
					std::string modelscreenname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(screenname);

					// check if we already have this screenname taken
					
					for (size_t i = 0; i<Models.size(); ++i)
					  if ( modelscreenname==Models[i]->GetScreenName() )
							vml::os::Message::Error("ObjectManager : ", "Object3d Instance ' ", ScreenName.c_str(), " ' : ScreenName must be unique");
					
					//Log.Out("Object", ScreenName.c_str(), vml::strings::StringFormat::Text("Adding new model to object '{0}'", filename.c_str()).c_str());

					// create a new model

					vml::models::Model3d_2 *newmodel= new vml::models::Model3d_2( modelscreenname,filename,pos,rot,scale,flags );
					
					// store this model into object's model array

					Models.emplace_back ( newmodel );
					
					return newmodel;
						
				}
				
				// -------------------------------------------------------------
				// link parts togheter
								
				void Link( const std::string &parent, const std::string &child )
				{
					if ( parent.empty() )
						vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : Source Name cannot be null" );

					if ( child.empty() )
						vml::os::Message::Error("ObjectManager : ", "Object3d Instance ' ", ScreenName.c_str(), " ' : Dest Name cannot be null" );
								
					// find root node

					std::string parentname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(parent);

					vml::models::Model3d_2* parentmodel = GetModelAt(parentname);

					if (parentmodel)
					{
						// find child node

						std::string childname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(child);

						vml::models::Model3d_2* childmodel = GetModelAt(childname);

						if (childmodel)
						{
							// check if same nodes are linked

							if (parentmodel == childmodel)
								vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : Cannot link same nodes" );

							// check for cyclic connections, this is a fatal error

							for (size_t i = 0; i < childmodel->GetChildCount(); ++i)
								if ( childmodel->GetChild(i)==parentmodel)
									vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : cyclic connections aren't allowed from ' ",
										childmodel->GetChild(i)->GetScreenName().c_str()," ' to ' " , parentname.c_str(), " '");

							// link child to parent and parent to child

							childmodel->SetParent(parentmodel);
							parentmodel->AddChild(childmodel);

		//						std::cout << "Linking : " << childmodel->GetScreenName() << " To " << parentmodel->GetScreenName() << std::endl;

						}
						else
						{
							vml::os::Message::Error("ObjectManager : ","Object3d Instance ' " , ScreenName.c_str()," ' : Cannot find child model ' ", childname.c_str(), " '");
						}

					}
					else
					{
						vml::os::Message::Error("ObjectManager : ","Object3d Instance '", ScreenName.c_str() , " ' : Cannot find parent model ' ", parentname.c_str(), " '");
					}

				}
				
				//-----------------------------------------------------------------
				// scale object

				void Scale(const glm::vec3& scaling)
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					if (fabs(scaling.x) < vml::math::EPSILON ||
						fabs(scaling.x) < vml::math::EPSILON ||
						fabs(scaling.x) < vml::math::EPSILON)
							vml::os::Message::Error("ObjectManager : ", "Object scaling factor is too small");

					// scale the entire model according to scaling factor

					for (size_t i = 0; i < Models.size(); ++i)
					{
						// scale model

						Models[i]->SetScale(Models[i]->GetScaling() * scaling);
						Models[i]->SetOriginalScale(Models[i]->GetScaling());

						// scale position

						Models[i]->SetPosition(Models[i]->GetPosition() * scaling);
						Models[i]->SetOriginalPosition(Models[i]->GetPosition());
					}

					// unscale root position

					Models[0]->SetPosition(Models[0]->GetPosition() / scaling);
					Models[0]->SetOriginalPosition(Models[0]->GetPosition());
				}

				// -------------------------------------------------------------
				// finalize object

				Object3d_2* Finalize( const glm::vec3 &pos=glm::vec3(0,0,0),
									  const glm::vec3 &rot=glm::vec3(0,0,0),
									  const glm::vec3 &scaling=glm::vec3(1,1,1) )
				{
								
					if (Models.size() == 0)
						vml::os::Message::Error("ObjectManager : ", "Object3d instance : Object ' ", ScreenName.c_str()," ' has no models attached");

					// compute stack for transformation and rendering

					ComputeStack();
				
					//	Log.Out("Object", ScreenName.c_str(), "Finalized");

					// set finalized flag to true

					vml::utils::bits32::SetToTrue(InternalFlags, vml::utils::InternalFlags::FINALIZED);

					// position root model at given postion and angles

					GetRootModel()->SetPosition(pos);
				
					// set model angles

					GetRootModel()->SetAngles(rot);

					// scale the entire model according to scaling factor
					
					Scale(scaling);
					
					// kickstart trasnformation pipeline

					Transform();
						
					// set current model as root model

					CurrentModel = 0;

					return this;
								
				}
							
				// -------------------------------------------------------------
				// transform objet to view

				void TransformToView(vml::views::View *view)
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					for (size_t i = 0; i < Models.size(); ++i)
						Models[i]->TransformToView(view);
				}
			
				// -------------------------------------------------------------
				// transform pipeline for object
				// non recursive breadth-first tree traversal

				void Transform()
				{
					
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
						
					// transfomr root model

					for (size_t i = 0; i < Models.size(); ++i)
					{
						TransformModel(Stack[i]);
					}

					// compute compound bounding box

					TransformBoundingBoxes();

					// transform colliders

				}

				//------------------------------------------------------------------
				// cull compound bounding box

				void Cull(vml::views::View* view)
				{

					switch (vml::views::frustum::TestAABBox(view->GetFrustumPlanes(), 
						                                    AABoundingBox.GetCenter(), 
						                                    AABoundingBox.GetHalfExtents()))
					{

						case vml::views::frustum::OUTSIDE :
							
								// set culling flag to outside, and all of its component models

								CullingFlags = vml::views::frustum::OUTSIDE;

								for (size_t i = 0; i < Models.size(); ++i)
									Models[i]->SetCullingFlags(vml::views::frustum::OUTSIDE);

						break;

						case vml::views::frustum::INSIDE :

								// set culling flag to inside, and all of its component models

								CullingFlags = vml::views::frustum::INSIDE;

								for (size_t i = 0; i < Models.size(); ++i)
									Models[i]->SetCullingFlags(vml::views::frustum::INSIDE);

						break;

						case vml::views::frustum::INTERSECTED :

								// set culling flag to intersected, all of its component models
								// must be checkd against frustum to determine which is inside,outside or intersected

								CullingFlags = vml::views::frustum::INTERSECTED;
								
								for (size_t i = 0; i < Models.size(); ++i)
								{
									Models[i]->SetCullingFlags(vml::views::frustum::TestAABBox(view->GetFrustumPlanes(),
															   Models[i]->GetAABoundingBox().GetCenter(),
															   Models[i]->GetAABoundingBox().GetHalfExtents()));

									/*
									switch (vml::views::frustum::TestAABBox(view->GetFrustumPlanes(),
																			Models[i]->GetAABoundingBox().GetCenter(),
																			Models[i]->GetAABoundingBox().GetHalfExtents()))
									{
										case vml::views::frustum::OUTSIDE	  : Models[i]->SetCullingFlags(vml::views::frustum::OUTSIDE);	  break;
										case vml::views::frustum::INSIDE	  : Models[i]->SetCullingFlags(vml::views::frustum::INSIDE);	  break;
										case vml::views::frustum::INTERSECTED : Models[i]->SetCullingFlags(vml::views::frustum::INTERSECTED); break;
									}
									*/
								}

						break;

					}

				}

				//------------------------------------------------------------------
				// returns if object is finalized

				bool IsFinalized() const
				{
					return vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED);
				}

				//------------------------------------------------------------------

				const std::string& GetFileName() const
				{
					return FileName;
				}

				//------------------------------------------------------------------

				size_t GetModelsCount() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return Models.size();
				}
	
				//------------------------------------------------------------------

				const std::vector<vml::models::Model3d_2*>* GetModelArray() const
				{
					return &Models;
				}

				//------------------------------------------------------------------
				// gets model by position

				vml::models::Model3d_2* GetModelAt(size_t pos) const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					return Models[pos];
				}

				const std::string& GetScreenName() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return ScreenName;
				}

				unsigned int GetInternalFlags() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return InternalFlags;
				}
		
				unsigned int GetCullingFlags() const 
				{ 
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return CullingFlags; 
				}

				unsigned int IsInFrustum() const
				{
					return CullingFlags != vml::views::frustum::OUTSIDE;
				}

				//------------------------------------------------------------------
				// gets model by screen name

				vml::models::Model3d_2* GetModelAt(const std::string& fullpathscreenname) const
				{
					for (size_t i = 0; i < Models.size(); ++i)
						if (fullpathscreenname == Models[i]->GetScreenName())
							return Models[i];
					return nullptr;
				}
	
				//------------------------------------------------------------------
				// gets root model, which is always the first model
				// position of the entire model is related to the root model

				vml::models:: Model3d_2* GetRootModel() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					return Models[0];
				}
				
				// Get axis aligned BoundingBox

				const vml::geo3d::AABBox& GetAABoundingBox() const
				{
					return AABoundingBox;
				}

				// Get axis aligned BoundingBox

				const vml::geo3d::AOBBox& GetAOBoundingBox() const
				{
					return AOBoundingBox;
				}

				// get radius

				float GetRadius() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return Radius;
				}

				// Get Forward vector

				const glm::vec3& GetForwardVector() const
				{
					return Forward;
				}

				// Get Right vector

				const glm::vec3& GetRightVector() const
				{
					return Right;
				}

				// Get Up vector

				const glm::vec3& GetUpVector() const
				{
					return Up;
				}

				// get position from model mattrix

				glm::vec3 GetTransformedRootPosition() const
				{
					const float* m = Models[0]->GetMptr();
					return glm::vec3(m[12], m[13], m[14]);
				}

				//-------------------------------------------------------------
				// get current model

				vml::models::Model3d_2 *GetCurrentModel() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// increases currentmodel index and get model's pointer

				vml::models::Model3d_2* GetNextModel() 
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					
					CurrentModel++;
					int modelscount = (int)Models.size() - 1;
					if (CurrentModel > modelscount) CurrentModel = modelscount;
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// decreases currentmodel index and get model's pointer

				vml::models::Model3d_2* GetPrevModel()
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
				
					CurrentModel--;
					if (CurrentModel <= 0) CurrentModel = 0;
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// set current model

				vml::models::Model3d_2* SetCurrentModel(size_t pos) 
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					for (CurrentModel= 0; CurrentModel < Models.size(); ++CurrentModel)
						if (CurrentModel ==pos)
							return Models[(size_t)CurrentModel];

					vml::os::Message::Error("ObjectManager : ", "Cannot find Object at pos %d",pos);
				}

				//-------------------------------------------------------------
				// specialize in your derived class
		
				virtual void Controller(vml::views::View *view)
				{

				}
				
			public:
				
				// ------------------------------------------------------------------
				// ctor / dtor

				Object3d_2(const std::string &screenname)
				{
					
					FileName		 = "";
					ScreenName       = vml::strings::StringUtils::TrimAndLower(screenname);
					InternalFlags    = vml::utils::InternalFlags::INITTED;
					CullingFlags	 = 0;
					StackCounter     = 0;
					Stack            = nullptr;
					CurrentModel	 =-1;
					 
					Right	= glm::vec3(1, 0, 0);
					Up		= glm::vec3(0, 1, 0);
					Forward = glm::vec3(0, 0, 1);

					// initialise metrics

					AABoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AOBoundingBox = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius		  = 0.0f;

					// log out

					vml::utils::Logger::GetInstance()->Info("Object : Creating new object : " + ScreenName);
					
				}

				virtual ~Object3d_2()
				{
					ReleaseAll();
					vml::utils::Logger::GetInstance()->Info("Object : Releasing object : " + ScreenName);
				}

		};

	}	// end of objects namespace

} // end of namespace vml

