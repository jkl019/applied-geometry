#ifndef __GLCONTEXTWRAPPER_H__
#define __GLCONTEXTWRAPPER_H__


// qt
#include <QObject>
class QOpenGLContext;
class QOffscreenSurface;

// stl
#include <memory>


class GLContextSurfaceWrapper : public QObject {
  Q_OBJECT

public:
  explicit GLContextSurfaceWrapper(QOpenGLContext* context );

public slots:
  void      makeCurrent();
  void      doneCurrent();

private:
  std::shared_ptr<QOpenGLContext>       _context;
  std::shared_ptr<QOffscreenSurface>    _surface;
};

#endif // __GLCONTEXTWRAPPER_H__
