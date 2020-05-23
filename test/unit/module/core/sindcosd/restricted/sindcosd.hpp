//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <eve/function/sindcosd.hpp>
#include <eve/function/cosd.hpp>
#include <eve/function/sind.hpp>
#include <eve/constant/valmax.hpp>
#include <eve/constant/valmin.hpp>
#include <eve/constant/pi.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/types.hpp>
#include <tuple>

TTS_CASE_TPL("Check eve::restricted_(eve::sindcosd) return type", EVE_TYPE)
{
  TTS_EXPR_IS(eve::restricted_(eve::sindcosd)(T()), (std::tuple<T, T>));
}

TTS_CASE_TPL("Check eve::restricted_(eve::sindcosd) behavior", EVE_TYPE)
{
  using v_t = eve::element_type_t<T>;

  auto base = v_t(180);

  v_t x[] = { base/8, -base/8, base/4, -base/4};

  for(auto v : x)
  {
    auto [sin_, cos_] = eve::restricted_(eve::sindcosd)(T(v));
    TTS_ULP_EQUAL(sin_, eve::sind(T(v)), 0.5);
    TTS_ULP_EQUAL(cos_, eve::cosd(T(v)), 0.5);
  }
  {
    auto [sin_, cos_] = eve::restricted_(eve::sindcosd)(T(base));
    TTS_IEEE_EQUAL(sin_, eve::Nan<T>());
    TTS_IEEE_EQUAL(cos_, eve::Nan<T>());
  }

  {
    auto [sin_, cos_] = eve::restricted_(eve::sindcosd)(T(-base));
    TTS_IEEE_EQUAL(sin_, eve::Nan<T>());
    TTS_IEEE_EQUAL(cos_, eve::Nan<T>());
  }
}
