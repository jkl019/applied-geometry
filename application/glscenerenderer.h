#ifndef __GLSCENERENDERER_H__
#define __GLSCENERENDERER_H__

#include <QQuickItem>


#include "gltexturerenderer.h"
#include "gmlibwrapper.h"


class GLContextSurfaceWrapper;


// gmlib
#include <opengl/gmtexture>
#include <opengl/gmprogram>
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <scene/render/rendertargets/gmtexturerendertarget.h>

// qt
#include <QOpenGLShaderProgram>
#include <QDebug>


// stl
#include <memory>
#include <mutex>




class GLSceneRenderer : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QString name     READ textureName WRITE setTextureName)


public:
  explicit GLSceneRenderer();
  ~GLSceneRenderer();

  const QString&        textureName() const;
  void                  setTextureName( const QString& tex_name );

public slots:
  void                  sync();
  void                  cleanup();

signals:
  void                  signViewportChanged( const QString& name, const QRectF& size );
  void                  signMousePressed( const QString& name, QMouseEvent* event );
  void                  signMouseReleased( const QString& name, QMouseEvent* event );
  void                  signMouseDoubleClicked( const QString& name, QMouseEvent* event );
  void                  signMouseMoved( const QString& name, QMouseEvent* event );
  void                  signKeyPressed( const QString& name, QKeyEvent* event );
  void                  signKeyReleased( const QString& name, QKeyEvent* event );
  void                  signWheelEventOccurred( const QString& name, QWheelEvent* event);

private:
  std::shared_ptr<GLTextureRenderer>          _renderer;
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;
  QString                                     _name;

private slots:
  void                  itemChange(ItemChange change, const ItemChangeData& value ) override;
  QSGNode*              updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  void                  mousePressEvent(QMouseEvent *event) override;
  void                  mouseReleaseEvent(QMouseEvent *event) override;
  void                  mouseDoubleClickEvent(QMouseEvent *event) override;
  void                  mouseMoveEvent(QMouseEvent *event) override;
  void                  keyPressEvent(QKeyEvent *event) override;
  void                  keyReleaseEvent(QKeyEvent *event) override;
  void                  wheelEvent(QWheelEvent *event) override;
  void                  handleWindowChanged( QQuickWindow * window );

};



#endif // __GLSCENERENDERER_H__
