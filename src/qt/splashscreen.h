// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_QT_SPLASHSCREEN_H
#define BITCOINII_QT_SPLASHSCREEN_H

#include <QWidget>

#include <memory>

class NetworkStyle;

namespace interfaces {
class Handler;
class Node;
class Wallet;
};

/** Class for the splashscreen with information of the running client.
 *
 * @note this is intentionally not a QSplashScreen. BitcoinII Core initialization
 * can take a long time, and in that case a progress window that cannot be
 * moved around and minimized has turned out to be frustrating to the user.
 */
class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(const NetworkStyle *networkStyle);
    ~SplashScreen();
    void setNode(interfaces::Node& node);

protected:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public Q_SLOTS:
    /** Show message and progress */
    void showMessage(const QString &message, int alignment, const QColor &color);

    /** Handle wallet load notifications. */
    void handleLoadWallet();

protected:
    bool eventFilter(QObject * obj, QEvent * ev) override;

private:
    /** Connect core signals to splash screen */
    void subscribeToCoreSignals();
    /** Disconnect core signals to splash screen */
    void unsubscribeFromCoreSignals();
    /** Initiate shutdown */
    void shutdown();

    QPixmap pixmap;
    QString curMessage;
    QColor curColor;
    int curAlignment{0};

    interfaces::Node* m_node = nullptr;
    bool m_shutdown = false;
    std::unique_ptr<interfaces::Handler> m_handler_init_message;
    std::unique_ptr<interfaces::Handler> m_handler_show_progress;
    std::unique_ptr<interfaces::Handler> m_handler_init_wallet;
    std::unique_ptr<interfaces::Handler> m_handler_load_wallet;
    std::list<std::unique_ptr<interfaces::Wallet>> m_connected_wallets;
    std::list<std::unique_ptr<interfaces::Handler>> m_connected_wallet_handlers;
};

#endif // BITCOINII_QT_SPLASHSCREEN_H
