#ifndef Diffuser_Model7
#define Diffuser_Model7

#include <complex>
#include <iostream>
#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"
#include "Model.h"

class Model7 : public Model {
  private:
    double p1, p2, p3, phi01, phi02, phi03, rm1, rm2, rm3, sigma;
    void model();

  public:
    void from_prior(DNest4::RNG &rng);
    double perturb(DNest4::RNG &rng);
    std::string description() const;
    void print(std::ostream &out) const;
};
#endif
