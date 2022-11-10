#ifndef LISSAJOUS_H
#define LISSAJOUS_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class LissajousCurve : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(LissajousCurve)
    public:
          LissajousCurve();
      LissajousCurve( const GMlib::DVector<GMlib::Vector<T,3>>& p);
      virtual ~LissajousCurve();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

    };
  }
#include "lissajous.c"

#endif // LISSAJOUS_H
