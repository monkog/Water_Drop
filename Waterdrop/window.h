#ifndef WINDOW
#define WINDOW

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
	explicit OpenGLWindow(QVector<QPoint> vertices, QWindow *parent = 0);
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
private:
    bool m_update_pending;
    bool m_animating;
	bool m_isButtonPressed;
	QVector<QPoint> m_vertices;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif // WINDOW

