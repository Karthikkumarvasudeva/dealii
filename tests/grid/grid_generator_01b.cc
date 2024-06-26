// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2013 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// Test if refining a quarter_hyper_shell using SphericalManifold yields
// correct results in 2d and 3d.

#include <deal.II/base/tensor.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>

#include "../tests.h"



template <int dim>
void
test(std::ostream &out)
{
  Point<dim> p1;
  p1[0] = 2.;

  SphericalManifold<dim> boundary_description(p1);
  GridOut                go;
  GridOut::OutputFormat  format = GridOut::gnuplot;

  {
    deallog << "quarter_hyper_ball" << std::endl;
    Triangulation<dim> tr;
    GridGenerator::quarter_hyper_ball(tr, p1, 3.);
    tr.set_manifold(0, boundary_description);

    tr.refine_global(2);
    go.write(tr, out, format);
  }
}


int
main()
{
  initlog();

  deallog.get_file_stream() << std::setprecision(9);

  deallog.push("2d");
  test<2>(deallog.get_file_stream());
  deallog.pop();

  deallog.push("3d");
  test<3>(deallog.get_file_stream());
  deallog.pop();
}
