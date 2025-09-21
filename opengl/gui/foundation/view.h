#pragma once

namespace vml
{
	namespace views
	{
		////////////////////////////////////////////////////////////////////////////////
		// view class 
		// replace the name view with camera and you get the point

		class View
		{

			private:

				int		  ViewPortWidth;		// viewport width
				int		  ViewPortHeight;		// viewport height
				float     Aspect;				// viewport aspect
				float	  NearClippingPlane;	// near clipping plane
				float	  FarClippingPlane;		// far clipping plane
				float	  Fov;					// field of view
				glm::mat4 V;					// view matrix
				glm::mat4 P;					// projection matrix
				glm::mat4 VP;					// projection * view
				glm::vec3 OldAngles;			// euler angles
				glm::vec3 OldPosition;			// position

			public:

				// ----------------------------------------------------------------
				// public data

				glm::vec3 Angles;			// euler angles
				glm::vec3 Position;			// position

				// ----------------------------------------------------------------
				//  orbit camera
				// example : View->UpdateView(vml::GlWindow,Model->GetPosition(),40);

				void UpdateView(const float w, const float h, const glm::vec3& pos, float distance)
				{

					// Set aspect

					ViewPortWidth = w;
					ViewPortHeight = h;

					// check if view port is valid
					// this happens when window is minimized

					if (ViewPortHeight == 0)
						return;

					Aspect = (float)ViewPortWidth / (float)ViewPortHeight;

					// projection matrix

					P = glm::perspective(Fov, Aspect, NearClippingPlane, FarClippingPlane);

					float sx = sin(Angles.x);
					float sy = sin(Angles.y);
					float cx = cos(Angles.x);
					float cy = cos(Angles.y);

					glm::vec3 eye = glm::vec3(cy * sx, sy, cy * cx) * distance + pos;

					float* ptr = glm::value_ptr(V);

					glm::vec3 up(0, 1, 0);
					glm::vec3 f(glm::normalize(pos - eye));
					glm::vec3 s(glm::normalize(glm::cross(f, up)));
					glm::vec3 u(glm::cross(s, f));

					ptr[0] = s.x;
					ptr[1] = u.x;
					ptr[2] = -f.x;
					ptr[3] = 0;
					ptr[4] = s.y;
					ptr[5] = u.y;
					ptr[6] = -f.y;
					ptr[7] = 0;
					ptr[8] = s.z;
					ptr[9] = u.z;
					ptr[10] = -f.z;
					ptr[11] = 0;
					ptr[12] = -dot(s, eye);
					ptr[13] = -dot(u, eye);
					ptr[14] = dot(f, eye);
					ptr[15] = 1;

					// compute vp matrix

					VP = P * V;

					// remeber to set the view port soon after calling this function
					//	glViewport(0, 0, ViewPortWidth, ViewPortHeight);

				}

				// ----------------------------------------------------------------
				//  fps camera

				void UpdateView(const float w, const float h)
				{

					// Set aspect

					ViewPortWidth = w;
					ViewPortHeight = h;

					// check if view port is valid
					// this happens when window is minimized

					if (ViewPortHeight == 0)
						return;

					Aspect = (float)ViewPortWidth / (float)ViewPortHeight;

					// projection matrix

					P = glm::perspective(Fov, Aspect, NearClippingPlane, FarClippingPlane);

					float* ptr = glm::value_ptr(V);

					// compute view matrix

					float sx = sin(Angles.x);
					float sy = sin(Angles.y);
					float sz = sin(Angles.z);
					float cx = cos(Angles.x);
					float cy = cos(Angles.y);
					float cz = cos(Angles.z);

					ptr[0] = cx * cz;
					ptr[1] = sx * sy * cz + cy * sz;
					ptr[2] = -cy * sx * cz + sy * sz;
					ptr[3] = 0.0f;
					ptr[4] = -cx * sz;
					ptr[5] = -sx * sy * sz + cy * cz;
					ptr[6] = cy * sx * sz + sy * cz;
					ptr[7] = 0.0f;
					ptr[8] = sx;
					ptr[9] = -sy * cx;
					ptr[10] = cx * cy;
					ptr[11] = 0.0f;
					ptr[12] = -Position.x * ptr[0] - Position.y * ptr[4] - Position.z * ptr[8];
					ptr[13] = -Position.x * ptr[1] - Position.y * ptr[5] - Position.z * ptr[9];
					ptr[14] = -Position.x * ptr[2] - Position.y * ptr[6] - Position.z * ptr[10];
					ptr[15] = 1.0f;

					// direction vectors

					// compute vp matrix

					VP = P * V;

					// set viewport

			//		glViewport(0, 0, ViewPortWidth, ViewPortHeight);

				}

				// ----------------------------------------------------------------
				//

				void UpdateGlViewPort() const
				{
					glViewport(0, 0, ViewPortWidth, ViewPortHeight);
				}

				// ----------------------------------------------------------------
				// reset parameters

				void Reset()
				{
					Position = OldPosition;
					Angles = OldAngles;
				}

				// ----------------------------------------------------------------
				//  generates a ray from mouse screen position

				void UnprojectPosition(float x, float y, glm::vec3& from, glm::vec3& to) const
				{
					float my = (float)ViewPortHeight - y - 1.0f;
					glm::vec4 viewport = glm::vec4(0, 0, (float)ViewPortWidth, (float)ViewPortHeight);
					from = glm::unProject(glm::vec3(x, my, 0), V, P, viewport);
					to = glm::unProject(glm::vec3(x, my, 1), V, P, viewport);
				}

				// value getting functions

				constexpr int		   GetViewPortWidth()	const { return ViewPortWidth; }
				constexpr int		   GetViewPortHeight()	const { return ViewPortHeight; }

				//----------------------------------------------------------------
				// ctor / dtor

				View(float nearclippingplane,
					float farclippingplane,
					float fov,
					glm::vec3 position,
					glm::vec3 angles)
				{
					Position = position;												// initial position
					Angles = angles;												// euler angles
					OldPosition = position;												// needed for resetting position
					OldAngles = angles;												// needed for resetting angles
					NearClippingPlane = nearclippingplane;									// near z clipping plane
					FarClippingPlane = farclippingplane;										// far z clipping plane
					Fov = fov * vml::math::DEGTORAD;								// field of view
					Aspect = 0;													// aspect ratio
					ViewPortWidth = 0;													// view port width
					ViewPortHeight = 0;													// view port height
					V = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);			// view matrix
					P = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);			// projection matrix
					VP = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);			// projection * view
				}

				~View()
				{
					//	vml::utils::Logger::GetInstance()->Out(vml::utils::Logger::GetInstance()->GetCurrentDate(), "View", "Releasing instance of View",ScreenName.c_str());
				}

		};

	} // end of view namespace

} //end of vml namespace

