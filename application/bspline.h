#ifndef BSPLINE_H
#define BSPLINE_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class BSpline : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(BSpline)
    public:
      BSpline( const GMlib::DVector<GMlib::Vector<T,3>>& c);
      BSpline( const GMlib::DVector<GMlib::Vector<T,3>>& p, int n);
      virtual ~BSpline();

      bool                isClosed() const override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;

      int _d;
      int _k;
      int _n;

      GMlib::DVector<GMlib::Vector<T,3>> _points;
      GMlib::DVector<GMlib::Vector<T,3>> _c; // Control points
      std::vector<T> _t; // Knot-vector

      int getBasis(T t, T& p1, T& p2, T& p3) const;
      void makeKnots(int n, bool closed = false);
      int findIndex(T t) const ;
      void makeControlPoints(const GMlib::DVector<GMlib::Vector<T,3>>& p);
      GMlib::DMatrix<T> getMatrixA() ;
      T findW(T t, int i, int d) const;

    };
  }
#include "bspline.c"

#endif // BSPLINE_H
