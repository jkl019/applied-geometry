#include "window.h"

//local
#include "fboinsgrenderer.h"

// qt
#include "QOpenGLContext"

Window::Window(QWindow *parent) : QQuickView(parent) {

  qmlRegisterType<FboInSGRenderer>("SceneGraphRendering", 1, 0, "Renderer");

  setSurfaceType(QSurface::OpenGLSurface);
  setPersistentOpenGLContext(true);
  setPersistentSceneGraph(true);

  // Turn off clear before rendering !!!!!!!!!!!!!
//  setClearBeforeRendering(false);

  QSurfaceFormat format;
  if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
//    format.setVersion(4,0);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions);
  }
  format.setDepthBufferSize(24);
  format.setSamples(4);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);
  setFormat(format);

  setResizeMode(SizeRootObjectToView);

//  resize(800,600);
  setMinimumSize( QSize( 800, 600 ) );
}
