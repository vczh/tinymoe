#ifndef VCZH_TINYMOESTL
#define VCZH_TINYMOESTL

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <functional>
#include <fstream>

using namespace std;

namespace tinymoe
{
	struct AssertFailedException{};
}

#define ASSERT(CONDITION) do{if(!(CONDITION)) throw AssertFailedException();}while(0)

#endif