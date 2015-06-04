#include "drop.h"
#include <QtGui/QMatrix4x4>

using namespace std;

Drop::Drop()
{}

Drop::Drop(QPoint xPosition)
{
	HitFaceIndex = -1;
	m_gravityVector = QVector3D(0, -1, 0);
	m_matrixTransforms = QMatrix4x4();
//	int newX = xPosition.x() + (rand() % (int)(xPosition.y() - xPosition.x() + 1));
	m_position = QVector4D(0, 0, 0, 1);
}

void Drop::UpdateDrop()
{
	m_matrixTransforms.translate(0, -1);
}

QVector4D Drop::getActualPosition()
{
	return m_position;
}