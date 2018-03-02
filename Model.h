#include <ostream>
#include <string>
#include <valarray>

#include "DNest4.h"

class Model {
  public:
    virtual void from_prior(DNest4::RNG& rng) = 0;
    virtual double perturb(DNest4::RNG& rng) = 0;
    virtual std::string description() const = 0;
    virtual void print(std::ostream& out) const = 0;
    double log_likelihood();

  protected:
    virtual void model() = 0;
    std::valarray<double> Q;
    std::valarray<double> U;

};
