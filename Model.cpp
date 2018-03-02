#include "Model.h"

#include <valarray>

#include "Data.h"

double Model::log_likelihood() {
  this->model();
  const auto& data = Data::get_instance();
  auto& q = data.get_q();
  auto& u = data.get_u();
  auto& dq = data.get_dq();
  auto& du = data.get_du();
  auto& dQU = data.get_dQU();

  double chisq = (
    std::pow((q - this->Q) / dq, 2.0) +
    std::pow((u - this->U) / du, 2.0) +
    dQU
  ).sum();

  return -chisq / 2.0;
}
