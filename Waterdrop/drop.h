#ifndef DROP
#define DROP

#include <QtGui/QMatrix4x4>

class Drop
{
public:
	Drop();
	Drop(QPoint xPosition);
	QVector4D getActualPosition();
	void UpdateDrop();
	int HitFaceIndex;
	QVector4D HitPoint;
private:
    double m_mass;
	QVector4D m_position;
	QVector3D m_gravityVector;
	QMatrix4x4 m_matrixTransforms;
    //vector<double> v;
};

#endif // DROP

