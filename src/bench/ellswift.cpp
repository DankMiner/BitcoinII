// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <bench/bench.h>

#include <key.h>
#include <random.h>

static void EllSwiftCreate(benchmark::Bench& bench)
{
    ECC_Start();

    CKey key = GenerateRandomKey();
    uint256 entropy = GetRandHash();

    bench.batch(1).unit("pubkey").run([&] {
        auto ret = key.EllSwiftCreate(MakeByteSpan(entropy));
        /* Use the first 32 bytes of the ellswift encoded public key as next private key. */
        key.Set(UCharCast(ret.data()), UCharCast(ret.data()) + 32, true);
        assert(key.IsValid());
        /* Use the last 32 bytes of the ellswift encoded public key as next entropy. */
        std::copy(ret.begin() + 32, ret.begin() + 64, MakeWritableByteSpan(entropy).begin());
    });

    ECC_Stop();
}

BENCHMARK(EllSwiftCreate, benchmark::PriorityLevel::HIGH);
