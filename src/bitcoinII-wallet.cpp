// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#if defined(HAVE_CONFIG_H)
#include <config/bitcoinII-config.h>
#endif

#include <chainparams.h>
#include <chainparamsbase.h>
#include <clientversion.h>
#include <common/args.h>
#include <common/system.h>
#include <common/url.h>
#include <compat/compat.h>
#include <interfaces/init.h>
#include <key.h>
#include <logging.h>
#include <pubkey.h>
#include <tinyformat.h>
#include <util/exception.h>
#include <util/translation.h>
#include <wallet/wallettool.h>

#include <exception>
#include <functional>
#include <string>
#include <tuple>

const std::function<std::string(const char*)> G_TRANSLATION_FUN = nullptr;
UrlDecodeFn* const URL_DECODE = nullptr;

static void SetupWalletToolArgs(ArgsManager& argsman)
{
    SetupHelpOptions(argsman);
    SetupChainParamsBaseOptions(argsman);

    argsman.AddArg("-version", "Print version and exit", ArgsManager::ALLOW_ANY, OptionsCategory::OPTIONS);
    argsman.AddArg("-datadir=<dir>", "Specify data directory", ArgsManager::ALLOW_ANY, OptionsCategory::OPTIONS);
    argsman.AddArg("-wallet=<wallet-name>", "Specify wallet name", ArgsManager::ALLOW_ANY | ArgsManager::NETWORK_ONLY, OptionsCategory::OPTIONS);
    argsman.AddArg("-dumpfile=<file name>", "When used with 'dump', writes out the records to this file. When used with 'createfromdump', loads the records into a new wallet.", ArgsManager::ALLOW_ANY | ArgsManager::DISALLOW_NEGATION, OptionsCategory::OPTIONS);
    argsman.AddArg("-debug=<category>", "Output debugging information (default: 0).", ArgsManager::ALLOW_ANY, OptionsCategory::DEBUG_TEST);
    argsman.AddArg("-descriptors", "Create descriptors wallet. Only for 'create'", ArgsManager::ALLOW_ANY, OptionsCategory::OPTIONS);
    argsman.AddArg("-legacy", "Create legacy wallet. Only for 'create'", ArgsManager::ALLOW_ANY, OptionsCategory::OPTIONS);
    argsman.AddArg("-format=<format>", "The format of the wallet file to create. Either \"bdb\" or \"sqlite\". Only used with 'createfromdump'", ArgsManager::ALLOW_ANY, OptionsCategory::OPTIONS);
    argsman.AddArg("-printtoconsole", "Send trace/debug info to console (default: 1 when no -debug is true, 0 otherwise).", ArgsManager::ALLOW_ANY, OptionsCategory::DEBUG_TEST);

    argsman.AddCommand("info", "Get wallet info");
    argsman.AddCommand("create", "Create new wallet file");
    argsman.AddCommand("salvage", "Attempt to recover private keys from a corrupt wallet. Warning: 'salvage' is experimental.");
    argsman.AddCommand("dump", "Print out all of the wallet key-value records");
    argsman.AddCommand("createfromdump", "Create new wallet file from dumped records");
}

static std::optional<int> WalletAppInit(ArgsManager& args, int argc, char* argv[])
{
    SetupWalletToolArgs(args);
    std::string error_message;
    if (!args.ParseParameters(argc, argv, error_message)) {
        tfm::format(std::cerr, "Error parsing command line arguments: %s\n", error_message);
        return EXIT_FAILURE;
    }
    const bool missing_args{argc < 2};
    if (missing_args || HelpRequested(args) || args.IsArgSet("-version")) {
        std::string strUsage = strprintf("%s bitcoinII-wallet version", PACKAGE_NAME) + " " + FormatFullVersion() + "\n";

        if (args.IsArgSet("-version")) {
            strUsage += FormatParagraph(LicenseInfo());
        } else {
            strUsage += "\n"
                        "bitcoinII-wallet is an offline tool for creating and interacting with " PACKAGE_NAME " wallet files.\n"
                        "By default bitcoinII-wallet will act on wallets in the default mainnet wallet directory in the datadir.\n"
                        "To change the target wallet, use the -datadir, -wallet and -regtest/-signet/-testnet arguments.\n\n"
                        "Usage:\n"
                        "  bitcoinII-wallet [options] <command>\n";
            strUsage += "\n" + args.GetHelpMessage();
        }
        tfm::format(std::cout, "%s", strUsage);
        if (missing_args) {
            tfm::format(std::cerr, "Error: too few parameters\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    // check for printtoconsole, allow -debug
    LogInstance().m_print_to_console = args.GetBoolArg("-printtoconsole", args.GetBoolArg("-debug", false));

    if (!CheckDataDirOption(args)) {
        tfm::format(std::cerr, "Error: Specified data directory \"%s\" does not exist.\n", args.GetArg("-datadir", ""));
        return EXIT_FAILURE;
    }
    // Check for chain settings (Params() calls are only valid after this clause)
    SelectParams(args.GetChainType());

    return std::nullopt;
}

MAIN_FUNCTION
{
    ArgsManager& args = gArgs;
#ifdef WIN32
    common::WinCmdLineArgs winArgs;
    std::tie(argc, argv) = winArgs.get();
#endif

    int exit_status;
    std::unique_ptr<interfaces::Init> init = interfaces::MakeWalletInit(argc, argv, exit_status);
    if (!init) {
        return exit_status;
    }

    SetupEnvironment();
    RandomInit();
    try {
        if (const auto maybe_exit{WalletAppInit(args, argc, argv)}) return *maybe_exit;
    } catch (const std::exception& e) {
        PrintExceptionContinue(&e, "WalletAppInit()");
        return EXIT_FAILURE;
    } catch (...) {
        PrintExceptionContinue(nullptr, "WalletAppInit()");
        return EXIT_FAILURE;
    }

    const auto command = args.GetCommand();
    if (!command) {
        tfm::format(std::cerr, "No method provided. Run `bitcoinII-wallet -help` for valid methods.\n");
        return EXIT_FAILURE;
    }
    if (command->args.size() != 0) {
        tfm::format(std::cerr, "Error: Additional arguments provided (%s). Methods do not take arguments. Please refer to `-help`.\n", Join(command->args, ", "));
        return EXIT_FAILURE;
    }

    ECC_Start();
    if (!wallet::WalletTool::ExecuteWalletToolFunc(args, command->command)) {
        return EXIT_FAILURE;
    }
    ECC_Stop();
    return EXIT_SUCCESS;
}
