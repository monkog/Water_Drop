#include "window.h"
#include "mesh.h"
#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QMatrix4x4>


OpenGLWindow::OpenGLWindow(QVector<QVector4D> vertices, QWindow *parent)
: QWindow(parent)
, m_update_pending(false)
, m_animating(false)
, m_context(0)
, m_device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
	m_vertices = vertices;
}

OpenGLWindow::~OpenGLWindow()
{
	delete m_device;
}

void OpenGLWindow::render(QPainter *painter)
{
	//  Q_UNUSED(painter);
	QVector<QPoint> readyVertices = transformVertices();
	painter->setPen(Qt::white);
	QPolygon polygon;
	for (int i = 0; i < readyVertices.size() - 1; i += 3)
	{
		polygon.clear();
		polygon << readyVertices[i] << readyVertices[i + 1] << readyVertices[i + 2];
		painter->drawPolyline(polygon);
	}
}

QVector<QPoint> OpenGLWindow::transformVertices()
{
	QVector<QPoint> transformedVertices;
	QVector4D v;
	for (int i = 0; i < m_vertices.size(); i++)
	{
		v = m_matrixTranslateBefore*m_matrixScaleBefore*m_matrixProjection*m_matrixTransforms*m_vertices[i];
		transformedVertices.push_back(QPoint((int)(v.x() / v.w()), (int)(v.y() / v.w())));
	}
	return transformedVertices;
}

void OpenGLWindow::initialize()
{
	m_matrixProjection = QMatrix4x4();
	m_matrixProjection(3, 2) = -1 / 500;
	m_matrixTranslateBefore = QMatrix4x4();
	m_matrixTranslateBefore.translate(400, 200);
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
	case Qt::Key::Key_X:
		m_matrixTransforms.rotate(0.5f, QVector3D(1.0, 0.0, 0.0));
		break;
	case Qt::Key::Key_Y:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, 1.0, 0.0));
		break;
	case Qt::Key::Key_Z:
		m_matrixTransforms.rotate(0.5f, QVector3D(0.0, 0.0, 1.0));
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

