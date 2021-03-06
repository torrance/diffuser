#include "Model7.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <valarray>

#include "Data.h"
#include "DNest4.h"

void Model7::from_prior(DNest4::RNG &rng) {
  this->p1 = std::exp(-4 + 4 * rng.rand());
  this->p2 = std::exp(-4 + 4 * rng.rand());
  this->p3 = std::exp(-4 + 4 * rng.rand());
  this->phi01 = rng.rand() * 180;
  this->phi02 = rng.rand() * 180;
  this->phi03 = rng.rand() * 180;
  this->rm1 = rng.randn() * 500;
  this->rm2 = rng.randn() * 500;
  this->rm3 = rng.randn() * 500;
  this->sigma = std::exp(-4 + 5 * rng.rand());
}

double Model7::perturb(DNest4::RNG &rng) {
  double log_H = 0;
  int which = rng.rand_int(10);

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
    case 2: {
      double log_p = std::log(this->p3);
      double log_p_dash = log_p + 4 * rng.randh();
      DNest4::wrap(log_p_dash, -4, 0);
      this->p3 = std::exp(log_p_dash);

      log_H =  log_p - log_p_dash;
      break;
    }
    case 3:
      this->phi01 += rng.randh() * 180;
      DNest4::wrap(this->phi01, 0, 180);
      log_H = 1;
      break;
    case 4:
      this->phi02 += rng.randh() * 180;
      DNest4::wrap(this->phi02, 0, 180);
      log_H = 1;
      break;
    case 5:
      this->phi03 += rng.randh() * 180;
      DNest4::wrap(this->phi03, 0, 180);
      log_H = 1;
      break;
    case 6:
      log_H -= -0.5 * std::pow(this->rm1 / 500, 2);
      this->rm1 += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm1 / 500, 2);
      break;
    case 7:
      log_H -= -0.5 * std::pow(this->rm2 / 500, 2);
      this->rm2 += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm2 / 500, 2);
      break;
    case 8:
      log_H -= -0.5 * std::pow(this->rm3 / 500, 2);
      this->rm3 += rng.randh() * 500;
      log_H += -0.5 * std::pow(this->rm3 / 500, 2);
      break;
    case 9: {
      double log_sigma = std::log(this->sigma);
      double log_sigma_dash = log_sigma + 9 * rng.randh();
      DNest4::wrap(log_sigma_dash, -4, 5);
      this->sigma = std::exp(log_sigma_dash);

      log_H = log_sigma - log_sigma_dash;
      break;
    }
  }

  return log_H;
}

void Model7::model() {
  const auto& lambda2 = Data::get_instance().get_lambda2();
  auto phi01_rad = (this->phi01 * M_PI) / 180.0;
  auto phi02_rad = (this->phi02 * M_PI) / 180.0;
  auto phi03_rad = (this->phi02 * M_PI) / 180.0;

  this->Q = (cos(
              lambda2 * this->rm1 * 2.0 + phi01_rad * 2.0
            ) * this->p1 +
            cos(
              lambda2 * this->rm2 * 2.0 + phi02_rad * 2.0
            ) * this->p2 +
            cos(
              lambda2 * this->rm3 * 2.0 + phi03_rad * 2.0
            ) * this->p3) * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma, 2.0) * -2.0
            );
  this->U = (sin(
              lambda2 * this->rm1 * 2.0 + phi01_rad * 2.0
            ) * this->p1 +
            sin(
              lambda2 * this->rm2 * 2.0 + phi02_rad * 2.0
            ) * this->p2 +
            sin(
              lambda2 * this->rm3 * 2.0 + phi03_rad * 2.0
            ) * this->p3) * std::exp(
              std::pow(lambda2, 2.0) * std::pow(this->sigma, 2.0) * -2.0
            );}

std::string Model7::description() const {
  return std::string("p1, phi01, rm1, p2, phi02, rm2, p3, phi03, rm3, sigma");
}

void Model7::print(std::ostream &out) const {
  out << p1 << ' ' << phi01 << ' ' << rm1
      << ' ' << p2 << ' ' << phi02 << ' ' << rm2 << ' '
      << p3 << ' ' << phi03 << ' ' << rm3 << ' ' << sigma;;
}

