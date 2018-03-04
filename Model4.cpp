#include "Model4.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <valarray>

#include "Data.h"
#include "DNest4.h"

void Model4::from_prior(DNest4::RNG &rng) {
  this->p1 = std::exp(-4 + 4 * rng.rand());
  this->p2 = std::exp(-4 + 4 * rng.rand());
  this->phi01 = rng.rand() * 180;
  this->phi02 = rng.rand() * 180;
  this->rm1 = rng.randn() * 500;
  this->rm2 = rng.randn() * 500;
  this->sigma1 = std::exp(-5 + 5 * rng.rand());
  this->sigma2 = std::exp(-5 + 5 * rng.rand());
}

double Model4::perturb(DNest4::RNG &rng) {
  double log_H = 0;
  int which = rng.rand_int(8);

  switch (which) {
    case 0: {
      double log_p = std::log(this->p1);
      double log_p_dash = log_p + 4 * rng.randh();
      DNest4::wrap(log_p_dash, -4, 0);
      this->p1 = std::exp(log_p_dash);

      log_H =  log_p - log_p_dash;
      break;
    }
    case 1: {
      double log_p = std::log(this->p2);
      double log_p_dash = log_p + 4 * rng.randh();
      DNest4::wrap(log_p_dash, -4, 0);
      this->p2 = std::exp(log_p_dash);

      log_H =  log_p - log_p_dash;
      break;
    }
    case 2:
      this->phi01 += rng.randh() * 180;
      DNest4::wrap(this->phi01, 0, 180);
      log_H = 1;
      break;
    case 3:
      this->phi02 += rng.randh() * 180;
      DNest4::wrap(this->phi02, 0, 180);
      log_H = 1;
      break;
    case 4:
      log_H -= -0.5 * std::pow(this->rm1 / 500, 2);
      this->rm1 += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm1 / 500, 2);
      break;
    case 5:
      log_H -= -0.5 * std::pow(this->rm2 / 500, 2);
      this->rm2 += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm2 / 500, 2);
      break;
    case 6: {
      double log_sigma = std::log(this->sigma1);
      double log_sigma_dash = log_sigma + 10 * rng.randh();
      DNest4::wrap(log_sigma_dash, -5, 5);
      this->sigma1 = std::exp(log_sigma_dash);

      log_H = log_sigma - log_sigma_dash;
      break;
    }
    case 7: {
      double log_sigma = std::log(this->sigma2);
      double log_sigma_dash = log_sigma + 10 * rng.randh();
      DNest4::wrap(log_sigma_dash, -5, 5);
      this->sigma2 = std::exp(log_sigma_dash);

      log_H = log_sigma - log_sigma_dash;
      break;
    }
  }

  return log_H;
}

void Model4::model() {
  const auto& lambda2 = Data::get_instance().get_lambda2();
  auto phi01_rad = (this->phi01 * M_PI) / 180.0;
  auto phi02_rad = (this->phi02 * M_PI) / 180.0;

  this->Q = cos(
              lambda2 * this->rm1 * 2.0 + phi01_rad * 2.0
            ) * this->p1 * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma1, 2.0) * -2.0
            ) +
            cos(
              lambda2 * this->rm2 * 2.0 + phi02_rad * 2.0
            ) * this->p2 * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma2, 2.0) * -2.0
            );
  this->U = sin(
              lambda2 * this->rm1 * 2.0 + phi01_rad * 2.0
            ) * this->p1 * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma1, 2.0) * -2.0
            ) +
            sin(
              lambda2 * this->rm2 * 2.0 + phi02_rad * 2.0
            ) * this->p2 * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma2, 2.0) * -2.0
            );
}

std::string Model4::description() const {
  return std::string("p1, phi01, rm1, p2, phi02, rm2, sigma1, sigma2");
}

void Model4::print(std::ostream &out) const {
  out << p1 << ' ' << phi01 << ' ' << rm1
      << ' ' << p2 << ' ' << phi02 << ' ' << rm2 << ' ' << sigma1 << ' ' << sigma2;
}

