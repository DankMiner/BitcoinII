// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <bench/bench.h>
#include <bench/data.h>

#include <consensus/validation.h>
#include <node/blockstorage.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <util/chaintype.h>
#include <validation.h>

static FlatFilePos WriteBlockToDisk(ChainstateManager& chainman)
{
    DataStream stream{benchmark::data::block413567};
    CBlock block;
    stream >> TX_WITH_WITNESS(block);

    return chainman.m_blockman.SaveBlockToDisk(block, 0, nullptr);
}

static void ReadBlockFromDiskTest(benchmark::Bench& bench)
{
    const auto testing_setup{MakeNoLogFileContext<const TestingSetup>(ChainType::MAIN)};
    ChainstateManager& chainman{*testing_setup->m_node.chainman};

    CBlock block;
    const auto pos{WriteBlockToDisk(chainman)};

    bench.run([&] {
        const auto success{chainman.m_blockman.ReadBlockFromDisk(block, pos)};
        assert(success);
    });
}

static void ReadRawBlockFromDiskTest(benchmark::Bench& bench)
{
    const auto testing_setup{MakeNoLogFileContext<const TestingSetup>(ChainType::MAIN)};
    ChainstateManager& chainman{*testing_setup->m_node.chainman};

    std::vector<uint8_t> block_data;
    const auto pos{WriteBlockToDisk(chainman)};

    bench.run([&] {
        const auto success{chainman.m_blockman.ReadRawBlockFromDisk(block_data, pos)};
        assert(success);
    });
}

BENCHMARK(ReadBlockFromDiskTest, benchmark::PriorityLevel::HIGH);
BENCHMARK(ReadRawBlockFromDiskTest, benchmark::PriorityLevel::HIGH);
