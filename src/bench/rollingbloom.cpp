// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.



#include <bench/bench.h>
#include <common/bloom.h>
#include <crypto/common.h>

#include <vector>

static void RollingBloom(benchmark::Bench& bench)
{
    CRollingBloomFilter filter(120000, 0.000001);
    std::vector<unsigned char> data(32);
    uint32_t count = 0;
    bench.run([&] {
        count++;
        WriteLE32(data.data(), count);
        filter.insert(data);

        WriteBE32(data.data(), count);
        filter.contains(data);
    });
}

static void RollingBloomReset(benchmark::Bench& bench)
{
    CRollingBloomFilter filter(120000, 0.000001);
    bench.run([&] {
        filter.reset();
    });
}

BENCHMARK(RollingBloom, benchmark::PriorityLevel::HIGH);
BENCHMARK(RollingBloomReset, benchmark::PriorityLevel::HIGH);
