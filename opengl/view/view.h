#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

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
						
						int			 ViewPortWidth;		// viewport width
						int			 ViewPortHeight;	// viewport height
						float        Aspect;			// viewport aspect
						float		 NearClippingPlane;	// near clipping plane
						float		 FarClippingPlane;	// far clipping plane
						float		 Radius;			// frustum radius
						float		 Fov;				// field of view
						glm::vec3	 Right;				// right vector
						glm::vec3	 Direction;			// direction
						glm::vec3	 Up;				// up vector
						glm::mat4    V;					// view matrix
						glm::mat4    P;					// projection matrix
						glm::mat4    VP;				// projection * view
						glm::vec3    OldAngles;			// euler angles
						glm::vec3    OldPosition;		// position
						glm::vec4    Planes[6];			// frustum planes
						glm::quat    Quaternion;		// quaternion
						unsigned int Flags;				// flags
						std::string  ScreenName;		// screenname
						
					public:
						
						// ----------------------------------------------------------------
						// rotation type flags

						static const unsigned int QUATERNIONS = vml::utils::bits32::BIT0;
						static const unsigned int EULER		  = vml::utils::bits32::BIT1;
						
						// ----------------------------------------------------------------
						// public data

						float	  RotationSpeed;	// rotation speed
						float	  TranslationSpeed;	// translation speed
						glm::vec3 Angles;			// euler angles
						glm::vec3 Position;			// position
						
						// ----------------------------------------------------------------
						//  orbit camera
						// example : View->UpdateView(window->width,window->height,Model->GetPosition(),40);
						// remeber to set the view port soon after calling this function

						void UpdateView(const float w, const float h, const glm::vec3 &pos, float distance)
						{

							// Set aspect

							ViewPortWidth  = w;
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

							glm::vec3 eye = glm::vec3(cy*sx, sy, cy*cx)*distance + pos;

							float *ptr = glm::value_ptr(V);

							glm::vec3 up(0, 1, 0);
							glm::vec3 f(glm::normalize(pos - eye));
							glm::vec3 s(glm::normalize(glm::cross(f, up)));
							glm::vec3 u(glm::cross(s, f));
							
							ptr[ 0] =  s.x;
							ptr[ 1] =  u.x;
							ptr[ 2] = -f.x;
							ptr[ 3] =  0;
							ptr[ 4] =  s.y;
							ptr[ 5] =  u.y;
							ptr[ 6] = -f.y;
							ptr[ 7] =  0;
							ptr[ 8] =  s.z;
							ptr[ 9] =  u.z;
							ptr[10] = -f.z;
							ptr[11] =  0;
							ptr[12] = -dot(s, eye);
							ptr[13] = -dot(u, eye);
							ptr[14] =  dot(f, eye);
							ptr[15] =  1;
											
							// extract frustum planes

							ptr = glm::value_ptr(VP);

							Right.x = ptr[0];
							Right.y = ptr[4];
							Right.z = ptr[8];

							Up.x = ptr[1];
							Up.y = ptr[5];
							Up.z = ptr[9];

							Direction.x = ptr[2];
							Direction.y = ptr[6];
							Direction.z = ptr[10];

							// compute vp matrix

							VP = P * V;

							// Left clipping plane

							Planes[0].x = ptr[ 3] + ptr[ 0];
							Planes[0].y = ptr[ 7] + ptr[ 4];
							Planes[0].z = ptr[11] + ptr[ 8];
							Planes[0].w = ptr[15] + ptr[12];

							// Right clipping plane

							Planes[1].x = ptr[ 3] - ptr[ 0];
							Planes[1].y = ptr[ 7] - ptr[ 4];
							Planes[1].z = ptr[11] - ptr[ 8];
							Planes[1].w = ptr[15] - ptr[12];

							// Bottom clipping plane

							Planes[2].x = ptr[ 3] + ptr[ 1];
							Planes[2].y = ptr[ 7] + ptr[ 5];
							Planes[2].z = ptr[11] + ptr[ 9];
							Planes[2].w = ptr[15] + ptr[13];

							// Bottom clipping plane

							Planes[3].x = ptr[ 3] - ptr[ 1];
							Planes[3].y = ptr[ 7] - ptr[ 5];
							Planes[3].z = ptr[11] - ptr[ 9];
							Planes[3].w = ptr[15] - ptr[13];

							// Near clipping plane

							Planes[4].x = ptr[ 3] + ptr[ 2];
							Planes[4].y = ptr[ 7] + ptr[ 6];
							Planes[4].z = ptr[11] + ptr[10];
							Planes[4].w = ptr[15] + ptr[14];

							// Far clipping plane

							Planes[5].x = ptr[ 3] - ptr[ 2];
							Planes[5].y = ptr[ 7] - ptr[ 6];
							Planes[5].z = ptr[11] - ptr[10];
							Planes[5].w = ptr[15] - ptr[14];

							// Normalize the plane equations

							float d;

							d = sqrtf(Planes[0].x*Planes[0].x + Planes[0].y*Planes[0].y + Planes[0].z*Planes[0].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							d = 1.0f / d;

							Planes[0].x *= d;
							Planes[0].y *= d;
							Planes[0].z *= d;
							Planes[0].w *= d;

							d = sqrtf(Planes[1].x*Planes[1].x + Planes[1].y*Planes[1].y + Planes[1].z*Planes[1].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							d = 1.0f / d;

							Planes[1].x *= d;
							Planes[1].y *= d;
							Planes[1].z *= d;
							Planes[1].w *= d;

							d = sqrtf(Planes[2].x*Planes[2].x + Planes[2].y*Planes[2].y + Planes[2].z*Planes[2].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[2].x *= d;
							Planes[2].y *= d;
							Planes[2].z *= d;
							Planes[2].w *= d;

							d = sqrtf(Planes[3].x*Planes[3].x + Planes[3].y*Planes[3].y + Planes[3].z*Planes[3].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[3].x *= d;
							Planes[3].y *= d;
							Planes[3].z *= d;
							Planes[3].w *= d;

							d = sqrtf(Planes[4].x*Planes[4].x + Planes[4].y*Planes[4].y + Planes[4].z*Planes[4].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[4].x *= d;
							Planes[4].y *= d;
							Planes[4].z *= d;
							Planes[4].w *= d;

							d = sqrtf(Planes[5].x*Planes[5].x + Planes[5].y*Planes[5].y + Planes[5].z*Planes[5].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[5].x *= d;
							Planes[5].y *= d;
							Planes[5].z *= d;
							Planes[5].w *= d;

							// minimum bounding sphere of a view frustum
							// https://lxjk.github.io/2017/04/15/Calculate-Minimal-Bounding-Sphere-of-Frustum.html

							d = ViewPortWidth * ViewPortWidth;
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = vml::math::EPSILON;
							else d = 1.0f / d;

							float k1 = tanf(Fov*0.5f);
							float k2 = (1.0f + (ViewPortHeight * ViewPortHeight * d)) * k1 *k1;

							if (k2 > (FarClippingPlane - NearClippingPlane) / (FarClippingPlane + NearClippingPlane))
							{
								// glm::vec3 Center = glm::vec3(0, 0, FarClippingPlane);

								Radius = FarClippingPlane * std::sqrt(k2);
							}
							else
							{
								// glm::vec3 Center = glm::vec3(0, 0, 0.5f * (FarClippingPlane + NearClippingPlane) * (1.0f + k2));

								Radius = 0.5f * std::sqrt(		(FarClippingPlane - NearClippingPlane) * (FarClippingPlane - NearClippingPlane) +
															2 * (FarClippingPlane * FarClippingPlane   + NearClippingPlane * NearClippingPlane) * k2 +
																(FarClippingPlane + NearClippingPlane) * (FarClippingPlane + NearClippingPlane) * k2 * k2);
							}

						}
						
						// ----------------------------------------------------------------
						//  fps camera
						// remeber to set the view port soon after calling this function

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

							float *ptr = glm::value_ptr(V);

							if (vml::utils::bits32::Get(Flags, EULER))
							{
								// compute view matrix

								float sx = sin(Angles.x);
								float sy = sin(Angles.y);
								float sz = sin(Angles.z);
								float cx = cos(Angles.x);
								float cy = cos(Angles.y);
								float cz = cos(Angles.z);

								ptr[ 0] =  cx * cz;
								ptr[ 1] =  sx * sy*cz + cy * sz;
								ptr[ 2] = -cy * sx*cz + sy * sz;
								ptr[ 3] =  0.0f;
								ptr[ 4] = -cx * sz;
								ptr[ 5] = -sx * sy*sz + cy * cz;
								ptr[ 6] =  cy * sx*sz + sy * cz;
								ptr[ 7] =  0.0f;
								ptr[ 8] =  sx;
								ptr[ 9] = -sy * cx;
								ptr[10] =  cx * cy;
								ptr[11] =  0.0f;
								ptr[12] = -Position.x*ptr[0] - Position.y*ptr[4] - Position.z*ptr[ 8];
								ptr[13] = -Position.x*ptr[1] - Position.y*ptr[5] - Position.z*ptr[ 9];
								ptr[14] = -Position.x*ptr[2] - Position.y*ptr[6] - Position.z*ptr[10];
								ptr[15] =  1.0f;

							}
							else if (vml::utils::bits32::Get(Flags, QUATERNIONS))
							{

								Quaternion = glm::quat(glm::vec3(Angles.y, Angles.x, Angles.z));
								V = glm::toMat4(Quaternion);
								ptr[12] = -Position.x*ptr[0] - Position.y*ptr[4] - Position.z*ptr[ 8];
								ptr[13] = -Position.x*ptr[1] - Position.y*ptr[5] - Position.z*ptr[ 9];
								ptr[14] = -Position.x*ptr[2] - Position.y*ptr[6] - Position.z*ptr[10];
								ptr[15] = 1.0f;
							}

							// direction vectors

							Right.x = ptr[0];
							Right.y = ptr[4];
							Right.z = ptr[8];

							Up.x = ptr[1];
							Up.y = ptr[5];
							Up.z = ptr[9];

							Direction.x = ptr[2];
							Direction.y = ptr[6];
							Direction.z = ptr[10];

							// compute vp matrix

							VP = P * V;

							// extract frustum planes

							ptr = glm::value_ptr(VP);

							// Left clipping plane

							Planes[0].x = ptr[ 3] + ptr[ 0];
							Planes[0].y = ptr[ 7] + ptr[ 4];
							Planes[0].z = ptr[11] + ptr[ 8];
							Planes[0].w = ptr[15] + ptr[12];

							// Right clipping plane

							Planes[1].x = ptr[ 3] - ptr[ 0];
							Planes[1].y = ptr[ 7] - ptr[ 4];
							Planes[1].z = ptr[11] - ptr[ 8];
							Planes[1].w = ptr[15] - ptr[12];

							// Bottom clipping plane

							Planes[2].x = ptr[ 3] + ptr[ 1];
							Planes[2].y = ptr[ 7] + ptr[ 5];
							Planes[2].z = ptr[11] + ptr[ 9];
							Planes[2].w = ptr[15] + ptr[13];

							// Bottom clipping plane

							Planes[3].x = ptr[ 3] - ptr[ 1];
							Planes[3].y = ptr[ 7] - ptr[ 5];
							Planes[3].z = ptr[11] - ptr[ 9];
							Planes[3].w = ptr[15] - ptr[13];

							// Near clipping plane

							Planes[4].x = ptr[ 3] + ptr[ 2];
							Planes[4].y = ptr[ 7] + ptr[ 6];
							Planes[4].z = ptr[11] + ptr[10];
							Planes[4].w = ptr[15] + ptr[14];

							// Far clipping plane

							Planes[5].x = ptr[ 3] - ptr[ 2];
							Planes[5].y = ptr[ 7] - ptr[ 6];
							Planes[5].z = ptr[11] - ptr[10];
							Planes[5].w = ptr[15] - ptr[14];

							// Normalize the plane equations

							float d;

							d = sqrtf(Planes[0].x*Planes[0].x + Planes[0].y*Planes[0].y + Planes[0].z*Planes[0].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							d = 1.0f / d;

							Planes[0].x *= d;
							Planes[0].y *= d;
							Planes[0].z *= d;
							Planes[0].w *= d;

							d = sqrtf(Planes[1].x*Planes[1].x + Planes[1].y*Planes[1].y + Planes[1].z*Planes[1].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							d = 1.0f / d;

							Planes[1].x *= d;
							Planes[1].y *= d;
							Planes[1].z *= d;
							Planes[1].w *= d;

							d = sqrtf(Planes[2].x*Planes[2].x + Planes[2].y*Planes[2].y + Planes[2].z*Planes[2].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[2].x *= d;
							Planes[2].y *= d;
							Planes[2].z *= d;
							Planes[2].w *= d;

							d = sqrtf(Planes[3].x*Planes[3].x + Planes[3].y*Planes[3].y + Planes[3].z*Planes[3].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[3].x *= d;
							Planes[3].y *= d;
							Planes[3].z *= d;
							Planes[3].w *= d;

							d = sqrtf(Planes[4].x*Planes[4].x + Planes[4].y*Planes[4].y + Planes[4].z*Planes[4].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[4].x *= d;
							Planes[4].y *= d;
							Planes[4].z *= d;
							Planes[4].w *= d;

							d = sqrtf(Planes[5].x*Planes[5].x + Planes[5].y*Planes[5].y + Planes[5].z*Planes[5].z);
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							Planes[5].x *= d;
							Planes[5].y *= d;
							Planes[5].z *= d;
							Planes[5].w *= d;

							// minimum bounding sphere of a view frustum
							// https://lxjk.github.io/2017/04/15/Calculate-Minimal-Bounding-Sphere-of-Frustum.html

							d = ViewPortWidth * ViewPortWidth;
							if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
							else d = 1.0f / d;

							float k1 = tanf(Fov*0.5f);
							float k2 = (1.0f + (ViewPortHeight * ViewPortHeight * d )) * k1 *k1;

							if (k2 > (FarClippingPlane - NearClippingPlane) / (FarClippingPlane + NearClippingPlane))
							{
								// glm::vec3 Center = glm::vec3(0, 0, FarClippingPlane);

								Radius = FarClippingPlane * std::sqrt(k2);
							}
							else
							{
								// glm::vec3 Center = glm::vec3(0, 0, 0.5f * (FarClippingPlane + NearClippingPlane) * (1.0f + k2));

								Radius = 0.5f * std::sqrt(      (FarClippingPlane - NearClippingPlane) * (FarClippingPlane - NearClippingPlane) +
															2 * (FarClippingPlane * FarClippingPlane   + NearClippingPlane * NearClippingPlane) * k2 +
															    (FarClippingPlane + NearClippingPlane) * (FarClippingPlane + NearClippingPlane) * k2 * k2);
							}

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
						//  transforms a vector using view matrix

						glm::vec3 TransformDirection(const glm::vec3 &dir)
						{
							const float *ptr = glm::value_ptr(V);
							return glm::vec3(ptr[0] * dir[0] + ptr[1] * dir[1] + ptr[ 2] * dir[2],
											 ptr[4] * dir[0] + ptr[5] * dir[1] + ptr[ 6] * dir[2],
											 ptr[8] * dir[0] + ptr[9] * dir[1] + ptr[10] * dir[2]);
						}

						// ----------------------------------------------------------------
						// reset parameters

						void Reset()
						{
							Position = OldPosition;
							Angles   = OldAngles;
						}

						// ----------------------------------------------------------------
						// default method of handling view transformation

						void UpdateInput(const vml::OpenGLContextWindow *window)
						{
							double mousex   = window->GetMouseXDelta();
							double mousey   = window->GetMouseYDelta();
							int leftbutton  = window->GetLeftButtonState();
							int rightbutton = window->GetRightButtonState();

							// rotation

							if (leftbutton && !rightbutton)
							{
								Angles.x += mousex*RotationSpeed;
								Angles.y -= mousey*RotationSpeed;
							}

							// forward movement

							if (rightbutton && !leftbutton)
							{

								if (mousey<0)
								{
									Position -= Direction * TranslationSpeed;
								}
								else if (mousey>0)
								{
									Position += Direction * TranslationSpeed;
								}

							}

							// strafe movement

							if (leftbutton && rightbutton)
							{

								if (mousex<0)
								{
									Position -= Right*TranslationSpeed;
								}
								else if (mousex>0)
								{
									Position += Right*TranslationSpeed;
								}

								if (mousey>0)
								{
									Position -= Up*TranslationSpeed;
								}
								else if (mousey<0)
								{
									Position += Up*TranslationSpeed;
								}
							}
						}

						// ----------------------------------------------------------------
						//  generates a ray from mouse screen position

						void UnprojectPosition( float x,float y,glm::vec3 &from,glm::vec3 &to ) const
						{
							float my=(float)ViewPortHeight-y-1.0f;
							glm::vec4 viewport = glm::vec4(0,0,(float)ViewPortWidth,(float)ViewPortHeight);
							from = glm::unProject(glm::vec3(x, my, 0), V, P, viewport);
							to   = glm::unProject(glm::vec3(x, my, 1), V, P, viewport);
						}
				
						// ----------------------------------------------------------------
						//  

						float HFOVtoVFOV(float HFOV_deg) const
						{
							const float rhf = HFOV_deg * vml::math::DEGTORAD;
							return 2.f*atanf(tanf(rhf*.5f) / Aspect) * vml::math::DEGTORAD;
						}

						// ----------------------------------------------------------------
						//  

						float VFOVtoHFOV(float VFOV_deg) const
						{
							const float rhf = VFOV_deg * vml::math::DEGTORAD;
							return 2.f*atanf( Aspect * tanf(rhf*.5f)) * vml::math::DEGTORAD;
						}

						// ----------------------------------------------------------------
						// value getting functions

						constexpr float		   GetNearPlaneDist()	const { return NearClippingPlane; }
						constexpr float		   GetFarPlaneDist()	const { return FarClippingPlane; }
						constexpr float		   GetAspect()			const { return Aspect; }
						constexpr float		   GetFov()				const { return Fov; }
						constexpr int		   GetViewPortWidth()	const { return ViewPortWidth; }
						constexpr int		   GetViewPortHeight()	const { return ViewPortHeight; }
						constexpr unsigned int GetFlags()			const { return Flags; }
						constexpr float		   GetRadius()		    const { return Radius; }

						// ----------------------------------------------------------------
						// const pointers getters

						const glm::vec3    &GetDirection()		const { return Direction; }
						const glm::vec3    &GetUp()				const { return Up; }
						const glm::vec3    &GetRight()          const { return Right; }
						const glm::mat4    &GetViewProjection()	const { return VP; }
						const glm::mat4    &GetView()			const { return V; }
						const glm::mat4    &GetProjection()		const { return P; }
						const glm::vec3	   &GetPosition()		const { return Position; }
						const glm::vec3    &GetAngle()			const { return Angles; }

						// ----------------------------------------------------------------
						// const pointers getters

						const glm::vec4    *GetFrustumPlanes()	const { return Planes; }
						float			   *GetVPptr()				  { return glm::value_ptr(VP); }
						float			   *GetPptr()				  { return glm::value_ptr(P); }
						float			   *GetVptr()				  { return glm::value_ptr(V); }
					
						// ---------------------------------------------------------------
						// set rotation mode

						void SetQuaternionsRotation()
						{
							vml::utils::bits32::SetToTrue(Flags, QUATERNIONS);
							vml::utils::bits32::SetToFalse(Flags, EULER);
						}

						void SetEulerRotation()
						{
							vml::utils::bits32::SetToFalse(Flags, QUATERNIONS);
							vml::utils::bits32::SetToTrue(Flags, EULER);
						}

						bool IsRotationModeQuaternions() const
						{
							return vml::utils::bits32::Get(Flags, QUATERNIONS);
						}

						bool IsRotationModeEuler() const
						{
							return vml::utils::bits32::Get(Flags, EULER);
						}

						int GetRotationMode() const
						{
							if (vml::utils::bits32::Get(Flags, QUATERNIONS)) return QUATERNIONS;
							else return EULER;
						}

						//----------------------------------------------------------------
						// dump info

						const std::string Dump()
						{
							std::string text;
							
							text =  std::format("ViewPort Width : {}\n", ViewPortWidth);
							text += std::format("ViewPort Height : {0}\n", ViewPortHeight);
							text += std::format("Aspect : {0}\n", Aspect);
							text += std::format("Near Clipping Plane : {0}\n", NearClippingPlane);
							text += std::format("Far Clipping Plane : {0}\n", FarClippingPlane);
							text += std::format("Fov: {0}\n", Fov);
							text += std::format("Position: {0} , {1} , {2}\n", Position.x, Position.y, Position.z);
							text += std::format("Angles {0} , {1} , {2}\n", Angles.x, Angles.y, Angles.z);
							text += std::format("Translation Speed: {0}\n", TranslationSpeed);
							text += std::format("Rotation Speed: {0}\n", RotationSpeed);
							text += std::format("Bitflag array: {0}\n", Flags);
							/*
							text = vml::strings::StringFormat::Text("ViewPort Width : {0}\n", ViewPortWidth);
							text += vml::strings::StringFormat::Text("ViewPort Height : {0}\n", ViewPortHeight);
							text += vml::strings::StringFormat::Text("Aspect : {0}\n",Aspect);
							text += vml::strings::StringFormat::Text("Near Clipping Plane : {0}\n", NearClippingPlane);
							text += vml::strings::StringFormat::Text("Far Clipping Plane : {0}\n", FarClippingPlane);
							text += vml::strings::StringFormat::Text("Fov: {0}\n", Fov);
							text += vml::strings::StringFormat::Text("Position: {0} , {1} , {2}\n", Position.x, Position.y, Position.z);
							text += vml::strings::StringFormat::Text("Angles {0} , {1} , {2}\n", Angles.x, Angles.y, Angles.z);
							text += vml::strings::StringFormat::Text("Translation Speed: {0}\n", TranslationSpeed);
							text += vml::strings::StringFormat::Text("Rotation Speed: {0}\n", RotationSpeed);
							text += vml::strings::StringFormat::Text("Bitflag array: {0}\n", Flags);
							*/
							return text;
						}
						
						//----------------------------------------------------------------
						// ctor / dtor

						View( const std::string &screenname, 
							  float				 nearclippingplane,
							  float				 farclippingplane,
							  float				 fov,
							  glm::vec3			 position,
							  glm::vec3			 angles,
							  float				 translationspeed,
							  float				 rotationspeed,
							  unsigned int		 flags )
						{
							Position		    = position;												// initial position
							Angles			    = angles;												// euler angles
							OldPosition		    = position;												// needed for resetting position
							OldAngles		    = angles;												// needed for resetting angles
							NearClippingPlane   = nearclippingplane;									// near z clipping plane
							FarClippingPlane    = farclippingplane;										// far z clipping plane
							Fov                 = fov*vml::math::DEGTORAD;								// field of view
							Flags               = flags;												// flags contatining infos
							RotationSpeed       = rotationspeed*vml::math::DEGTORAD;					// rotation speed
							TranslationSpeed    = translationspeed;										// translation speed
							Right				= glm::vec3(1, 0, 0);									// right vector	( x axis )
							Up					= glm::vec3(0, 1, 0);									// up vector ( y axis )
							Direction			= glm::vec3(0, 0, 1);									// direction vector ( z axis )
							Aspect				= 0;													// aspect ratio
							ViewPortWidth		= 0;													// view port width
							ViewPortHeight		= 0;													// view port height
							Radius				= 0;													// radius
							V				    = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// view matrix
							P				    = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// projection matrix
							VP				    = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// projection * view
							Planes[0]		    = glm::vec4(0,0,0,0);									// frustum planes
							Planes[1]		    = glm::vec4(0,0,0,0);									// frustum planes
							Planes[2]		    = glm::vec4(0,0,0,0);									// frustum planes
							Planes[3]		    = glm::vec4(0,0,0,0);									// frustum planes
							Planes[4]		    = glm::vec4(0,0,0,0);									// frustum planes
							Planes[5]		    = glm::vec4(0,0,0,0);									// frustum planes
							Quaternion		    = glm::quat(0,0,0,0);									// quaternion
							ScreenName			= vml::strings::StringUtils::MakeLower(screenname);		// screenname

							// check if both flags are set to true

							bool quaternions = vml::utils::bits32::Get(Flags, QUATERNIONS);
							bool euler = vml::utils::bits32::Get(Flags, EULER);

							if ((!quaternions && !euler) || (quaternions && euler))
							{
								vml::utils::bits32::SetToTrue(Flags, EULER);
								vml::utils::bits32::SetToFalse(Flags, QUATERNIONS);
							}

							if (quaternions)
							{
								vml::utils::bits32::SetToTrue(Flags, QUATERNIONS);
								vml::utils::bits32::SetToFalse(Flags, EULER);
							}

							if (euler)
							{
								vml::utils::bits32::SetToTrue(Flags, EULER);
								vml::utils::bits32::SetToFalse(Flags, QUATERNIONS);
							}

						}

						~View()
						{
						//	vml::utils::Logger::GetInstance()->Out(vml::utils::Logger::GetInstance()->GetCurrentDate(), "View", "Releasing instance of View",ScreenName.c_str());
						}
						
				};
				
	} // end of view namespace

} //end of vml namespace

