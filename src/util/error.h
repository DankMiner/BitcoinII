// Copyright (c) 2010-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINII_UTIL_ERROR_H
#define BITCOINII_UTIL_ERROR_H

/**
 * util/error.h is a common place for definitions of simple error types and
 * string functions. Types and functions defined here should not require any
 * outside dependencies.
 *
 * Error types defined here can be used in different parts of the
 * codebase, to avoid the need to write boilerplate code catching and
 * translating errors passed across wallet/node/rpc/gui code boundaries.
 */

#include <string>

struct bilingual_str;

enum class TransactionError {
    OK, //!< No error
    MISSING_INPUTS,
    ALREADY_IN_CHAIN,
    P2P_DISABLED,
    MEMPOOL_REJECTED,
    MEMPOOL_ERROR,
    INVALID_PSBT,
    PSBT_MISMATCH,
    SIGHASH_MISMATCH,
    MAX_FEE_EXCEEDED,
    MAX_BURN_EXCEEDED,
    EXTERNAL_SIGNER_NOT_FOUND,
    EXTERNAL_SIGNER_FAILED,
    INVALID_PACKAGE,
};

bilingual_str TransactionErrorString(const TransactionError error);

bilingual_str ResolveErrMsg(const std::string& optname, const std::string& strBind);

bilingual_str InvalidPortErrMsg(const std::string& optname, const std::string& strPort);

bilingual_str AmountHighWarn(const std::string& optname);

bilingual_str AmountErrMsg(const std::string& optname, const std::string& strValue);

#endif // BITCOINII_UTIL_ERROR_H
