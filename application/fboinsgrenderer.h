#ifndef FBOINSGRENDERER_H
#define FBOINSGRENDERER_H

#include <QtQuick/QQuickFramebufferObject>

class FboInSGRenderer : public QQuickFramebufferObject {
  Q_OBJECT
  Q_PROPERTY(QString rcpair_name     READ rcPairName WRITE setRcPairName)
public:
  FboInSGRenderer();

  const QString&    rcPairName() const;
  void              setRcPairName( const QString& name );

  Renderer*         createRenderer() const;

private:

  QString     _name {};
};

#endif
