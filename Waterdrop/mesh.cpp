#include "mesh.h"
#include <QtGui/QMatrix4x4>

#include <wrap/io_trimesh/io_mask.h>
#include <wrap/io_trimesh/import_stl.h>
#include <vcg/complex/algorithms/update/normal.h>

using namespace std;
HillMesh mesh;
bool Mesh::loadStlMesh(string path)
{
    int s = 0;
	string p = path.c_str();
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

QVector<QVector4D> Mesh::getVerticesVector()
{
	QVector<QVector4D> vertices;
	HillMesh::FaceIterator vi;
	for (vi = mesh.face.begin(); vi != mesh.face.end(); ++vi)
	{
		vertices.push_back(QVector4D((*vi).V(0)->P()[0], (*vi).V(0)->P()[1], (*vi).V(0)->P()[2], 1.0f));
		vertices.push_back(QVector4D((*vi).V(1)->P()[0], (*vi).V(1)->P()[1], (*vi).V(1)->P()[2], 1.0f));
		vertices.push_back(QVector4D((*vi).V(2)->P()[0], (*vi).V(2)->P()[1], (*vi).V(2)->P()[2], 1.0f));
	}
	return vertices;
}

