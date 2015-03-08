#include "Helper.h"

#include <windows.h>

using namespace std;

string Helpers::Helper::ExePath()
{
	char buffer[500];
	GetModuleFileName(NULL, buffer, 500);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}