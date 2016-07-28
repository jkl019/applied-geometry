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


  qRegisterMetaType<HidInputEvent::HidInputParams> ("HidInputEvent::HidInputParams");

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
  _window.rootContext()->setContextProperty( "hidmanager_model", _hidmanager.getModel() );
  _window.setSource(QUrl("qrc:///qml/main.qml"));

  _window.show();
}

GuiApplication::~GuiApplication() {

  _scenario.stop();
  _window.setPersistentOpenGLContext(false);
  _window.setPersistentSceneGraph(false);
  _window.releaseResources();
  _instance.release();

  qDebug() << "Bye bye ^^, ~~ \"emerge --oneshot life\"";
}

void
GuiApplication::onSceneGraphInitialized() {

  // Init GMlibWrapper
  _scenario.initialize();
  _hidmanager.init(_scenario);
  connect( &_scenario,  &GMlibWrapper::signFrameReady,
           &_window,    &Window::update );

  // Init test scene of the GMlib wrapper
  _scenario.initializeScenario();
  _scenario.prepare();

  emit signOnSceneGraphInitializedDone();
}

void GuiApplication::onSceneGraphInvalidated() {

  _scenario.cleanUp();
}

void
GuiApplication::afterSceneGraphInitialized() {

  // Update RCPair name model
  _scenario.updateRCPairNameModel();

  // Start simulator
  _scenario.start();

  // Hidmanager setup
  _hidmanager.setupDefaultHidBindings();

  connect( &_window, &Window::signKeyPressed,         &_hidmanager, &StandardHidManager::registerKeyPressEvent );
  connect( &_window, &Window::signKeyReleased,        &_hidmanager, &StandardHidManager::registerKeyReleaseEvent );
  connect( &_window, &Window::signMouseDoubleClicked, &_hidmanager, &StandardHidManager::registerMouseDoubleClickEvent);
  connect( &_window, &Window::signMouseMoved,         &_hidmanager, &StandardHidManager::registerMouseMoveEvent );
  connect( &_window, &Window::signMousePressed,       &_hidmanager, &StandardHidManager::registerMousePressEvent );
  connect( &_window, &Window::signMouseReleased,      &_hidmanager, &StandardHidManager::registerMouseReleaseEvent );
  connect( &_window, &Window::signWheelEventOccurred, &_hidmanager, &StandardHidManager::registerWheelEvent );

  connect( &_window,     &Window::beforeRendering,
           &_hidmanager, &DefaultHidManager::triggerOGLActions,
           Qt::DirectConnection );

  connect( &_hidmanager, &DefaultHidManager::signToggleSimulation,
           &_scenario,   &GMlibWrapper::toggleSimulation );

  connect( &_hidmanager,          SIGNAL(signOpenCloseHidHelp()),
           _window.rootObject(),  SIGNAL(toggleHidBindView()) );

}

void GuiApplication::onBeforeSync() {

//  if(_hidmanager) _hidmanager->forceUpdate();
}

Window&            GuiApplication::window()     {  return _window; }
std::shared_ptr<GMlib::Scene> GuiApplication::scene() {  return _scenario.scene(); }
const GuiApplication& GuiApplication::instance() {  return *_instance; }
