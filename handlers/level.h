#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////

#include <vml4.0/math/2d/pathfinder.h>

namespace vml
{
	namespace scenes
	{

		// -------------------------------------------------------------------
		// Level class, this class holds everything about 
		// the level mesh, collision and navigation data

		class Level
		{

			private:

				std::string				   LevelName;
				std::string			       MainPath;
				std::string			       MapMeshFileName;
				std::string			       ColMeshFileName;
				std::string			       NavMeshFileName;
				std::string			       NavMaskFileName;
				uint32_t				   InternalFlags;
				vml::octree::OctTree*      OctTree;
				vml::geo2d::PathFinder*    PathFinder;
				vml::meshes::Mesh3d*       MapMesh;
				vml::meshes::Mesh3d*       CollisionMesh;
				vml::meshes::Mesh3d*       NavMesh;

				// -------------------------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					vml::os::SafeDelete(MapMesh);
					vml::os::SafeDelete(CollisionMesh);
					vml::os::SafeDelete(NavMesh);
					vml::os::SafeDelete(OctTree);
					vml::os::SafeDelete(PathFinder);

					// sets data members to initial values

					LevelName		= "";
					MainPath		= "";
					MapMeshFileName = "";
					ColMeshFileName = "";
					NavMeshFileName = "";
					InternalFlags	=  0;

				}
				
			public:

				// -------------------------------------------------------------------
				// 
				
				bool Load(const std::string& levelname)
				{
					
					if (levelname.empty())
						vml::os::Message::Error("Level : ", "FileName is empty");

					// reset data

					ReleaseAll();

					// set filname

					LevelName = levelname;
					MainPath = vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "\\levels\\" + LevelName;
					MapMeshFileName = MainPath + "\\" + LevelName + ".3df";
					ColMeshFileName = MainPath + "\\" + LevelName + "_col.3df";
					NavMeshFileName = MainPath + "\\" + LevelName + "_nav.3df";
					NavMaskFileName = MainPath + "\\" + LevelName + "_nav_mask.nvm";

					vml::utils::Logger::GetInstance()->Info("Level : Loading Level : " + MainPath);
					vml::utils::Logger::GetInstance()->Info("Level : MapMesh : "	   + MapMeshFileName);
					vml::utils::Logger::GetInstance()->Info("Level : CollisionMesh : " + ColMeshFileName);
					vml::utils::Logger::GetInstance()->Info("Level : NavMesh : "	   + NavMeshFileName);

					// allocate meshes

					MapMesh = new vml::meshes::Mesh3d(MapMeshFileName, { vml::meshes::Mesh3d::RETAIN_DATA });
					
					// Collison mesh and navmesh are not mandatory
					
					// load collision mesh

					if (std::filesystem::exists(ColMeshFileName)) 
					{
						vml::utils::Logger::GetInstance()->Info("Level : Loading Collison Mesh : " + ColMeshFileName);
						CollisionMesh = new vml::meshes::Mesh3d(ColMeshFileName, { vml::meshes::Mesh3d::RETAIN_DATA });
					}

					// load nav mesh, if nav mesh exists , create pathfinder

					if (std::filesystem::exists(NavMeshFileName)) 
					{
						vml::utils::Logger::GetInstance()->Info("Level : Loading Nav Mesh : " + NavMeshFileName);
						NavMesh = new vml::meshes::Mesh3d(NavMeshFileName, {});
						// log out mesh loading
						vml::utils::Logger::GetInstance()->Info("Level : Loading MapMesh : " + MapMeshFileName);
						// load bitmap mask for pathfinding 
						PathFinder = new vml::geo2d::PathFinder(NavMaskFileName);
					}
					
					// compile level

					vml::utils::Logger::GetInstance()->Info("Level : Octree Compiling : " + LevelName);
					
					// create octree

					OctTree = new vml::octree::OctTree;
					
					// compile map

					OctTree->Go(MapMesh);

					// log out
					
					vml::utils::Logger::GetInstance()->Info("Level : Octree Compiling : " + LevelName +" : Done");

					// we can release map mesh data since we don't use it anymore once the octree is computed

					MapMesh->ReleaseVertexBufferData();
					
					// log out

					vml::utils::Logger::GetInstance()->Info("Level : Loading Level : Done");
					
					// set flags

					InternalFlags = vml::utils::InternalFlags::INITTED;
					
					return true;
				}

				
				// -------------------------------------------------------------------
				// getters

				vml::octree::OctTree      *GetOctTree()	          const { return OctTree; }
				vml::meshes::Mesh3d       *GetMapMesh()	          const { return MapMesh; }
				vml::meshes::Mesh3d       *GetCollisionMesh()     const { return CollisionMesh; }
				vml::meshes::Mesh3d       *GetNavMesh()           const { return NavMesh; }
				vml::geo2d::PathFinder    *GetPathFinder()	      const { return PathFinder; }
				const std::string         &GetLevelName()	      const { return LevelName; }
				const std::string         &GetMapMeshFileName()	  const { return MapMeshFileName; }
				const std::string         &GetColMeshFileName()	  const { return ColMeshFileName; }
				const std::string         &GetNavMeshFileName()	  const { return NavMeshFileName; }

				// -------------------------------------------------------------------
				//

				bool IsInitted() const
				{
					return vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED);
				}

				// -------------------------------------------------------------------
				// ctor / dtor

				Level()
				{
					InternalFlags  = 0;
					MapMesh        = nullptr;
					CollisionMesh  = nullptr;
					NavMesh		   = nullptr;
					OctTree        = nullptr;
					PathFinder	   = nullptr;
					vml::utils::Logger::GetInstance()->Info("Level : Initting Level");
				}

				~Level()
				{
					ReleaseAll();
					vml::utils::Logger::GetInstance()->Info("Level : Releasing Level : " + LevelName);
				}

		};

	}
}
