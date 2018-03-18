#include <climits>
#include <experimental/filesystem>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>
#include <valarray>

#include "Data.h"
#include "DNest4.h"
#include "Model1.h"
#include "Model2.h"
#include "Model3.h"
#include "Model4.h"
#include "Model7.h"
#include "Model8.h"
#include "popl.h"

int main(int argc, char** argv)
{
  int threads, particles, new_level_interval, save_interval, thread_steps, max_levels,
      lambda, beta, max_saves;
  popl::OptionParser op("Allowed options");
  auto filename = op.add<popl::Value<std::string>>("f", "file", "file path to data (required)");
  auto model = op.add<popl::Value<int>>("m", "model", "model number [1-8]");
  op.add<popl::Value<int>>("t", "threads", "number of threads", 1, &threads);
  op.add<popl::Value<int>>("p", "particles", "number of particles per thread", 8, &particles);
  op.add<popl::Value<int>>("n", "newlevel", "new level interval", 10000, &new_level_interval);
  op.add<popl::Value<int>>("s", "saveinterval", "save interval", 100, &save_interval);
  op.add<popl::Value<int>>("", "threadsteps", "number of steps each thread should do independently before communication", 100, &thread_steps);
  op.add<popl::Value<int>>("", "maxlevels", "maximum number of levels", 0, &max_levels);
  op.add<popl::Value<int>>("", "lambda", "backtracking scale length", 10, &lambda);
  op.add<popl::Value<int>>("", "beta", "strength of effect to force historgram to equal", 100, &beta);
  op.add<popl::Value<int>>("x", "maxsaves", "maximum number of saves", 0, &max_saves);
  auto help = op.add<popl::Switch>("h", "help", "show this help");
  op.parse(argc, argv);

  if (help->is_set()) {
    std::cout << op << "\n";
    return -1;
  }
  if (!filename->is_set()) {
    std::cout << "Error: -f --filename must be set\n\n";
    std::cout << op << "\n";
    return -1;
  }
  if (!model->is_set()) {
    std::cout << "Error: -m --model must be set\n\n";
    std::cout << op << "\n";
    return -1;
  }

  int model_number = model->value();
  switch (model_number) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
      // Do nothing
      break;
    default:
      std::cout << "Error: -m --model must be number [1-8] (required)\n\n";
      std::cout << op << "\n";
      return -1;
  }

  // Load data file
  auto& data = Data::get_instance();
  data.load(filename->value());
  std::cout << "# Data successfully loaded\n";
  auto obs = data.get_lambda2().size();
  std::cout << "# " << obs << " data points\n";

  // Create paths of additional output files
  std::experimental::filesystem::path path(filename->value());
  path.replace_filename(std::string("ds") + std::to_string(model_number));
  try {
    std::experimental::filesystem::create_directory(path);
  }
  catch (const std::exception& e) {
    std::cout << "Failed to create output directory: " << path << "\n";
    std::cout << e.what();
    return 1;
  }
  auto sampletxt = path / "sample.txt";
  auto sampleinfotxt = path / "sample_info.txt";
  auto levels = path / "levels.txt";
  std::cout << "# Saving output files to " << sampletxt << ", "
            << sampleinfotxt << ", and " << levels << "\n";

  // Create options object for DNest4
  // DNest4 options doesn't allow us to set filenames
  // without resorting to its istream interface.
  DNest4::Options options;
  std::stringstream s;
  s << particles << ' ' << new_level_interval << ' ' << save_interval << ' '
    << thread_steps << ' ' << max_levels << ' ' << lambda << ' ' << beta << ' '
    << max_saves <<  ' ' << sampletxt.string() << ' ' << sampleinfotxt.string() << ' '
    << levels.string();
  s >> options;
  std::cout << options;

  // Create random seed
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<unsigned int> uni(0, UINT_MAX);
  unsigned int seed = uni(rng);
  std::cout << "# Using rng seed: " << seed << "\n";

  switch (model_number) {
    case 1: {
      std::cout << "# Beginning model 1\n";
      DNest4::Sampler<Model1> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
    case 2: {
      std::cout << "# Beginning model 2\n";
      DNest4::Sampler<Model2> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
    case 3: {
      std::cout << "# Beginning model 3\n";
      DNest4::Sampler<Model3> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
    case 4: {
      std::cout << "# Beginning model 4\n";
      DNest4::Sampler<Model4> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
    case 7: {
      std::cout << "# Beginning model 7\n";
      DNest4::Sampler<Model7> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
    case 8: {
      std::cout << "# Beginning model 8\n";
      DNest4::Sampler<Model8> sampler(threads, M_E, options, true);
      sampler.initialise(seed);
      sampler.run();
      break;
    }
  }

  return 0;
}
