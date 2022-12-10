#ifndef CORNOID_H
#define CORNOID_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class Cornoid : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Cornoid)
    public:
          Cornoid();
      virtual ~Cornoid();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

    };
  }
#include "cornoid.c"

#endif // CORNOID_H
