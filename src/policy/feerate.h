// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_POLICY_FEERATE_H
#define BITCOINII_POLICY_FEERATE_H

#include <consensus/amount.h>
#include <serialize.h>


#include <cstdint>
#include <string>
#include <type_traits>

const std::string CURRENCY_UNIT = "BC2"; // One formatted unit
const std::string CURRENCY_ATOM = "sat"; // One indivisible minimum value unit

/* Used to determine type of fee estimation requested */
enum class FeeEstimateMode {
    UNSET,        //!< Use default settings based on other criteria
    ECONOMICAL,   //!< Force estimateSmartFee to use non-conservative estimates
    CONSERVATIVE, //!< Force estimateSmartFee to use conservative estimates
    BC2_KVB,      //!< Use BC2/kvB fee rate unit
    SAT_VB,       //!< Use sat/vB fee rate unit
};

/**
 * Fee rate in satooshis per kilovirtualbyte: CAmount / kvB
 */
class CFeeRate
{
private:
    /** Fee rate in sat/kvB (satooshis per 1000 virtualbytes) */
    CAmount nSatoshisPerK;

public:
    /** Fee rate of 0 satooshis per kvB */
    CFeeRate() : nSatoshisPerK(0) { }
    template<typename I>
    explicit CFeeRate(const I _nSatoshisPerK): nSatoshisPerK(_nSatoshisPerK) {
        // We've previously had bugs creep in from silent double->int conversion...
        static_assert(std::is_integral<I>::value, "CFeeRate should be used without floats");
    }

    /**
     * Construct a fee rate from a fee in satooshis and a vsize in vB.
     *
     * param@[in]   nFeePaid    The fee paid by a transaction, in satooshis
     * param@[in]   num_bytes   The vsize of a transaction, in vbytes
     */
    CFeeRate(const CAmount& nFeePaid, uint32_t num_bytes);

    /**
     * Return the fee in satooshis for the given vsize in vbytes.
     * If the calculated fee would have fractional satooshis, then the
     * returned fee will always be rounded up to the nearest satoshi.
     */
    CAmount GetFee(uint32_t num_bytes) const;

    /**
     * Return the fee in satooshis for a vsize of 1000 vbytes
     */
    CAmount GetFeePerK() const { return nSatoshisPerK; }
    friend bool operator<(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK < b.nSatoshisPerK; }
    friend bool operator>(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK > b.nSatoshisPerK; }
    friend bool operator==(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK == b.nSatoshisPerK; }
    friend bool operator<=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK <= b.nSatoshisPerK; }
    friend bool operator>=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK >= b.nSatoshisPerK; }
    friend bool operator!=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK != b.nSatoshisPerK; }
    CFeeRate& operator+=(const CFeeRate& a) { nSatoshisPerK += a.nSatoshisPerK; return *this; }
    std::string ToString(const FeeEstimateMode& fee_estimate_mode = FeeEstimateMode::BC2_KVB) const;
    friend CFeeRate operator*(const CFeeRate& f, int a) { return CFeeRate(a * f.nSatoshisPerK); }
    friend CFeeRate operator*(int a, const CFeeRate& f) { return CFeeRate(a * f.nSatoshisPerK); }

    SERIALIZE_METHODS(CFeeRate, obj) { READWRITE(obj.nSatoshisPerK); }
};

#endif // BITCOINII_POLICY_FEERATE_H
