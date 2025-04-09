// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma once
#include "apclient.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <chrono>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <filesystem>
#include "core.h"
#include "gameobject.h"
#include "level.h"
#include "gamestate.h"
#include "hero.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "imgui_impl_win32.h"

#endif //PCH_H
