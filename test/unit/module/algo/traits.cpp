//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

#include "test.hpp"

#include <eve/module/algo.hpp>

TTS_CASE("eve.algo basic traits testing")
{
  {
    []<typename Traits>(Traits) {
      TTS_CONSTEXPR_EXPECT_NOT(Traits::contains(eve::algo::unroll_key));
      TTS_CONSTEXPR_EXPECT(eve::algo::get_unrolling<Traits>() == 1);
    } (eve::algo::traits());
  }
  {
    auto just_unroll = eve::algo::traits(eve::algo::unroll<4>);
    []<typename Traits>(Traits){
      TTS_CONSTEXPR_EXPECT(eve::algo::get_unrolling<Traits>() == 4);
    }(just_unroll);
  }
  {
    auto divisible_by_cardinal = eve::algo::traits(eve::algo::divisible_by_cardinal);
    []<typename Traits>(Traits){
      TTS_CONSTEXPR_EXPECT(Traits::contains(eve::algo::divisible_by_cardinal));
    }(divisible_by_cardinal);
  }
};

TTS_CASE("eve.algo defaulting")
{
  {
    eve::algo::traits out =
      eve::algo::default_to(eve::algo::traits(eve::algo::unroll<1>), eve::algo::traits(eve::algo::unroll<4>));
    TTS_CONSTEXPR_EXPECT(eve::algo::get_unrolling<decltype(out)>() == 1);
    TTS_CONSTEXPR_EXPECT(!decltype(out)::contains((eve::algo::divisible_by_cardinal)));
    TTS_CONSTEXPR_EXPECT(!decltype(out)::contains((eve::algo::no_aligning)));
  }
  {
    eve::algo::traits out = eve::algo::default_to(
      eve::algo::traits(eve::algo::divisible_by_cardinal),
      eve::algo::traits(eve::algo::unroll<4>,
                        eve::algo::no_aligning));
    TTS_CONSTEXPR_EXPECT(eve::algo::get_unrolling<decltype(out)>() == 4);
    TTS_CONSTEXPR_EXPECT(decltype(out)::contains((eve::algo::divisible_by_cardinal)));
    TTS_CONSTEXPR_EXPECT(decltype(out)::contains((eve::algo::no_aligning)));
  }
  {
    constexpr auto expected = eve::algo::traits(eve::algo::divisible_by_cardinal);
    constexpr auto actual = eve::algo::default_to(eve::algo::traits(), expected);

    TTS_TYPE_IS(decltype(expected), decltype(actual));
  }
};

TTS_CASE("eve.algo.traits consider types")
{
  eve::algo::traits expected{ eve::algo::consider_types<double, int, std::int8_t> };
  {
    eve::algo::traits tr;
    eve::algo::traits def{eve::algo::consider_types<double, int, std::int8_t>};
    auto actual = eve::algo::default_to(tr, def);
    TTS_TYPE_IS(decltype(expected), decltype(actual));
  }
  {
    eve::algo::traits tr{eve::algo::consider_types<double, int, std::int8_t>};
    eve::algo::traits def;
    auto actual = eve::algo::default_to(tr, def);
    TTS_TYPE_IS(decltype(expected), decltype(actual));
  }
  {
    eve::algo::traits tr{eve::algo::consider_types<double>};
    eve::algo::traits def{eve::algo::consider_types<int, std::int8_t>};
    auto actual = eve::algo::default_to(tr, def);
    TTS_TYPE_IS(decltype(expected), decltype(actual));
  }
  {
    eve::algo::traits tr;
    TTS_TYPE_IS((eve::algo::get_types_to_consider_for<decltype(tr), int*>), kumi::tuple<int>);
  }
  {
    eve::algo::traits tr{eve::algo::consider_types<double>};
    TTS_TYPE_IS((eve::algo::get_types_to_consider_for<decltype(tr), int*>), (kumi::tuple<double, int>));
  }
};

TTS_CASE("eve.algo.traits, type and cardinal")
{
  {
    eve::algo::traits tr;
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr), int*>), eve::fixed<eve::nofs_cardinal_v<int>>);
    eve::algo::traits tr1{eve::algo::allow_frequency_scaling};
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr1), int*>), eve::fixed<eve::expected_cardinal_v<int>>);
  }
  {
    eve::algo::traits tr{eve::algo::force_cardinal<2>};
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr), int*>), eve::fixed<2>);
    eve::algo::traits tr1{eve::algo::force_cardinal<2>, eve::algo::allow_frequency_scaling};
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr1), int*>), eve::fixed<2>);
  }
  {
    eve::algo::traits tr{eve::algo::consider_types<double>};
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr), int*>), eve::fixed<eve::nofs_cardinal_v<double>>);
    eve::algo::traits tr1{eve::algo::consider_types<double>, eve::algo::allow_frequency_scaling};
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr1), int*>), eve::fixed<eve::expected_cardinal_v<double>>);
  }
  {
    eve::algo::traits tr;
    eve::algo::traits big_step{eve::algo::force_cardinal<64>};
    eve::algo::traits tr2 = eve::algo::default_to(tr, big_step);
    TTS_TYPE_IS((eve::algo::iteration_cardinal_t<decltype(tr2), std::int8_t*>), eve::fixed<64>);
  }
};

TTS_CASE("eve.algo.traits, no_unrolling") {
  eve::algo::traits expected{ eve::algo::unroll<1> };
  eve::algo::traits actual  {eve::algo::no_unrolling};
  TTS_TYPE_IS (decltype(actual), decltype(expected));
};

TTS_CASE("eve.algo.traits, expensive_callable") {
  eve::algo::traits expected{eve::algo::no_aligning, eve::algo::unroll<1>, eve::algo::single_pass};
  eve::algo::traits actual  {eve::algo::expensive_callable};

  TTS_TYPE_IS( decltype(expected), decltype(process_equivalents(actual)) );
};

TTS_CASE("eve.algo.traits, index_type") {
  // default
  {
    eve::algo::traits tr;
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int8_t*>), (std::uint16_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), short*>), (std::uint16_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int32_t*>), (std::uint32_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int64_t*>), (std::uint64_t));

    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), kumi::tuple<std::int8_t, std::int32_t>*>), (std::uint32_t));
  }

  // overriden
  {
    eve::algo::traits tr {eve::algo::index_type<std::uint8_t>};
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int8_t*>),  (std::uint8_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), short*>),        (std::uint8_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int32_t*>), (std::uint8_t));
    TTS_TYPE_IS( (eve::algo::get_index_type_t<decltype(tr), std::int64_t*>), (std::uint8_t));
  }
};

TTS_CASE("eve.algo.traits, oveflow")
{
    using before_max_idx_t =
        std::conditional_t<sizeof(ptrdiff_t) == 4, std::uint16_t, std::uint32_t>;
    eve::algo::traits tr {eve::algo::overflow<std::numeric_limits<before_max_idx_t>::max()>};
    TTS_CONSTEXPR_EQUAL(eve::algo::get_overflow<decltype(tr)>(),
                        std::numeric_limits<before_max_idx_t>::max());
};

// Funciton with traits support

template <typename TraitsSupport>
struct func_ : TraitsSupport
{
  using traits_type = typename TraitsSupport::traits_type;

  constexpr std::ptrdiff_t get_unrolling() const
  {
    return eve::algo::get_unrolling<traits_type>();
  }

  constexpr bool is_divisible_by_cardinal() const
  {
    return traits_type::contains(eve::algo::divisible_by_cardinal);
  }
};

inline constexpr auto func = eve::algo::function_with_traits<func_>;

TTS_CASE("eve.algo.support_traits")
{
  constexpr auto unroll = func[eve::algo::traits{eve::algo::unroll<2>}];
  TTS_CONSTEXPR_EQUAL(unroll.get_unrolling(), 2);

  constexpr auto is_divisible = unroll[eve::algo::divisible_by_cardinal];
  TTS_CONSTEXPR_EXPECT(is_divisible.is_divisible_by_cardinal());
  TTS_CONSTEXPR_EQUAL(is_divisible.get_unrolling(), 2);
};
