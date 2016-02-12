#include "standardhidmanager.h"
#include "../glcontextsurfacewrapper.h"

// local
#include "hidinputevent.h"
#include "hidkbmouseinput.h"

// gmlib
#include <gmSceneModule>
#include <gmParametricsModule>
using namespace GMlib;

// qt
#include <QtCore>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#define QT_NO_OPENGL
//#include <QtWidgets>

// stl
#include <cmath>
#include <cassert>

// Local Defines
#define SNAP 0.01





//StandardHidManager::StandardHidManager(Scene* scene, Camera* camera, QObject *parent) :
//  HidManager( parent ), _scene(scene), _camera(camera) {

//  // Default values
//  _previous_scene_pos = _current_scene_pos = Vector<int,2>(0.0f);

//  _key_event_type = KEY_NONE;
//  _mouse_event_type = MOUSE_NONE;
//  _wheel_state = false;

//  // Init GMWindow states
////  _move_border = false;
//}

StandardHidManager::StandardHidManager( std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera,
                                        std::shared_ptr<GMlib::DefaultSelectRenderer> select_renderer,
                                        QObject* parent
                                        ) :
  HidManager(parent), _scene{scene}, _camera{camera}, _select_renderer{select_renderer} {

  // Default values
  _previous_scene_pos = _current_scene_pos = Vector<int,2>(0.0f);

  _key_event_type = KEY_NONE;
  _mouse_event_type = MOUSE_NONE;
  _wheel_state = false;
}

void StandardHidManager::registerKeyPressEvent(const QString& name, QKeyEvent *e) {

  registerKey( Qt::Key(e->key()), e->modifiers() );
  setKeyEventType( KEY_PRESS );
  generateEvent();
}

void StandardHidManager::registerKeyReleaseEvent(const QString& name, QKeyEvent *e) {

  unregisterKey( Qt::Key(e->key()), e->modifiers() );
  setKeyEventType( KEY_RELEASE );
  generateEvent();
}

void StandardHidManager::registerMouseDoubleClickEvent(const QString& name, QMouseEvent* e ) {

  setPos( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  setMouseEventType( MOUSE_DBL_CLICK );

  generateEvent();
}

void StandardHidManager::registerMouseMoveEvent(const QString& name, QMouseEvent* e) {

  setPos( e->pos() );
  setMouseEventType( MOUSE_MOVE );

  generateEvent();
}

void StandardHidManager::registerMousePressEvent(const QString& name, QMouseEvent* e) {

  setPos( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  setMouseEventType( MOUSE_CLICK );

  generateEvent();
}

void StandardHidManager::registerMouseReleaseEvent(const QString& name, QMouseEvent* e){

  setPos( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  setMouseEventType( MOUSE_RELEASE );

  generateEvent();
}


void StandardHidManager::registerKey(Qt::Key key, Qt::KeyboardModifiers modifiers) {

  _keymods = modifiers;

  if( isKeyPressed( key ) )
    return;

  _keymap[key] = true;
}

void StandardHidManager::registerMouseButtons(Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers ) {

  _mouse_buttons = buttons;
  _keymods = modifiers;
}

void StandardHidManager::setKeyEventType(StandardHidManager::KeyEventType type) {

  _key_event_type = type;
}

void StandardHidManager::setMouseEventType(StandardHidManager::MouseEventType type ) {

  _mouse_event_type = type;
}

void StandardHidManager::setWheelDelta(int delta) {

  _wheel_delta = delta;
}

void StandardHidManager::setWheelState(bool state) {

  _wheel_state = state;
}

void StandardHidManager::setupDefaultHidBindings() {

  //// Register Hid Actions

  // Camera
//  QString ha_id_view_move_border_or_camera =
//      registerHidAction("View",
//                        "Move Border/Camera",
//                        "Move the border/camera. ",
//                        this, SLOT(heMoveBorderOrCamera()) );

  QString ha_id_view_move_camera =
      registerHidAction("View",
                        "Move Camera",
                        "Move the camera. "
                        "If not locked to the scene, it will pan the camera in the view plane. "
                        "If locked it will rotate the camera about the center of the scene." ,
                        this, SLOT(heMoveCamera()) );

  QString ha_id_view_pan_h =
      registerHidAction("View",
                        "Pan Horizontally",
                        "Pan horizontally",
                        this, SLOT(hePanHorizontal()) );

  QString ha_id_view_pan_v =
      registerHidAction("View",
                        "Pan Vertically",
                        "Pan vertically",
                        this, SLOT(hePanVertical()) );

  QString ha_id_view_zoom=
      registerHidAction("View",
                        "Zoom",
                        "Zoom",
                        this, SLOT(heZoom()) );

  QString ha_id_view_lock_to =
      registerHidAction("View",
                        "Lock To ...",
                        "Lock camera to an object or to the scene.",
                        this, SLOT(heLockTo()) );

//  QString ha_id_view_moveborder =
//      registerHidAction( "View",
//                         "Move Border",
//                         "Move the border",
//                         this, SLOT(heMoveBorder()) );

//  QString ha_id_view_push_pop_viewset =
//      registerHidAction( "View",
//                         "Push/Pop View",
//                         "Push or pop the current view set.",
//                         this, SLOT(hePushPopViewSets()) );

  // Object Transformation
  QString ha_id_objtrans_scale =
      registerHidAction("Object transformation",
                        "Scale Objects",
                        "Scale objects",
                        this, SLOT(heScaleSelectedObjects()) );

  QString ha_id_objtrans_move =
      registerHidAction("Object transformation",
                        "Move Objects",
                        "Move objects",
                        this, SLOT(heMoveSelectedObjects(HidInputEvent::HidInputParams)) );

  QString ha_id_objtrans_rotate =
      registerHidAction("Object transformation",
                        "Rotate Objects",
                        "Rotate objects",
                        this, SLOT(heRotateSelectedObjects()) );

  // Object Selection
  QString ha_id_objsel_toggle_all =
      registerHidAction("Object selection",
                        "Toggle: Select all objects",
                        "Toggle Selection for all objects",
                        this, SLOT(heToggleSelectAllObjects()) );

  QString ha_id_objsel_select =
      registerHidAction("Object selection",
                        "Select Object",
                        "Select object",
                        this, SLOT(heSelectObject()) );

  QString ha_id_objsel_select_multi =
      registerHidAction("Object selection",
                        "Select Objects",
                        "Select objects",
                        this, SLOT(heSelectObjects()) );

  // Object Interaction
  QString ha_id_objint_toggle_edit =
      registerHidAction( "Object interaction",
                         "Toggle: Edit mode",
                         "Toggle edit mode for editable objects",
                         this, SLOT(heEdit()) );

  QString ha_id_objint_replot_high =
      registerHidAction( "Object interaction",
                         "Replot: QuickHigh",
                         "Replot with \"high\" resolution",
                         this, SLOT(heReplotQuickHigh()) );

  QString ha_id_objint_replot_med =
      registerHidAction( "Object interaction",
                         "Replot: QuickMedium",
                         "Replot with \"medium\" resolution",
                         this, SLOT(heReplotQuickMedium()) );

  QString ha_id_objint_replot_low =
      registerHidAction( "Object interaction",
                         "Replot: QuickLow",
                         "Replot with \"low\" resolution",
                         this, SLOT(heReplotQuickLow()) );



  // Rendering
  QString ha_id_render_toggle_shademode =
      registerHidAction( "Rendering",
                         "Toggle: Shading mode",
                         "Toggle shadeing mode",
                         this, SLOT(heToggleObjectDisplayMode()) );

  // Simulator
  QString ha_id_sim_toggle =
      registerHidAction( "Simulator",
                         "Toggle: Simulation",
                         "Toggle simulation",
                         this, SLOT(heToggleSimulation()) );

  // Various cleanup
  QString ha_id_var_lm_rel =
      registerHidAction( "Various",
                         "Left Mouse Release",
                         "Stuff that happens on left mouse release",
                         this, SLOT(heLeftMouseReleaseStuff()) );



  //// Set up initial mapping
  registerHidMapping( ha_id_objsel_toggle_all,            new KeyPressInput( Qt::Key_A ) );
  registerHidMapping( ha_id_objint_toggle_edit,           new KeyPressInput( Qt::Key_E ) );
  registerHidMapping( ha_id_objint_replot_high,           new KeyPressInput( Qt::Key_P, Qt::ShiftModifier ) );
  registerHidMapping( ha_id_objint_replot_med,            new KeyPressInput( Qt::Key_P ) );
  registerHidMapping( ha_id_objint_replot_low,            new KeyPressInput( Qt::Key_P, Qt::ControlModifier) );
  registerHidMapping( ha_id_sim_toggle,                   new KeyPressInput( Qt::Key_R ) );
  registerHidMapping( ha_id_render_toggle_shademode,      new KeyPressInput( Qt::Key_Z ) );

  registerHidMapping( ha_id_objsel_select,                new MousePressInput( Qt::RightButton ) );
  registerHidMapping( ha_id_view_lock_to,                 new MousePressInput( Qt::RightButton, Qt::ControlModifier ) );
  registerHidMapping( ha_id_objsel_select_multi,          new MousePressInput( Qt::RightButton, Qt::ShiftModifier ) );

//  registerHidMapping( ha_id_view_push_pop_viewset,        new MouseDoubleClickInput( Qt::LeftButton ) );

//  registerHidMapping( ha_id_view_move_border_or_camera,   new MouseMoveInput( Qt::LeftButton ) );
  registerHidMapping( ha_id_objtrans_scale,               new MouseMoveInput( Qt::LeftButton, Qt::ControlModifier | Qt::AltModifier ) );
  registerHidMapping( ha_id_objtrans_move,                new MouseMoveInput( Qt::LeftButton, Qt::ShiftModifier ) );
  registerHidMapping( ha_id_objtrans_rotate,              new MouseMoveInput( Qt::LeftButton, Qt::ControlModifier ) );

  registerHidMapping( ha_id_var_lm_rel,                   new MouseReleaseInput( Qt::LeftButton ) );

  registerHidMapping( ha_id_view_pan_h,                   new WheelInput( Qt::ControlModifier ) );
  registerHidMapping( ha_id_view_pan_v,                   new WheelInput( Qt::ShiftModifier ) );
  registerHidMapping( ha_id_view_zoom,                    new WheelInput() );
}

void StandardHidManager::unregisterKey(Qt::Key key, Qt::KeyboardModifiers modifiers) {


  // Update modifier
  _keymods       = modifiers;

  // Update keymap
  if( !isKeyPressed(key) )
    return;

  _keymap.remove( key );
}

StandardHidManager::KeyEventType StandardHidManager::getKeyEventType() const {

  return _key_event_type;
}

StandardHidManager::MouseEventType StandardHidManager::getMouseEventType() const {

  return _mouse_event_type;
}

void StandardHidManager::registerWheelEvent(const QString& name, QWheelEvent *e) {

  // Save position and wheel delta
  setPos( e->pos() );
  setWheelDelta( e->delta() );
  setWheelState(true);

  generateEvent();
}

bool StandardHidManager::isKeyPressed(Qt::Key key) const {

  return _keymap.value(key, false);
}

bool StandardHidManager::isKeysPressed() const {

  return _keymap.size() > 0;
}

bool StandardHidManager::isModifierPressed(Qt::KeyboardModifier keymod) const {

  // No modifier is a special case
  if( keymod == Qt::NoModifier ) {
    return _keymods == Qt::NoModifier;
  }

  return (_keymods & keymod) == keymod;
}

bool StandardHidManager::isMouseButtonPressed(Qt::MouseButton button) const {

  // No button is a special case
  if( button == Qt::NoButton ) {
    return _mouse_buttons == Qt::NoButton;
  }

  return (_mouse_buttons & button) == button;
}


int StandardHidManager::getWheelDelta() const {

  return _wheel_delta;
}

bool StandardHidManager::getWheelState() const {

  return _wheel_state;
}

void StandardHidManager::generateEvent() {

  HidInputEvent::HidInputParams params;
  params["curr_pos"] = QVariant( getPos2() );
  params["prev_pos"] = QVariant( getPPos2() );

  if( getMouseEventType() == MOUSE_MOVE ) {
        QCoreApplication::sendEvent( this, new HidInputEvent( MouseMoveInput( _mouse_buttons, _keymods ), params ) );
    setMouseEventType( MOUSE_NONE );
  }
  else if( getWheelState() ) {
        QCoreApplication::sendEvent( this, new HidInputEvent( WheelInput( _keymods ) ) );
    setWheelState(false);
  }
  else if( getMouseEventType() != MOUSE_NONE ) {

    switch( getMouseEventType() ) {
      case MOUSE_DBL_CLICK:
        QCoreApplication::sendEvent( this, new HidInputEvent( MouseDoubleClickInput( _mouse_buttons, _keymods ) ) );
        break;
      case MOUSE_CLICK:
        QCoreApplication::sendEvent( this, new HidInputEvent( MousePressInput( _mouse_buttons, _keymods ) ) );
        break;
      case MOUSE_RELEASE:
        QCoreApplication::sendEvent( this, new HidInputEvent( MouseReleaseInput( _mouse_buttons, _keymods ) ) );
        break;
    }

    setMouseEventType( MOUSE_NONE );
  }
  else if( getKeyEventType() != KEY_NONE ) {

    switch( getKeyEventType() ) {
      case KEY_PRESS: {
        QCoreApplication::sendEvent( this, new HidInputEvent( KeyPressInput( _keymap, _keymods ) ) );
      } break;
      case KEY_RELEASE: {
        QCoreApplication::sendEvent( this, new HidInputEvent( KeyReleaseInput( _keymap, _keymods ) ) );
      } break;
    }

    setKeyEventType( KEY_NONE );
  }


  savePos();



//  qDebug() << "HidManager::processEvents()";
//  qDebug() << "  Keys:                  " << _no_keys_pressed;
//  qDebug() << "  Mouse buttons:         " << _mouse_buttons;
//  qDebug() << "  Modifiers:             " << _keymod;
//  qDebug() << "  Mouse move:            " << _mouse_move;
//  qDebug() << "  Mouse double click:    " << _mouse_dc;


  ///////////////////////
  // Mouse input handling
  ///////////////////////
//  if( getMouseMoveState() ) {

//    if( isMouseButtonPressed( Qt::LeftButton ) ) {
//      if( isModifierPressed( Qt::NoModifier ) ) {
//        if( _move_border )
//          heMoveBorder();
//        else
//          heMoveCamera();
//      }
//      else if( isModifierPressed( Qt::ControlModifier ) && isModifierPressed( Qt::AltModifier ) ) {
//        heScaleSelectedObjects();
//      }
//      else if( isModifierPressed( Qt::ShiftModifier ) ) {
//        heMoveSelectedObjects();
//      }
//      else if( isModifierPressed(Qt::ControlModifier ) ) {
//        heRotateSelectedObjects();
//      }
//    }
//    setMouseMoveState(false);
//  }
//  else if( getWheelState() ) {

//    // Pan "x" direction
//    if( isModifierPressed( Qt::ControlModifier ) )
//      hePanHorizontal();

//    // Pan "y" direction
//    else if( isModifierPressed( Qt::ShiftModifier ) )
//      hePanVertical();

//    // Zoom
//    else if( isModifierPressed( Qt::NoModifier ) )
//      heZoom();

//    setWheelState(false);
//  }
//  else {

//    if( getMouseDoubleClickState() && isMouseButtonPressed( Qt::LeftButton ) ) {

//      hePushPopViewSets();
//    }
//    else {

//      if( isMouseButtonPressed( Qt::LeftButton ) ) {

//        if( isModifierPressed( Qt::NoModifier ) ) {

//          Camera *cam = getActiveCamera();
//          if( !cam )
//            _move_border = true;
//        }
//      }
//      else if( isMouseButtonPressed( Qt::RightButton ) ) {

//        if( isModifierPressed( Qt::ShiftModifier ) ) {

//          heSelectObject();
//        }
//        else if( isModifierPressed( Qt::ControlModifier ) ) {

//          heLockCameraToObject();
//        }
//        else {

//          if( getSelectObject() )
//            heSelectObject();
//          else
//            heLockCameraToScene();
//        }
//      }
//      else {

//        if( _move_border )
//          _move_border = false;
//      }
//    }


//    /////////////////////
//    // Key input handling
//    /////////////////////
//    if( isKeysPressed() ) {

//      // Select/deselect all
//      if( isKeyPressed(Qt::Key_A ) )
//        heToggleSelectAllObjects();

//      // Edit
//      else if( isKeyPressed(Qt::Key_E) )
//        heEdit();

//      // Replot
//      else if( isKeyPressed(Qt::Key_P) ) {


//        if( isModifierPressed(Qt::ShiftModifier) )
//          heReplotQuickHigh();
//        else if( isModifierPressed(Qt::NoModifier) )
//          heReplotQuickLow();
//      }

//      // Quit
//      else if( isKeyPressed(Qt::Key_Q) )
//        heQuit();

//      // Toggle run
//      else if( isKeyPressed(Qt::Key_R) )
//        heToggleSimulation();

//      // Toggle shade mode for an object
//      else if( isKeyPressed(Qt::Key_Z) )
//        heToggleObjectDisplayMode();

//      // Grab framebuffer
//      else if( isKeyPressed(Qt::Key_F) )
//        emit signGrabFrameBuffer();
//    }
//  }

  // Save coords
//  savePos();
}






///////////////////////////////////////////////////////////////////////
//   *******   GMlib Scene Stuff
///////////////////////////////////////////////////////////////////////
void StandardHidManager::heDeSelectAllObjects() {

  scene()->removeSelections();
}

void StandardHidManager::heEdit() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject *sel_obj = sel_objs(i);

    // ERBS
    PERBSCurve<float> *ecObj = dynamic_cast<PERBSCurve<float>*>( sel_obj );
    PERBSSurf<float> *esObj = dynamic_cast<PERBSSurf<float>*>( sel_obj );
    PERBSTriangle<float> *etObj = dynamic_cast<PERBSTriangle<float>*>( sel_obj );

    // Bezier
    PBezierCurve<float> *bcObj = dynamic_cast<PBezierCurve<float>*>( sel_obj );
    PBezierSurf<float> *bsObj = dynamic_cast<PBezierSurf<float>*>( sel_obj );
    PBezierTriangle<float> *btObj = dynamic_cast<PBezierTriangle<float>*>( sel_obj );

    // Arc
    PArc<float> *acObj = dynamic_cast<PArc<float>*>( sel_obj );

    // ERBS
    if( ecObj ) {

      if( ecObj->isLocalCurvesVisible() )
        ecObj->hideLocalCurves();
      else
        ecObj->showLocalCurves();
    }
    else if( esObj ) {

      if( esObj->isLocalPatchesVisible() )
        esObj->hideLocalPatches();
      else
        esObj->showLocalPatches();
    }
    else if( etObj ) {
      if( etObj->isLocalPatchesVisible() )
        etObj->hideLocalPatches();
      else
        etObj->showLocalPatches();
    }
    // Bezier
    else if( bcObj ) {

      PERBSCurve<float> *parent = dynamic_cast<PERBSCurve<float>*>( bcObj->getParent() );
      if( parent ) {

        if( bcObj->toggleCollapsed() )
          bcObj->hideSelectors();
        else
          bcObj->showSelectors();
      }
      else {

        if( bcObj->isSelectorsVisible() )
          bcObj->hideSelectors();
        else
          bcObj->showSelectors();
      }
    }
    else if( bsObj ) {

      PERBSSurf<float> *parent = dynamic_cast<PERBSSurf<float>*>( bsObj->getParent());
      if( parent ) {

        if( bsObj->toggleCollapsed() )
          bsObj->hideSelectors();
        else
          bsObj->showSelectors(true);
      }
      else {

        if( bsObj->isSelectorsVisible() )
          bsObj->hideSelectors();
        else
          bsObj->showSelectors(true);
      }
    }
    else if( btObj ) {

      PERBSTriangle<float> *parent = dynamic_cast<PERBSTriangle<float>*>( btObj->getParent() );
      if( parent ) {

        if( btObj->toggleCollapsed() )
          btObj->hideSelectors();
        else
          btObj->showSelectors(true);
      }
      else {

        if( btObj->isSelectorsVisible() )
          btObj->hideSelectors();
        else
          btObj->showSelectors(true);
      }
    }
    else if( acObj ) {

      acObj->toggleCollapsed();
    }

  }
}

void StandardHidManager::heLockTo() {

  SceneObject *sel_obj = activeSelectObject();
  if( sel_obj )
    heLockCameraToObject();
  else
    heLockCameraToScene();
}

void StandardHidManager::heLockCameraToObject() {

  Camera *cam = activeCamera();
  SceneObject *sel_obj = activeSelectObject();
  if( !cam || !sel_obj )
    return;

  // Lock camera to selected object
  cam->lock( sel_obj );
}

void StandardHidManager::heLockCameraToScene() {

  Camera *cam = activeCamera();
  SceneObject *sel_obj = activeSelectObject();
  if( !cam || sel_obj )
    return;

  // Lock/unlock camera to scene
  if( cam->isLocked() )
    cam->unLock();
  else {

    cam->lock(
      ( scene()->getSphereClean().getPos() - cam->getPos() ) *
      cam->getDir() );
  }
}

//void StandardHidManager::heMoveBorderOrCamera() {

//  Camera *cam = getActiveCamera();
//  if( !cam )
//    heMoveBorder();
//  else {
//    if( _move_border )
//      heMoveBorder();
//    else
//      heMoveCamera();
//  }
//}

//void StandardHidManager::heMoveBorder() {


//  _move_border = true;

//  const Vector<int,2> pos = getPos();

//  getWindow()->moveBorder(pos(0),pos(1));
//}

void StandardHidManager::heMoveCamera() {

  Camera *cam = activeCamera();
  if( !cam )
    return;

  const float scale = cameraSpeedScale( cam );
  const Vector<int,2> pos = cursorPos();
  const Vector<int,2> prev = previousCursorPos();
  const Vector<float,2> delta (
     (pos(0) - prev(0)) * scale / cam->getViewportW(),
     (prev(1) - pos(1)) * scale / cam->getViewportH()
  );
  cam->move( delta );
}

void StandardHidManager::heMoveSelectedObjects( const HidInputEvent::HidInputParams& params ) {

  Camera *cam = activeCamera();
  if( !cam )
    return;

  QPointF q_pos       = params["curr_pos"].toPointF();
  QPointF q_prev_pos  = params["prev_pos"].toPointF();

//  qDebug() << "//////////////////////";
//  qDebug() << "Pos: " << q_pos;
//  qDebug() << "Pos: " << q_prev_pos;

  const Vector<int,2> pos( q_pos.x(), q_pos.y() );
  const Vector<int,2> prev( q_prev_pos.x(), q_prev_pos.y() );
//  const Vector<int,2> pos = getPos();
//  const Vector<int,2> prev = getPPos();

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject* obj = sel_objs(i);

    const double dh = cam->deltaTranslate( obj );
    const Vector<float,3> deltav(
      ( ( prev(0) - pos(0) ) * dh ) * cam->getSide() +
      ( ( pos(1) - prev(1) ) * dh ) * cam->getUp() );

    if( deltav.getLength() > SNAP && deltav.getLength() < 1000.0 ) {

      if( obj->getTypeId() != GM_SO_TYPE_SELECTOR )
        obj->translateGlobal( deltav );
      else if( obj->getTypeId()== GM_SO_TYPE_SELECTOR )
        obj->editPos(deltav);
    }
  }
}

void StandardHidManager::hePanHorizontal() {

  Camera *cam = activeCamera();
  if( cam )
    cam->move(
      Vector<float,2>(
        getWheelDelta() * cameraSpeedScale(cam) / cam->getViewportH(),
        0.0f
        ));
}

void StandardHidManager::hePanVertical() {

  Camera *cam = activeCamera();
  if( cam )
    cam->move(
      Vector<float,2>(
        0.0f,
        getWheelDelta() * cameraSpeedScale(cam) / cam->getViewportW()
            ));
}

//void StandardHidManager::hePushPopViewSets() {

//  Camera *cam = activeCamera();
//  if( !cam )
//    return;

//  GMWindow *window = getWindow();

//  if( window->getViewSetStackSize() > 2 )
//    window->popViewSet();
//  else
//    window->addViewSet( window->getCameraIndex( getActiveCamera() ) );

//  window->prepareViewSets();
//}

void StandardHidManager::heReplotQuick(int factor) {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    GMlib::SceneObject *sel_obj = sel_objs(i);

    GMlib::PCurve<float,3> *curve = dynamic_cast<GMlib::PCurve<float,3>*>( sel_obj );
    GMlib::PSurf<float,3> *surf = dynamic_cast<GMlib::PSurf<float,3>*>( sel_obj );

    if( curve ) {

      GMlib::PERBSCurve<float> *erbs = dynamic_cast<GMlib::PERBSCurve<float>*>(curve);
      if( erbs )
        erbs->replot(
          (erbs->getLocalCurves().getDim()-1)*factor + 1,
          1 );
      else
        curve->replot( std::pow<int>( factor, 2 ) * 100, 2 );
    }
    else if( surf ) {

      GMlib::PERBSSurf<float> *erbs = dynamic_cast<GMlib::PERBSSurf<float>*>(surf);
      if( erbs )
        erbs->replot(
          (erbs->getLocalPatches().getDim1()-1)*factor + 1,
          (erbs->getLocalPatches().getDim2()-1)*factor + 1,
          2, 2 );
      else
        surf->replot( 10 * factor, 10 * factor, 2, 2 );
    }
  }
}

void StandardHidManager::heReplotQuickHigh() {

  heReplotQuick(20);
}

void StandardHidManager::heReplotQuickLow() {

  heReplotQuick(1);
}

void StandardHidManager::heReplotQuickMedium() {

  heReplotQuick(10);
}

void StandardHidManager::heRotateSelectedObjects() {

  Camera *cam = activeCamera();
  if( !cam )
    return;

  const Vector<int,2> pos = cursorPos();
  const Vector<int,2> prev = previousCursorPos();
  const Array<SceneObject*> &objs = scene()->getSelectedObjects();

  // Compute rotation axis and angle in respect to the camera and view.
  const UnitVector<float,3> rot_v =
    float( pos(0) - prev(0) ) * cam->getUp() -
    float( prev(1) - pos(1) ) * cam->getSide();
  const Angle ang(
    M_2PI * sqrt(
      pow( double( pos(0) - prev(0) ) / cam->getViewportW(), 2 ) +
      pow( double( prev(1) - pos(1) ) / cam->getViewportH(), 2 ) ) );


  int no_objs = 0;
  Sphere<float,3> sphere;
  for( int i = 0; i < objs.getSize(); ++i )
    if( objs(i)->getTypeId() != GM_SO_TYPE_SELECTOR ) {
      sphere += objs(i)->getSurroundingSphereClean();
      no_objs++;
    }

  for( int i = 0; i < objs.getSize(); ++i )
    if( objs(i)->getTypeId() != GM_SO_TYPE_SELECTOR )
      if( std::abs(pos(0)-prev(0)) > POS_TOLERANCE || std::abs(pos(1)-prev(1)) > POS_TOLERANCE )
        no_objs > 1 ? objs(i)->rotate( ang, sphere.getPos(), rot_v) : objs(i)->rotate( ang, rot_v);
}

void StandardHidManager::heScaleSelectedObjects() {

  Camera *cam = activeCamera();
  if( !cam )
    return;

  const Vector<int,2> pos = cursorPos();
  const Vector<int,2> prev = previousCursorPos();

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject* obj = sel_objs(i);

    const double dh = cam->deltaTranslate( obj );
    const Vector<float,3> deltav(
      ( ( prev(0) - pos(0) ) * dh ) * cam->getSide() +
      ( ( pos(1) - prev(1) ) * dh ) * cam->getUp() );


    if( deltav.getLength() < 1000.0 )
      obj->scale( Vector<float,3>( 1.0f + deltav(1) ) );
  }
}

void StandardHidManager::heSelectAllObjects() {

  Scene *scene = this->scene();
  for( int i = 0; i < scene->getSize(); ++i )
    heSelectObjectTree( (*scene)[i] );
}

void StandardHidManager::heSelectObject() {

  SceneObject *sel_obj = activeSelectObject();
  if( !sel_obj )
    return;

  bool selected = sel_obj->isSelected();

  heDeSelectAllObjects();

  sel_obj->setSelected( !selected );
}

void StandardHidManager::heSelectObjects() {

  SceneObject *sel_obj = activeSelectObject();

  if( !sel_obj )
    return;

  sel_obj->toggleSelected();
}

void StandardHidManager::heSelectObjectTree( SceneObject* obj ) {

  // Do not select cameras or lights
  GMlib::Camera *cam   = dynamic_cast<GMlib::Camera*>( obj );
  GMlib::Light  *light = dynamic_cast<GMlib::Light*>( obj );
  if( !cam && !light )
    obj->setSelected(true);

  // Recursive Propagation
  for( int i = 0; i < obj->getChildren().getSize(); i++ )
    heSelectObjectTree( (obj->getChildren())[i] );
}

void StandardHidManager::heToggleObjectDisplayMode() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {


    auto obj = sel_objs(i);
    GMlib::Array<GMlib::Visualizer*> &visus = obj->getVisualizers();
    for( int i = 0; i < visus.getSize(); i++ ) {

      visus[i]->toggleDisplayMode();
    }
  }
}

void StandardHidManager::heToggleSimulation() {

  emit signToggleSimulation();
}

void StandardHidManager::heToggleSelectAllObjects() {

  if( scene()->getSelectedObjects().getSize() > 0 )
    heDeSelectAllObjects();
  else
    heSelectAllObjects();
}

void StandardHidManager::heUnlockCamera() {

  Camera *cam = activeCamera();
  if( cam )
    cam->unLock();
}

void StandardHidManager::heZoom() {

  int delta = getWheelDelta();
  Camera *cam = activeCamera();
  Camera *isocam = dynamic_cast<IsoCamera*>( cam );
  if( isocam ) {

    if( delta < 0 ) isocam->zoom( 1.05 );
    if( delta > 0 ) isocam->zoom( 0.95 );
  }
  else if( cam ) {

    double scale;
    if( cam->isLocked() )
      scale = cam->getLockDist();
    else
      scale = scene()->getSphere().getRadius();

    cam->move( delta*scale / cam->getViewportH() );
  }
}

void StandardHidManager::heLeftMouseReleaseStuff() {

//  _move_border = false;
}


Camera* StandardHidManager::activeCamera() const {

  return _camera.get();
//  return getWindow()->findCamera( getPos() );
}

float StandardHidManager::activeCameraSpeedScale() {

  return cameraSpeedScale( activeCamera() );
}

float StandardHidManager::cameraSpeedScale(Camera *cam) {

  if( !cam )
    return 1.0f;

  if(cam->isLocked())
    return M_2PI * cam->getLockDist();

  return scene()->getSphere().getRadius();
}


const Vector<int,2>& StandardHidManager::cursorPos() const {

  return _current_scene_pos;
}

const Vector<int,2>& StandardHidManager::previousCursorPos() const {

  return _previous_scene_pos;
}

QPointF StandardHidManager::getPos2() const {

  return QPointF( _current_scene_pos(0), _current_scene_pos(1) );
}

QPointF StandardHidManager::getPPos2() const {

  return QPointF( _previous_scene_pos(0), _previous_scene_pos(1) );
}

Scene* StandardHidManager::scene() const {

  return _scene.get();
}

SceneObject* StandardHidManager::activeSelectObject() {

  Camera *cam = activeCamera();
  if( !cam )
    return 0x0;

  SceneObject *sel_obj;


  _select_renderer->setCamera(cam);

  auto pos = cursorPos();
  auto size = cam->getViewport();

  _select_renderer->reshape( size );

  _select_renderer->select( GM_SO_TYPE_SELECTOR );
  _select_renderer->prepare();
  sel_obj = _select_renderer->findObject(pos(0),size(1)-pos(1)-1);

  if(!sel_obj) {

    _select_renderer->select( -GM_SO_TYPE_SELECTOR );
    _select_renderer->prepare();
    sel_obj = _select_renderer->findObject(pos(0),size(1)-pos(1)-1);
  }

  return sel_obj;
}


//GMWindow* StandardHidManager::getWindow() const {

//  return _gmwindow;
//}






void StandardHidManager::savePos() {

  _previous_scene_pos = _current_scene_pos;
}


void StandardHidManager::setPos( const QPointF& pos ) {

//  _state.setPos( Vector<int,2>( int(pos.x()), _gmwindow->getViewportHeight() - int(pos.y()) - 1 ) );
  _current_scene_pos = Vector<int,2>( int(pos.x()), activeCamera()->getViewportH() - int(pos.y()) - 1 );
}
