// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <crypto/hkdf_sha256_32.h>

#include <assert.h>
#include <string.h>

CHKDF_HMAC_SHA256_L32::CHKDF_HMAC_SHA256_L32(const unsigned char* ikm, size_t ikmlen, const std::string& salt)
{
    CHMAC_SHA256((const unsigned char*)salt.data(), salt.size()).Write(ikm, ikmlen).Finalize(m_prk);
}

void CHKDF_HMAC_SHA256_L32::Expand32(const std::string& info, unsigned char hash[OUTPUT_SIZE])
{
    // expand a 32byte key (single round)
    assert(info.size() <= 128);
    static const unsigned char one[1] = {1};
    CHMAC_SHA256(m_prk, 32).Write((const unsigned char*)info.data(), info.size()).Write(one, 1).Finalize(hash);
}
