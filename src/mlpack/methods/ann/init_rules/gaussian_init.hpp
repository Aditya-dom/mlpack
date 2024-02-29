/**
 * @file methods/ann/init_rules/gaussian_init.hpp
 * @author Kris Singh
 *
 * Intialization rule for the neural networks. This simple initialization is
 * performed by assigning a gaussian matrix with a given mean and variance
 * to the weight matrix.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_INIT_RULES_GAUSSIAN_INIT_HPP
#define MLPACK_METHODS_ANN_INIT_RULES_GAUSSIAN_INIT_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/core/math/random.hpp>

namespace mlpack {

/**
 * This class is used to initialize weigth matrix with a gaussian.
 */
class GaussianInitialization
{
 public:
  /**
   * Initialize the gaussian with the given mean and variance.
   *
   * @param mean Mean of the gaussian.
   * @param variance Variance of the gaussian.
   */
  GaussianInitialization(const double mean = 0, const double variance = 1) :
      mean(mean), variance(variance)
  {
    // Nothing to do here.
  }

  /**
   * Initialize the elements weight matrix using a Gaussian Distribution.
   *
   * @param W Weight matrix to initialize.
   * @param rows Number of rows.
   * @param cols Number of columns.
   */
  template<typename MatType>
  void Initialize(MatType& W,
                  const size_t rows,
                  const size_t cols)
  {
    if (W.is_empty())
      W.set_size(rows, cols);

    W = randn<MatType>(rows, cols, distr_param(0.0, std::sqrt(variance)));
  }

  /**
   * Initialize the elements weight matrix using a Gaussian Distribution.
   *
   * @param W Weight matrix to initialize.
   */
  template<typename MatType>
  void Initialize(MatType& W,
      const typename std::enable_if_t<IsMatrix<MatType>::value>* = 0)
  {
    if (W.is_empty())
      Log::Fatal << "Cannot initialize an empty matrix." << std::endl;

    W = randn<MatType>(W.n_rows, W.n_cols,
        distr_param(mean, std::sqrt(variance)));
  }

  /**
   * Initialize randomly the elements of the specified weight 3rd order tensor.
   *
   * @param W Weight matrix to initialize.
   * @param rows Number of rows.
   * @param cols Number of columns.
   * @param slices Number of slices.
   */
  template<typename CubeType>
  void Initialize(CubeType& W,
                  const size_t rows,
                  const size_t cols,
                  const size_t slices)
  {
    if (W.is_empty())
      W.set_size(rows, cols, slices);

    for (size_t i = 0; i < slices; ++i)
      Initialize(W.slice(i), rows, cols);
  }

  /**
   * Initialize randomly the elements of the specified weight 3rd order tensor.
   *
   * @param W Weight matrix to initialize.
   */
  template<typename CubeType>
  void Initialize(CubeType& W,
      const typename std::enable_if_t<IsCube<CubeType>::value>* = 0)
  {
    if (W.is_empty())
      Log::Fatal << "Cannot initialize an empty matrix." << std::endl;

    for (size_t i = 0; i < W.n_slices; ++i)
      Initialize(W.slice(i));
  }
 
  /**
   * Serialize the initialization.
   */
  template<typename Archive>
  void serialize(Archive& ar, const uint32_t /* version */)
  {
    ar(CEREAL_NVP(mean));
    ar(CEREAL_NVP(variance));
  }

 private:
  //! Mean of the gaussian.
  double mean;

  //! Variance of the gaussian.
  double variance;
}; // class GaussianInitialization

} // namespace mlpack

#endif
