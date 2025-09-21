#pragma once

namespace fa2040
{
	
	class Drone : public vml::objects::Object3d_2
	{
		
		private:

			int						PathCount;
			glm::vec3			   *Path;
			float				   *PathInvSegmentLength;
			float				   *PathSegmentLength;
			glm::vec3			   *PathDir;
			int						MaxPathCount;
			int						State;
			int					    PathFinderStartingCellId;
			int						AvoidPlayer;
			int						PathIndex;
			glm::vec3			    Position;
	//		fa2040::CleoCopter	   *Player;
			glm::vec3				NearestPathDestPoint;
			glm::vec3				NearestPathDir;
			glm::ivec2				StartPathA;				// Start Origin, this will be the starting path wehen the drone will return after chase state
			glm::ivec2				StartPathB;				// End Origin, this will be the starting path wehen the drone will return after chase state
			glm::vec3				StartOrigin;			// StartOriign is the current drone position which will be used when switching pathfinding to avoid jittering
			glm::ivec2				CurPathA;
			glm::ivec2				CurPathB;
			glm::vec3				CurOrigin;	
			
			vml::geo2d::PathFinder* PathFinder;
			int						NavMeshCellId;			// id of cell in the navmesh the navmesh is segmented into 'islands'
															// this value indicates in which of these islands the 
															// cleocopter belongs to see pathfinder for more info
			float					PathLenght;
			float					InvPathLenght;
			float					T;
			float					CurPath;

			float					CruiseSpeed;
			float					PrevCruiseSpeed;

			float					V0;						// initial velocity
			float				    Vmax;					// max velocity
			float					Xl;						// accellerating distance vefore reaching max velocity
			float				    Xm;						// decellerating distance vefore stopping
			float					M;						// accelleration factor
			float					DeCell;					// decelleration factor when drone stops and resume motion


	//		float					T0;						// Distance from path where acc\dec starts
	//		float					Acc;					// Accelleration whne drone arrives\leaves start\end point

	//		bool					Firing;
					
	//		int						TurnState;
			int					    StopState;
			int						AvoidState;

			int						Priority;

	//		float					Yaw;					// yaw angle
	//		float					Roll;					// roll angle
	//		float					Pitch;					// pich angle
	//		float					PrevYaw;				// store previous angular value used in bumping
	//		float					PrevPitch;				// store previous angular value used in bumping
			
	//		float					TurnSpeed;				// yaw turn speed
	//		float					RollAngleLimit;			// pitch angle limit when turining
	//		float				    ClimbAngleLimit;		// max inclination when levelling up or down
	//		float					RollDampening;			// return to 0 pitch rate
			
	//		float					RollSpeed;				// pitch decrease when reaching left or right turining limits
	//		float					DampingEps;				// eps to check if damping is coming to a rest, the lower, the latter the sate will be set to true			
			
	//		float					BumpSpeed;
	//		float					BowLimit;
	//		float					BowTau;
	//		float					ClimbSpeed;
			
	//		bool					Boost;
	//		bool					DampingIsOver;			// check when harmonic damping is nearly finished
	//		float					PreviousSpeed;

			// animation control data members

	//		float aT;
	//		float Theta0;
	//		float Theta1;
	//		float ThetaLimit;
	//		float BladesSpeed;
	//		float RotorRotationSpeed;

	//		bool UpKeyPressed;
	//		bool DownKeyPressed;
	//		bool LeftKeyPressed;
	//		bool RightKeyPressed;
		
			// ----------------------------------------------------------

			static const unsigned int NEUTRAL_POSE = 0;
			static const unsigned int LEFT_POSE = 1;
			static const unsigned int RIGHT_POSE = 2;
			static const unsigned int BUMP_RIGHT = 3;
			static const unsigned int BUMP_LEFT = 4;
			static const unsigned int BUMP_DOWN = 5;
			static const unsigned int BUMP_UP = 6;

			// ----------------------------------------------------------
			// copy path rom path finder to current path
			// we do this becasue the pathfinder act just
			// like a server, we ask for a path it computes and 
			// stays in its internal data , we need to copy 
			// that data , if we do not , the next time
			// we ask for a path, it will delete the previous
						
			bool SetRoute(const glm::vec3& origin, const glm::ivec2& a, const glm::ivec2& b)
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");

				PathIndex      = 0;
				PathCount	   = 0;
				State          = NEUTRAL;
				Position       = glm::vec3(0, 0, 0);
				PathLenght     = 0.0f;
				InvPathLenght  = 0.0f;
				T			   = 0.0f;
				CurPath		   = 0.0f;
				CruiseSpeed    = 0.0f;
		
				// compute path
				
				glm::ivec2 ia = b;
				glm::ivec2 ib = a;

				// ity could happne that the drone is in a non valid cell, if this happens
				// then we need to find the nearest valid cell or the pasth will be null
				
				if (PathFinder)
				{

					if (PathFinder->GetCellIdFromIndices(a.x, a.y) == -1) ia = PathFinder->FindNearestCellToAnotherCell(a.x, a.y, NavMeshCellId);
					if (PathFinder->GetCellIdFromIndices(b.x, b.y) == -1) ib = PathFinder->FindNearestCellToAnotherCell(b.x, b.y, NavMeshCellId);

					PathFinder->FindPath(origin, ia.x, ia.y, ib.x, ib.y);

					// if route has no path, then stay in neutral state

					if (PathFinder->GetPathCount() > 0)
					{
						PathLenght = PathFinder->GetPathLength();
						PathCount = PathFinder->GetPathCount();
						InvPathLenght = 1.0f / PathLenght;

						// copy path , the pathfinder acts as a server
						// so each drone has its own path

						for (size_t i = 0; i < PathCount; ++i)
						{
							Path[i] = PathFinder->GetPAvgrOut()[i];
							PathDir[i] = PathFinder->GetPAvgrDir()[i];
							PathInvSegmentLength[i] = PathFinder->GetPAvgrInvSegmentLength()[i];
							PathSegmentLength[i] = PathFinder->GetPAvgrSegmentLength()[i];
						}

						Position = Path[0];
						GetRootModel()->SetPosition(Position);
						return true;
					}

				}

				return false;
			}
			
		public:
			
			// ----------------------------------------------------------
			//

	//		float ChaseSpeed;

	//		vml::models::Model3d_2* BodyModel;
	//		vml::models::Model3d_2* LowerBodyModel;
	//		vml::models::Model3d_2* SupportShaftModel;
	//		vml::models::Model3d_2* Rotor1Model;
	//		vml::models::Model3d_2* Rotor2Model;
	//		vml::models::Model3d_2* GunPivotModel;
	//		vml::models::Model3d_2* GunModel;
	//		vml::models::Model3d_2* Blade1Model;
	//		vml::models::Model3d_2* Blade2Model;
		//	ObjectFrustm			 Frustum;

			// ----------------------------------------------------------
			//

			static const unsigned int NEUTRAL               = 0;
			static const unsigned int START_PATH            = 1;
			static const unsigned int START_FORWARD_PATROL  = 2;
			static const unsigned int START_BACKWARD_PATROL = 3;
			static const unsigned int CHASE_PLAYER          = 4;
			static const unsigned int STOP_CHASE            = 5;
			static const unsigned int GO_BACK_TO_ROUTE      = 6;
			
			static const unsigned int STOP_STATE_0 = 0;
			static const unsigned int STOP_STATE_1 = 1;
			static const unsigned int STOP_STATE_2 = 2;
			static const unsigned int STOP_STATE_3 = 3;
			static const unsigned int STOP_STATE_4 = 4;

			static const unsigned int AVOID_STATE_0 = 0;
			static const unsigned int AVOID_STATE_1 = 1;

			// ----------------------------------------------------------

			JPH::Hit WallHit;
			bool CastRay;

			// ----------------------------------------------------------
			// this is used when the drone needs a fixed patrolling route
			// if you need to change the route in realtime use changerout

			bool SetInitialRoute(const glm::vec3& origin, const glm::ivec2& a, const glm::ivec2& b)
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");

				StartPathA = a;
				StartPathB = b;
				StartOrigin = origin;
				CurPathA = a;
				CurPathB = b;
				CurOrigin = origin;
				if (PathFinder)
				{
					PathFinderStartingCellId = PathFinder->GetCellIdFromIndices(StartPathA.x, StartPathA.y);
					return SetRoute(StartOrigin, StartPathA, StartPathB);
				}
				return false;
			}
			
			// -------------------------------------------------------------------
			//
			
			glm::ivec2 GetNavMeshCell()
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");

				glm::vec3 p = GetRootModel()->GetPosition();
		
				if (PathFinder)
				{
					glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(GetRootModel()->GetPosition());
					if (PathFinder->GetCellIdFromIndices(ci.x, ci.y) == -1)
					{
						// NavMeshCellId its the isle in the navmesh bitmap where originally the drone had started
						return PathFinder->FindNearestCellToAnotherCell(ci.x, ci.y, NavMeshCellId);
					}
					return ci;
				}
				return glm::ivec2(-1, -1);
			}

			/*
			// ----------------------------------------------------------
			//

			void DrawPath(vml::views::View* view)
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");
				for (int i = 0; i < PathCount - 1; ++i)
					vml::debugrendering::OpenglDebugRender2::GetInstance()->DrawLine(view, Path[i], Path[i + 1], vml::colors::White, true);
			}
			*/

			// ----------------------------------------------------------
			//

			void StartCourse()			 { State = START_PATH; }
			void StopChase()			 { State = STOP_CHASE; }
			void ChasePlayer()			 { State = CHASE_PLAYER; }
			void SetAvoidPlayerToTrue()  { AvoidPlayer = 1; }
			void SetAvoidPlayerToFalse() { AvoidPlayer = 0; }
			void StopCourse()			 { StopState = STOP_STATE_1; }
			void ResumeCourse()			 { StopState = STOP_STATE_4; }
			void SetAvoidStateToFalse()  { AvoidState = AVOID_STATE_0; }
			void SetAvoidStateToTrue()	 { AvoidState = AVOID_STATE_1; }

			// ----------------------------------------------------------
			//

			bool		 IsFollowingPath()  const { return State == START_FORWARD_PATROL || State == START_BACKWARD_PATROL; }
			bool		 IsGoingBack()	    const { return State == STOP_CHASE || State == GO_BACK_TO_ROUTE; }
			bool		 IsChasingPlayer()  const { return State == CHASE_PLAYER; }
			bool		 IsNeutral()	    const { return State == NEUTRAL; }
			int			 GetPriority()	    const { return Priority; }
			int			 GetNavMeshCellId() const { return NavMeshCellId; }
			bool		 IsAvoiding()		const { return AvoidState==AVOID_STATE_1; }

			// ----------------------------------------------------------
			//

			void PrintPath() const
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");

				for (int i = 0; i < PathCount; ++i)
					std::cout << "AddPoint ( " << Path[i].x << " , " << Path[i].y << " );" << std::endl;
				std::cout << std::endl;
			}

			// ----------------------------------------------------------
			//

			void ComputeSpeed2(const float d)
			{
				// cruise speed, accelle / decell control
			
				if (StopState == STOP_STATE_0)
				{
					if (d < Xl) CruiseSpeed = V0 + d * M;
					else if (d > Xm)
						CruiseSpeed = Vmax - (d - Xm) * M;
					else CruiseSpeed = Vmax;
				}
				
				// if stop state is 1 , the decelleration is
				// taking place, this state saves the 
				// current speed used to resume the motion once
				// requested

				if (StopState == STOP_STATE_1)
				{
					PrevCruiseSpeed = CruiseSpeed;
					StopState = STOP_STATE_2;
				}
				
				// decelleration procedure
				// once hte drone is stopped , the 
				// state is set to a neutral state

				if (StopState == STOP_STATE_2)
				{
					CruiseSpeed -= DeCell;
					if (CruiseSpeed < 0)
					{
						CruiseSpeed = 0;
						StopState = STOP_STATE_3;
					}
				}

				// in case the motion is resumed
				// the dorne is set in motion
				// until the previous speed is 
				// saved in the 'stop state 1' is achieved 

				if (StopState == STOP_STATE_4)
				{
					CruiseSpeed += DeCell;
					if (CruiseSpeed > PrevCruiseSpeed)
					{
						CruiseSpeed = PrevCruiseSpeed;
						StopState = STOP_STATE_0;
					}
				}

//				std::cout << d << " " << Xl << std::endl;
			}

			// ----------------------------------------------------------
			//
			
			void Controller(vml::views::View* view)
			{
				 
			//	std::cout << "called controller for object " << GetRootModel()->GetScreenName() << std::endl;


			//	Accelleration = ForceDirection * ForceMagnitude / Mass - Velocity*1.5f;
			//	Velocity += Accelleration;
			//	GetRootModel()->Move(Velocity);

				/*
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");
				
				std::cout << "called controller for object " << GetRootModel()->GetScreenName() << std::endl;
				
				// start path

				if (State == START_PATH)
				{
					
					//	std::cout << "Controller for object :" << GetScreenName() << std::endl;

					SetRoute(CurOrigin, CurPathA, CurPathB);

					// start path state

					State		    = START_FORWARD_PATROL;
					StopState	    = STOP_STATE_0;
					T			    = 0.0f;						// T is initialize in the SetRoute function
					CurPath		    = 0.0f;						// CurPath is initialize in the SetRoute function
					CruiseSpeed     = 0.0f;
					PrevCruiseSpeed = 0.0f;
					PathIndex       = 0;

//					std::cout << "path started" << std::endl;
				}

				// path to end point

				if (State == START_FORWARD_PATROL)
				{
					
					Position = Path[PathIndex] + T * PathDir[PathIndex];

					GetRootModel()->SetPosition(Position);

					// step 'time' 

					T += CruiseSpeed * PathInvSegmentLength[PathIndex];

					// decelleration \ accellereation when reaching 
					// initial or final position 

					ComputeSpeed2(CurPath);

					if (T > 1.0f)
					{
						CurPath += PathSegmentLength[PathIndex]  * InvPathLenght;
						PathIndex++;
						T = 0.0f;
				
						if (PathIndex > PathCount - 2)
						{
							PathIndex = PathCount - 1;
							Position = Path[PathIndex];
							GetRootModel()->SetPosition(Position);
							State = START_BACKWARD_PATROL;
						//	State = NEUTRAL;
							CruiseSpeed = 0.0f;
							CurPath = 1.0f;
						}
					}
					
				}

				// path to start point

				if (State == START_BACKWARD_PATROL)
				{
					
		            int j = PathIndex - 1;

					Position = Path[PathIndex] - T * PathDir[j];

					GetRootModel()->SetPosition(Position);

					// step 'time' 

					T += CruiseSpeed * PathInvSegmentLength[j];

					// decelleration \ accellereation when reaching 
					// initial or final position 
	
					ComputeSpeed2(CurPath);

					if (T > 1.0f)
					{
						CurPath -= PathSegmentLength[j] * InvPathLenght;
						PathIndex--;
						T = 0.0f;

						if (PathIndex < 1)
						{
							PathIndex = 0;
							Position = Path[PathIndex];
							GetRootModel()->SetPosition(Position);
							State = START_FORWARD_PATROL;
							CruiseSpeed = 0.0f;
							CurPath = 0.0f;
						}
						
					}
					
				}
			
				// chase player 
				
				if (State == CHASE_PLAYER)
				{
					
					float slowingradius = 5.0f;
					float spehreepsilon = 1.5f;

					Position = GetRootModel()->GetPosition();

					glm::vec3 dist = Player->GetRootModel()->GetPosition() - Position;
					float distance = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z*dist.z);

					if (distance > slowingradius)
					{
						float dv = (distance - slowingradius) ;
						glm::vec3 newvelocity = dist * ChaseSpeed * dv / distance;
						newvelocity.z = 0.0f;
						GetRootModel()->Move(newvelocity);
					}
					else
					{
						GetRootModel()->MoveY(dist.y* ChaseSpeed*2.0f / distance);

						float ri = GetRadius() * spehreepsilon;
						float rj = Player->GetRadius() * spehreepsilon;
						float disp = distance - ri - rj;

						if (disp < 0)
						{
							glm::vec3 d = dist * disp / distance;
							d.z = 0;
							GetRootModel()->Move(d);
						}
					}
				
				}
				
				// stops chase and go back to starting route

				if (State == STOP_CHASE)
				{
					
					// its possible that the path is null 
					// this never happens , but its a possibility
					// in that case the drone will not move

					if (SetRoute(GetRootModel()->GetPosition(), GetNavMeshCell(), StartPathA))
					{
						State = GO_BACK_TO_ROUTE;
					}
					
				}

				// go back after stopping chase to starting route to replay initial route

				if (State == GO_BACK_TO_ROUTE)
				{
					
					Position.x = Path[PathIndex].x + T * PathDir[PathIndex].x;
					Position.y = Path[PathIndex].y + T * PathDir[PathIndex].y;
					Position.z = Path[PathIndex].z + T * PathDir[PathIndex].z;

					GetRootModel()->SetPosition(Position);

					T += CruiseSpeed * PathInvSegmentLength[PathIndex];

					// decelleration \ accellereation when reaching 
					// initial position 
					
				//	std::cout << CurPath << std::endl;

					if (CurPath < T0) CruiseSpeed += Acc;
					if (CurPath > 1 - T0) CruiseSpeed -= Acc;

					if (T > 1.0f)
					{
						CurPath += PathSegmentLength[PathIndex] / PathLenght;
						PathIndex++;
						T = 0.0f;

						if (PathIndex > PathCount - 2)
						{
							PathIndex = PathCount - 1;
							Position.x = Path[PathIndex].x;
							Position.y = Path[PathIndex].y;
							
							Position.z = Path[PathIndex].z;

							GetRootModel()->SetPosition(Position);
							CurOrigin = Position;
							State = START_PATH;
						}
					}
					
				}

				// recoil position

			//	glm::vec3 q = GetRootModel()->GetPosition();
			//	float recoilspeed = 0.05f;
			//	if (GetRootModel()->GetPosition().z < 0)
			//		GetRootModel()->SetPosition(glm::vec3(q.x, q.y, q.z + recoilspeed));
			*/
			}
			
			// ----------------------------------------------------------
			// attach pathfinder to drone

			void AttachPathFinder(vml::geo2d::PathFinder* pathfinder, const glm::vec3& pos)
			{
				// attach pathfinder server

				if (pathfinder)
				{
					PathFinder			 = pathfinder;
					MaxPathCount		 = PathFinder->GetSize();
					Path				 = new glm::vec3[MaxPathCount];
					PathDir			     = new glm::vec3[MaxPathCount];;
					PathInvSegmentLength = new float[MaxPathCount];
					PathSegmentLength	 = new float[MaxPathCount];

					// * Important *
					// get copter position to retrieve navmesh cell index 
					// for the pathfiunder in the case due to navmesh segmentation
					// the player could intercept a non traversable cell 
					// this interrupting the chase function for drones
					// each isolated region in the navmesh bitmap has a value
					// assigned, we need to know in which of these region 

					glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(pos);
					NavMeshCellId = PathFinder->GetCellIdFromIndices(ci.x, ci.y);

				}
				else
				{
					vml::os::Message::Error("Drone :", "Null PathFinder (2)");
				}
			}
			
			// ----------------------------------------------------------
			//

			void CreateObject(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling)
			{
				// add model along with its relative position, rotation and scaling
				// default rotation mode

				int flags = vml::models::Model3d_2::POLAR;

				AddModel("Base",
						 vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\meshes\\cube_imp\\3dfs\\cube.3df",
						 glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
						 flags);

				// finalize object

				Finalize(pos, rot, scaling);

				/*
				// add model along with its relative position, rotation and scaling
				// default rotation mode

			//	int flags = vml::objects::Model3d_2::EULER;

			//	BodyModel = AddModel("Body",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/upperbody.3df",
			//		glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		vml::objects::Model3d_2::POLAR);

			//	LowerBodyModel = AddModel("LowerBody",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/lowerbody.3df",
			//		glm::vec3(1.0f, -6.2f, 4.6f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	SupportShaftModel = AddModel("SupportShaft",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/supports.3df",
			//		glm::vec3(0.0f, 0.8f, -4.5f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	Rotor1Model = AddModel("Rotor1",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/rotor1.3df",
			//		glm::vec3(-11.5f, 1.2f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
					flags);

			//	Rotor2Model = AddModel("Rotor2",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/rotor2.3df",
			//		glm::vec3(11.5f, 1.2f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	Blade1Model = AddModel("Blade1",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/blade.3df",
			//		glm::vec3(0.0f, 0.2f, 0.8f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	Blade2Model = AddModel("Blade2",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/blade.3df",
			//		glm::vec3(0, -0.2f, 0.8f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	GunPivotModel = AddModel("GunPivot",
			//		sourcepath + "/content/fa2040/meshes/cleocopter2/newcleocopter16_imp/3dfs/gunpivot.3df",
			//		glm::vec3(0.3f, -8.6f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

			//	GunModel = AddModel("Gun",
			//		sourcepath + "/content/fa2040/meshes/drones/finaldrone16_imp/3dfs/gun.3df",
			//		glm::vec3(0.0f, -0.4f, 5.6f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
			//		flags);

				// link models

			//	Link("Body", "LowerBody");
			//	Link("Body", "SupportShaft");
			//	Link("Body", "Rotor1");
			//	Link("Body", "Rotor2");
			//	Link("Body", "GunPivot");
			//	Link("GunPivot", "Gun");
			//	Link("Rotor1", "Blade1");
			//	Link("Rotor2", "Blade2");

				// finalize object

			//	Finalize(pos, rot, scaling);

				// attach frustum

			//	Frustum.AttachToModel(BodyModel);
			//	Frustum.RelativePosition = glm::vec3(0, -2, 0);

				// add ellipsoid collider

			//	GetRootModel()->AddEllipsoidCollider(glm::vec3(2, 1.5f, 1));
				*/
			}
			
			//------------------------------------------------------------------
			// physics getters
			
			float	   GetMass() const { return Mass; }
			void	   SetMass(float mass) { Mass = mass; }
			float	   GetForceMagnitude() const { return ForceMagnitude; }
			float	   GetRBForceMagnitude() const { return RbForceMagnitude; }
			glm::vec3& GetForceDirection() { return ForceDirection; }
			glm::vec3& GetRBForceDirection() { return RbForceDirection; }
			glm::vec3& GetAccelleration() { return Accelleration; }
			glm::vec3& GetVelocity() { return Velocity; }
			float	   GetRestitutionFactor() const { return RestitutionFactor; }
		
			//-------------------------------------------------------------

			void ResetForces()
			{
				Accelleration    = glm::vec3(0, 0, 0);
				Velocity         = glm::vec3(0, 0, 0);
				ForceDirection   = glm::vec3(0, 0, 0);
				RbForceDirection = glm::vec3(0, 0, 0);
			}

			void SetForceMagnitude(float force)
			{
				ForceMagnitude = force;
			}

			void SetForceDirection(const glm::vec3& dir)
			{
				ForceDirection = dir;
			}

			void AddForceDirection(const glm::vec3& dir)
			{
				ForceDirection += dir;
			}

			glm::vec3				RbForceDirection;		// resistive viscous force direction
			float					RbForceMagnitude;		// resistive viscous force magnitude
			glm::vec3				ForceDirection;			// resultant force direction
			float					ForceMagnitude;			// resultant force magnitude
			glm::vec3				Velocity;				// velocity
			glm::vec3				Accelleration;			// accelleration
			float					Mass;					// Newtonian Mass
			float					RestitutionFactor;		// Restitution factor

			// ----------------------------------------------------------
			//

			Drone(const std::string& screenname, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling, int priority) : vml::objects::Object3d_2(screenname)
			{
				//	Player					 =  nullptr;
				PathFinder				 =  nullptr;
				Path					 =  nullptr;
				PathInvSegmentLength	 =  nullptr;
				PathSegmentLength		 =  nullptr;
				PathDir					 =  nullptr;
				PathCount				 =  0;
				MaxPathCount			 =  0;
				State					 =  NEUTRAL;
				PathIndex				 = -1;
				PathFinderStartingCellId = -1;
				Priority				 =  priority;
				AvoidPlayer				 =  0;
				StartPathA				 =  glm::ivec2(-1, -1);
				StartPathB				 =  glm::ivec2(-1, -1);
				CurPathA				 =  glm::ivec2(-1, -1);
				CurPathB				 =  glm::ivec2(-1, -1);

				Position				 =  glm::vec3(0, 0, 0);
				/* attenzione qui l'ho sovrascritto per testare la dinamica*/
				Position = pos;

				NearestPathDestPoint	 =  glm::vec3(0, 0, 0);
				NearestPathDir			 =  glm::vec3(0, 0, 0);
				
				StartOrigin				 =  glm::vec3(0, 0, 0);
				CurOrigin				 =  glm::vec3(0, 0, 0);
				CruiseSpeed				 =  0.0f;
				PrevCruiseSpeed			 =  0.0f;
			//	ChaseSpeed				 =  0.05f;
			//	PreviousSpeed			 =  0.0f;

				NavMeshCellId			 = -1;
				PathLenght				 =  0.0f;
				InvPathLenght			 =  0.0f;
				T						 =  0.0f;

				V0						 =  0.01f;
				Vmax					 =  0.05f;
				Xl						 =  0.1f;
				Xm						 =  1 - Xl;
				M						 =  (Vmax - V0) / Xl;
				DeCell					 =  0.00125f;
				
				ForceMagnitude   =  0.005f;
				RbForceMagnitude =  0.001f;
				Mass			 =  1.0f;
				ForceDirection   =  glm::vec3(0, 0, 0);
				RbForceDirection =  glm::vec3(0, 0, 0);
				Accelleration    =  glm::vec3(0, 0, 0);
				Velocity		 =  glm::vec3(0, 0, 0);
				RestitutionFactor = 1.0f;

				//	
			//	T0						 =  0.2f;
			//	Acc						 =  1.0f / 1000.0f;
				CurPath					 =  0.0f;
			//	Firing					 =  false;

				CastRay = false;
				WallHit = {};

				//
				
			//	UpKeyPressed    = false;
			//	DownKeyPressed  = false;
			//	LeftKeyPressed  = false;
			//	RightKeyPressed = false;
				
			//	Boost = false;

			//	Yaw       = 0.0f;
			//	Roll      = 0.0f;
			//	Pitch     = 0.0f;
			//	PrevYaw   = 0.0f;
			//	PrevPitch = 0.0f;
						
			//	DampingIsOver = 0;

			//	TurnSpeed	  = 0.2f * 16.0f;
			//	RollDampening = 0.9f;
				
			//	ClimbSpeed = 0.1f * 16.0f;
			//	RollSpeed  = 0.2f * 16.0f;
				
			//	BumpSpeed = 0.05f;
				
			//	RollAngleLimit	   = 45.0f;			// maximum pitch angle
			//	ClimbAngleLimit	   = 25.0f;		// Angle limit when climbing up or down
			//	BowLimit		   = -15.0f;
			//	BowTau			   = 100.0f;
			//	DampingEps		   = 0.01f;
			//	RotorRotationSpeed = 0.05f;
							
			//	aT = 0.0f;
							
			//	TurnState = NEUTRAL_POSE;
				StopState  = STOP_STATE_0;
				AvoidState = AVOID_STATE_1;

			//	Theta0 = 0.0f;
			//	Theta1 = 0.0f;
			//	ThetaLimit = 45.0f;
			//	BladesSpeed = 5.0f;

			//	BodyModel		  = nullptr;
			//	LowerBodyModel    = nullptr;
			//	SupportShaftModel = nullptr;
			//	Rotor1Model       = nullptr;
			//	Rotor2Model       = nullptr;
			//	GunPivotModel     = nullptr;
			//	GunModel          = nullptr;
			//	Blade1Model       = nullptr;
			//	Blade2Model       = nullptr;
				//	ObjectFrustm			 Frustum;

				//
				
				// create object

				CreateObject(pos, rot, scaling);

				// add ellipsoid collider

			//	GetRootModel()->AddEllipsoidCollider(glm::vec3(2, 1.5f, 1));

				// attach player

			//	if (player)
			//	{
			//		Player = player;
			//	}
			//	else
			//	{
			//		vml::os::Message::Error("Drone : ", "Null Player");
			//	}
			}

			~Drone()
			{
				vml::os::SafeDeleteArray(Path);
				vml::os::SafeDeleteArray(PathDir);
				vml::os::SafeDeleteArray(PathInvSegmentLength);
				vml::os::SafeDeleteArray(PathSegmentLength);
			}
		
	};

}
