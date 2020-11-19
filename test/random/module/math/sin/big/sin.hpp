//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <eve/function/sin.hpp>
#include <eve/constant/valmin.hpp>
#include <eve/constant/valmax.hpp>
#include <eve/constant/one.hpp>
#include <eve/constant/mone.hpp>
#include "producers.hpp"
#include <cmath>

TTS_CASE_TPL("wide random check on sin", EVE_TYPE)
{
  auto std_sin = [](auto e) { return std::sin(e); };

//  eve::uniform_prng<EVE_VALUE> p(eve::one(eve::as<EVE_VALUE>()), eve::valmax(eve::as<EVE_VALUE>()));
//  eve::uniform_prng<EVE_VALUE> p(eve::valmin(eve::as<EVE_VALUE>()), eve::mone(eve::as<EVE_VALUE>()));
//  eve::uniform_prng<EVE_VALUE> p(eve::mone(eve::as<EVE_VALUE>()), eve::one(eve::as<EVE_VALUE>()));
  eve::uniform_prng<EVE_VALUE> p(eve::valmin(eve::as<EVE_VALUE>()), eve::valmax(eve::as<EVE_VALUE>()));
  TTS_RANGE_CHECK(p, std_sin, eve::big(eve::sin));
}
