#include "renderer.h"
#include "window.h"
#include "glcontextsurfacewrapper.h"
#include "gmlibwrapper.h"

// Qt
#include <QQuickWindow>

// stl
#include <iostream>



Renderer::Renderer() {}

void Renderer::initCanvasAndProg() {


  const std::string unique_name = "qt_qml_renderer";

  // Check wether prog is initialized
  if( _prog.isValid() )
    return;

  // Check if prog and canvas can be acquired, if not; create them
  auto canvas_ok = _canvas_vbo.acquire(unique_name);
  auto prog_ok = _prog.acquire(unique_name);

  assert(canvas_ok == prog_ok);

  if(canvas_ok and prog_ok)
    return;


  GMlib::GL::VertexShader   vs;
  GMlib::GL::FragmentShader fs;
  vs.create();
  fs.create();

  vs.setSource(
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

  fs.setSource(
        "#version 150 compatibility\n"
        "layout(std140) uniform;\n"
        "\n"
        "uniform sampler2D u_tex0;"
        "in vec2 coords;"
        "void main() {"
        "    gl_FragColor = texture( u_tex0, coords.st );"
        "}"
        );

  if( !vs.compile() ) {
    std::cout << "Vertex shader compile error: " << vs.getCompilerLog() << std::endl;
    exit(-666);
  }
  if(!fs.compile()) {
    std::cout << "Fragment shader compile error: " << fs.getCompilerLog() << std::endl;
    exit(-666);
  }

  _prog.create(unique_name);
  _prog.attachShader(vs);
  _prog.attachShader(fs);
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

  _canvas_vbo.create(unique_name);
  _canvas_vbo.bufferData( 2 * 4 * sizeof(GLfloat), values, GL_STATIC_DRAW );
}

const GMlib::GL::VertexBufferObject&Renderer::canvas() const {

  return _canvas_vbo;
}

const GMlib::GL::Program&Renderer::prog() const {

  return _prog;
}

