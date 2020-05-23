#include <eve/function/is_denormal.hpp>
#include <eve/wide.hpp>
#include <eve/constant/inf.hpp>
#include <eve/constant/minf.hpp>
#include <eve/constant/nan.hpp>
#include <eve/constant/mindenormal.hpp>
#include <iostream>

using wide_ft = eve::wide<float, eve::fixed<8>>;

int main()
{
  wide_ft pf = {0.0f,1.0f, -1.0f, -2.0f,
                eve::Mindenormal<float>(), eve::Inf<float>(),
                eve::Minf<float>(), eve::Nan<float>()};

  std::cout << "---- simd" << '\n'
            << "<- pf              = " << pf << '\n'
            << "-> is_denormal(pf) = " << eve::is_denormal(pf) << '\n';

  float xf = 1.0f;
  float yf = eve::Mindenormal<float>();

  std::cout << "---- scalar" << '\n'
            << "<- xf              = " << xf << '\n'
            << "-> is_denormal(xf) = " << eve::is_denormal(xf) << '\n'
            << "<- yf              = " << yf << '\n'
            << "-> is_denormal(yf) = " << eve::is_denormal(yf) << '\n';
  return 0;
}
