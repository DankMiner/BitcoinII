// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINII_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
#define BITCOINII_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H

#include <support/cleanse.h>

#include <memory>
#include <vector>

template <typename T>
struct zero_after_free_allocator {
    using value_type = T;

    zero_after_free_allocator() noexcept = default;
    template <typename U>
    zero_after_free_allocator(const zero_after_free_allocator<U>&) noexcept
    {
    }

    T* allocate(std::size_t n)
    {
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n)
    {
        if (p != nullptr)
            memory_cleanse(p, sizeof(T) * n);
        std::allocator<T>{}.deallocate(p, n);
    }

    template <typename U>
    friend bool operator==(const zero_after_free_allocator&, const zero_after_free_allocator<U>&) noexcept
    {
        return true;
    }
    template <typename U>
    friend bool operator!=(const zero_after_free_allocator&, const zero_after_free_allocator<U>&) noexcept
    {
        return false;
    }
};

/** Byte-vector that clears its contents before deletion. */
using SerializeData = std::vector<std::byte, zero_after_free_allocator<std::byte>>;

#endif // BITCOINII_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
