#ifndef BLENDINGSPLINECURVE_H
#define BLENDINGSPLINECURVE_H

#include <parametrics/gmpcurve.h>
#include <parametrics/curves/gmpsubcurve.h>

namespace my_namespace {

    template <typename T>
    class Blendingsplinecurve : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Blendingsplinecurve)
    public:
      Blendingsplinecurve(GMlib::PCurve<T,3> *mc, int n = 4, int d = 1);
      virtual ~Blendingsplinecurve();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

      int _d;
      int _k;
      int _n;
      std::vector<T> _t;
      GMlib::PCurve<T,3> *_mc;
      std::vector<GMlib::PCurve<T,3>*> _lc;

      int getBasis(T t, T& p1, T& p2) const;
      void makeKnots();
      void createLocalCurves();
      int findIndex(T t) const ;
      T b(T t) const;
      T findW(T t, int i) const;

    };
  }
#include "blendingsplinecurve.c"

#endif // BLENDINGSPLINECURVE_H
