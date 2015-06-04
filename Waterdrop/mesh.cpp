#include "mesh.h"
#include "drop.h"
#include <QtGui/QMatrix4x4>
#include <wrap/io_trimesh/io_mask.h>
#include <wrap/io_trimesh/import_stl.h>
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/space/intersection3.h>
#include <vcg/space/distance3.h>
#include <vcg/space/ray3.h>

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

	// Kawalek kodu ktory nie chce dzialac, bo wymaga #include <vcg/complex/algorithms/clean.h>
	//vcg::tri::Clean<myMesh>::RemoveDuplicateFace(mm->mesh);
	//vcg::tri::Clean<myMesh>::RemoveUnreferencedVertex(mm->mesh);
	//vcg::tri::Clean<myMesh>::RemoveZeroAreaFace(mm->mesh);
	//vcg::tri::Clean<myMesh>::RemoveDegenerateFace(mm->mesh);


	vcg::tri::UpdateTopology<HillMesh>::FaceFace(mesh);
	vcg::tri::UpdateNormal<HillMesh>::PerFaceNormalized(mesh);
	vcg::tri::UpdateNormal<HillMesh>::NormalizePerFace(mesh);

	return true;
}


QVector<QVector4D> Mesh::getPolygonsVertices()
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



void Mesh::drawPath(HillFace face, QVector4D firstEnterPoint)
{
	vcg::Point3<float> n = vcg::Point3<float>(face.NORMX, face.NORMY, face.NORMZ);
	vcg::Segment3<float> seg;
	vcg::Plane3<float> tmp;

	QVector4D nextEnterPoint;
	tmp.Init(vcg::Point3f(firstEnterPoint.x(), firstEnterPoint.y(), firstEnterPoint.z()), vcg::Point3f(n.X(), n.Y(), n.Z()));
	vcg::IntersectionPlaneTriangle(tmp, face, seg);
	float p0Angle = vcg::AngleN<float>(n, vcg::Point3<float>(seg.P0()[0] - firstEnterPoint.x(), seg.P0()[1] - firstEnterPoint.y(), seg.P0()[2] - firstEnterPoint.z()));
	float p1Angle = vcg::AngleN<float>(n, vcg::Point3<float>(seg.P1()[0] - firstEnterPoint.x(), seg.P1()[1] - firstEnterPoint.y(), seg.P1()[2] - firstEnterPoint.z()));
	if (p0Angle < p1Angle)
	{
		m_pathPoints.append(QVector4D(seg.P0()[0], seg.P0()[1], seg.P0()[2], 1));
		nextEnterPoint = QVector4D(seg.P0()[0], seg.P0()[1], seg.P0()[2], 1);
	}
		
	else
	{
		m_pathPoints.append(QVector4D(seg.P1()[0], seg.P1()[1], seg.P1()[2], 1));
		nextEnterPoint = QVector4D(seg.P1()[0], seg.P1()[1], seg.P1()[2], 1);
	}

	HillFace *e1, *e2, *e3;
	HillFace nextFace = face;
	e1 = nextFace.FFp(0);
	e2 = nextFace.FFp(1);
	e3 = nextFace.FFp(2);

	// TUTAJ W CELACH TESTOWYCH SOBIE SPRAWDZAM WIERZCHOLKI KAZDEGO Z 3 SASIADOW AKTUALNEJ SCIANY
	// ALE NARAZIE kazda trojka wierzcholkow jest taka sama jak oryginalnej sciany
	vcg::Point3<float> e00 = face.P(0);
	vcg::Point3<float> e01 = face.P(1);
	vcg::Point3<float> e02 = face.P(2);

	vcg::Point3<float> e10 = e1->P(0); 
	vcg::Point3<float> e11 = e1->P(1);
	vcg::Point3<float> e12 = e1->P(2);

	vcg::Point3<float> e20 = e2->P(0);
	vcg::Point3<float> e21 = e2->P(1);
	vcg::Point3<float> e22 = e2->P(2);

	vcg::Point3<float> e30 = e3->P(0);
	vcg::Point3<float> e31 = e3->P(1);
	vcg::Point3<float> e32 = e3->P(2);

	/* TO TRZEBA ODKOMENTOWAC JAK BEDA SIE WCZYTYWAC POPRAWNIE SASIEDNIE SCIANY BO TERAZ NIE MA SENSU
	vcg::Point3<float> clo;
	float da1, da2, da3, min;
	int counter = 0;
	while (vcg::AngleN<float>(n, vcg::Point3<float>(0,-1,0)) > 0 && counter<5)
	{

		e1 = *nextFace.FFp(0);
		e2 = *nextFace.FFp(1);
		e3 = *nextFace.FFp(2);
		 vcg::SegmentPointSquaredDistance(vcg::Segment3f(nextFace.P(0), nextFace.P(1)), seg.P1(), clo, da1);
		 vcg::SegmentPointSquaredDistance(vcg::Segment3f(nextFace.P(1), nextFace.P(2)), seg.P1(), clo, da2);
		 vcg::SegmentPointSquaredDistance(vcg::Segment3f(nextFace.P(2), nextFace.P(0)), seg.P1(), clo, da3);
		min = vcg::math::Min(da1, da2, da3);
		if (min == da1)
			nextFace = e1;
		else if (min == da2)
			nextFace = e2;
		else
			nextFace = e3;

		n = vcg::Point3<float>(nextFace.NORMX, nextFace.NORMY, nextFace.NORMZ);
		tmp.Init(vcg::Point3f(nextEnterPoint.x(), nextEnterPoint.y(), nextEnterPoint.z()), vcg::Point3f(n.X(), n.Y(), n.Z()));
		vcg::IntersectionPlaneTriangle(tmp, nextFace, seg);
		float p0Angle = vcg::AngleN<float>(n, vcg::Point3<float>(seg.P0()[0] - nextEnterPoint.x(), seg.P0()[1] - nextEnterPoint.y(), seg.P0()[2] - nextEnterPoint.z()));
		float p1Angle = vcg::AngleN<float>(n, vcg::Point3<float>(seg.P1()[0] - nextEnterPoint.x(), seg.P1()[1] - nextEnterPoint.y(), seg.P1()[2] - nextEnterPoint.z()));
		if (p0Angle < p1Angle)
		{
			m_pathPoints.append(QVector4D(seg.P0()[0], seg.P0()[1], seg.P0()[2], 1));
			nextEnterPoint = QVector4D(seg.P0()[0], seg.P0()[1], seg.P0()[2], 1);
		}
		else
		{
			m_pathPoints.append(QVector4D(seg.P1()[0], seg.P1()[1], seg.P1()[2], 1));
			nextEnterPoint = QVector4D(seg.P1()[0], seg.P1()[1], seg.P1()[2], 1);
		}
		counter++;
	}
*/

}



QVector4D Mesh::findIntersectionWithDrop(Drop &waterDrop)
{
	vcg::Ray3<float, false> ray;
	ray.SetOrigin(vcg::Point3f(waterDrop.getActualPosition().x(), waterDrop.getActualPosition().y(), waterDrop.getActualPosition().z()));
	ray.SetDirection(vcg::Point3f(0, -1, 0));
	float t, u, v, w;
	vcg::Point3f a, b, c;
	HillMesh::FaceIterator vi;
	HillFace f;
	int index = 0;
	for (vi = mesh.face.begin(); vi != mesh.face.end(); ++vi)
	{
		if (vcg::IntersectionRayTriangle(ray, (*vi).P(0), (*vi).P(1), (*vi).P(2), t, u, v))
		{
			a = (*vi).P(0);
			b = (*vi).P(1);
			c = (*vi).P(2);
			waterDrop.HitFaceIndex = index;
			f = (*vi);
			break;
		}
		index++;
	}

	if (waterDrop.HitFaceIndex != -1)
	{
		w = 1 - u - v;
		float x = u*f.P(0).X() + v*f.P(1).X() + w*f.P(2).X();
		float y = u*f.P(0).Y() + v*f.P(1).Y() + w*f.P(2).Y();
		float z = u*f.P(0).Z() + v*f.P(1).Z() + w*f.P(2).Z();
		m_pathPoints.append(QVector4D(x, y, z, 1));
		m_pathFaces.append(f);
		drawPath(f, QVector4D(x, y, z, 1));

		return QVector4D(x, y, z, 1);
	}
	else
		return QVector4D(0, 0, 0, 0);
}

QPoint Mesh::findX()
{
	float xMax = FLT_MIN;
	float xMin = FLT_MAX;

	HillMesh::VertexIterator vi;
	for (vi = mesh.vert.begin(); vi != mesh.vert.end(); ++vi)
	{
		if ((*vi).P().X() > xMax)
			xMax = (*vi).P().X();
		if ((*vi).P().X() < xMin)
			xMin = (*vi).P().X();
	}

	return QPoint(xMin, xMax);
}
