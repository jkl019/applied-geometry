#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H


class GLContextSurfaceWrapper;
class DefaultHidManager;

// qt
#include <QGuiApplication>

// stl
#include <memory>


class Window;
class GMlibWrapper;


class GuiApplication : public QGuiApplication {
  Q_OBJECT
public:
  explicit GuiApplication(int& argc, char* argv[]);
  ~GuiApplication();

private:
  std::shared_ptr<Window>                     _window;
  std::shared_ptr<GMlibWrapper>               _gmlib;
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;

  std::shared_ptr<DefaultHidManager>         _hidmanager;

  void                                        setupScene();

private slots:
  void                                        onSGInit();

  void                                        beforeHidAction();
  void                                        afterHidAction();



private:
  static std::unique_ptr<GuiApplication>    _instance;
public:
  static const GuiApplication&            getInstance();
};

#endif // GUIAPPLICATION_H
