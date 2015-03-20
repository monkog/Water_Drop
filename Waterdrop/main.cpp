#include "mesh.h"

using namespace std;

int main(int argc, char **argv)
{
    string MESH_PATH = "..\\was_was_was.stl";
    Mesh mesh;
    mesh.loadStlMesh(MESH_PATH);

    system("pause");
    return 0;
}
#include <QApplication>
#include <QDesktopWidget>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if( !TwInit(TW_OPENGL, NULL) )
    {
        fprintf(stderr, "AntTweakBar initialization failed: %s\n", TwGetLastError());
        return 1;
    }
    GLWidget window;
    window.show();
    return app.exec();
}
