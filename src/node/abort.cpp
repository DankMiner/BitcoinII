// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <node/abort.h>

#include <logging.h>
#include <node/interface_ui.h>
#include <util/signalinterrupt.h>
#include <util/translation.h>
#include <warnings.h>

#include <atomic>
#include <cstdlib>
#include <string>

namespace node {

void AbortNode(util::SignalInterrupt* shutdown, std::atomic<int>& exit_status, const std::string& debug_message, const bilingual_str& user_message)
{
    SetMiscWarning(Untranslated(debug_message));
    LogPrintf("*** %s\n", debug_message);
    InitError(user_message.empty() ? _("A fatal internal error occurred, see debug.log for details") : user_message);
    exit_status.store(EXIT_FAILURE);
    if (shutdown && !(*shutdown)()) {
        LogPrintf("Error: failed to send shutdown signal\n");
    };
}
} // namespace node
