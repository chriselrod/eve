//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <eve/function/log10.hpp>
#include <eve/constant/valmax.hpp>
#include <tts/tests/range.hpp>
#include "measures.hpp"
#include "producers.hpp"
#include <cmath>

TTS_CASE_TPL("wide random check on log10", EVE_TYPE)
{
  using v_t = eve::element_type_t<T>;
  auto std_log10 = tts::vectorize<T>( [](auto e) { return std::log10(e); } );

  eve::rng_producer<T> p(v_t(0), eve::Valmax<v_t>());
  TTS_RANGE_CHECK(p, std_log10, eve::log10);
}
