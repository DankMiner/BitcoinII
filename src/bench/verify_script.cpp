// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#if defined(HAVE_CONFIG_H)
#include <config/bitcoinII-config.h>
#endif

#include <bench/bench.h>
#include <key.h>
#if defined(HAVE_CONSENSUS_LIB)
#include <script/bitcoinIIconsensus.h>
#endif
#include <script/script.h>
#include <script/interpreter.h>
#include <streams.h>
#include <test/util/transaction_utils.h>

#include <array>

// Microbenchmark for verification of a basic P2WPKH script. Can be easily
// modified to measure performance of other types of scripts.
static void VerifyScriptBench(benchmark::Bench& bench)
{
    ECC_Start();

    const uint32_t flags{SCRIPT_VERIFY_WITNESS | SCRIPT_VERIFY_P2SH};
    const int witnessversion = 0;

    // Key pair.
    CKey key;
    static const std::array<unsigned char, 32> vchKey = {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
        }
    };
    key.Set(vchKey.begin(), vchKey.end(), false);
    CPubKey pubkey = key.GetPubKey();
    uint160 pubkeyHash;
    CHash160().Write(pubkey).Finalize(pubkeyHash);

    // Script.
    CScript scriptPubKey = CScript() << witnessversion << ToByteVector(pubkeyHash);
    CScript scriptSig;
    CScript witScriptPubkey = CScript() << OP_DUP << OP_HASH160 << ToByteVector(pubkeyHash) << OP_EQUALVERIFY << OP_CHECKSIG;
    const CMutableTransaction& txCredit = BuildCreditingTransaction(scriptPubKey, 1);
    CMutableTransaction txSpend = BuildSpendingTransaction(scriptSig, CScriptWitness(), CTransaction(txCredit));
    CScriptWitness& witness = txSpend.vin[0].scriptWitness;
    witness.stack.emplace_back();
    key.Sign(SignatureHash(witScriptPubkey, txSpend, 0, SIGHASH_ALL, txCredit.vout[0].nValue, SigVersion::WITNESS_V0), witness.stack.back());
    witness.stack.back().push_back(static_cast<unsigned char>(SIGHASH_ALL));
    witness.stack.push_back(ToByteVector(pubkey));

    // Benchmark.
    bench.run([&] {
        ScriptError err;
        bool success = VerifyScript(
            txSpend.vin[0].scriptSig,
            txCredit.vout[0].scriptPubKey,
            &txSpend.vin[0].scriptWitness,
            flags,
            MutableTransactionSignatureChecker(&txSpend, 0, txCredit.vout[0].nValue, MissingDataBehavior::ASSERT_FAIL),
            &err);
        assert(err == SCRIPT_ERR_OK);
        assert(success);

#if defined(HAVE_CONSENSUS_LIB)
        DataStream stream;
        stream << TX_WITH_WITNESS(txSpend);
        int csuccess = bitcoinIIconsensus_verify_script_with_amount(
            txCredit.vout[0].scriptPubKey.data(),
            txCredit.vout[0].scriptPubKey.size(),
            txCredit.vout[0].nValue,
            (const unsigned char*)stream.data(), stream.size(), 0, flags, nullptr);
        assert(csuccess == 1);
#endif
    });
    ECC_Stop();
}

static void VerifyNestedIfScript(benchmark::Bench& bench)
{
    std::vector<std::vector<unsigned char>> stack;
    CScript script;
    for (int i = 0; i < 100; ++i) {
        script << OP_1 << OP_IF;
    }
    for (int i = 0; i < 1000; ++i) {
        script << OP_1;
    }
    for (int i = 0; i < 100; ++i) {
        script << OP_ENDIF;
    }
    bench.run([&] {
        auto stack_copy = stack;
        ScriptError error;
        bool ret = EvalScript(stack_copy, script, 0, BaseSignatureChecker(), SigVersion::BASE, &error);
        assert(ret);
    });
}

BENCHMARK(VerifyScriptBench, benchmark::PriorityLevel::HIGH);
BENCHMARK(VerifyNestedIfScript, benchmark::PriorityLevel::HIGH);
