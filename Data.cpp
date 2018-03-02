#include "Data.h"

#include <fstream>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <valarray>
#include <vector>

#include "polyfit.h"

Data Data::instance;

void Data::load(const std::string filename) {
  std::vector<double> lambda2;
  std::vector<double> i;
  std::vector<double> q;
  std::vector<double> u;
  std::vector<double> dq;
  std::vector<double> du;

  std::fstream fin(filename, std::ios::in);
  if (!fin.is_open()) {
    std::cout << "Failed to open file\n";
    return;
  }

  // Temp vars
  double tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, _;

  while (
      fin >> tmp1 &&  // lambda2
      fin >> tmp2 &&  // i
      fin >> tmp3 &&  // q
      fin >> tmp4 &&  // u
      fin >> _ &&     // di
      fin >> tmp5 &&  // dq
      fin >> tmp6     // du
  ) {
    if (std::isnan(tmp1) || std::isnan(tmp2) ||
        std::isnan(tmp3) || std::isnan(tmp4)) {
      throw std::runtime_error("NaN value present in data");
    }

    lambda2.push_back(pow(299792458.0 / tmp1, 2));
    i.push_back(tmp2);
    q.push_back(tmp3);
    u.push_back(tmp4);
    dq.push_back(tmp5);
    du.push_back(tmp6);
  }

  // Make polynomial fit and resize stokes q, u as fractional polarizations
  double coeff[5];  // 4th order polynomial
  auto ret = polynomialfit(i.size(), 4, lambda2.data(), i.data(), coeff);
  if (!ret) {
    std::cout << "Polynomial fitting failed (return code: " << ret << ")\n";
    throw std::runtime_error("Polynomial fitting failed");
  }
  std::cout << "Polynomial fit for stokes i found:\n";
  std::cout << coeff[0] << " " << coeff[1] << " " << coeff[2] \
            << " " << coeff[3] << " " << coeff[4] << "\n";

  this->lambda2 = std::valarray<double>(lambda2.data(), lambda2.size());
  std::valarray<double> i_fit((int) 0, i.size());
  i_fit += coeff[0];
  i_fit += this->lambda2 * coeff[1];
  i_fit += std::pow(this->lambda2, 2.0) * coeff[2];
  i_fit += std::pow(this->lambda2, 3.0) * coeff[3];
  i_fit += std::pow(this->lambda2, 4.0) * coeff[4];

  this->q = std::valarray<double>(q.data(), q.size()) / i_fit;
  this->u = std::valarray<double>(u.data(), u.size()) / i_fit;
  this->dq = std::valarray<double>(dq.data(), dq.size()) / i_fit;
  this->du = std::valarray<double>(du.data(), du.size()) / i_fit;
  this->dQU = std::sqrt(std::pow(this->dq, 2.0) + std::pow(this->du, 2.0));
  this->dQU = std::log(std::pow(this->dQU, 2.0) * M_PI * 2.0);

}

const std::valarray<double> &Data::get_lambda2() const {
  return this->lambda2;
}

const std::valarray<double> &Data::get_u() const {
  return this->u;
}

const std::valarray<double> &Data::get_q() const {
  return this->q;
}

const std::valarray<double> &Data::get_du() const {
  return this->du;
}

const std::valarray<double> &Data::get_dq() const {
  return this->dq;
}

const std::valarray<double> &Data::get_dQU() const {
  return this->dQU;
}
