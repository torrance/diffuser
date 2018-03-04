#ifndef Diffuser_Model4
#define Diffuser_Model4

#include <complex>
#include <iostream>
#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"
#include "Model.h"

class Model4 : public Model {
  private:
    double p1, p2, phi01, phi02, rm1, rm2, sigma1, sigma2;
    void model();

  public:
    void from_prior(DNest4::RNG &rng);
    double perturb(DNest4::RNG &rng);
    std::string description() const;
    void print(std::ostream &out) const;
};
#endif
