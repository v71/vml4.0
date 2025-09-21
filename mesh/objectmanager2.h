#pragma once

namespace vml
{
	namespace objects
	{
		class ObjectManager_2
		{

			private:
			
				std::vector<Object3d_2*>  Objects;				// Objects array	

				// ----------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					// release object array

					for (auto it = Objects.begin(); it != Objects.end(); ++it)
						vml::os::SafeDelete(*it);
					
					// clear objects array

					Objects.clear();
				}
				
			public:
				
				// ----------------------------------------------------
				// Add object

				void AddObject(Object3d_2 *object)
				{
					// check if object pointer is null

					if (object)
					{
						if (!object->IsFinalized())
							vml::os::Message::Error("Object Manager : ","Object is not finalized");

						// duplicates names for object aren't allowed

						for (size_t i = 0; i < Objects.size(); ++i)
						{
							if (Objects[i]->GetScreenName() == object->GetScreenName())
							{
								vml::os::Message::Error("Object Manager : ","Duplicated names for objects are not allowed");
							}
						}

						// if we get here, we can store the object in the pointer vector

						Objects.emplace_back(object);

					}
					else
					{
						vml::os::Message::Error("Object Manager : ","Object pointer is null");
					}

				}
				
				// -----------------------------------------------------------------
				// retrieves objects given its name

				Object3d_2* GetObjectAt(const size_t pos) const
				{
					return Objects[pos];
				}

				// -----------------------------------------------------------------
				// get objcet by screenanme

				Object3d_2* GetObjectByName(const std::string &screenname) const
				{
					for (auto it = Objects.begin(); it != Objects.end(); ++it)
						if ((*it)->GetScreenName() == screenname) return *it;
					return nullptr;
				}

				// -----------------------------------------------------------------
				// get object array

				const std::vector<Object3d_2*>* GetObjectArray() const
				{
					return &Objects;
				}

				// -----------------------------------------------------------------
				// gets number of objects

				size_t GetObjectsCount() const
				{
					return Objects.size();
				}

				// ------------------------------------------------------------------
				// tranform object 

				void TransformObject(vml::views::View* view,vml::objects::Object3d_2* object)
				{

					// transform objects

					object->Transform();

					// cull objects

					object->Cull(view);

					// if object is in frustum, transform view objects

					if (object->GetCullingFlags() != vml::views::frustum::OUTSIDE)
					{
						object->TransformToView(view);
					}

				}

				// ----------------------------------------------------
				// call all objects controllers
				
				void CallObjectsController(vml::views::View* view)
				{
					for (size_t i = 0; i < Objects.size(); ++i)
					{
						Objects[i]->Controller(view);
					}
				}

				// ----------------------------------------------------
				// tranform objects 

				void TransformPipeline(vml::views::View* view)
				{
					for (size_t i = 0; i < Objects.size(); ++i)
					{
						TransformObject(view,Objects[i]);
					}
				}

				// -----------------------------------------------------------------
				// removes an object

				void RemoveObject(int pos)
				{
					if (pos<0 || pos>Objects.size())
						vml::os::Message::Error("Object Manager : ","Object index is out of range");

					// release object memenory 

					vml::os::SafeDelete(Objects[pos]);

					// remove from array

					Objects.erase(Objects.begin() + pos);
				}
				
				// -----------------------------------------------------------------
				// remove all objects 

				void RemoveAllObjects()
				{
					ReleaseAll();

					vml::utils::Logger::GetInstance()->Info("Object Handler : Resetting Object Handler");
				}
				
				// -----------------------------------------------------------------
				// dump objects
				
				void Dump()
				{
					for (size_t i = 0; i < Objects.size(); ++i)
					{
						std::cout << i << " : " << Objects[i]->GetScreenName() << std::endl;
					}
				}

				// ----------------------------------------------------
				// ctor / dtor

				ObjectManager_2()
				{
					vml::utils::Logger::GetInstance()->Info("Object Handler : Initting Object Handler");
				}

				~ObjectManager_2()
				{
					ReleaseAll();
					vml::utils::Logger::GetInstance()->Info("Object Handler : Releasing Object Handler");
				}

		};

	}	// end of objects namespace

} // end of namespace vml

// example of adding aon obejct

//	void AddCube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1))
//	{
//
//		// default rotation mode
//
//		int rotmode = vml::objects::Model3d_2::EULER;
//
//		// add obejcts
//
//		// generate screenname automatically
//
//		vml::objects::Object3d_2* currentobject;
//
//		// create an integer 'name' for this object
//
//		currentobject = new vml::objects::Object3d_2(vml::strings::StringConverter::GetUUID());
//
//		// add model to object
//
//		currentobject->AddModel("Root",
//								vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\meshes\\cube.3df",
//								glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
//								rotmode);
//
//		// finalize object
//
//		currentobject->Finalize(pos, rot, scaling);
//
//		// add this shit
//
//		ObjectManager->AddObject(currentobject);
//
//	}
