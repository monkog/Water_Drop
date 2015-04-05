#ifndef WINDOW
#define WINDOW
#include <QWheelEvent>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit OpenGLWindow(QVector<QVector4D> vertices, QWindow *parent = 0);
	~OpenGLWindow();

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	void setAnimating(bool animating);
	public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event) Q_DECL_OVERRIDE;
	void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
	void mouseWheelEvent(QWheelEvent *event);
	void keyPressedEvent(QKeyEvent *event);
	QVector<QPoint> transformVertices();
private:
	bool m_update_pending;
	bool m_animating;
	QMatrix4x4 m_matrixProjection;
	QMatrix4x4 m_matrixScaleBefore;
	QMatrix4x4 m_matrixTranslateBefore;
	QMatrix4x4 m_matrixTransforms;
	QVector<QVector4D> m_vertices;
	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;
};

#endif // WINDOW

