#include <iostream>
#include <string>
#include <valarray>

#include "Data.h"
#include "DNest4.h"
#include "Model1.h"

int main(int argc, char** argv)
{
  std::string filename("real.tsv");
  auto& data = Data::get_instance();
  data.load(filename);
  std::cout << "Data successfully loaded\n";
  auto obs = data.get_lambda2().size();
  std::cout << obs << " data points\n";
  auto sampler = DNest4::setup<Model1>(argc, argv);
  sampler.run();
  return 0;
}
