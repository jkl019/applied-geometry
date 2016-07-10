#include "fboinsgrenderer.h"

#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QOpenGLFunctions>

#include "gmlibwrapper.h"

#include <scene/gmrendertarget>

class QQuickFboInlineRenderTarget : public GMlib::RenderTarget {
public:
  QQuickFboInlineRenderTarget() { _gl.initializeOpenGLFunctions(); }
  GLint _fbo;

private:
  mutable QOpenGLFunctions _gl;

  void doPrepare()  const override {}
  void doBind()   const override { _gl.glBindFramebuffer(GL_FRAMEBUFFER,_fbo); }
  void doUnbind() const override { _gl.glBindFramebuffer(GL_FRAMEBUFFER,0x0); }
  void doResize()  override {}
};

class GMlibInFboRenderer : public QQuickFramebufferObject::Renderer {

public:
  GMlibInFboRenderer() { _gl.initializeOpenGLFunctions(); }

  void render() {

    if(!_rcpair_name.length()) return;

    // Pick up the FBO set by the QQuickFrameBufferObject upon the render() call
    _gl.glGetIntegerv(GL_FRAMEBUFFER_BINDING,&_rt._fbo);

    // Prepare render and camera
    auto &gmlib = GMlibWrapper::instance();
    gmlib.render(_rcpair_name,QRect(QPoint(0,0),QSize(_size)),_rt);

    // Restore to QML's GLState;
    // we do not know what GMlib has done
    _item->window()->resetOpenGLState();

    // Not necessary, but for clarity let's restore the full GL state as we entered the render() method
    _gl.glBindFramebuffer(GL_FRAMEBUFFER,_rt._fbo);

    // Throttle
    update();
  }

  QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {

    _size = size;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
  }

  void synchronize(QQuickFramebufferObject *item) {

    _item = static_cast<FboInSGRenderer*>(item);
    _rcpair_name = _item->rcPairName();
  }

  QOpenGLFunctions            _gl;
  FboInSGRenderer*            _item;
  QSize                       _size;
  QQuickFboInlineRenderTarget _rt;
  QString                     _rcpair_name;
};




FboInSGRenderer::FboInSGRenderer() { setMirrorVertically(true); }

const QString&
FboInSGRenderer::rcPairName() const { return _name; }

void
FboInSGRenderer::setRcPairName(const QString& name) {
  _name = name;
  update();
}

QQuickFramebufferObject::Renderer* FboInSGRenderer::createRenderer() const { return new GMlibInFboRenderer(); }
