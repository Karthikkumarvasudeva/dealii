// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2005 - 2024 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// like _01 but instead for random points on the face. note that the normal
// vector on a bilinearly mapped face is also a bilinear function



#include <deal.II/base/quadrature_lib.h>

#include <deal.II/fe/fe_q.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include "../tests.h"



void
create_triangulation(const unsigned int case_no, Triangulation<3> &tria)
{
  switch (case_no)
    {
      case 0:
        GridGenerator::hyper_cube(tria, 1., 3.);
        break;
      case 1:
        {
          GridGenerator::hyper_cube(tria, 1., 3.);
          Point<3> &v0 = tria.begin_active()->vertex(0);
          v0           = Point<3>(0, -0.5, -1);
          Point<3> &v1 = tria.begin_active()->vertex(1);
          v1           = Point<3>(1.25, 0.25, 0.25);
          break;
        }
      default:
        DEAL_II_NOT_IMPLEMENTED();
    };
}



int
main()
{
  initlog();
  deallog << std::setprecision(3);
  deallog << std::fixed;

  FE_Q<2> linear_interpolator(1);

  Triangulation<3>               tria;
  FlatManifold<3>                boundary;
  Manifold<3>::FaceVertexNormals normals;
  for (unsigned int case_no = 0; case_no < 2; ++case_no)
    {
      deallog << "Case" << case_no << std::endl;
      create_triangulation(case_no, tria);
      const Triangulation<3>::active_cell_iterator cell = tria.begin_active();
      Triangulation<3>::face_iterator              face;
      for (const unsigned int face_no : GeometryInfo<3>::face_indices())
        {
          face = cell->face(face_no);
          boundary.get_normals_at_vertices(face, normals);

          for (double xi = 0; xi <= 1; xi += 0.234)
            for (double eta = 0; eta <= 1; eta += 0.234)
              {
                Point<3>     p;
                Tensor<1, 3> normal;

                for (unsigned int v = 0; v < GeometryInfo<3>::vertices_per_face;
                     ++v)
                  {
                    p += face->vertex(v) *
                         linear_interpolator.shape_value(v, Point<2>(xi, eta));
                    normal +=
                      normals[v] *
                      linear_interpolator.shape_value(v, Point<2>(xi, eta));
                  }
                normal /= normal.norm();

                deallog << "p=" << p
                        << ", n=" << boundary.normal_vector(face, p)
                        << std::endl;

                Assert((boundary.normal_vector(face, p) - normal).norm() <
                         1e-10,
                       ExcInternalError());
              }
        }

      tria.clear();
    }

  deallog << "OK" << std::endl;
}
