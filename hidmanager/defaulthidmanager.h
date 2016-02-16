#ifndef DEFAULTHIDMANAGER_H
#define DEFAULTHIDMANAGER_H

#include "standardhidmanager.h"

// local
class GMlibWrapper;


class DefaultHidManager : public StandardHidManager {
  Q_OBJECT
public:
  explicit DefaultHidManager( std::shared_ptr<GMlibWrapper> gmlib, QObject* parent = 0x0 );

  void                        setupDefaultHidBindings();

signals:
  void signToggleSimulation();
  void signOpenCloseHidHelp();

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
  GMlib::SceneObject*               findSceneObject(const QString& view_name, const GMlib::Point<int,2>& pos);

  GMlib::Point<int,2>               toGMlibViewPoint(const QString& view_name, const QPoint& pos);

  std::shared_ptr<GMlibWrapper>     _gmlib;

};

#endif // DEFAULTHIDMANAGER_H
