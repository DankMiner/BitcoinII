// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_QT_PSBTOPERATIONSDIALOG_H
#define BITCOINII_QT_PSBTOPERATIONSDIALOG_H

#include <QDialog>
#include <QString>

#include <psbt.h>
#include <qt/clientmodel.h>
#include <qt/walletmodel.h>

namespace Ui {
class PSBTOperationsDialog;
}

/** Dialog showing transaction details. */
class PSBTOperationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PSBTOperationsDialog(QWidget* parent, WalletModel* walletModel, ClientModel* clientModel);
    ~PSBTOperationsDialog();

    void openWithPSBT(PartiallySignedTransaction psbtx);

public Q_SLOTS:
    void signTransaction();
    void broadcastTransaction();
    void copyToClipboard();
    void saveTransaction();

private:
    Ui::PSBTOperationsDialog* m_ui;
    PartiallySignedTransaction m_transaction_data;
    WalletModel* m_wallet_model;
    ClientModel* m_client_model;

    enum class StatusLevel {
        INFO,
        WARN,
        ERR
    };

    size_t couldSignInputs(const PartiallySignedTransaction &psbtx);
    void updateTransactionDisplay();
    QString renderTransaction(const PartiallySignedTransaction &psbtx);
    void showStatus(const QString &msg, StatusLevel level);
    void showTransactionStatus(const PartiallySignedTransaction &psbtx);
};

#endif // BITCOINII_QT_PSBTOPERATIONSDIALOG_H
