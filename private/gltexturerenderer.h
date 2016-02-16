#ifndef __GLTEXTURERENDERER_H__
#define __GLTEXTURERENDERER_H__

#include <QQuickItem>


#include "../gmlibwrapper.h"


class GLContextSurfaceWrapper;


// gmlib
#include <opengl/gmtexture>
#include <opengl/gmprogram>
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <scene/render/rendertargets/gmtexturerendertarget.h>

// qt
#include <QDebug>




namespace Private {




  // Invariants
  // Name must be valid!
  class GLTextureRenderer: public QObject {
    Q_OBJECT
  public:
    GLTextureRenderer( const std::string& name );
    ~GLTextureRenderer();

    void      setName( const std::string& name );

  public slots:
    void      paint();
    void      setViewport( const QRectF& rect );
    void      setViewport( int x, int y, int w, int h );

  private:
    QRectF                          _viewport;

    GMlib::GL::VertexBufferObject   _vbo;
    std::string                     _name;

    GMlib::GL::Program              _prog;
    GMlib::GL::VertexShader         _vs;
    GMlib::GL::FragmentShader       _fs;

  }; // END namespace GLTextureRenderer

}



#endif // __GLTEXTURERENDERER_H__
