// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_KERNEL_CHAIN_H
#define BITCOINII_KERNEL_CHAIN_H

#include<iostream>

class CBlock;
class CBlockIndex;
namespace interfaces {
struct BlockInfo;
} // namespace interfaces

namespace kernel {
//! Return data from block index.
interfaces::BlockInfo MakeBlockInfo(const CBlockIndex* block_index, const CBlock* data = nullptr);

} // namespace kernel

//! This enum describes the various roles a specific Chainstate instance can take.
//! Other parts of the system sometimes need to vary in behavior depending on the
//! existence of a background validation chainstate, e.g. when building indexes.
enum class ChainstateRole {
    // Single chainstate in use, "normal" IBD mode.
    NORMAL,

    // Doing IBD-style validation in the background. Implies use of an assumed-valid
    // chainstate.
    BACKGROUND,

    // Active assumed-valid chainstate. Implies use of a background IBD chainstate.
    ASSUMEDVALID,
};

std::ostream& operator<<(std::ostream& os, const ChainstateRole& role);

#endif // BITCOINII_KERNEL_CHAIN_H
