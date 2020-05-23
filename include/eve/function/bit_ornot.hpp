//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef EVE_FUNCTION_BIT_ORNOT_HPP_INCLUDED
#define EVE_FUNCTION_BIT_ORNOT_HPP_INCLUDED

#include <eve/function/definition/bit_ornot.hpp>
#include <eve/arch.hpp>
#include <eve/module/core/function/generic/bit_ornot.hpp>

#if defined(EVE_HW_ARM)
#  include <eve/module/core/function/simd/arm/neon/bit_ornot.hpp>
#endif


#endif
