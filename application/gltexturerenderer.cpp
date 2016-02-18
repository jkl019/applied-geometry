#include "gltexturerenderer.h"
#include "window.h"
#include "glcontextsurfacewrapper.h"

// Qt
#include <QQuickWindow>
#include <QDebug>

// stl
#include <iostream>




GLTextureRenderer::GLTextureRenderer (const std::string& name) : _name{name} {}

GLTextureRenderer::~GLTextureRenderer() {}

void
GLTextureRenderer::paint() {

//      qDebug() << "Painting on viewport " << _name.c_str() << ": " << _viewport;

  assert(_name.length());

//    qDebug() << "GLTextureRenderer::paint; " << _name.c_str();

  const GMlib::TextureRenderTarget &render_tex = GMlibWrapper::getInstance().renderTextureOf(_name);
  const GMlib::GL::Texture& tex = render_tex.getTexture();

  if( !_prog.isValid() ) {
//      std::cout << "Prog ! valid: setting up." << std::endl;

    _vs.create();
    _fs.create();
    _prog.create();

    _vs.setSource(
          "#version 150 compatibility\n"
          "layout(std140) uniform;\n"
          "in  vec4 vertices;"
          "out vec2 coords;"
          "out vec4 gl_Position;"
          "void main() {"
          "    gl_Position = vertices;"
          "    coords = (vertices.xy + vec2(1.0,1.0))*0.5;"
          "}"
          );

    _fs.setSource(
          "#version 150 compatibility\n"
          "layout(std140) uniform;\n"
          "\n"
          "uniform sampler2D u_tex0;"
          "in vec2 coords;"
          "void main() {"
          "    gl_FragColor = texture( u_tex0, coords.st );"
          "}"
          );

    if( !_vs.compile() ) {
      std::cout << "Vertex shader compile error: " << _vs.getCompilerLog() << std::endl;
      exit(-666);
    }
    if(!_fs.compile()) {
      std::cout << "Fragment shader compile error: " << _fs.getCompilerLog() << std::endl;
      exit(-666);
    }
    _prog.attachShader(_vs);
    _prog.attachShader(_fs);
    if( _prog.link() != GL_TRUE ) {
      std::cout << "Render prog link error: " << _prog.getLinkerLog() << std::endl;
      exit(-666);
    }


    const GLfloat values[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    _vbo.create();
    _vbo.bufferData( 2 * 4 * sizeof(GLfloat), values, GL_STATIC_DRAW );
  }


//      std::cout << "Prog OK!" << std::endl;


  glViewport(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height());

//      glDisable(GL_DEPTH_TEST);

//      glClearColor(0, 0, 0, 1);
//      glClear(GL_COLOR_BUFFER_BIT);

//      glDisable(GL_BLEND);
//      glEnable(GL_BLEND);
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  _prog.bind(); {

//        _prog.setUniform( "u_tex0", _tex, GL_TEXTURE0, GLuint(0) );
    _prog.setUniform( "u_tex0", tex, GL_TEXTURE0, GLuint(0) );

    GMlib::GL::AttributeLocation vert_loc = _prog.getAttributeLocation("vertices");

    _vbo.bind();
    _vbo.enableVertexArrayPointer( vert_loc, 2, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(nullptr) );
    GL_CHECK(::glDrawArrays(GL_TRIANGLE_STRIP,0,4));
    _vbo.disable(vert_loc);
    _vbo.unbind();

  } _prog.unbind();


}

void
GLTextureRenderer::setName(const std::string &name) {
  _name = name;
}

void
GLTextureRenderer::setViewport(const QRectF &viewport ) {
  _viewport = viewport;
}

void
GLTextureRenderer::setViewport(int x, int y, int w, int h) {
  setViewport( QRectF(x,y,w,h) );
}

