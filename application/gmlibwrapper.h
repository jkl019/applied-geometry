#ifndef GMLIBWRAPPER_H
#define GMLIBWRAPPER_H

class TestTorus;
class GLContextSurfaceWrapper;

// gmlib
#include <core/gmpoint>

namespace GMlib {

  class Scene;
  class SceneObject;
  class Camera;
  class PointLight;
  class DefaultRenderer;
  class DefaultSelectRenderer;
  class TextureRenderTarget;

  template<typename T, int n>
  class PSurf;
}

// qt
#include <QObject>
#include <QSize>
#include <QRectF>
#include <QStringListModel>
class QOpenGLContext;
class QOpenGLFramebufferObject;
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;


// stl
#include <memory>
#include <unordered_map>


struct RenderCamPair {
  RenderCamPair() : render{nullptr}, camera{nullptr} {}
  std::shared_ptr<GMlib::DefaultRenderer>     render;
  std::shared_ptr<GMlib::Camera>              camera;
  struct {
    QRectF                      geometry { QRectF(0,0,200,200) };
    bool                        changed {true};
  } viewport;
};





class GMlibWrapper : public QObject {
  Q_OBJECT
public:
  explicit GMlibWrapper(std::shared_ptr<GLContextSurfaceWrapper> wrapper );
  ~GMlibWrapper();

  void                                              start();
  void                                              stop();

  const std::shared_ptr<GMlib::Scene>&              scene() const;
  const GMlib::TextureRenderTarget&                 renderTextureOf( const std::string& name ) const;
  const std::shared_ptr<GMlib::Camera>&             camera( const std::string& name ) const;

  void                                              init();

  GMlib::SceneObject*                               findSceneObject( const QString& rc_name, const GMlib::Point<int,2>& pos );
  QStringListModel&                                 rcNameModel();

  RenderCamPair&                                    rcPair(const QString& name);
  RenderCamPair&                                    createRCPair( const QString& name );

  std::shared_ptr<GMlib::DefaultSelectRenderer>     defaultSelectRenderer() const;



public slots:
  void                                              changeRenderGeometry( const QString& name,
                                                                          const QRectF &new_geometry );

  void                                              toggleSimulation();

protected:
  void                                              timerEvent(QTimerEvent *e);


private:

  int                                               _timer_id;

  std::shared_ptr<GLContextSurfaceWrapper>          _glsurface;
  std::shared_ptr<GMlib::Scene>                     _scene;

  std::unordered_map<std::string, RenderCamPair>    _rc_pairs;
  std::shared_ptr<GMlib::DefaultSelectRenderer>     _select_renderer;

  int                                               _replot_low_medium_high {1};
  bool                                              _move_object_button_pressed {false};
  bool                                              _select_multiple_objects_pressed {false};
  bool                                              _rotate_object_button_pressed {false};
  GMlib::Point<int,2>                               _current_mouse_pos;
  GMlib::Point<int,2>                               _prev_mouse_pos;

  QStringListModel                                  _rc_name_model;
signals:
  void                                              signFrameReady();



  // "Singleton instance"
private:
  static std::unique_ptr<GMlibWrapper>              _instance;
public:
  static const GMlibWrapper&                        getInstance();
};



#endif // GMLIBWRAPPER_H
