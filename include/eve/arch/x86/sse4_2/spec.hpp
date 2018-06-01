//==================================================================================================
/**
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
**/
//==================================================================================================
#ifndef EVE_ARCH_X86_SSE4_2_SPEC_HPP_INCLUDED
#define EVE_ARCH_X86_SSE4_2_SPEC_HPP_INCLUDED

#include <eve/arch/x86/sse4_2/tags.hpp>

#if !defined(EVE_CURRENT_ABI)
  #if EVE_HW_X86 == EVE_SSE4_2_VERSION
    #define EVE_CURRENT_ABI ::eve::sse_
    #define EVE_CURRENT_API ::eve::sse4_2_
  #endif
#endif

#if EVE_HW_X86 >= EVE_SSE4_2_VERSION
  #include <nmmintrin.h>
  #include <eve/arch/x86/sse2/abi_of.hpp>
  #include <eve/arch/x86/sse2/as_register.hpp>
#endif

#endif
