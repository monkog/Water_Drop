#include "Mesh.h"

#include <wrap/io_trimesh/io_mask.h>
#include <wrap/io_trimesh/import_stl.h>
#include <vcg/complex/algorithms/update/normal.h>

using namespace std;

bool Mesh::loadStlMesh(string path)
{
	HillMesh mesh;
	int s = 0;
	int errorcode = vcg::tri::io::ImporterSTL<HillMesh>::Open(mesh, path.c_str(), s, 0);
	if (errorcode != 0)
	{
		string error = vcg::tri::io::ImporterSTL<HillMesh>::ErrorMsg(errorcode);
		cerr << "An error occured while loading " << path << ": " << error << endl;
		return false;
	}

	vcg::tri::RequirePerVertexNormal(mesh);
	vcg::tri::UpdateNormal<HillMesh>::PerVertexNormalized(mesh);
	printf("Input mesh vn:%i fn:%i\n", mesh.VN(), mesh.FN());
	printf("Mesh has %i vert and %i faces\n", mesh.VN(), mesh.FN());
	return true;
}