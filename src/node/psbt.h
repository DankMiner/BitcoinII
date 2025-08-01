// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_NODE_PSBT_H
#define BITCOINII_NODE_PSBT_H

#include <psbt.h>

#include <optional>

namespace node {
/**
 * Holds an analysis of one input from a PSBT
 */
struct PSBTInputAnalysis {
    bool has_utxo; //!< Whether we have UTXO information for this input
    bool is_final; //!< Whether the input has all required information including signatures
    PSBTRole next; //!< Which of the BIP 174 roles needs to handle this input next

    std::vector<CKeyID> missing_pubkeys; //!< Pubkeys whose BIP32 derivation path is missing
    std::vector<CKeyID> missing_sigs;    //!< Pubkeys whose signatures are missing
    uint160 missing_redeem_script;       //!< Hash160 of redeem script, if missing
    uint256 missing_witness_script;      //!< SHA256 of witness script, if missing
};

/**
 * Holds the results of AnalyzePSBT (miscellaneous information about a PSBT)
 */
struct PSBTAnalysis {
    std::optional<size_t> estimated_vsize;      //!< Estimated weight of the transaction
    std::optional<CFeeRate> estimated_feerate;  //!< Estimated feerate (fee / weight) of the transaction
    std::optional<CAmount> fee;                 //!< Amount of fee being paid by the transaction
    std::vector<PSBTInputAnalysis> inputs;      //!< More information about the individual inputs of the transaction
    PSBTRole next;                              //!< Which of the BIP 174 roles needs to handle the transaction next
    std::string error;                          //!< Error message

    void SetInvalid(std::string err_msg)
    {
        estimated_vsize = std::nullopt;
        estimated_feerate = std::nullopt;
        fee = std::nullopt;
        inputs.clear();
        next = PSBTRole::CREATOR;
        error = err_msg;
    }
};

/**
 * Provides helpful miscellaneous information about where a PSBT is in the signing workflow.
 *
 * @param[in] psbtx the PSBT to analyze
 * @return A PSBTAnalysis with information about the provided PSBT.
 */
PSBTAnalysis AnalyzePSBT(PartiallySignedTransaction psbtx);
} // namespace node

#endif // BITCOINII_NODE_PSBT_H
