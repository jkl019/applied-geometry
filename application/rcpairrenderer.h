#ifndef __RCPAIRRENDERER_H__
#define __RCPAIRRENDERER_H__


#include "renderer.h"

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




class RCPairRenderer : public Renderer {
  Q_OBJECT
  Q_PROPERTY(QString name     READ rcPairName WRITE setRcPairName)


public:
  explicit RCPairRenderer();
  ~RCPairRenderer();

  const QString&        rcPairName() const;
  void                  setRcPairName( const QString& tex_name );

public slots:
  void                  sync();
  void                  cleanup();

signals:
  void                  signRcPairActiveStateChanged( const QString& name, bool state );
  void                  signRcPairViewportChanged( const QString& name, const QRectF& geometry );

  void                  signMousePressed( const QString& name, QMouseEvent* event );
  void                  signMouseReleased( const QString& name, QMouseEvent* event );
  void                  signMouseDoubleClicked( const QString& name, QMouseEvent* event );
  void                  signMouseMoved( const QString& name, QMouseEvent* event );
  void                  signKeyPressed( const QString& name, QKeyEvent* event );
  void                  signKeyReleased( const QString& name, QKeyEvent* event );
  void                  signWheelEventOccurred( const QString& name, QWheelEvent* event);

private:
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;
  QString                                     _name;

  QRectF                                      _viewport;

private slots:
//  void                  itemChange(ItemChange change, const ItemChangeData& value ) override;
  QSGNode*              updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  void                  mousePressEvent(QMouseEvent *event) override;
  void                  mouseReleaseEvent(QMouseEvent *event) override;
  void                  mouseDoubleClickEvent(QMouseEvent *event) override;
  void                  mouseMoveEvent(QMouseEvent *event) override;
  void                  keyPressEvent(QKeyEvent *event) override;
  void                  keyReleaseEvent(QKeyEvent *event) override;
  void                  wheelEvent(QWheelEvent *event) override;
  void                  handleWindowChanged( QQuickWindow * window );


  void                  paint();

};



#endif // __RCPAIRRENDERER_H__
