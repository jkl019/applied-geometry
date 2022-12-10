#ifndef TEARDROP_H
#define TEARDROP_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class Teardrop : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Teardrop)
    public:
          Teardrop();
      virtual ~Teardrop();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

    };
  }
#include "teardrop.c"

#endif // TEARDROP_H
