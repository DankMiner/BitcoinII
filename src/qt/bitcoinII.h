// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_QT_BITCOINII_H
#define BITCOINII_QT_BITCOINII_H

#if defined(HAVE_CONFIG_H)
#include <config/bitcoinII-config.h>
#endif

#include <interfaces/node.h>
#include <qt/initexecutor.h>

#include <assert.h>
#include <memory>
#include <optional>

#include <QApplication>

class BitcoinIIGUI;
class ClientModel;
class NetworkStyle;
class OptionsModel;
class PaymentServer;
class PlatformStyle;
class SplashScreen;
class WalletController;
class WalletModel;
namespace interfaces {
class Init;
} // namespace interfaces


/** Main BitcoinII application object */
class BitcoinIIApplication: public QApplication
{
    Q_OBJECT
public:
    explicit BitcoinIIApplication();
    ~BitcoinIIApplication();

#ifdef ENABLE_WALLET
    /// Create payment server
    void createPaymentServer();
#endif
    /// parameter interaction/setup based on rules
    void parameterSetup();
    /// Create options model
    [[nodiscard]] bool createOptionsModel(bool resetSettings);
    /// Initialize prune setting
    void InitPruneSetting(int64_t prune_MiB);
    /// Create main window
    void createWindow(const NetworkStyle *networkStyle);
    /// Create splash screen
    void createSplashScreen(const NetworkStyle *networkStyle);
    /// Create or spawn node
    void createNode(interfaces::Init& init);
    /// Basic initialization, before starting initialization/shutdown thread. Return true on success.
    bool baseInitialize();

    /// Request core initialization
    void requestInitialize();

    /// Get window identifier of QMainWindow (BitcoinIIGUI)
    WId getMainWinId() const;

    /// Setup platform style
    void setupPlatformStyle();

    interfaces::Node& node() const { assert(m_node); return *m_node; }

public Q_SLOTS:
    void initializeResult(bool success, interfaces::BlockAndHeaderTipInfo tip_info);
    /// Request core shutdown
    void requestShutdown();
    /// Handle runaway exceptions. Shows a message box with the problem and quits the program.
    void handleRunawayException(const QString &message);

    /**
     * A helper function that shows a message box
     * with details about a non-fatal exception.
     */
    void handleNonFatalException(const QString& message);

Q_SIGNALS:
    void requestedInitialize();
    void requestedShutdown();
    void windowShown(BitcoinIIGUI* window);

protected:
    bool event(QEvent* e) override;

private:
    std::optional<InitExecutor> m_executor;
    OptionsModel* optionsModel{nullptr};
    ClientModel* clientModel{nullptr};
    BitcoinIIGUI* window{nullptr};
    QTimer* pollShutdownTimer{nullptr};
#ifdef ENABLE_WALLET
    PaymentServer* paymentServer{nullptr};
    WalletController* m_wallet_controller{nullptr};
#endif
    const PlatformStyle* platformStyle{nullptr};
    std::unique_ptr<QWidget> shutdownWindow;
    SplashScreen* m_splash = nullptr;
    std::unique_ptr<interfaces::Node> m_node;

    void startThread();
};

int GuiMain(int argc, char* argv[]);

#endif // BITCOINII_QT_BITCOINII_H
