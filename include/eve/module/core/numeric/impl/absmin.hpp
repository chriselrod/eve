//==================================================================================================
/*
  EVE - Expressive Vector Engine
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <eve/traits/common_value.hpp>
#include <eve/concept/value.hpp>
#include <eve/detail/apply_over.hpp>
#include <eve/detail/implementation.hpp>
#include <eve/detail/skeleton_calls.hpp>
#include <eve/module/core/decorator/numeric.hpp>
#include <eve/module/core/numeric/min.hpp>
#include <eve/module/core/regular/abs.hpp>
#include <eve/module/core/regular/all.hpp>
#include <eve/module/core/regular/is_nan.hpp>
#include <eve/module/core/regular/min.hpp>
#include <eve/arch/platform.hpp>

#include <type_traits>

namespace eve::detail
{
template<ordered_value T, ordered_value U>
EVE_FORCEINLINE auto
absmin_(EVE_SUPPORTS(cpu_),
        numeric_type const&,
        T const& a,
        U const& b) noexcept
-> decltype(absmin(a, b))
{
  return arithmetic_call(numeric(absmin), a, b);
}

template<ordered_value T>
EVE_FORCEINLINE auto
absmin_(EVE_SUPPORTS(cpu_), numeric_type const&, T const& a, T const& b) noexcept
{
  return eve::abs(numeric(eve::min)(a, b));
}

//================================================================================================
// N parameters
//================================================================================================
template<ordered_value T0, ordered_value T1, ordered_value... Ts>
auto
absmin_(EVE_SUPPORTS(cpu_), numeric_type const&, T0 a0, T1 a1, Ts... args)
{
  return eve::abs(numeric(eve::min)(a0, a1, args...));
}

}
