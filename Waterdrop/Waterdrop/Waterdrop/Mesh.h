#include <vcg/complex/complex.h>

class HillVertex; class HillEdge; class HillFace;
struct HillUsedTypes : public vcg::UsedTypes<vcg::Use<HillVertex> ::AsVertexType
							, vcg::Use<HillEdge> ::AsEdgeType
							, vcg::Use<HillFace> ::AsFaceType>{};

class HillVertex : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
class HillFace : public vcg::Face< HillUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags > {};
class HillEdge : public vcg::Edge< HillUsedTypes> {};

class HillMesh : public vcg::tri::TriMesh< std::vector<HillVertex>, std::vector<HillFace>, std::vector<HillEdge> > {};

class HillVertex0 : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags >{};
class HillVertex1 : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
class HillVertex2 : public vcg::Vertex< HillUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Color4b, vcg::vertex::CurvatureDirf,
	vcg::vertex::Qualityf, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};

class Mesh
{
public:
	//Loads the mesh from the STL file
	bool loadStlMesh(std::string path);

private:
};