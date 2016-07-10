#ifndef FBOINSGRENDERER_H
#define FBOINSGRENDERER_H

#include <QtQuick/QQuickFramebufferObject>

class FboInSGRenderer : public QQuickFramebufferObject {
  Q_OBJECT
  Q_PROPERTY(QString rcpair_name READ rcPairName WRITE setRcPairName)
public:
  FboInSGRenderer();

  const QString&    rcPairName() const;
  void              setRcPairName( const QString& name );

  Renderer*         createRenderer() const;

private slots:
  void              onWindowChanged( QQuickWindow* window );


signals:
  void              signKeyPressed( const QString& name, QKeyEvent* event );
  void              signKeyReleased( const QString& name, QKeyEvent* event );
  void              signMouseDoubleClicked( const QString& name, QMouseEvent* event );
  void              signMouseMoved( const QString& name, QMouseEvent* event );
  void              signMousePressed( const QString& name, QMouseEvent* event );
  void              signMouseReleased( const QString& name, QMouseEvent* event );
  void              signWheelEventOccurred( const QString& name, QWheelEvent* event);

private:
  void              keyPressEvent(QKeyEvent *event);
  void              keyReleaseEvent(QKeyEvent *event);
  void              mousePressEvent(QMouseEvent *event);
  void              mouseReleaseEvent(QMouseEvent *event);
  void              mouseDoubleClickEvent(QMouseEvent *event);
  void              mouseMoveEvent(QMouseEvent *event);
  void              wheelEvent(QWheelEvent *event);

  QString           _name {};
};

#endif
