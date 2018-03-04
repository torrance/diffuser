#ifndef Diffuser_Model8
#define Diffuser_Model8

#include <complex>
#include <iostream>
#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"
#include "Model.h"

class Model8 : public Model {
  private:
    double p1, p2, p3, phi01, phi02, phi03, rm1, rm2, rm3, sigma1, sigma2, sigma3;
    void model();

  public:
    void from_prior(DNest4::RNG &rng);
    double perturb(DNest4::RNG &rng);
    std::string description() const;
    void print(std::ostream &out) const;
};
#endif
