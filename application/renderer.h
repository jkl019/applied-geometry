#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <QQuickItem>


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




class Renderer : public QQuickItem {
  Q_OBJECT

public:
  explicit Renderer();

  void                                        initCanvasAndProg();

protected:
  const GMlib::GL::VertexBufferObject&        canvas() const;
  const GMlib::GL::Program&                   prog() const;

private:
  GMlib::GL::VertexBufferObject               _canvas_vbo;
  GMlib::GL::Program                          _prog;

};



#endif // __RENDERER_H__
