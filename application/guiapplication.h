#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H


class Window;
class GMlibWrapper;
class GLContextSurfaceWrapper;
class DefaultHidManager;

// gmlib
namespace GMlib {
  class Scene;
}

// qt
#include <QGuiApplication>

// stl
#include <memory>




class GuiApplication : public QGuiApplication {
  Q_OBJECT
public:
  explicit GuiApplication(int& argc, char* argv[]);
  ~GuiApplication();

private:
  std::shared_ptr<Window>                     _window;
  std::shared_ptr<GMlibWrapper>               _gmlib;
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;
  std::shared_ptr<DefaultHidManager>          _hidmanager;

  void                                        setupScene();

  virtual void                                initializeScenario() = 0;
  virtual void                                cleanupScenario() = 0;

private slots:

  // Void waranty on re-implementation
  virtual void                                onSGInit();

  void                                        beforeHidAction();
  void                                        afterHidAction();

protected:

  std::shared_ptr<Window>                     window();
  std::shared_ptr<GMlibWrapper>               gmlib();
  std::shared_ptr<GLContextSurfaceWrapper>    glsurface();
  std::shared_ptr<DefaultHidManager>          hidmanager();
  std::shared_ptr<GMlib::Scene>               scene();



private:
  static std::unique_ptr<GuiApplication>      _instance;
public:
  static const GuiApplication&                instance();
};

#endif // GUIAPPLICATION_H
