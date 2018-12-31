/*
    Copyright (C) 2018 Martin Klapetek <mklapetek@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "database/dbdao.h"
#include "accountmanager.h"
#include "datamodels/qaccount.h"
#include "datamodels/qtransaction.h"

#include <QDebug>

Q_GLOBAL_STATIC(AccountManager, accountManagerSharedInstance)
AccountManager* AccountManager::sharedInstance()
{
    return accountManagerSharedInstance;
}

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
{

}

void AccountManager::setAccount(QAccount *account)
{
    m_account = account;
}

void AccountManager::setFromDate(const QDate &date)
{
    m_fromDate = date;
    reloadTransactions();
}

void AccountManager::setToDate(const QDate &date)
{
    m_toDate = date;
    reloadTransactions();
}

void AccountManager::setDates(const QDate &fromDate, const QDate &toDate)
{
    m_fromDate = fromDate;
    m_toDate = toDate;

    reloadTransactions();
}

void AccountManager::setTags(const QStringList &tags)
{
    m_tags = tags;
    filterTransactions();
}

QList<QTransaction *> AccountManager::allTransactions() const
{
    return m_account->transactions();
}

QList<QTransaction*> AccountManager::transactions() const
{
    if (m_tags.isEmpty()) {
        return m_account->transactions();
    } else {
        return m_filteredTransactions;
    }
}

void AccountManager::reloadTransactions()
{
    if (m_account == nullptr) {
        qWarning() << "Trying to reload transactions for null account!";
        return;
    }

    auto transactions = DbDao::sharedInstance()->transactions(m_account->accountId(), m_fromDate, m_toDate);
    m_account->setTransactions(transactions);

    // This function will call both `Q_EMIT transactionsChanged()`
    // and `recalculateAmounts()`
    filterTransactions();
}

void AccountManager::recalculateAmounts()
{
    double expenses = 0.0;
    double credits = 0.0;
    double total = 0.0;

    qDebug() << "Filtering amounts for tags" << m_tags;

    Q_FOREACH (QTransaction *transaction, m_account->transactions()) {
        bool containsTags = m_tags.isEmpty() || transaction->tags().toSet().intersects(m_tags.toSet());
        if (!containsTags) {
            continue;
        }

        double amount = transaction->amount();
        total += amount;

        if (amount < 0) {
            expenses += amount;
        }

        if (amount > 0) {
            credits += amount;
        }
    }

    m_amounts.insert(QAccount::AmountType::Expenses, expenses);
    m_amounts.insert(QAccount::AmountType::Credits, credits);
    m_amounts.insert(QAccount::AmountType::Total, total);

    Q_EMIT amountsChanged(m_amounts);
}

void AccountManager::filterTransactions()
{
    m_filteredTransactions.clear();

    if (!m_tags.isEmpty()) {
        Q_FOREACH (QTransaction *transaction, m_account->transactions()) {
            bool containsTags = transaction->tags().toSet().intersects(m_tags.toSet());
            if (!containsTags) {
                continue;
            }

            m_filteredTransactions.append(transaction);
        }
    }

    Q_EMIT transactionsChanged();
    recalculateAmounts();
}
