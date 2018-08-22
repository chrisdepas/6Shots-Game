// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// SFML 
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

// SFML GUI
#include <TGUI/TGUI.hpp>

// Physics
#include <Box2D\Box2D.h>

// RapidXML - Disable warnings
#pragma warning(push, 0)  
	#include <rapidxml.hpp>
	#include <rapidxml_utils.hpp>
#pragma warning(pop)

// LUA scripting language
// Need to get lua for msvc14
//#include <lua.hpp>

// Boost
#include <boost\filesystem.hpp>

// stdlib
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <cstdarg>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <math.h>
#include <map>
#include <fstream>
#include <functional>
#include <queue>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <time.h>
#include <vector>

// Bring string scope
using std::string;
using std::to_string;
