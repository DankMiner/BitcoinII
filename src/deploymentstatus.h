// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_DEPLOYMENTSTATUS_H
#define BITCOINII_DEPLOYMENTSTATUS_H

#include <chain.h>
#include <versionbits.h>

#include <limits>

/** Determine if a deployment is active for the next block */
inline bool DeploymentActiveAfter(const CBlockIndex* pindexPrev, const Consensus::Params& params, Consensus::BuriedDeployment dep, [[maybe_unused]] VersionBitsCache& versionbitscache)
{
    assert(Consensus::ValidDeployment(dep));
    return (pindexPrev == nullptr ? 0 : pindexPrev->nHeight + 1) >= params.DeploymentHeight(dep);
}

inline bool DeploymentActiveAfter(const CBlockIndex* pindexPrev, const Consensus::Params& params, Consensus::DeploymentPos dep, VersionBitsCache& versionbitscache)
{
    assert(Consensus::ValidDeployment(dep));
    return ThresholdState::ACTIVE == versionbitscache.State(pindexPrev, params, dep);
}

/** Determine if a deployment is active for this block */
inline bool DeploymentActiveAt(const CBlockIndex& index, const Consensus::Params& params, Consensus::BuriedDeployment dep, [[maybe_unused]] VersionBitsCache& versionbitscache)
{
    assert(Consensus::ValidDeployment(dep));
    return index.nHeight >= params.DeploymentHeight(dep);
}

inline bool DeploymentActiveAt(const CBlockIndex& index, const Consensus::Params& params, Consensus::DeploymentPos dep, VersionBitsCache& versionbitscache)
{
    assert(Consensus::ValidDeployment(dep));
    return DeploymentActiveAfter(index.pprev, params, dep, versionbitscache);
}

/** Determine if a deployment is enabled (can ever be active) */
inline bool DeploymentEnabled(const Consensus::Params& params, Consensus::BuriedDeployment dep)
{
    assert(Consensus::ValidDeployment(dep));
    return params.DeploymentHeight(dep) != std::numeric_limits<int>::max();
}

inline bool DeploymentEnabled(const Consensus::Params& params, Consensus::DeploymentPos dep)
{
    assert(Consensus::ValidDeployment(dep));
    return params.vDeployments[dep].nStartTime != Consensus::BIP9Deployment::NEVER_ACTIVE;
}

#endif // BITCOINII_DEPLOYMENTSTATUS_H
