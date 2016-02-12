
#ifndef STANDARDHIDMANAGER_H
#define STANDARDHIDMANAGER_H

// local
#include "hidmanager.h"
#include "hidkbmouseinput.h"
#include "hidinputevent.h"

class GLContextSurfaceWrapper;

// gmlib
namespace GMlib {

  class Camera;
  class Scene;
  class DefaultSelectRenderer;
}

// Qt
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;


// stl
#include <memory>


class StandardHidManager : public HidManager {
  Q_OBJECT
public:
  explicit StandardHidManager( std::shared_ptr<GMlib::Scene> scene,  std::shared_ptr<GMlib::Camera>,
                               std::shared_ptr<GMlib::DefaultSelectRenderer> select_renderer,
                               QObject* parent = 0x0 );



  //**********************************
  //
  // Standard Hid manager setup

  enum MouseEventType {
    MOUSE_NONE,
    MOUSE_CLICK,
    MOUSE_DBL_CLICK,
    MOUSE_RELEASE,
    MOUSE_MOVE
  };

  enum KeyEventType {
    KEY_NONE,
    KEY_PRESS,
    KEY_RELEASE
  };


  bool                        isKeyPressed( Qt::Key key ) const;
  bool                        isKeysPressed() const;
  bool                        isModifierPressed( Qt::KeyboardModifier keymod ) const;
  bool                        isMouseButtonDoublePressed() const;
  bool                        isMouseButtonPressed( Qt::MouseButton button ) const;

  virtual void                generateEvent();


  KeyEventType                getKeyEventType() const;
  void                        setKeyEventType( KeyEventType type );

  MouseEventType              getMouseEventType() const;
  void                        setMouseEventType( MouseEventType type );


  int                         getWheelDelta() const;
  void                        setWheelDelta( int delta );

  bool                        getWheelState() const;
  void                        setWheelState( bool state );



  void                        setupDefaultHidBindings();


public slots:
  virtual void                registerMousePressEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseReleaseEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseDoubleClickEvent(  const QString& name, QMouseEvent* event );
  virtual void                registerMouseMoveEvent( const QString& name, QMouseEvent* event );
  virtual void                registerKeyPressEvent( const QString& name, QKeyEvent* event );
  virtual void                registerKeyReleaseEvent( const QString& name,  QKeyEvent* event );
  virtual void                registerWheelEvent( const QString& name, QWheelEvent* event );


private:
  void                        registerKey( Qt::Key key,
                                           Qt::KeyboardModifiers modifiers );

  void                        registerMouseButtons( Qt::MouseButtons buttons,
                                                    Qt::KeyboardModifiers modifiers );
  void                        unregisterKey( Qt::Key key,
                                             Qt::KeyboardModifiers modifiers );

  KeyInput::Keymap            _keymap;
  Qt::KeyboardModifiers       _keymods;

  Qt::MouseButtons            _mouse_buttons;

  KeyEventType                _key_event_type;
  MouseEventType              _mouse_event_type;

  bool                        _wheel_state;
  int                         _wheel_delta;




  //**********************************
  //
  // GMlib scene control setup

public slots:
  virtual void                      heDeSelectAllObjects();
  virtual void                      heEdit();
  virtual void                      heLockTo();
  virtual void                      heLockCameraToObject();
  virtual void                      heLockCameraToScene();
//  virtual void                      heMoveBorderOrCamera();
//  virtual void                      heMoveBorder();
  virtual void                      heMoveCamera();
  virtual void                      heMoveSelectedObjects(  const HidInputEvent::HidInputParams& params );
  virtual void                      hePanHorizontal();
  virtual void                      hePanVertical();
//  virtual void                      hePushPopViewSets();
  virtual void                      heReplotQuick( int factor );
  virtual void                      heReplotQuickHigh();
  virtual void                      heReplotQuickLow();
  virtual void                      heReplotQuickMedium();
  virtual void                      heRotateSelectedObjects();
  virtual void                      heScaleSelectedObjects();
  virtual void                      heSelectAllObjects();
  virtual void                      heSelectObject();
  virtual void                      heSelectObjects();
  virtual void                      heSelectObjectTree( GMlib::SceneObject* obj );
  virtual void                      heToggleObjectDisplayMode();
  virtual void                      heToggleSimulation();
  virtual void                      heToggleSelectAllObjects();
  virtual void                      heUnlockCamera();
  virtual void                      heZoom();

  virtual void                      heLeftMouseReleaseStuff();

private:
  GMlib::Camera*                    activeCamera() const;
  float                             activeCameraSpeedScale();
  float                             cameraSpeedScale( GMlib::Camera* cam );
  const GMlib::Vector<int,2>&       cursorPos() const;
  const GMlib::Vector<int,2>&       previousCursorPos() const;
  QPointF       getPos2() const;
  QPointF       getPPos2() const;
  GMlib::Scene*                     scene() const;
  GMlib::SceneObject*               activeSelectObject();

  void                              savePos();
  void                              setPos( const QPointF& pos );

  std::shared_ptr<GMlib::Scene>     _scene;
  std::shared_ptr<GMlib::Camera>    _camera;
  std::shared_ptr<GMlib::DefaultSelectRenderer>     _select_renderer;

  GMlib::Vector<int,2>              _current_scene_pos;   // Current mouse scene pos
  GMlib::Vector<int,2>              _previous_scene_pos;  // Previous mouse scene pos

signals:
  void signToggleSimulation();

};



#endif // STANDARDHIDMANAGER_H







