// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHCUTILS_P_HPP
#define HKHCUTILS_P_HPP

#include <QMetaEnum>

namespace HKUtils
{
template<typename T>
struct function_traits;

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...)> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
};

template<typename F>
using function_ret_t = typename function_traits<decltype(&F::operator())>::return_type;

template<typename F>
using function_args_t = typename function_traits<decltype(&F::operator())>::args_tuple;

template<std::size_t N, typename F>
using function_arg_t = std::tuple_element_t<N, function_args_t<F>>;

template<typename T, auto min = std::numeric_limits<T>::min(), auto max = std::numeric_limits<T>::max()>
[[nodiscard]] constexpr T clamp(auto &&value) noexcept
{
    if (value < min) {
        return std::forward<decltype(min)>(min);
    }
    if (value > max) {
        return std::forward<decltype(max)>(max);
    }
    return static_cast<T>(std::forward<decltype(value)>(value));
}

template<typename T>
Q_ALWAYS_INLINE const char *enumKey(T value)
{
    return QMetaEnum::fromType<T>().valueToKey(value);
}
} // namespace HKUtils

#endif // HKHCUTILS_P_HPP
