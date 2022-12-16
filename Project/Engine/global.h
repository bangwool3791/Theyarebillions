#pragma once


#include <Windows.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <array>
#include <iterator>
#include <algorithm>
#include <queue>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "precision.h"

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::back_insert_iterator;

using std::string;
using std::wstring;

using std::array;
using std::queue;

using std::begin;
using std::cbegin;

using std::end;
using std::cend;

using std::cin;
using std::cout;
using std::endl;

#include <typeinfo>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#include <d3dcommon.h>
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"

