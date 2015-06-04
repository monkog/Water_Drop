#include "window.h"
#include "mesh.h"
#include "drop.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QMatrix4x4>


OpenGLWindow::OpenGLWindow(Mesh model, Drop waterDrop, QWindow *parent)
: QWindow(parent)
, m_update_pending(false)
, m_animating(false)
, m_context(0)
, m_device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
	m_model = model;
	m_drop = waterDrop;
}

OpenGLWindow::~OpenGLWindow()
{
	delete m_device;
}

void OpenGLWindow::render(QPainter *painter)
{
	QVector4D waterDropPosition, intersectionPoint;
	painter->setPen(Qt::white);
	QVector<QPoint> readyVertices = transformVertices(1);
	QVector4D m_selected_face = m_model.findIntersectionWithDrop(m_drop);

	int face = 0;
	QPolygon polygon;
	for (int i = 0; i < readyVertices.size() - 1; i += 3)
	{
		polygon.clear();
		polygon << readyVertices[i] << readyVertices[i + 1] << readyVertices[i + 2];
		painter->drawPolygon(polygon);
		if (face == m_drop.HitFaceIndex)
		{
			QBrush brush; brush.setColor(Qt::green);
			brush.setStyle(Qt::SolidPattern);
			QPainterPath path;
			path.addPolygon(polygon);
			painter->fillPath(path, brush);
		}
		face++;
	}
	painter->setPen(Qt::blue);

	intersectionPoint = m_matrixTranslateBefore*m_matrixScaleBefore*m_matrixProjection*m_matrixTransforms*m_selected_face;
	m_matrixTranslateBefore.translate(0, -400);
	waterDropPosition = m_matrixTranslateBefore*m_matrixScaleBefore*m_matrixProjection*m_matrixTransforms*m_drop.getActualPosition();
	m_matrixTranslateBefore.translate(0, 400);
	//painter->drawLine(waterDropPosition.x() / waterDropPosition.w(), waterDropPosition.y() / waterDropPosition.w(), intersectionPoint.x() / intersectionPoint.w(), intersectionPoint.y() / intersectionPoint.w());
	painter->drawEllipse(QPoint(waterDropPosition.x() / waterDropPosition.w(), waterDropPosition.y() / waterDropPosition.w()), 10, 10);
	painter->drawEllipse(QPoint(intersectionPoint.x() / intersectionPoint.w(), intersectionPoint.y() / intersectionPoint.w()), 2, 2);
	painter->setPen(Qt::red);
	readyVertices = transformVertices(2);
	for (int i = 0; i < readyVertices.size() - 1; i++)
		painter->drawLine(readyVertices[i].x(), readyVertices[i].y(), readyVertices[i + 1].x(), readyVertices[i + 1].y());

}

QVector<QPoint> OpenGLWindow::transformVertices(int option)
{
	QVector<QVector4D> vertices = option==1 ? m_model.getPolygonsVertices() : m_model.m_pathPoints;
	QVector<QPoint> transformedVertices;
	QVector4D v;
	for (int i = 0; i < vertices.size(); i++)
	{
		v = m_matrixTranslateBefore*m_matrixScaleBefore*m_matrixProjection*m_matrixTransforms*vertices[i];
		transformedVertices.push_back(QPoint((int)(v.x() / v.w()), (int)(v.y() / v.w())));
	}
	return transformedVertices;
}

void OpenGLWindow::initialize()
{
	m_matrixProjection = QMatrix4x4();
	m_matrixProjection(3, 2) = -1 / 500;
	m_matrixTranslateBefore = QMatrix4x4();
	m_matrixTranslateBefore.translate(600, 400);
	m_matrixScaleBefore = QMatrix4x4();
	m_matrixScaleBefore.scale(10);
	m_matrixTransforms = QMatrix4x4();
}

void OpenGLWindow::render()
{
	if (!m_device)
		m_device = new QOpenGLPaintDevice;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_device->setSize(size());

	QPainter painter(m_device);
	render(&painter);
}

void OpenGLWindow::renderLater()
{
	if (!m_update_pending) {
		m_update_pending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

void OpenGLWindow::mouseWheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	float numSteps = numDegrees / 15;
	m_matrixTransforms.scale(1 + numSteps / 50);
	event->accept();
}



void OpenGLWindow::keyPressedEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key::Key_O:
	{

	}
	case Qt::Key::Key_X:
		m_matrixTransforms.rotate(0.5f, QVector3D(1.0, 0.0, 0.0));
		break;
	case Qt::Key::Key_Y:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, 1.0, 0.0));
		break;
	case Qt::Key::Key_Z:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, 0.0, 1.0));
		break;
	case Qt::Key::Key_W:
		m_matrixTransforms.rotate(0.5f, QVector3D(-1.0, 0.0, 0.0));
		break;
	case Qt::Key::Key_T:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, -1.0, 0.0));
		break;
	case Qt::Key::Key_Q:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, 0.0, -1.0));
		break;
	case Qt::Key::Key_Up:
		m_matrixTransforms.translate(0, -1);
		break;
	case Qt::Key::Key_Down:
		m_matrixTransforms.translate(0, 1);
		break;
	case Qt::Key::Key_Left:
		m_matrixTransforms.translate(-1, 0);
		break;
	case Qt::Key::Key_Right:
		m_matrixTransforms.translate(1, 0);
		break;
	}
}

bool OpenGLWindow::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::UpdateRequest:
		m_update_pending = false;
		renderNow();
		return true;
	case QEvent::KeyPress:
		keyPressedEvent((QKeyEvent*)event);
		return true;
	case QEvent::Wheel:
		mouseWheelEvent((QWheelEvent*)event);
		return true;
	default:
		return QWindow::event(event);
	}
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
	Q_UNUSED(event);

	if (isExposed())
		renderNow();
}

void OpenGLWindow::renderNow()
{
	if (!isExposed())
		return;

	bool needsInitialize = false;

	if (!m_context) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_context->swapBuffers(this);

	if (m_animating)
		renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
	m_animating = animating;

	if (animating)
		renderLater();
}

