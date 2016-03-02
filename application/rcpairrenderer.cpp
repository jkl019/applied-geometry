#include "rcpairrenderer.h"
#include "window.h"
#include "glcontextsurfacewrapper.h"
#include "gmlibwrapper.h"

// Qt
#include <QQuickWindow>

// stl
#include <iostream>



RCPairRenderer::RCPairRenderer() : _glsurface{nullptr}, _name{} {

  setAcceptedMouseButtons(Qt::AllButtons);
  setFlag(ItemHasContents);
  setSmooth(false);
  connect( this, &QQuickItem::windowChanged, this, &RCPairRenderer::handleWindowChanged );
}

RCPairRenderer::~RCPairRenderer() {}

const QString&
RCPairRenderer::textureName() const {

  return _name;
}

void
RCPairRenderer::setTextureName(const QString& name) {

  _name = name;
}

void
RCPairRenderer::sync() {

  auto prog_valid = prog().isValid();

  if(!prog_valid && _name.length() > 0) {

    Window *w = static_cast<Window*>(window());
    _glsurface = w->glSurface();
    connect( w, &Window::beforeRendering, this, &RCPairRenderer::paint, Qt::DirectConnection );
  }

  if( !prog_valid )
    return;

  QRectF r = mapRectToScene(boundingRect());
  QRectF cr(r.left(),window()->height()-r.bottom(), r.width(), r.height());
  _viewport = cr;

  emit signViewportChanged(_name,r);
}

void
RCPairRenderer::cleanup() {

  std::cout << "Cleanup" << std::endl;
}

void
RCPairRenderer::handleWindowChanged(QQuickWindow* window) {

  if( !window ) return;

  Window *w = dynamic_cast<Window*>(window);
  if( !w ) return;

  connect( w, &Window::beforeSynchronizing, this, &RCPairRenderer::sync );
  connect( w, &Window::sceneGraphInvalidated, this, &RCPairRenderer::cleanup );
  connect( w, &Window::signFrameReady, this, &QQuickItem::update );
  connect( this, &RCPairRenderer::signViewportChanged, w, &Window::signGuiViewportChanged );
  connect( this, &RCPairRenderer::signMousePressed, w, &Window::signMousePressed );
  connect( this, &RCPairRenderer::signMouseReleased, w, &Window::signMouseReleased);
  connect( this, &RCPairRenderer::signMouseDoubleClicked, w, &Window::signMouseDoubleClicked);
  connect( this, &RCPairRenderer::signMouseMoved, w, &Window::signMouseMoved );
  connect( this, &RCPairRenderer::signKeyPressed, w, &Window::signKeyPressed );
  connect( this, &RCPairRenderer::signKeyReleased, w, &Window::signKeyReleased);
  connect( this, &RCPairRenderer::signWheelEventOccurred, w, &Window::signWheelEventOccurred);
}

void RCPairRenderer::paint() {

  const GMlib::TextureRenderTarget &render_tex = GMlibWrapper::getInstance().renderTextureOf(_name.toStdString());
  const GMlib::GL::Texture& tex = render_tex.getTexture();

  initCanvasAndProg();

  GL_CHECK(::glViewport(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height()));

  prog().bind(); {

    prog().setUniform( "u_tex0", tex, GL_TEXTURE0, GLuint(0) );

    GMlib::GL::AttributeLocation vert_loc = prog().getAttributeLocation("vertices");

    canvas().bind();
    canvas().enableVertexArrayPointer( vert_loc, 2, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(nullptr) );
    GL_CHECK(::glDrawArrays(GL_TRIANGLE_STRIP,0,4));
    canvas().disable(vert_loc);
    canvas().unbind();

  } prog().unbind();

}

void
RCPairRenderer::itemChange(ItemChange change, const ItemChangeData& value) {

//  qDebug() << "RCPairRenderer changes: " << _name << ", change: " << change << ", value: " << value.boolValue;
  if(change == QQuickItem::ItemVisibleHasChanged && !value.boolValue)
    disconnect( window(), &Window::beforeRendering, this, &RCPairRenderer::paint );

  QQuickItem::itemChange(change,value);
}

QSGNode*
RCPairRenderer::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*) {

//  qDebug() << "updatePaintNode; item has content: " << ((flags() & QQuickItem::ItemHasContents) == QQuickItem::ItemHasContents ? "yes" : "no");
  return node;
}

void
RCPairRenderer::mouseMoveEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseMoved(_name,event);
}

void
RCPairRenderer::mousePressEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMousePressed(_name,event);
}

void
RCPairRenderer::mouseReleaseEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseReleased(_name,event);
}

void
RCPairRenderer::keyPressEvent(QKeyEvent* event) {

  emit signKeyPressed(_name,event);
}

void
RCPairRenderer::keyReleaseEvent(QKeyEvent* event) {

  emit signKeyReleased(_name,event);
}

void
RCPairRenderer::wheelEvent(QWheelEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signWheelEventOccurred(_name,event);
}

void
RCPairRenderer::mouseDoubleClickEvent(QMouseEvent* event) {

  setFocus(true,Qt::MouseFocusReason);
  emit signMouseDoubleClicked(_name,event);
}

