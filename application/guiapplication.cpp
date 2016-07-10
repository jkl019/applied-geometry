#include "guiapplication.h"

// local
#include "window.h"
#include "gmlibwrapper.h"

// hidmanager
#include "../hidmanager/defaulthidmanager.h"
#include "../hidmanager/hidmanagertreemodel.h"

// qt
#include <QQmlContext>
#include <QQuickItem>
#include <QStringListModel>

// stl
#include <cassert>


std::unique_ptr<GuiApplication> GuiApplication::_instance {nullptr};


GuiApplication::GuiApplication(int& argc, char **argv) : QGuiApplication(argc, argv) {

  assert(!_instance);
  _instance = std::unique_ptr<GuiApplication>(this);

  connect( &_window, &Window::sceneGraphInitialized,
           this,     &GuiApplication::onSceneGraphInitialized,
           Qt::DirectConnection );

  connect( &_window, &Window::sceneGraphInvalidated,
           this,     &GuiApplication::onSceneGraphInvalidated,
           Qt::DirectConnection );

  connect( this, &GuiApplication::signOnSceneGraphInitializedDone,
           this, &GuiApplication::afterSceneGraphInitialized );

  connect( this, &QGuiApplication::lastWindowClosed,
           this, &QGuiApplication::quit );

  _window.rootContext()->setContextProperty( "rc_name_model", &_scenario.rcNameModel() );
  _window.setSource(QUrl("qrc:///qml/main.qml"));

  _window.show();
}

GuiApplication::~GuiApplication() {

  _scenario.stop();
  _window.setPersistentOpenGLContext(false);
  _window.setPersistentSceneGraph(false);
  _window.releaseResources();
  _instance.release();
}

void
GuiApplication::onSceneGraphInitialized() {

  // Init GMlibWrapper
  _scenario.initialize();
  connect( &_scenario,  &GMlibWrapper::signFrameReady,
           &_window,    &Window::update );

//  connect( &_window,    &Window::signRcPairViewportChanged,
//           &_scenario,  &GMlibWrapper::changeRcPairViewport );

//  connect( &_window,    &Window::signRcPairActiveStateChanged,
//           &_scenario,  &GMlibWrapper::changeRcPairActiveState );

//  // Create hidmanager
//  _window->rootContext()->setContextProperty( "hidmanager_model", _hidmanager->getModel() );
//  _window->rootContext()->setContextProperty( "rc_name_model", &_gmlib->rcNameModel() );


//  connect( _hidmanager.get(), &StandardHidManager::signBeforeHidAction, this,  &GuiApplication::beforeHidAction, Qt::DirectConnection );
//  connect( _hidmanager.get(), &StandardHidManager::signAfterHidAction,  this,  &GuiApplication::afterHidAction );

  // Init test scene of the GMlib wrapper
  _scenario.initializeScenario();
  _scenario.prepare();

  emit signOnSceneGraphInitializedDone();
}

void GuiApplication::onSceneGraphInvalidated() {

  qDebug() << "on SceneGraph Invalidated";

  _scenario.cleanUp();
}

void
GuiApplication::afterSceneGraphInitialized() {

  // Update RCPair name model
  _scenario.updateRCPairNameModel();

  // Start simulator
  _scenario.start();

  _hidmanager = std::make_shared<DefaultHidManager>( &_scenario );
  connect( &_window, &Window::signKeyPressed,         _hidmanager.get(), &StandardHidManager::registerKeyPressEvent );
  connect( &_window, &Window::signKeyReleased,        _hidmanager.get(), &StandardHidManager::registerKeyReleaseEvent );
  connect( &_window, &Window::signMouseDoubleClicked, _hidmanager.get(), &StandardHidManager::registerMouseDoubleClickEvent);
  connect( &_window, &Window::signMouseMoved,         _hidmanager.get(), &StandardHidManager::registerMouseMoveEvent );
  connect( &_window, &Window::signMousePressed,       _hidmanager.get(), &StandardHidManager::registerMousePressEvent );
  connect( &_window, &Window::signMouseReleased,      _hidmanager.get(), &StandardHidManager::registerMouseReleaseEvent );
  connect( &_window, &Window::signWheelEventOccurred, _hidmanager.get(), &StandardHidManager::registerWheelEvent );

  _hidmanager->setupDefaultHidBindings();
}

Window&            GuiApplication::window()     {  return _window; }
//GMlibWrapper&      GuiApplication::gmlib()      {  return _gmlib; }
//DefaultHidManager& GuiApplication::hidmanager() {  return _hidmanager; }
std::shared_ptr<GMlib::Scene> GuiApplication::scene() {  return _scenario.scene(); }
const GuiApplication& GuiApplication::instance() {  return *_instance; }
