// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2013 - 2024 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// check SparseMatrix::Tvmult, Tvmult_add with
// LinearAlgebra::TpetraWrappers::Vector<double>

#include <deal.II/base/utilities.h>

#include <deal.II/lac/la_parallel_vector.h>
#include <deal.II/lac/read_write_vector.h>
#include <deal.II/lac/trilinos_tpetra_sparse_matrix.h>

#include <iostream>
#include <vector>

#include "../tests.h"


void
test(LinearAlgebra::TpetraWrappers::Vector<double, MemorySpace::Default> &v,
     LinearAlgebra::TpetraWrappers::Vector<double, MemorySpace::Default> &w)
{
  LinearAlgebra::ReadWriteVector<double> read_write_v(v.size());
  LinearAlgebra::ReadWriteVector<double> read_write_w(w.size());

  LinearAlgebra::TpetraWrappers::SparseMatrix<double, MemorySpace::Default> m(
    v.size(), w.size(), w.size());
  for (unsigned int i = 0; i < m.m(); ++i)
    for (unsigned int j = 0; j < m.n(); ++j)
      m.set(i, j, i + 2 * j);

  for (unsigned int i = 0; i < v.size(); ++i)
    read_write_v(i) = i;

  m.compress(VectorOperation::insert);
  v.import_elements(read_write_v, VectorOperation::insert);

  // w:=Mv
  m.Tvmult(w, v);

  // make sure we get the expected result
  read_write_w.import_elements(w, VectorOperation::insert);
  for (unsigned int i = 0; i < m.n(); ++i)
    {
      double result = 0;
      for (unsigned int j = 0; j < m.m(); ++j)
        result += (j + 2 * i) * j;
      AssertThrow(read_write_w(i) == result, ExcInternalError());
    }

  m.Tvmult_add(w, v);
  // make sure we get the expected result
  read_write_w.import_elements(w, VectorOperation::insert);
  for (unsigned int i = 0; i < m.n(); ++i)
    {
      double result = 0;
      for (unsigned int j = 0; j < m.m(); ++j)
        result += (j + 2 * i) * j;
      AssertThrow(read_write_w(i) == result + result, ExcInternalError());
    }

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
        LinearAlgebra::TpetraWrappers::Vector<double, MemorySpace::Default> v(
          complete_index_set(95), MPI_COMM_SELF);
        LinearAlgebra::TpetraWrappers::Vector<double, MemorySpace::Default> w(
          complete_index_set(100), MPI_COMM_SELF);
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
