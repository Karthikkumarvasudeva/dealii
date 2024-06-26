// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2005 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// compute the tensor C*unit_symmetric_tensor -- I needed this tensor for an
// application, so thought I'd make a test out of it

#include <deal.II/base/symmetric_tensor.h>
#include <deal.II/base/tensor.h>

#include "../tests.h"


template <int dim>
void
test()
{
  deallog << "dim=" << dim << std::endl;

  SymmetricTensor<4, dim> t;
  for (unsigned int i = 0; i < dim; ++i)
    for (unsigned int j = 0; j < dim; ++j)
      for (unsigned int k = 0; k < dim; ++k)
        for (unsigned int l = 0; l < dim; ++l)
          t[i][j][k][l] =
            10000 * (i == j && k == l ? 1 : 0) +
            100 * ((i == k && j == l ? 1 : 0) + (i == l && j == k ? 1 : 0));

  deallog << "t=" << std::endl;
  for (unsigned int i = 0; i < dim; ++i)
    for (unsigned int j = 0; j < dim; ++j)
      for (unsigned int k = 0; k < dim; ++k)
        for (unsigned int l = 0; l < dim; ++l)
          deallog << i << ' ' << j << ' ' << k << ' ' << l << ": "
                  << (int)t[i][j][k][l] << std::endl;

  // multiply t by the unit symmetric tensor
  const SymmetricTensor<2, dim> t_times_1 = t * unit_symmetric_tensor<dim>();

  deallog << "t*1=" << std::endl;
  for (unsigned int i = 0; i < dim; ++i)
    for (unsigned int j = 0; j < dim; ++j)
      deallog << i << ' ' << j << ": " << (int)(t_times_1[i][j]) << std::endl;

  // t_times_1 should be a multiple of the
  // unit tensor, given the structure we have
  // given to it
  AssertThrow((t_times_1 -
               (dim * 10000 + 2 * 100) * unit_symmetric_tensor<dim>())
                  .norm() < 1e-14 * t_times_1.norm(),
              ExcInternalError());
}



int
main()
{
  initlog();
  deallog << std::setprecision(3);

  test<1>();
  test<2>();
  test<3>();

  deallog << "OK" << std::endl;
}
