// ---------------------------------------------------------------------
//
// Copyright (C) 2004 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------



// check LinearAlgebra::TpetraWrappers::Vector<double>::operator*(Vector) on two
// vectors that are not orthogonal

#include <deal.II/base/utilities.h>

#include <deal.II/lac/trilinos_tpetra_vector.h>

#include <iostream>
#include <vector>

#include "../tests.h"


void
test(LinearAlgebra::TpetraWrappers::Vector<double> &v,
     LinearAlgebra::TpetraWrappers::Vector<double> &w)
{
  // set only certain elements of each
  // vector, and record the expected scalar
  // product
  double product = 0;
  for (unsigned int i = 0; i < v.size(); ++i)
    {
      v(i) = i;
      if (i % 3 == 0)
        {
          w(i) = i + 1.;
          product += i * (i + 1);
        }
    }

  v.compress(VectorOperation::insert);
  w.compress(VectorOperation::insert);

  // make sure the scalar product is zero
  AssertThrow(v * w == product, ExcInternalError());

  deallog << "OK" << std::endl;
}



int
main(int argc, char **argv)
{
  initlog();

  Utilities::MPI::MPI_InitFinalize mpi_initialization(
    argc, argv, testing_max_num_threads());


  try
    {
      {
        LinearAlgebra::TpetraWrappers::Vector<double> v;
        v.reinit(complete_index_set(100), MPI_COMM_WORLD);
        LinearAlgebra::TpetraWrappers::Vector<double> w;
        w.reinit(complete_index_set(100), MPI_COMM_WORLD);
        test(v, w);
      }
    }
  catch (const std::exception &exc)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
