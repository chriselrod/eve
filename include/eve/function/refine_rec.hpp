//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2020 Joel FALCOU
  Copyright 2020 Jean-Thierry LAPRESTE

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef EVE_FUNCTION_REFINE_REC_HPP_INCLUDED
#define EVE_FUNCTION_REFINE_REC_HPP_INCLUDED

#include <eve/function/definition/refine_rec.hpp>
#include <eve/arch.hpp>
#include <eve/module/core/function/generic/refine_rec.hpp>

#if defined(EVE_HW_ARM)
#  include <eve/module/core/function/simd/arm/neon/refine_rec.hpp>
#endif

#endif
