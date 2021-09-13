#pragma once

#include <Windows.h>
#include <assert.h>

#include <string>

using namespace std;

#include "Utilities/Vector.h"

#include "Utilities/APIFuncs.h"
#include "Utilities/Singleton.h"

#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Time.h"

#include "Rect.h"

#define WinMaxWidth 1280
#define WinMaxHeight 720

#define SAFE_DELETE(p) { if(p) { delete(p); p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[](p); p = nullptr; } }

extern HDC hdc;
extern HWND handle;