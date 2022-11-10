
#include <iostream>

#include "scenario.h"
#include "testtorus.h"
#include "application/bspline.h"
#include "application/subdivision.h"
#include "application/lissajous.h"
#include "application/hypotrochoids.h"
#include "application/clelia.h"
#include "parametrics/curves/gmpcircle.h"
#include "parametrics/curves/gmpline.h"
#include <QDebug>

// hidmanager
#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <scene/light/gmpointlight.h>
#include <scene/sceneobjects/gmpathtrack.h>
#include <scene/sceneobjects/gmpathtrackarrows.h>

// qt
#include <QQuickItem>

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  out << v.size() << std::endl;
  for (uint i = 0; i < v.size(); i++)
    out << " " << v[i];
  out << std::endl;
  return out;
}

void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat, 3> init_light_pos(2.0, 4.0, 10);
  GMlib::PointLight *light =
      new GMlib::PointLight(GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                            GMlib::GMcolor::white(), init_light_pos);
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight(light, false);

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float, 3> init_cam_pos(0.0f, 0.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_dir(0.0f, 1.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_up(1.0f, 0.0f, 0.0f);

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos, init_cam_dir, init_cam_up);
  proj_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  proj_rcpair.camera->rotateGlobal(GMlib::Angle(-45),
                                   GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f));
  proj_rcpair.camera->translateGlobal(
      GMlib::Vector<float, 3>(0.0f, -20.0f, 20.0f));
  scene()->insertCamera(proj_rcpair.camera.get());
  proj_rcpair.renderer->reshape(
      GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  /***************************************************************************
   *                                                                         *
   * Standar example, including path track and path track arrows             *
   *                                                                         *
   ***************************************************************************/

  GMlib::Material mm(GMlib::GMmaterial::polishedBronze());
  mm.set(45.0);

//    auto ptom = new TestTorus(1.0f, 0.4f, 0.6f);
//    ptom->toggleDefaultVisualizer();
//    ptom->sample(60, 60, 1, 1);
//    this->scene()->insert(ptom);
//    auto ptrack = new GMlib::PathTrack();
//    ptrack->setLineWidth(2);
//    ptom->insert(ptrack);
//    auto ptrack2 = new GMlib::PathTrackArrows();
//    ptrack2->setArrowLength(2);
//    ptom->insert(ptrack2);

       // FIRST CONSTRUCTOR
//    GMlib::Vector<float,3> p;
//    GMlib::DVector<GMlib::Vector<float,3>> c;
//    p[0] = 0.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 1.0;
//    p[1] = 1.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 2.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 3.0;
//    p[1] = 1.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 4.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 5.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    auto line = new my_namespace::BSpline<float>(c);
//    line->toggleDefaultVisualizer();
//    line->sample(20, 0);
//    this->scene()->insert(line);

      // SECOND CONSTRUCTOR
//    auto circle = new GMlib::PCircle<float>(5);
//    circle->toggleDefaultVisualizer();
//    circle->sample(50, 0);
//    this->scene()->insert(circle);


//    GMlib::DVector<GMlib::Vector<float,3>> points;
//    int number = 30;
//    float dt = circle->getParDelta() / (number -1);


//    for(int i = 0; i < number; i++){
//        auto point = circle->getPosition(circle->getParStart() + (i * dt));
//        points.push_back(point);
//    };
//    auto spline = new my_namespace::BSpline<float>(points, 6);
//    spline->toggleDefaultVisualizer();
//    spline->sample(30, 0);
//    this->scene()->insert(spline);

    // SUBDIVISION CURVE
//    GMlib::Vector<float,3> p;
//    GMlib::DVector<GMlib::Vector<float,3>> c;
//    p[0] = 0.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 0.0;
//    p[1] = 3.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 1.0;
//    p[1] = 2.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 3.0;
//    p[1] = 3.0;
//    p[2] = 0.0;
//    c.push_back(p);
//    p[0] = 3.0;
//    p[1] = 0.0;
//    p[2] = 0.0;
//    c.push_back(p);

//    auto subdivCurve = new my_namespace::Subdivision<float>(c);
//    subdivCurve->toggleDefaultVisualizer();
//    subdivCurve->sample(3, 3);
//    this->scene()->insert(subdivCurve);

    // MODEL CURVES

//    auto lissajousCurve = new my_namespace::LissajousCurve<float>();
//    lissajousCurve->toggleDefaultVisualizer();
//    lissajousCurve->sample(50, 0);
//    this->scene()->insert(lissajousCurve);

//    auto hypotrochoid = new my_namespace::Hypotrochoids<float>();
//    hypotrochoid->toggleDefaultVisualizer();
//    hypotrochoid->sample(50, 0);
//    this->scene()->insert(hypotrochoid);

  float n = 4.0;
  auto clelia = new my_namespace::Clelia<float>(1.0, n);
  clelia->toggleDefaultVisualizer();
  clelia->sample(100, 0);
  this->scene()->insert(clelia);
}

void Scenario::cleanupScenario() {}

void Scenario::callDefferedGL() {

    GMlib::Array<const GMlib::SceneObject *> e_obj;
    this->scene()->getEditedObjects(e_obj);

    for (int i = 0; i < e_obj.getSize(); i++)
      if (e_obj(i)->isVisible())
        e_obj[i]->replot();
}
