#pragma once

namespace vml
{
	namespace handlers
	{
		class Scene
		{

			private:
				
				// -------------------------------------------------------------------
				// business logic

				std::string  Name;
				uint32_t	 InternalFlags;

				// -------------------------------------------------------------------
				// scene handlers

				vml::objects::ObjectManager_2 *ObjectManager;
				vml::scenes::Level*			   Level;
		
				// -------------------------------------------------------------------
				//

				void ReleaseAll()
				{
					vml::os::SafeDelete(ObjectManager);
					vml::os::SafeDelete(Level);

					// sets data members to initial values

					Name		  = "";
					InternalFlags = 0;
				}

			public:
								
				// -------------------------------------------------------------------
				//
				
				vml::objects::Object3d_2* AddCube(const std::string &screenname,
					                              const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1),
					                              int flags= vml::models::Model3d_2::EULER)
				{
					
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");
					
					// add object

					vml::objects::Object3d_2* currentobject;
					
					// create an integer 'name' for this object

					currentobject = new vml::objects::Object3d_2(screenname);
					
					// add model to object

					currentobject->AddModel("Root",
											vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\meshes\\cube_imp\\3dfs\\cube.3df",
											vml::geo3d::NullVec, vml::geo3d::NullVec, vml::geo3d::UnaryVec,
											flags );
					
					// finalize object

					currentobject->Finalize(pos, rot, scaling);

					// add this shit

					ObjectManager->AddObject(currentobject);

					// return object pointer

					return currentobject;
				}
				
				// -------------------------------------------------------------------
				//

				vml::objects::Object3d_2* AddPlane(const std::string& scname,  
					                               const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1),
												   int flags = vml::models::Model3d_2::EULER)
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");

					// add object

					// generate screenname automatically

					vml::objects::Object3d_2* currentobject;

					// create an integer 'name' for this object

					currentobject = new vml::objects::Object3d_2(scname);

					// add model to object

					currentobject->AddModel("Root",
											vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\meshes\\plane_imp\\3dfs\\plane.3df",
											vml::geo3d::NullVec, vml::geo3d::NullVec, vml::geo3d::UnaryVec,
											flags);

					// finalize object

					currentobject->Finalize(pos, rot, scaling);

					// add this shit

					ObjectManager->AddObject(currentobject);

					// return object pointer

					return currentobject;
				}

				// -------------------------------------------------------------------
				//
				
				bool IsInitted() const
				{
					return vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED);
				}

				// -------------------------------------------------------------------
				//

				vml::objects::ObjectManager_2* GetObjectManager() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return ObjectManager;
				}

				// -------------------------------------------------------------------
				//

				vml::scenes::Level* GetLevel() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level;
				}

				// -------------------------------------------------------------------
				//

				vml::octree::OctTree* GetLevelOctTree() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level->GetOctTree();
				}

				// -------------------------------------------------------------------
				//

				vml::meshes::Mesh3d* GetLevelNavMesh() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level->GetNavMesh();
				}

				// -------------------------------------------------------------------
				//

				vml::meshes::Mesh3d* GetLevelCollisionMesh() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level->GetCollisionMesh();
				}

				// -------------------------------------------------------------------
				//

				void TransformObjects(vml::views::View* view)
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");
			
					// transfomr pipeline

					ObjectManager->TransformPipeline(view);
				}
				
				// -------------------------------------------------------------------
				//
				
				void Clear()
				{
					// release memory

					vml::os::SafeDelete(ObjectManager);
					vml::os::SafeDelete(Level);

					// reallocate classes

					ObjectManager = new vml::objects::ObjectManager_2();
					Level		  = new vml::scenes::Level();

					// set flags

					InternalFlags = vml::utils::InternalFlags::INITTED;

					// log message

					vml::utils::Logger::GetInstance()->Info("Scene : Clearing Scene : " + Name);

				}

				// -------------------------------------------------------------------
				// ctor / dtor
				
				Scene(const std::string &name)
				{
					if ( name.empty())
						vml::os::Message::Error("Scene : Name cannot be null");

					// set name

					Name = vml::strings::StringUtils::TrimAndLower(name);

					// set obejct manager

					ObjectManager = new vml::objects::ObjectManager_2();
					Level		  = new vml::scenes::Level();

					// set flags

					InternalFlags = vml::utils::InternalFlags::INITTED;

					// log out

					vml::utils::Logger::GetInstance()->Info("Scene : Initting Scene : " + Name);

				}

				virtual ~Scene()
				{
					ReleaseAll();
					vml::utils::Logger::GetInstance()->Info("Scene : Releasing Scene : " + Name);
				}
		};

	}
}
