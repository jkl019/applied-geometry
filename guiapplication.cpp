#include "guiapplication.h"

// local
#include "glcontextsurfacewrapper.h"
#include "window.h"
#include "gmlibwrapper.h"

// hidmanager
#include "hidmanager/defaulthidmanager.h"
#include "hidmanager/hidmanagertreemodel.h"

// qt
#include <QQmlContext>
#include <QQuickItem>

// stl
#include <cassert>


std::unique_ptr<GuiApplication> GuiApplication::_instance {nullptr};


GuiApplication::GuiApplication(int& argc, char *argv[])
  : QGuiApplication(argc, argv),
    _window{std::make_shared<Window>()}, _gmlib{nullptr}, _glsurface{nullptr}
{

  assert(!_instance);
  _instance = std::unique_ptr<GuiApplication>(this);

  connect( _window.get(), &Window::sceneGraphInitialized, this, &GuiApplication::onSGInit );

  _window->show();
}

GuiApplication::~GuiApplication() {

  _glsurface->makeCurrent(); {

    _gmlib.reset();
    _window.reset();

  } _glsurface->doneCurrent();
}

void
GuiApplication::onSGInit() {

  // Init GLSurface
  _window->initGLSurface();

  // Init GMlibWrapper
  _glsurface = _window->getGLSurface();
  _gmlib = std::make_shared<GMlibWrapper>(_glsurface);
//  connect( _gmlib.get(),  &GMlibWrapper::signFrameReady,   _window.get(), &Window::signFrameReady );
  connect( _gmlib.get(),  &GMlibWrapper::signFrameReady,   _window.get(), &Window::update );
  connect( _window.get(), &Window::signGuiViewportChanged, _gmlib.get(),  &GMlibWrapper::changeRenderGeometry );

  // Init test scene of the GMlib wrapper
  _gmlib->initScene();

  // Create hidmanager (must be after initScene)
  _hidmanager = std::make_shared<DefaultHidManager>( _gmlib );
  _window->rootContext()->setContextProperty( "hidmanager_model", _hidmanager->getModel() );

  connect( _window.get(), &Window::signMousePressed,       _hidmanager.get(), &StandardHidManager::registerMousePressEvent );
  connect( _window.get(), &Window::signMouseReleased,      _hidmanager.get(), &StandardHidManager::registerMouseReleaseEvent );
  connect( _window.get(), &Window::signMouseDoubleClicked, _hidmanager.get(), &StandardHidManager::registerMouseDoubleClickEvent);
  connect( _window.get(), &Window::signMouseMoved,         _hidmanager.get(), &StandardHidManager::registerMouseMoveEvent );
  connect( _window.get(), &Window::signKeyPressed,         _hidmanager.get(), &StandardHidManager::registerKeyPressEvent );
  connect( _window.get(), &Window::signKeyReleased,        _hidmanager.get(), &StandardHidManager::registerKeyReleaseEvent );
  connect( _window.get(), &Window::signWheelEventOccurred, _hidmanager.get(), &StandardHidManager::registerWheelEvent );


  connect( _hidmanager.get(), &DefaultHidManager::signToggleSimulation,
           _gmlib.get(),      &GMlibWrapper::toggleSimulation );

  connect( _hidmanager.get(), &StandardHidManager::signBeforeHidAction, this,  &GuiApplication::beforeHidAction, Qt::DirectConnection );
  connect( _hidmanager.get(), &StandardHidManager::signAfterHidAction,  this,  &GuiApplication::afterHidAction );

  _hidmanager->setupDefaultHidBindings();

  // Load gui qml
  _window->setSource( QUrl("qrc:/qml/main.qml") );

  connect( _hidmanager.get(), SIGNAL(signOpenCloseHidHelp()), _window->rootObject(), SIGNAL(toggleHidBindView()) );



  // Start simulator
  _gmlib->start();
}

void GuiApplication::beforeHidAction() {

//  qDebug() << "BeforeHidAction:";
  _glsurface->makeCurrent();
}

void GuiApplication::afterHidAction() {

//  qDebug() << "AfterHidAction:";
  _glsurface->doneCurrent();
}

const GuiApplication&
GuiApplication::getInstance() {

  return *_instance;
}
