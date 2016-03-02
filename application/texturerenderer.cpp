#include "texturerenderer.h"
#include "window.h"
#include "glcontextsurfacewrapper.h"
#include "gmlibwrapper.h"

// Qt
#include <QQuickWindow>


// stl
#include <iostream>



TextureRenderer::TextureRenderer() : _glsurface{nullptr} {

  setAcceptedMouseButtons(Qt::AllButtons);
  setFlag(ItemHasContents);
  setSmooth(false);
  connect( this, &QQuickItem::windowChanged, this, &TextureRenderer::handleWindowChanged );
}

TextureRenderer::~TextureRenderer() {}

void
TextureRenderer::setTextureName(const QString& name) {

  _tex.acquire(name.toStdString());
}

void
TextureRenderer::setTextureId(const QVariant& id) {

  _tex.acquire(id.toUInt());
}

QStringList
TextureRenderer::textureIds() const {

  QStringList ids;
  for( const auto& d : GMlib::GL::Texture::getData() )
    ids << QString("%1").arg(d.id);
  return ids;
}

QStringList
TextureRenderer::textureNames() const {

  QStringList ids;
  for( const auto& d : GMlib::GL::Texture::getData() )
    if( d.name.size() ) ids << d.name.c_str();
  return ids;
}

void
TextureRenderer::sync() {

  auto prog_valid = prog().isValid();

  if(!prog_valid && _tex.isValid() > 0) {

    Window *w = static_cast<Window*>(window());
    _glsurface = w->glSurface();
    connect( w, &Window::beforeRendering, this, &TextureRenderer::paint, Qt::DirectConnection );
  }

  if( !prog_valid )
    return;

  QRectF r = mapRectToScene(boundingRect());
  QRectF cr(r.left(),window()->height()-r.bottom(), r.width(), r.height());
  _viewport = cr;
}

void
TextureRenderer::cleanup() {

  std::cout << "Cleanup" << std::endl;
}

void
TextureRenderer::handleWindowChanged(QQuickWindow* window) {

  if( !window ) return;

  Window *w = dynamic_cast<Window*>(window);
  if( !w ) return;

  connect( w, &Window::beforeSynchronizing, this, &TextureRenderer::sync );
  connect( w, &Window::sceneGraphInvalidated, this, &TextureRenderer::cleanup );
  connect( w, &Window::signFrameReady, this, &QQuickItem::update );
}

void TextureRenderer::paint() {

//  GMlib::GL::Texture tex;
////  if(!tex.acquire(_name.toStdString())) return;
//  if(!tex.acquire(_id)) return;

  if(!_tex.isValid()) return;

  initCanvasAndProg();

  GL_CHECK(::glViewport(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height()));

  GL_CHECK(::glDisable(GL_BLEND));
  prog().bind(); {

    prog().setUniform( "u_tex0", _tex, GL_TEXTURE0, GLuint(0) );

    GMlib::GL::AttributeLocation vert_loc = prog().getAttributeLocation("vertices");

    canvas().bind();
    canvas().enableVertexArrayPointer( vert_loc, 2, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(nullptr) );
    GL_CHECK(::glDrawArrays(GL_TRIANGLE_STRIP,0,4));
    canvas().disable(vert_loc);
    canvas().unbind();

  } prog().unbind();

}

void
TextureRenderer::itemChange(ItemChange change, const ItemChangeData& value) {

//  qDebug() << "TextureRenderer changes: " << _name << ", change: " << change << ", value: " << value.boolValue;
  if(change == QQuickItem::ItemVisibleHasChanged && !value.boolValue)
    disconnect( window(), &Window::beforeRendering, this, &TextureRenderer::paint );

  QQuickItem::itemChange(change,value);
}

QSGNode*
TextureRenderer::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*) {

//  qDebug() << "updatePaintNode; item has content: " << ((flags() & QQuickItem::ItemHasContents) == QQuickItem::ItemHasContents ? "yes" : "no");
  return node;
}
