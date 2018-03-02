#ifndef Diffuser_Model1
#define Diffuser_Model1

#include <complex>
#include <iostream>
#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"
#include "Model.h"

class Model1 : public Model {
  private:
    double p, phi0, rm;
    void model();

  public:
    void from_prior(DNest4::RNG &rng);
    double perturb(DNest4::RNG &rng);
    std::string description() const;
    void print(std::ostream &out) const;
};
#endif
