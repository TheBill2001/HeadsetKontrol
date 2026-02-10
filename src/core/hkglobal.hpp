// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKGLOBAL_HPP
#define HKGLOBAL_HPP

#include "hklogging.hpp" // IWYU pragma: export

// Ref: https://stackoverflow.com/a/77894709
#define HK_VALUE_IFNOT_TEST(...) __VA_ARGS__
#define HK_VALUE_IFNOT_TEST0(...) __VA_ARGS__
#define HK_VALUE_IFNOT_TEST1(...)
#define HK_VALUE_IFNOT(COND, ...) HK_VALUE_IFNOT_TEST##COND(__VA_ARGS__)

#define HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(Class, Type, Name)                                                                                                \
    void Name##Changed(Type value)                                                                                                                             \
    {                                                                                                                                                          \
        Q_Q(Class);                                                                                                                                            \
        Q_EMIT q->Name##Changed(value, Class::QPrivateSignal{});                                                                                               \
    }                                                                                                                                                          \
    Q_OBJECT_BINDABLE_PROPERTY(Class##Private, Type, Name, &Class##Private::Name##Changed)

#define HK_PRIVATE_BINDABLE_PROPERTY(Class, Type, Name)                                                                                                        \
    void Name##Changed(const Type &value)                                                                                                                      \
    {                                                                                                                                                          \
        Q_Q(Class);                                                                                                                                            \
        Q_EMIT q->Name##Changed(value, Class::QPrivateSignal{});                                                                                               \
    }                                                                                                                                                          \
    Q_OBJECT_BINDABLE_PROPERTY(Class##Private, Type, Name, &Class##Private::Name##Changed)

#define STD_STRVIEW_TO_QSTR(StringView) QUtf8StringView(StringView).toString()

#define Q_D_SINGLETON(Class) Class##Private *const d = static_cast<Class *>(instance())->d_func() // NOLINT(*identifier-length)

#define HK_DEBUG(...) qCDebug(HK_LOGGING __VA_OPT__(, ) __VA_ARGS__) HK_VALUE_IFNOT(__VA_OPT__(1), .noquote())
#define HK_INFO(...) qCInfo(HK_LOGGING __VA_OPT__(, ) __VA_ARGS__) HK_VALUE_IFNOT(__VA_OPT__(1), .noquote())
#define HK_WARN(...) qCWarning(HK_LOGGING __VA_OPT__(, ) __VA_ARGS__) HK_VALUE_IFNOT(__VA_OPT__(1), .noquote())
#define HK_CRITICAL(...) qCCritical(HK_LOGGING __VA_OPT__(, ) __VA_ARGS__) HK_VALUE_IFNOT(__VA_OPT__(1), .noquote())
#define HK_FATAL(...) qCFatal(HK_LOGGING __VA_OPT__(, ) __VA_ARGS__) HK_VALUE_IFNOT(__VA_OPT__(1), .noquote())

template<typename T>
struct HKFunctionTraits;

template<typename R, typename C, typename... Args>
struct HKFunctionTraits<R (C::*)(Args...) const> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
};

template<typename R, typename C, typename... Args>
struct HKFunctionTraits<R (C::*)(Args...)> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
};

template<typename F>
using HKFunctionReturnType = typename HKFunctionTraits<decltype(&F::operator())>::return_type;

template<typename F>
using HKFunctionArgumentTypes = typename HKFunctionTraits<decltype(&F::operator())>::args_tuple;

template<std::size_t N, typename F>
using HKFunctionArgumentType = std::tuple_element_t<N, HKFunctionArgumentTypes<F>>;

template<typename T>
concept HKNumeric = std::integral<T> || std::floating_point<T>;

#endif // HKGLOBAL_HPP
