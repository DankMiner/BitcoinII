// Copyright (c) 2023 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINII_TEST_UTIL_COINS_H
#define BITCOINII_TEST_UTIL_COINS_H

#include <primitives/transaction.h>

class CCoinsViewCache;

/**
 * Create a Coin with DynamicMemoryUsage of 80 bytes and add it to the given view.
 * @param[in,out] coins_view  The coins view cache to add the new coin to.
 * @returns the COutPoint of the created coin.
 */
COutPoint AddTestCoin(CCoinsViewCache& coins_view);

#endif // BITCOINII_TEST_UTIL_COINS_H
