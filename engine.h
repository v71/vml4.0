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
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER7
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:6386)
#pragma warning(disable:26451)

#define _CRT_SECURE_NO_WARNINGS

////////////////////////////////////////////////////////////////////////////////////
// include directories

#include <vml4.0/os/common.h>

////////////////////////////////////////////////////////////////////////////////////
// host system

#ifdef _WIN32 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <vml4.0/os/errormsg.h>		// error/warning/trace messaging , platform dependent
#include <vml4.0/os/cpuinfo2.h>		// system info detection class ,platform dependent
#include <vml4.0/os/timer.h>		// timer class ,platform dependent
#endif	

////////////////////////////////////////////////////////////////////////////////////
// bits utils

#include <vml4.0/utils/bitarray8.h>
#include <vml4.0/utils/bitarray16.h>
#include <vml4.0/utils/bitarray32.h>
#include <vml4.0/utils/uniqueid.h>

////////////////////////////////////////////////////////////////////////////////////
// os

#include <vml4.0/os/internalflags.h>
#include <vml4.0/os/preferencesflags.h>

////////////////////////////////////////////////////////////////////////////////////
// string utils

#include <vml4.0/strings/stringutils.h>
#include <vml4.0/strings/stringpath.h>

////////////////////////////////////////////////////////////////////////////////////
// logger

#include <vml4.0/utils/logger.h>

////////////////////////////////////////////////////////////////////////////////////
// assetstore

#include <vml4.0/utils/assetstore.h>

////////////////////////////////////////////////////////////////////////////////////
// session

#include <vml4.0/utils/globalpaths.h>

////////////////////////////////////////////////////////////////////////////////////
// json

#include <vml4.0/utils/json.hpp>

////////////////////////////////////////////////////////////////////////////////////
// math

#include <vml4.0/math/mathutils.h>
#include <vml4.0/math/2d/geometry2d.h>
#include <vml4.0/math/3d/geometry3d.h>

////////////////////////////////////////////////////////////////////////////////////
// rendering context

#include <vml4.0/opengl/opengl.h>
#include <vml4.0/opengl/debugrendering/rgbutils.h>
#include <vml4.0/libs/stb/stb_image.h>
#include <vml4.0/opengl/textures/texture.h>

/////////////////////////////////////////////////////////////////////////////////////
// stores

namespace vml
{
	namespace stores
	{
		extern vml::utils::SharedResourceStore* ShaderStore;	// shader stores
		extern vml::utils::SharedResourceStore* MeshStore;		// mesh stores
		extern vml::utils::SharedResourceStore* TextureStore;	// mesh stores
	}
}

////////////////////////////////////////////////////////////////////////////////////
// meshes

#include <vml4.0/mesh/mesh.h>

////////////////////////////////////////////////////////////////////////////////////
// jolt physics engine

#include <vml4.0/physics/jolt.h>

#include <vml4.0/mesh/model.h>
#include <vml4.0/mesh/object.h>
#include <vml4.0/mesh/objectmanager2.h>

////////////////////////////////////////////////////////////////////////////////////
// importer & mesh optimizer
// in case you want to add directly in a .cpp file, use this define
//#define TINYOBJLOADER_IMPLEMENTATION

#include <vml4.0/libs/tinyobjloader/tiny_obj_loader.h>
#include <vml4.0/libs/meshoptimizer/meshoptimizer.h>
#include <vml4.0/mesh/importer.h>

////////////////////////////////////////////////////////////////////////////////////
// handlers

#include <vml4.0/octree/octree.h>
#include <vml4.0/handlers/level.h>
#include <vml4.0/handlers/scene.h>

////////////////////////////////////////////////////////////////////////////////////
// debug rendering

#include <vml4.0/opengl/debugrendering/debugrendering.h>

////////////////////////////////////////////////////////////////////////////////////
// core

#include <vml4.0/os/core.h>
