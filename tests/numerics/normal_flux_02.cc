// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2008 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// check the creation of normal flux boundary conditions for a finite
// element that consists of more than dim components and where
// therefore we have to pick the vector components from somewhere in
// the middle (opposite constraints as no_flux_02.cc does).


#include <deal.II/base/function.h>
#include <deal.II/base/quadrature_lib.h>

#include <deal.II/dofs/dof_handler.h>

#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/mapping_q1.h>

#include <deal.II/grid/grid_generator.h>

#include <deal.II/lac/affine_constraints.h>
#include <deal.II/lac/vector.h>

#include <deal.II/numerics/vector_tools.h>

#include "../tests.h"



template <int dim>
void
test(const Triangulation<dim> &tr, const FiniteElement<dim> &fe)
{
  DoFHandler<dim> dof(tr);
  dof.distribute_dofs(fe);

  for (const unsigned int i : GeometryInfo<dim>::face_indices())
    {
      deallog << "FE=" << fe.get_name() << ", case=" << i << std::endl;

      std::set<types::boundary_id> boundary_ids;
      for (unsigned int j = 0; j <= i; ++j)
        boundary_ids.insert(j);

      AffineConstraints<double> cm;
      VectorTools::compute_normal_flux_constraints(dof, 1, boundary_ids, cm);

      cm.print(deallog.get_file_stream());
    }
}


template <int dim>
void
test_hyper_cube()
{
  Triangulation<dim> tr;
  GridGenerator::hyper_cube(tr);

  for (const unsigned int i : GeometryInfo<dim>::face_indices())
    tr.begin_active()->face(i)->set_boundary_id(i);

  tr.refine_global(2);

  for (unsigned int degree = 1; degree < 4; ++degree)
    {
      FESystem<dim> fe(FE_Q<dim>(degree + 1),
                       1,
                       FE_Q<dim>(degree),
                       dim,
                       FE_Q<dim>(degree + 1),
                       1);
      test(tr, fe);
    }
}


int
main()
{
  initlog();
  deallog << std::setprecision(2);
  deallog << std::fixed;

  test_hyper_cube<2>();
  test_hyper_cube<3>();
}
