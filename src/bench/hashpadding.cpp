// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <hash.h>
#include <random.h>
#include <uint256.h>


static void PrePadded(benchmark::Bench& bench)
{

    CSHA256 hasher;

    // Setup the salted hasher
    uint256 nonce = GetRandHash();
    hasher.Write(nonce.begin(), 32);
    hasher.Write(nonce.begin(), 32);
    uint256 data = GetRandHash();
    bench.run([&] {
        unsigned char out[32];
        CSHA256 h = hasher;
        h.Write(data.begin(), 32);
        h.Finalize(out);
    });
}

BENCHMARK(PrePadded, benchmark::PriorityLevel::HIGH);

static void RegularPadded(benchmark::Bench& bench)
{
    CSHA256 hasher;

    // Setup the salted hasher
    uint256 nonce = GetRandHash();
    uint256 data = GetRandHash();
    bench.run([&] {
        unsigned char out[32];
        CSHA256 h = hasher;
        h.Write(nonce.begin(), 32);
        h.Write(data.begin(), 32);
        h.Finalize(out);
    });
}

BENCHMARK(RegularPadded, benchmark::PriorityLevel::HIGH);
