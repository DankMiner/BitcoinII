// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>

#include <bech32.h>
#include <util/strencodings.h>

#include <string>
#include <vector>


static void Bech32Encode(benchmark::Bench& bench)
{
    std::vector<uint8_t> v = ParseHex("c97f5a67ec381b760aeaf67573bc164845ff39a3bb26a1cee401ac67243b48db");
    std::vector<unsigned char> tmp = {0};
    tmp.reserve(1 + 32 * 8 / 5);
    ConvertBits<8, 5, true>([&](unsigned char c) { tmp.push_back(c); }, v.begin(), v.end());
    bench.batch(v.size()).unit("byte").run([&] {
        bech32::Encode(bech32::Encoding::BECH32, "bc", tmp);
    });
}


static void Bech32Decode(benchmark::Bench& bench)
{
    std::string addr = "bc1qkallence7tjawwvy0dwt4twc62qjgaw8f4vlhyd006d99f09";
    bench.batch(addr.size()).unit("byte").run([&] {
        bech32::Decode(addr);
    });
}


BENCHMARK(Bech32Encode, benchmark::PriorityLevel::HIGH);
BENCHMARK(Bech32Decode, benchmark::PriorityLevel::HIGH);
