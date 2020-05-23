#include <eve/function/div.hpp>
#include <eve/literals.hpp>
#include <eve/wide.hpp>

int main()
{
  using namespace eve::literal;
  eve::wide<float, eve::fixed<4>> w([](auto i, auto) { return 1.f + i; }), y(3.f);
  std::cout << "div(3.f, 5.5f) = " << eve::div(3.f, 5.5f) << '\n';
  std::cout << "3._wf  = " << y << std::endl;
  std::cout << "w      = " << w << std::endl;
  std::cout << "div(3._wf, w) = " << eve::div(y, w) << '\n';
}
