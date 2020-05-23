#include <eve/function/atand.hpp>
#include <eve/wide.hpp>
#include <iostream>

using wide_ft = eve::wide <float, eve::fixed<4>>;

int main()
{
  wide_ft pf = { 0.0f, 1.0f, -1.0f, -0.5f};

  std::cout
    << "---- simd" << '\n'
    << "<- pf =            " << pf << '\n'
    << "-> eve::atand(pf) = " << eve::atand(pf) << '\n';

  float xf = 1.0f;
  float yf = -2.0f;

  std::cout
    << "---- scalar"  << '\n'
    << "<- xf =             " << xf << '\n'
    << "-> eve::atand(xf) = " << eve::atand(xf) << '\n'
    << "<- yf =             " << yf << '\n'
    << "-> eve::atand(yf) = " << eve::atand(yf) << '\n';
  return 0;
}
