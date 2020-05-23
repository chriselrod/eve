//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef EVE_MODULE_CORE_FUNCTION_GENERIC_ASIN_HPP_INCLUDED
#define EVE_MODULE_CORE_FUNCTION_GENERIC_ASIN_HPP_INCLUDED

#include <eve/detail/implementation.hpp>
#include <eve/detail/branch.hpp>
#include <eve/function/abs.hpp>
#include <eve/function/div.hpp>
#include <eve/function/fma.hpp>
#include <eve/function/fms.hpp>
#include <eve/function/if_else.hpp>
#include <eve/function/bitofsign.hpp>
#include <eve/function/bit_xor.hpp>
#include <eve/function/is_greater.hpp>
#include <eve/function/is_less.hpp>
#include <eve/function/oneminus.hpp>
#include <eve/function/rec.hpp>
#include <eve/function/sqr.hpp>
#include <eve/function/sqrt.hpp>
#include <eve/constant/allbits.hpp>
#include <eve/constant/half.hpp>
#include <eve/constant/pio_2.hpp>
#include <eve/constant/pio_4.hpp>
#include <eve/constant/nan.hpp>
#include <eve/constant/sqrteps.hpp>
#include <eve/module/core/detail/generic/horn.hpp>
#include <eve/module/core/detail/generic/horn1.hpp>
#include <eve/concept/value.hpp>
#include <eve/detail/apply_over.hpp>
#include <type_traits>

namespace eve::detail
{

  template<floating_real_value T>
  EVE_FORCEINLINE auto asin_(EVE_SUPPORTS(cpu_)
                            , T const &a0) noexcept
  {
    if constexpr(has_native_abi_v<T> )
    {
      using elt_t = element_type_t<T>;
      auto x = eve::abs(a0);
      auto sgn = eve::bitofsign(a0);
      if constexpr(std::is_same_v<elt_t, float>)
      {
        const auto x_larger_05 = x > Half<T>();
        T z = if_else(x_larger_05, Half<T>()*oneminus(x), eve::sqr(x));
        x = if_else(x_larger_05, sqrt(z), x);
        T z1 = horn<T, 0x3e2aaae4, 0x3d9980f6, 0x3d3a3ec7, 0x3cc617e3, 0x3d2cb352>(z);
        z1 = fma(z1, z*x, x);
        z = if_else(x_larger_05, Pio_2<T>()-(z1+z1), z1);
        return eve::bit_xor(z, sgn);
      }
      else if constexpr(std::is_same_v<elt_t, double>)
      {
        auto small = x < Sqrteps<T>();
        if constexpr(scalar_value<T>) //early scalar return
        {
          if (small) return a0;
          if ((x >  One<T>())) return Nan<T>();
        }
        else if constexpr(simd_value<T>) //simd preparation
        {
          x = if_else(x > One<T>(), eve::allbits_, x);
        }
        auto case_1 = [](const T & x){ // x < 0.625
          auto zz1 = eve::oneminus(x);
          auto num = zz1*horn<T, 0x403c896240f3081dll, 0xc03991aaac01ab68ll, 0x401bdff5baf33e6all,
              0xbfe2079259f9290fll, 0x3f684fc3988e9f08ll >(zz1);
          auto den = horn1<T, 0x40756709b0b644bell, 0xc077fe08959063eell, 0x40626219af6a7f42ll, 0xc035f2a2b6bf5d8cll>(zz1);
          auto vp =  num/den;
          zz1 =  sqrt(zz1+zz1);
          auto z = Pio_4<T>()-zz1;
          zz1 = fms(zz1, vp,  Constant<T, 0X3C91A62633145C07ULL>());//pio_2lo
          z =  z-zz1;
          zz1 = z+Pio_4<T>();
          return zz1;
        };
        auto case_2 =  [](const T & x){ // x >=  0.625
          T zz2 = sqr(x);
          auto num = zz2*horn<T,  0xc020656c06ceafd5ll, 0x40339007da779259ll, 0xc0304331de27907bll, 0x4015c74b178a2dd9ll,
            0xbfe34341333e5c16ll, 0x3f716b9b0bd48ad3ll >(zz2);
          auto den = horn1<T, 0xc04898220a3607acll, 0x4061705684ffbf9dll, 0xc06265bb6d3576d7ll,
            0x40519fc025fe9054ll, 0xc02d7b590b5e0eabll >(zz2);
          auto z =  num/den;
          zz2 = fma(x, z, x);
          return zz2;
        };
        auto ct1 = Constant<T, 0x3fe4000000000000ll>(); //0.625;
        auto xgtct1 =  x > ct1;
        auto res = branch<scalar_value<T>>(xgtct1, case_1, case_2)(x);
        if constexpr(simd_value<T>)
        {
          res = if_else(small, x, res);
        }
        return  bit_xor(res, sgn);
      }
    }
    else return apply_over(asin, a0);
  }
}

#endif
