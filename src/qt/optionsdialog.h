// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOINII_QT_OPTIONSDIALOG_H
#define BITCOINII_QT_OPTIONSDIALOG_H

#include <QDialog>
#include <QValidator>

class ClientModel;
class OptionsModel;
class QValidatedLineEdit;

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
class OptionsDialog;
}

/** Proxy address widget validator, checks for a valid proxy address.
 */
class ProxyAddressValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ProxyAddressValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Preferences dialog. */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent, bool enableWallet);
    ~OptionsDialog();

    enum Tab {
        TAB_MAIN,
        TAB_NETWORK,
    };

    void setClientModel(ClientModel* client_model);
    void setModel(OptionsModel *model);
    void setMapper();
    void setCurrentTab(OptionsDialog::Tab tab);

private Q_SLOTS:
    /* set OK button state (enabled / disabled) */
    void setOkButtonState(bool fState);
    void on_resetButton_clicked();
    void on_openBitcoinIIConfButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_showTrayIcon_stateChanged(int state);

    void togglePruneWarning(bool enabled);
    void showRestartWarning(bool fPersistent = false);
    void clearStatusLabel();
    void updateProxyValidationState();
    /* query the networks, for which the default proxy is used */
    void updateDefaultProxyNets();

Q_SIGNALS:
    void proxyIpChecks(QValidatedLineEdit *pUiProxyIp, uint16_t nProxyPort);
    void quitOnReset();

private:
    Ui::OptionsDialog *ui;
    ClientModel* m_client_model{nullptr};
    OptionsModel* model{nullptr};
    QDataWidgetMapper* mapper{nullptr};
};

#endif // BITCOINII_QT_OPTIONSDIALOG_H
