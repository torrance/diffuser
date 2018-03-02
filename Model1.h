#ifndef Diffuser_Model1
#define Diffuser_Model1

#include <complex>
#include <iostream>
#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"

class Model1 {
  private:
    int params = 3;
    double p, phi0, rm;

    std::valarray<double> Q;
    std::valarray<double> U;
    void model();

  public:
    double log_likelihood();
    void print(std::ostream &out) const;
    std::string description() const;
    void from_prior(DNest4::RNG &rng);
    double perturb(DNest4::RNG &rng);
};
#endif
