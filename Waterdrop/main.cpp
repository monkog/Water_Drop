#include "mesh.h"
#include "window.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

using namespace std;

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	string MESH_PATH = "..\\was_was_was.stl";
	Mesh mesh;
	mesh.loadStlMesh(MESH_PATH);

	QSurfaceFormat format;
	format.setSamples(16);

	OpenGLWindow window(mesh.getVerticesVector());
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	window.setAnimating(true);

	return app.exec();
}
