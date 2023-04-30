#pragma once

#include <iostream>
#include <memory>
#include <string>

template <typename T>
using ref = std::shared_ptr<T>;
template <typename T>
using unique = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr ref<T> create_ref(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
template <typename T, typename... Args>
constexpr unique<T> create_unique(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#define DEL(PTR) memset(PTR, 0, sizeof(PTR))
#define DELOBJ(OBJ) memset(&OBJ, 0, sizeof(OBJ))