#include "glscenerenderer.h"
#include "window.h"
#include "glcontextsurfacewrapper.h"

// Qt
#include <QQuickWindow>

// stl
#include <iostream>



GLSceneRenderer::GLSceneRenderer() : _renderer{nullptr}, _glsurface{nullptr}, _name{} {

  _renderer.reset();
  setAcceptedMouseButtons(Qt::AllButtons);
  setFlag(ItemHasContents);
  setSmooth(false);
  connect( this, &QQuickItem::windowChanged, this, &GLSceneRenderer::handleWindowChanged );
}

GLSceneRenderer::~GLSceneRenderer() {

  _glsurface->makeCurrent(); {
    if(_renderer)
      _renderer.reset();
  } _glsurface->doneCurrent();
}

const QString&
GLSceneRenderer::textureName() const {

  return _name;
}

void
GLSceneRenderer::setTextureName(const QString& name) {

  _name = name;

  if(!(_name.length() > 0))
    _renderer = nullptr;
//    _renderer.reset(nullptr);
  else if(_renderer)
    _renderer->setName(_name.toStdString());
}

void
GLSceneRenderer::sync() {

  if(!_renderer && _name.length() > 0) {

    Window *w = static_cast<Window*>(window());
    _renderer = std::make_shared<GLTextureRenderer,std::string>(_name.toStdString());
    _glsurface = w->glSurface();
    connect( w, &Window::beforeRendering, _renderer.get(), &GLTextureRenderer::paint, Qt::DirectConnection );
  }

  if( !_renderer )
    return;

  QRectF r = mapRectToScene(boundingRect());
  QRectF cr(r.left(),window()->height()-r.bottom(), r.width(), r.height());
  _renderer->setViewport(cr);

  emit signViewportChanged(_name,r);
}

void
GLSceneRenderer::cleanup() {

  std::cout << "Cleanup" << std::endl;
}

void
GLSceneRenderer::handleWindowChanged(QQuickWindow* window) {

  if( !window ) return;

  Window *w = dynamic_cast<Window*>(window);
  if( !w ) return;

  connect( w, &Window::beforeSynchronizing, this, &GLSceneRenderer::sync );
  connect( w, &Window::sceneGraphInvalidated, this, &GLSceneRenderer::cleanup );
  connect( w, &Window::signFrameReady, this, &QQuickItem::update );
  connect( this, &GLSceneRenderer::signViewportChanged, w, &Window::signGuiViewportChanged );
  connect( this, &GLSceneRenderer::signMousePressed, w, &Window::signMousePressed );
  connect( this, &GLSceneRenderer::signMouseReleased, w, &Window::signMouseReleased);
  connect( this, &GLSceneRenderer::signMouseDoubleClicked, w, &Window::signMouseDoubleClicked);
  connect( this, &GLSceneRenderer::signMouseMoved, w, &Window::signMouseMoved );
  connect( this, &GLSceneRenderer::signKeyPressed, w, &Window::signKeyPressed );
  connect( this, &GLSceneRenderer::signKeyReleased, w, &Window::signKeyReleased);
  connect( this, &GLSceneRenderer::signWheelEventOccurred, w, &Window::signWheelEventOccurred);
}

void
GLSceneRenderer::itemChange(ItemChange change, const ItemChangeData& value) {

//  qDebug() << "GLSceneRenderer changes: " << _name << ", change: " << change << ", value: " << value.boolValue;
  if(change == QQuickItem::ItemVisibleHasChanged && !value.boolValue)
    _renderer = nullptr;

  QQuickItem::itemChange(change,value);
}

QSGNode*
GLSceneRenderer::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*) {

//  qDebug() << "updatePaintNode; item has content: " << ((flags() & QQuickItem::ItemHasContents) == QQuickItem::ItemHasContents ? "yes" : "no");
  return node;
}

void
GLSceneRenderer::mouseMoveEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseMoved(_name,event);
}

void
GLSceneRenderer::mousePressEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMousePressed(_name,event);
}

void
GLSceneRenderer::mouseReleaseEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseReleased(_name,event);
}

void
GLSceneRenderer::keyPressEvent(QKeyEvent* event) {

  emit signKeyPressed(_name,event);
}

void
GLSceneRenderer::keyReleaseEvent(QKeyEvent* event) {

  emit signKeyReleased(_name,event);
}

void
GLSceneRenderer::wheelEvent(QWheelEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signWheelEventOccurred(_name,event);
}

void
GLSceneRenderer::mouseDoubleClickEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseDoubleClicked(_name,event);
}

