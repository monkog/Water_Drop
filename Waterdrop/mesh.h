#ifndef MESH
#define MESH

#include <vcg/complex/complex.h>
#include <QtGui/QMatrix4x4>

class HillVertex; class HillEdge; class HillFace;
struct HillUsedTypes : public vcg::UsedTypes<vcg::Use<HillVertex> ::AsVertexType
                            , vcg::Use<HillEdge> ::AsEdgeType
                            , vcg::Use<HillFace> ::AsFaceType>{};

class HillVertex : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
class HillFace : public vcg::Face< HillUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags > {};
class HillEdge : public vcg::Edge< HillUsedTypes > {};

//Deffinition of a mesh built from triangles
class HillMesh : public vcg::tri::TriMesh< std::vector<HillVertex>, std::vector<HillFace>, std::vector<HillEdge> > {};

class Mesh
{
public:
    //Loads the mesh from the STL file
    bool loadStlMesh(std::string path);
	//Gives vertices from model
	QVector<QVector4D> Mesh::getVerticesVector();
private:
};

#endif // MESH

