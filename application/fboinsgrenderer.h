#ifndef FBOINSGRENDERER_H
#define FBOINSGRENDERER_H

#include <QtQuick/QQuickFramebufferObject>

class FboInSGRenderer : public QQuickFramebufferObject {
    Q_OBJECT
public:
  FboInSGRenderer();

  Renderer*       createRenderer() const;
};

#endif
