//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <eve/function/convert.hpp>
#include <eve/constant/valmin.hpp>
#include <eve/constant/valmax.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/types.hpp>
#include <type_traits>

TTS_CASE_TPL("Check eve::convert return type", EVE_TYPE)
{
#if defined(EVE_SIMD_TESTS)
  using target_t = eve::wide<std::int16_t, eve::fixed<EVE_CARDINAL>>;
#else
  using target_t = std::int16_t;
#endif

  TTS_EXPR_IS(eve::convert(T(), eve::as<std::int16_t>()), target_t);
  TTS_EXPR_IS(eve::convert(T(), eve::int16_)     , target_t);
}

TTS_CASE_TPL("Check eve::convert behavior", EVE_TYPE)
{
#if defined(EVE_SIMD_TESTS)
  using target_t = eve::wide<std::int16_t, eve::fixed<EVE_CARDINAL>>;
#else
  using target_t = std::int16_t;
#endif

  using v_t = eve::element_type_t<T>;

  if constexpr(eve::signed_value<T>)
  {
    TTS_EQUAL(eve::convert((T(-42.69))    , eve::int16_), static_cast<target_t>(v_t(-42.69)) );
  }

  TTS_EQUAL(eve::convert((T(0))          , eve::int16_), static_cast<target_t>(0) );
  TTS_EQUAL(eve::convert((T(42.69))      , eve::int16_), static_cast<target_t>(v_t(42.69)) );

  if constexpr(eve::integral_value<T>)
  {
    // with floating value this test produces undefined behaviour
    TTS_EQUAL(eve::convert(eve::Valmin<T>(), eve::int16_), static_cast<target_t>(eve::Valmin<v_t>()) );
    TTS_EQUAL(eve::convert(eve::Valmax<T>(), eve::int16_), static_cast<target_t>(eve::Valmax<v_t>()) );
  }
}
