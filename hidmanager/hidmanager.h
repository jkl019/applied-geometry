#ifndef HIDMANAGER_H
#define HIDMANAGER_H


// local
#include "hidinput.h"
#include "hidbinding.h"

class HidAction;
//class HidManagerModel;
class HidManagerTreeModel;

// GMlib
#include <gmCoreModule>

namespace GMlib {
  class Camera;
  class GMWindow;
  class Scene;
  class SceneObject;
}



// Qt
#include <QtCore>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


// stl
#include <string>
#include <cassert>




class HidManager : public QObject {
  Q_OBJECT
public:
  typedef QSet<const HidAction*>    HidActions;
  typedef QList<const HidAction*>   HidActionsList;
  typedef QSet<const HidBinding>    HidBindings;

  explicit HidManager( QObject *parent = 0);

  const HidActions&         getHidActions() const;
  const HidBindings&        getHidBindings() const;

  QString                   registerHidAction( const QString& name,
                                               const QString& description,
                                               const QString& group,
                                               const QObject* receiver, const char* method );

  bool                      registerHidMapping( const QString& action_name, const HidInput* hid_input );

  HidManagerTreeModel*      getModel() const;

protected:
  void                      customEvent( QEvent* event );

private:
  HidActions              _hid_actions;
  HidBindings             _hid_bindings;

  HidManagerTreeModel     *_model;

  bool                    _ogl_action;


signals:
  void        signBeforeHidAction();
  void        signAfterHidAction();



}; // END class HidManager



#endif // HIDMANAGER_H
