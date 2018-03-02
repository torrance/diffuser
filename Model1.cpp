#include "Model1.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <valarray>

#include "Data.h"
#include "DNest4.h"

void Model1::from_prior(DNest4::RNG &rng) {
  this->p = exp(-4 + 4 * rng.rand());
  this->phi0 = rng.rand() * 180;
  this->rm = rng.randn() * 500;
}

double Model1::perturb(DNest4::RNG &rng) {
  double log_H = 0;
  int which = rng.rand_int(3);

  switch (which) {
    case 0: {
      double log_p = std::log(this->p);
      double log_p_dash = log_p + 4 * rng.randh();
      DNest4::wrap(log_p_dash, -4, 0);
      this->p = std::exp(log_p_dash);

      log_H =  log_p - log_p_dash;
    }
    case 1:
      this->phi0 += rng.randh() * 180;
      DNest4::wrap(this->phi0, 0, 180);
      log_H = 1;
    case 2:
      log_H -= -0.5 * std::pow(this->rm / 500, 2);
      this->rm += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm / 500, 2);
  }

  return log_H;
}

void Model1::model() {
  const auto& lambda2 = Data::get_instance().get_lambda2();
  auto  phi0_rad = (this->phi0 * M_PI) / 180.0;
  this->Q = cos(lambda2 * this->rm * 2.0 + phi0_rad * 2.0) * this->p;
  this->U = sin(lambda2 * this->rm * 2.0 + phi0_rad * 2.0) * this->p;
}

std::string Model1::description() const {
  return std::string("p, phi0, rm");
}

void Model1::print(std::ostream &out) const {
  out << p << ' ' << phi0 << ' ' << rm;
}

