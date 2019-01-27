#include "scenario.h"

#include "testtorus.h"

// gmlib
#include <scene/light/gmpointlight.h>
#include <parametrics/visualizers/gmpsurfnormalsvisualizer.h>

#include <trianglesystem/gmtrianglesystem.h>
#include <parametrics/curves/gmpcircle.h>

// qt
#include <QQuickItem>



void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8, 0.002, 0.0008);
  scene()->insertLight( light, false );

  // Insert Sun
  scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
  front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( front_rcpair.camera.get() );
  front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
  side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( side_rcpair.camera.get() );
  side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
  top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( top_rcpair.camera.get() );
  top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );



  // Init Test Torus example
  initializeTestTorusExample();

  // Init TriangleFacets example
//  initializeTriangleFacetExample();

}

void Scenario::cleanupScenario() {

  if (m_test_torus != nullptr)
    cleanupTestTorusExample();
}

void Scenario::initializeTestTorusExample()
{
  // Surface visualizers
  auto surface_visualizer = new GMlib::PSurfNormalsVisualizer<float,3>;

  // Surface
  m_test_torus = std::make_shared<TestTorus>();
  m_test_torus->toggleDefaultVisualizer();
  m_test_torus->insertVisualizer(surface_visualizer);
  m_test_torus->replot(200,200,1,1);
  scene()->insert(m_test_torus.get());

  m_test_torus->test01();
}

void Scenario::cleanupTestTorusExample()
{
  scene()->remove(m_test_torus.get());
  m_test_torus.reset();
}

void Scenario::initializeTriangleFacetExample() {

  auto circle = GMlib::PCircle<float>(4.0f);

  m_triangle_facets = std::make_shared<GMlib::TriangleFacets<float>>();

  for (auto i = 0UL; i < 10; ++i) {

    const auto dt_s = float(10);
    const auto t =
        float(circle.getParStart() + i * (circle.getParDelta() / dt_s));
    const auto circle_eval = circle.evaluate(t, 0);
    m_triangle_facets->insertVertex(
        GMlib::TSVertex<float>{circle_eval[0][0], circle_eval[0][1]});
  }
  m_triangle_facets->insertVertex(GMlib::TSVertex<float>{0.0f, 0.0f, 1.0f});

  m_triangle_facets->toggleDefaultVisualizer();
  m_triangle_facets->triangulateDelaunay();
  m_triangle_facets->replot();

  scene()->insert(m_triangle_facets.get());
}

void Scenario::cleanupTriangleFacetExample()
{
  scene()->remove(m_triangle_facets.get());
  m_triangle_facets.reset();
}
