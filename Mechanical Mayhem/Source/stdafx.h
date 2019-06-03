//------------------------------------------------------------------------------
//
// File Name:	stdafx.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

// Disable warnings for externals
#pragma warning(push, 0)

// Windows
#include <windows.h>
#include <gdiplus.h>

#define _USE_MATH_DEFINES
#include <cmath> // M_PI
#define M_PI_F (static_cast<float>(M_PI))
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// Used to track memory leaks to specific files and line numbers
#ifdef _DEBUG
	#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

// Used to clean /W4 unused parameters for functions that must match a function-pointer type 
// NOTE: copied from winnt.h, but we don't want to include that here, otherwise 
#define UNREFERENCED_PARAMETER(P) (P)

// Re-enable warnings
#pragma warning(pop)

// Collision group definitions

#define CG_GENERIC 0ull
#define CG_PLAYER1 1ull
#define CG_PLAYER2 2ull
#define CG_PLAYER3 3ull
#define CG_PLAYER4 4ull
#define CG_PLAYER5 5ull
#define CG_PLAYER6 62ull
#define CG_HAZARD 7ull

// Collision mask definitions

// Create a collision mask
#define CM_CREATE(CG) (1ull << (CG))

#define CM_GENERIC (CM_CREATE(CG_GENERIC))
#define CM_PLAYER1 (CM_CREATE(CG_PLAYER1))
#define CM_PLAYER2 (CM_CREATE(CG_PLAYER2))
#define CM_PLAYER3 (CM_CREATE(CG_PLAYER3))
#define CM_PLAYER4 (CM_CREATE(CG_PLAYER4))
#define CM_PLAYER5 (CM_CREATE(CG_PLAYER5))
#define CM_PLAYER6 (CM_CREATE(CG_PLAYER6))
#define CM_HAZARD (CM_CREATE(CG_HAZARD))
