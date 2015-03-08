#pragma once
#include "Helper.h"
#include "Mesh.h"

using namespace Helpers;
using namespace std;

int main(int argc, char **argv)
{
	string path = Helper::ExePath();
	string MESH_PATH = path + "\\was_was_was.stl";
	Mesh mesh;
	mesh.loadStlMesh(MESH_PATH);

	system("pause");
	return 0;
}