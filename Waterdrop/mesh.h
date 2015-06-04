#ifndef MESH
#define MESH

#include <vcg/complex/complex.h>
#include <QtGui/QMatrix4x4>
#include "drop.h"

class HillVertex; class HillEdge; class HillFace; class HillHalfEdge;
class HillUsedTypes : public vcg::UsedTypes<
	vcg::Use<HillVertex> ::AsVertexType
	, vcg::Use<HillEdge> ::AsEdgeType
	, vcg::Use<HillHalfEdge> ::AsHEdgeType
	, vcg::Use<HillFace> ::AsFaceType>{};

class HillVertex : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
class HillFace : public vcg::Face< HillUsedTypes, vcg::face::Normal3f, vcg::face::FFAdj, vcg::face::FHAdj, vcg::face::VertexRef, vcg::face::BitFlags  > {};
class HillEdge : public vcg::Edge<HillUsedTypes>{};
class HillHalfEdge : public vcg::HEdge< HillUsedTypes, vcg::hedge::HEdgeData, vcg::hedge::BitFlags > {};

//Deffinition of a mesh built from triangles
class HillMesh : public
	vcg::tri::TriMesh<	std::vector<HillVertex>, std::vector<HillFace>, std::vector<HillEdge>, std::vector<HillHalfEdge>> {};

class Mesh
{
public:
	bool loadStlMesh(std::string path);
	QVector<QVector4D> Mesh::getPolygonsVertices();
	QPoint findX();
	QVector4D findIntersectionWithDrop(Drop &waterdrop);
	void drawPath(HillFace face, QVector4D enterPoint);
	QVector<QVector4D> m_pathPoints;
private:

	QVector<HillFace> m_pathFaces;
};

#endif // MESH

