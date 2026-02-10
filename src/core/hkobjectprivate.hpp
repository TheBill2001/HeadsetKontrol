// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKOBJECTPRIVATE_HPP
#define HKOBJECTPRIVATE_HPP

#include <QObject>

template<typename T>
    requires(std::is_base_of_v<QObject, T>)
class HKObjectPrivate
{
    Q_DISABLE_COPY(HKObjectPrivate)
protected:
    T *const q_ptr;

    explicit HKObjectPrivate(T *const q_ptr)
        : q_ptr{q_ptr}
    {
    }

public:
    virtual ~HKObjectPrivate() = default;

    template<typename O>
        requires(std::is_base_of_v<QObject, O>)
    friend QBindingStorage *qGetBindingStorage(HKObjectPrivate<O> *object);

    template<typename O>
        requires(std::is_base_of_v<QObject, O>)
    friend const QBindingStorage *qGetBindingStorage(const HKObjectPrivate<O> *const object);
};

template<typename T>
    requires(std::is_base_of_v<QObject, T>)
[[nodiscard]] QBindingStorage *qGetBindingStorage(HKObjectPrivate<T> *object)
{
    return object->q_ptr->bindingStorage();
}

template<typename T>
    requires(std::is_base_of_v<QObject, T>)
[[nodiscard]] const QBindingStorage *qGetBindingStorage(const HKObjectPrivate<T> *const object)
{
    return static_cast<const T *>(object->q_ptr)->bindingStorage();
}

#endif // HKOBJECTPRIVATE_HPP
