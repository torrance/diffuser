#ifndef Diffuser_Data
#define Diffuser_Data

#include <string>
#include <valarray>

class Data {
  public:
    Data(Data const&) = delete;
    void operator=(Data const&) = delete;

    void load(const std::string filename);

    const std::valarray<double> &get_lambda2() const;
    const std::valarray<double> &get_q() const;
    const std::valarray<double> &get_u() const;
    const std::valarray<double> &get_dq() const;
    const std::valarray<double> &get_du() const;
    const std::valarray<double> &get_dQU() const;

  private:
    std::valarray<double> lambda2;
    std::valarray<double> q;
    std::valarray<double> u;
    std::valarray<double> dq;
    std::valarray<double> du;
    std::valarray<double> dQU;

  private:
    static Data instance;
    Data() {};

  public:
    static Data& get_instance() {
      return instance;
    }
};
#endif
