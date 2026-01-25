// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKMACROS_P_HPP
#define HKMACROS_P_HPP

#define HK_ASSERT_COPY_CONSTRUCTIBLE(Class) static_assert(std::is_copy_constructible_v<Class>, #Class " is not copy constructible!");

#define HK_ASSERT_COPY_ASSIGNABLE(Class) static_assert(std::is_copy_assignable_v<Class>, #Class " is not copy constructible!");

#define HK_ASSERT_COPYABLE(Class)                                                                                                                              \
    HK_ASSERT_COPY_CONSTRUCTIBLE(Class)                                                                                                                        \
    HK_ASSERT_COPY_ASSIGNABLE(Class)

#define HK_ASSERT_MOVE_CONSTRUCTIBLE(Class) static_assert(std::is_move_constructible_v<Class>, #Class " is not move constructible!");

#define HK_ASSERT_MOVE_ASSIGNABLE(Class) static_assert(std::is_move_assignable_v<Class>, #Class " is not move constructible!");

#define HK_ASSERT_MOVEABLE(Class)                                                                                                                              \
    HK_ASSERT_MOVE_CONSTRUCTIBLE(Class)                                                                                                                        \
    HK_ASSERT_MOVE_ASSIGNABLE(Class)

#define HK_ASSERT_COPYABLE_MOVEABLE(Class)                                                                                                                     \
    HK_ASSERT_COPYABLE(Class)                                                                                                                                  \
    HK_ASSERT_MOVEABLE(Class)

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

#endif // HKMACROS_P_HPP
