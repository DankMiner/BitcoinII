// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINII_TEST_FUZZ_FUZZ_H
#define BITCOINII_TEST_FUZZ_FUZZ_H

#include <span.h>

#include <cstdint>
#include <functional>
#include <string_view>

/**
 * Can be used to limit a theoretically unbounded loop. This caps the runtime
 * to avoid timeouts or OOMs.
 *
 * This can be used in combination with a check in the condition to confirm
 * whether the fuzz engine provided "good" data. If the fuzz input contains
 * invalid data, the loop aborts early. This will teach the fuzz engine to look
 * for useful data and avoids bloating the fuzz input folder with useless data.
 */
#define LIMITED_WHILE(condition, limit) \
    for (unsigned _count{limit}; (condition) && _count; --_count)

using FuzzBufferType = Span<const uint8_t>;

using TypeTestOneInput = std::function<void(FuzzBufferType)>;
struct FuzzTargetOptions {
    std::function<void()> init{[] {}};
    bool hidden{false};
};

void FuzzFrameworkRegisterTarget(std::string_view name, TypeTestOneInput target, FuzzTargetOptions opts);

#define FUZZ_TARGET(...) DETAIL_FUZZ(__VA_ARGS__)

#define DETAIL_FUZZ(name, ...)                                                        \
    void name##_fuzz_target(FuzzBufferType);                                          \
    struct name##_Before_Main {                                                       \
        name##_Before_Main()                                                          \
        {                                                                             \
            FuzzFrameworkRegisterTarget(#name, name##_fuzz_target, {__VA_ARGS__});    \
        }                                                                             \
    } const static g_##name##_before_main;                                            \
    void name##_fuzz_target(FuzzBufferType buffer)

#endif // BITCOINII_TEST_FUZZ_FUZZ_H
