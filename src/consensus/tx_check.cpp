// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <consensus/tx_check.h>

#include <consensus/amount.h>
#include <primitives/transaction.h>
#include <consensus/validation.h>

bool CheckTransaction(const CTransaction& tx, TxValidationState& state)
{
    // Basic checks that don't depend on any context
    if (tx.vin.empty())
        return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-vin-empty");
    if (tx.vout.empty())
        return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-vout-empty");
    // Size limits (this doesn't take the witness into account, as that hasn't been checked for malleability)
    if (::GetSerializeSize(TX_NO_WITNESS(tx)) * WITNESS_SCALE_FACTOR > MAX_BLOCK_WEIGHT) {
        return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-oversize");
    }

    // Check for negative or overflow output values (see CVE-2010-5139)
    CAmount nValueOut = 0;
    for (const auto& txout : tx.vout)
    {
        if (txout.nValue < 0)
            return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-vout-negative");
        if (txout.nValue > MAX_MONEY)
            return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-vout-toolarge");
        nValueOut += txout.nValue;
        if (!MoneyRange(nValueOut))
            return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-txouttotal-toolarge");
    }

    // Check for duplicate inputs (see CVE-2018-17144)
    // While Consensus::CheckTxInputs does check if all inputs of a tx are available, and UpdateCoins marks all inputs
    // of a tx as spent, it does not check if the tx has duplicate inputs.
    // Failure to run this check will result in either a crash or an inflation bug, depending on the implementation of
    // the underlying coins database.
    std::set<COutPoint> vInOutPoints;
    for (const auto& txin : tx.vin) {
        if (!vInOutPoints.insert(txin.prevout).second)
            return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-inputs-duplicate");
    }

    if (tx.IsCoinBase())
    {
        if (tx.vin[0].scriptSig.size() < 2 || tx.vin[0].scriptSig.size() > 100)
            return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-cb-length");
    }
    else
    {
        for (const auto& txin : tx.vin)
            if (txin.prevout.IsNull())
                return state.Invalid(TxValidationResult::TX_CONSENSUS, "bad-txns-prevout-null");
    }

    return true;
}
