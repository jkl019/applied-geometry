#ifndef __TEXTURERENDERER_H__
#define __TEXTURERENDERER_H__


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




class TextureRenderer : public Renderer {
  Q_OBJECT
  Q_PROPERTY(QStringList  textureIds    READ textureIds   NOTIFY signTextureIdsChanged )
  Q_PROPERTY(QStringList  textureNames  READ textureNames NOTIFY signTextureIdsChanged )


public:
  explicit TextureRenderer();
  ~TextureRenderer();

  Q_INVOKABLE void        setTextureName( const QString& tex_name );
  Q_INVOKABLE void        setTextureId( const QVariant& id );
  Q_INVOKABLE void        setColorMagnification( int magn );


  QStringList             textureIds() const;
  QStringList             textureNames() const;

public slots:
  void                    sync();
  void                    cleanup();

private:
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;
  QRectF                                      _viewport;

  QStringListModel        _textureid_model;
  GMlib::GL::Texture      _tex;
  GMlib::GL::Program      _prog;
  int                     _color_magn {1};

  void                    initTexMagnProg();

private slots:
  void                    itemChange(ItemChange change, const ItemChangeData& value ) override;
  QSGNode*                updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  void                    handleWindowChanged( QQuickWindow * window );


  void                    paint();

signals:
  void                    signTextureIdsChanged();
};



#endif // __TEXTURERENDERER_H__
