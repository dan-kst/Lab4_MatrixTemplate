#include "core/Matrix.hpp"
#include <gtest/gtest.h>

using namespace testing;
using namespace Core;
using namespace Core::StoragePolicy;

//~ // --- Suite 3: Expression Templates ---

TEST (MatrixExpressionTest, AdditionLazyEvaluation)
{
  Matrix<int> matrixA (Row{ 2 }, Column{ 2 }, 10);
  Matrix<int> matrixB (Row{ 2 }, Column{ 2 }, 20);

  // This creates a MatrixSum proxy, no new Matrix yet
  auto expr = matrixA + matrixB;

  // Evaluation happens during construction of matrixC
  Matrix<int> matrixC (expr);

  EXPECT_EQ (matrixC (Row{ 0 }, Column{ 0 }), 30);
  EXPECT_EQ (matrixC (Row{ 0 }, Column{ 0 }), 30);
}

TEST (MatrixExpressionTest, ComplexChain)
{
  Matrix<int> matrixA (Row{ 2 }, Column{ 2 }, 1);
  Matrix<int> matrixB (Row{ 2 }, Column{ 2 }, 2);
  Matrix<int> matrixC (Row{ 2 }, Column{ 2 }, 3);

  Matrix<int> result (matrixA + matrixB + matrixC);

  EXPECT_EQ (result (Row{ 0 }, Column{ 0 }), 6);
}
