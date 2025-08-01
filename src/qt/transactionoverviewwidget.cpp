// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <qt/transactionoverviewwidget.h>

#include <qt/transactiontablemodel.h>

#include <QListView>
#include <QSize>
#include <QSizePolicy>

TransactionOverviewWidget::TransactionOverviewWidget(QWidget* parent)
    : QListView(parent) {}

QSize TransactionOverviewWidget::sizeHint() const
{
    return {sizeHintForColumn(TransactionTableModel::ToAddress), QListView::sizeHint().height()};
}

void TransactionOverviewWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Minimum);
    setSizePolicy(sp);
}
