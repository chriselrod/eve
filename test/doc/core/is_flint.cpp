#include <eve/function/is_flint.hpp>
#include <eve/wide.hpp>
#include <eve/constant/inf.hpp>
#include <eve/constant/maxflint.hpp>
#include <eve/constant/valmax.hpp>
#include <eve/constant/minf.hpp>
#include <eve/constant/nan.hpp>
#include <iostream>

using wide_ft = eve::wide<float, eve::fixed<8>>;

int main()
{
  wide_ft pf = {0.0f, 1.0f, -1.5f, -2.0f, eve::Valmax<float>(),
                eve::Inf<float>(), eve::Maxflint<float>()*2, eve::Nan<float>()};

  std::cout << "---- simd" << '\n'
            << "<- pf                      = " << pf << '\n'
            << "-> is_flint(pf)            = " << eve::is_flint(pf) << '\n'
            << "-> pedantic_(is_flint)(pf) = " << eve::pedantic_(eve::is_flint)(pf) << '\n'  ;

  float xf = 1.0f;

  std::cout << "---- scalar" << '\n'
            << "<- xf                      = " << xf << '\n'
            << "-> is_flint(xf)            = " << eve::is_flint(xf) << '\n';
  return 0;
}
