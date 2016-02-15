
#ifndef STANDARDHIDMANAGER_H
#define STANDARDHIDMANAGER_H

// local
#include "hidmanager.h"
#include "hidkbmouseinput.h"
#include "hidinputevent.h"

class GMlibWrapper;

// Qt
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;


// stl
#include <memory>


class StandardHidManager : public HidManager {
  Q_OBJECT
public:
  explicit StandardHidManager( std::shared_ptr<GMlibWrapper> gmlib, QObject* parent = 0x0 );



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




  void                        setupDefaultHidBindings();


  static GMlib::Point<int,2>  toGMlibPoint( const QPoint& point );



public slots:
  virtual void                registerMousePressEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseReleaseEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseDoubleClickEvent(  const QString& name, QMouseEvent* event );
  virtual void                registerMouseMoveEvent( const QString& name, QMouseEvent* event );
  virtual void                registerKeyPressEvent( const QString& name, QKeyEvent* event );
  virtual void                registerKeyReleaseEvent( const QString& name,  QKeyEvent* event );
  virtual void                registerWheelEvent( const QString& name, QWheelEvent* event );


private:
  void                        registerKeyEventType( KeyEventType type );
  void                        registerMouseEventType( MouseEventType type );
  virtual void                generateEvent();

  void                        registerKey( Qt::Key key,
                                           Qt::KeyboardModifiers modifiers );
  void                        unregisterKey( Qt::Key key,
                                             Qt::KeyboardModifiers modifiers );

  void                        registerMouseButtons( Qt::MouseButtons buttons,
                                                    Qt::KeyboardModifiers modifiers );

  void                        registerWheelData( bool state, int delta );


  void                        registerWindowPosition(const QPoint& pos );
  void                        registerRCPairName( const QString& name );


  KeyInput::Keymap            _reg_keymap;
  Qt::KeyboardModifiers       _reg_keymods;

  Qt::MouseButtons            _reg_mouse_buttons;

  bool                        _reg_wheel_state;
  int                         _reg_wheel_delta;

  QString                     _reg_rcpair_name    = {""};    // Current view name
  QPoint                      _reg_view_pos       = {0,0};
  QPoint                      _reg_view_prev_pos  = {0,0};



  bool                        isKeyRegistered( Qt::Key key ) const;
  bool                        isAnyKeysRegistered() const;
  bool                        isModKeyRegistered( Qt::KeyboardModifier keymod ) const;
  bool                        isMouseButtonRegistered( Qt::MouseButton button ) const;









  KeyEventType                _reg_next_key_event_type;
  MouseEventType              _reg_next_mouse_event_type;

























  //**********************************
  //
  // GMlib scene control setup

private slots:
  virtual void                      heDeSelectAllObjects();
  virtual void                      heEdit();
  virtual void                      heLockTo( const HidInputEvent::HidInputParams& params );
  virtual void                      heMoveCamera( const HidInputEvent::HidInputParams& params );
  virtual void                      heMoveSelectedObjects( const HidInputEvent::HidInputParams& params );
  virtual void                      hePanHorizontal( const HidInputEvent::HidInputParams& params );
  virtual void                      hePanVertical( const HidInputEvent::HidInputParams& params );
  virtual void                      heReplotQuick( int factor );
  virtual void                      heReplotQuickHigh();
  virtual void                      heReplotQuickLow();
  virtual void                      heReplotQuickMedium();
  virtual void                      heRotateSelectedObjects( const HidInputEvent::HidInputParams& params );
  virtual void                      heScaleSelectedObjects( const HidInputEvent::HidInputParams& params );
  virtual void                      heSelectAllObjects();
  virtual void                      heSelectObject( const HidInputEvent::HidInputParams& params );
  virtual void                      heSelectObjects( const HidInputEvent::HidInputParams& params );
  virtual void                      heSelectObjectTree( GMlib::SceneObject* obj );
  virtual void                      heToggleObjectDisplayMode();
  virtual void                      heToggleSimulation();
  virtual void                      heToggleSelectAllObjects();
//  virtual void                      heUnlockCamera();
  virtual void                      heZoom( const HidInputEvent::HidInputParams& params );

  virtual void                      heLeftMouseReleaseStuff();
  virtual void                      heOpenCloseHidHelp();

private:
  GMlib::Camera*                    findCamera( const QString& view_name ) const;
  float                             cameraSpeedScale( GMlib::Camera* cam ) const;
  GMlib::Scene*                     scene() const;
  GMlib::SceneObject*               findSceneObject(const QString& view_name, const QPoint& pos);

  GMlib::Point<int,2>               toGMlibViewPosition(const QString& view_name, const QPoint& pos);

  std::shared_ptr<GMlibWrapper>     _gmlib;


signals:
  void signToggleSimulation();
  void signOpenCloseHidHelp();

};



#endif // STANDARDHIDMANAGER_H







