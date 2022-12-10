#ifndef HYPOTROCHOIDS_H
#define HYPOTROCHOIDS_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class Hypotrochoids : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Hypotrochoids)
    public:
          Hypotrochoids();
      virtual ~Hypotrochoids();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

    };
  }
#include "hypotrochoids.c"

#endif // HYPOTROCHOIDS_H
