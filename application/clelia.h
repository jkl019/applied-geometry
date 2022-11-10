#ifndef CLELIA_H
#define CLELIA_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class Clelia : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Clelia)
    public:
          Clelia(T r, int n);
      virtual ~Clelia();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

      T _r;
      int _n;

    };
  }
#include "clelia.c"

#endif // CLELIA_H
