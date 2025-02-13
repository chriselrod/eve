//==================================================================================================
/*
  EVE - Expressive Vector Engine
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <eve/arch/fundamental_cardinal.hpp>

namespace eve::detail
{

template<typename Internal,
         std::ptrdiff_t G,
         std::ptrdiff_t... I,
         typename T,
         typename N,
         typename... Ts>
EVE_FORCEINLINE auto
canonical_shuffle_adapter_impl_(EVE_SUPPORTS(cpu_),
                                Internal            internal,
                                pattern_t<I...>     p,
                                fixed<G>            g,
                                logical<wide<T, N>> x,
                                Ts... xs)
requires(abi_t<T, N>::is_wide_logical)
{
  auto shuffle_bits = canonical_shuffle_adapter_impl(internal, p, g, x.mask(), xs.mask()...);
  if constexpr( std::same_as<decltype(shuffle_bits), no_matching_shuffle_t> )
  {
    return no_matching_shuffle;
  }
  else
  {
    using N1 = typename decltype(shuffle_bits)::cardinal_type;
    return bit_cast(shuffle_bits, as<logical<wide<T, N1>>>{});
  }
}

template<typename Internal, std::ptrdiff_t G, std::ptrdiff_t... I> struct BundleLambda
{
  Internal internal;

  EVE_FORCEINLINE
  auto operator()(auto... xs) const noexcept
  {
    return canonical_shuffle_adapter_impl(internal, pattern<I...>, lane<G>, xs...);
  }
};

template<typename> struct had_no_matching_shuffle;
template<typename... Ts> struct had_no_matching_shuffle<kumi::tuple<Ts...>>
{
  static constexpr bool value = (std::same_as<Ts, no_matching_shuffle_t> || ...);
};

template<typename T> constexpr bool had_no_matching_shuffle_v = had_no_matching_shuffle<T>::value;

template<typename Internal,
         product_scalar_value T,
         typename N,
         std::ptrdiff_t G,
         std::ptrdiff_t... I,
         typename... Ts>
EVE_FORCEINLINE auto
canonical_shuffle_adapter_impl_(EVE_SUPPORTS(cpu_),
                                Internal internal,
                                pattern_t<I...>,
                                fixed<G>,
                                wide<T, N> x,
                                Ts... xs)
{
  auto each_part = kumi::map(BundleLambda<Internal, G, I...> {internal}, x, xs...);

  if constexpr( had_no_matching_shuffle_v<decltype(each_part)> ) { return no_matching_shuffle; }
  else { return wide<T, N>(each_part); }
}

template<std::size_t N>
constexpr auto
upscale_pattern_impl(std::array<std::ptrdiff_t, N> p)
    -> std::optional<std::array<std::ptrdiff_t, N / 2>>
{
  if( N == 1 ) return std::nullopt;

  std::array<std::ptrdiff_t, N / 2> res {};

  for( int i = 0; i != N / 2; i += 1 )
  {
    int            i2 = i + i;
    std::ptrdiff_t i0 = p[i2];
    std::ptrdiff_t i1 = p[i2 + 1];

    if( i0 == na_ || i1 == na_ )
    {
      if( i0 == i1 || i0 == we_ || i1 == we_ )
      {
        res[i] = na_;
        continue;
      }
      return std::nullopt;
    }

    if( i0 == we_ && i1 == we_ )
    {
      res[i] = we_;
      continue;
    }

    if( i0 == we_ ) i0 = i1 - 1;
    if( i1 == we_ ) i1 = i0 + 1;

    if( i0 + 1 != i1 || i0 % 2 != 0 ) { return std::nullopt; }

    res[i] = i0 / 2;
  }

  return res;
}

template<std::ptrdiff_t... I>
constexpr auto
upscale_pattern(pattern_t<I...> p)
{
  constexpr std::array    p_arr {I...};
  constexpr std::optional attempt = upscale_pattern_impl(p_arr);
  if constexpr( !attempt ) return p;
  else
  {
    constexpr auto res_arr = *attempt;
    return [&]<std::size_t... for_>(std::index_sequence<for_...>)
    { return pattern<res_arr[for_]...>; }(std::make_index_sequence<attempt->size()> {});
  }
}

template<typename G, typename P, typename... Ts> struct simplified_pattern
{
  kumi::tuple<Ts...> x;
  G                  g;
  P                  p;
};

template<typename G, typename P, typename... Ts>
simplified_pattern(kumi::tuple<Ts...>, G, P) -> simplified_pattern<G, P, Ts...>;

template<std::ptrdiff_t G, std::ptrdiff_t... I, typename T, typename N, typename... Ts>
EVE_FORCEINLINE auto
simplify_plain_shuffle1(pattern_t<I...> p, eve::fixed<G> g, eve::wide<T, N> x, Ts... xs)
{
  // On an emulation bitcasting types won't be helpful for anything.
  if constexpr( !supports_simd ) { return simplified_pattern {kumi::tuple {x, xs...}, g, p}; }
  else if constexpr( G >= 2 && sizeof(T) < 8 )
  {
    if constexpr( sizeof(T) == 4 && current_api == neon )
    {
      using up_t = eve::wide<std::uint64_t, typename N::split_type>;
      return simplify_plain_shuffle1(
          p, eve::lane<G / 2>, bit_cast(x, as<up_t> {}), bit_cast(xs, as<up_t> {})...);
    }
    else
    {
      using up_t = eve::wide<upgrade_t<T>, typename N::split_type>;
      return simplify_plain_shuffle1(
          p, eve::lane<G / 2>, bit_cast(x, as<up_t> {}), bit_cast(xs, as<up_t> {})...);
    }
  }
  else if constexpr( std::signed_integral<T> )
  {
    using u_t = eve::wide<std::make_unsigned_t<T>, N>;
    return simplify_plain_shuffle1(p, g, bit_cast(x, as<u_t> {}), bit_cast(xs, as<u_t> {})...);
  }
  else if constexpr( current_api == avx && std::same_as<T, std::uint32_t> && N::value == 8 )
  {
    using f_t = eve::wide<float, N>;
    return simplify_plain_shuffle1(p, g, bit_cast(x, as<f_t> {}), bit_cast(xs, as<f_t> {})...);
  }
  else if constexpr( current_api == avx && std::same_as<T, std::uint64_t> && N::value == 4 )
  {
    using f_t = eve::wide<double, N>;
    return simplify_plain_shuffle1(p, g, bit_cast(x, as<f_t> {}), bit_cast(xs, as<f_t> {})...);
  }
  else if constexpr( fundamental_cardinal_v<T> > N::value )
  {
    auto next_pattern = []<std::size_t... i>(std::index_sequence<i...>) {
      return pattern_t<I..., ((void)i, we_)...> {};
    }(std::make_index_sequence<fundamental_cardinal_v<T> - sizeof...(I)> {});
    using T1 = wide<T, fundamental_cardinal_t<T>>;
    return simplify_plain_shuffle1(
        next_pattern, g, bit_cast(x, as<T1> {}), bit_cast(xs, as<T1> {})...);
  }
  else { return simplified_pattern {kumi::tuple {x, xs...}, g, p}; }
}

template<std::ptrdiff_t G, std::ptrdiff_t... I, typename... Ts>
EVE_FORCEINLINE auto
simplify_plain_shuffle(pattern_t<I...>, eve::fixed<G> g, Ts... xs)
{
  constexpr auto p  = pattern<I...>;
  constexpr auto up = upscale_pattern(p);

  if constexpr( up != p ) return simplify_plain_shuffle(up, eve::lane<G * 2>, xs...);
  else return simplify_plain_shuffle1(p, g, xs...);
}

template<typename... Ts, std::size_t... i>
EVE_FORCEINLINE auto
sfinae_friendly_apply_impl(auto               invocable,
                           auto               p,
                           auto               g,
                           kumi::tuple<Ts...> t,
                           std::index_sequence<i...>) noexcept
    -> decltype(invocable(p, g, get<i>(t)...))
{
  return invocable(p, g, get<i>(t)...);
}

template<typename... Ts>
EVE_FORCEINLINE auto
sfinae_friendly_apply(auto                           invocable,
                      auto                           p,
                      auto                           g,
                      kumi::tuple<Ts...>             t,
                      std::index_sequence_for<Ts...> is = {}) noexcept
    -> decltype(sfinae_friendly_apply_impl(invocable, p, g, t, is))
{
  return sfinae_friendly_apply_impl(invocable, p, g, t, is);
}

template<typename Internal,
         std::ptrdiff_t G,
         std::ptrdiff_t... I,
         plain_scalar_value T,
         typename N,
         typename... Ts>
EVE_FORCEINLINE auto
canonical_shuffle_adapter_impl_(EVE_SUPPORTS(cpu_),
                                Internal        internal,
                                pattern_t<I...> p,
                                fixed<G>        g,
                                wide<T, N>      x,
                                Ts... xs)
{
  auto xgp = simplify_plain_shuffle(p, g, x, xs...);

  if constexpr( requires { sfinae_friendly_apply(internal, xgp.p, xgp.g, xgp.x); } )
  {
    auto r = sfinae_friendly_apply(internal, xgp.p, xgp.g, xgp.x);
    if constexpr( std::same_as<decltype(r), no_matching_shuffle_t> ) { return r; }
    else
    {
      using N1 = eve::fixed<sizeof...(I) * G >;
      return bit_cast(r, as<wide<T, N1>> {}); }
  }
  else { return no_matching_shuffle; }
}

}
