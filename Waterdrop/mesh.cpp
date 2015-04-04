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

QVector<QPoint> Mesh::getVerticesVector()
{
	QVector<QPoint> readyVertices;
	QVector4D v;
	HillMesh::VertexIterator vi;
	QMatrix4x4 matrixProjection = QMatrix4x4();
	QMatrix4x4 matrixTranslate = QMatrix4x4();
	QMatrix4x4 matrixScale = QMatrix4x4();

	matrixProjection.setToIdentity();
	matrixProjection(3, 2) = -1 / 500;
	matrixScale.scale(10);
	matrixTranslate.translate(400, 200);
	
	for (vi = mesh.vert.begin(); vi != mesh.vert.end(); ++vi)
	if (!(*vi).IsD())
	{
		v = QVector4D((*vi).P()[0], (*vi).P()[1], (*vi).P()[2], 1.0f);
		v = matrixTranslate*matrixScale*matrixProjection*v;
		readyVertices.push_back(QPoint((int)(v.x() / v.w()), (int)(v.y() / v.w())));
	}
	return readyVertices;
}

