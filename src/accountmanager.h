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

#ifndef KASA_ACCOUNTMANAGER_H
#define KASA_ACCOUNTMANAGER_H

#include <QObject>
#include <QDate>

#include "datamodels/qaccount.h"

class AccountManager : public QObject
{
    Q_OBJECT

public:
    AccountManager(QObject *parent = nullptr);
    void setAccount(QAccount *account);
    void setFromDate(const QDate &date);
    void setToDate(const QDate &date);
    void setDates(const QDate &fromDate, const QDate &toDate);
    void setTags(const QStringList &tags);

    QList<QTransaction*> allTransactions() const;
    QList<QTransaction*> transactions() const;

    static AccountManager* sharedInstance();

public Q_SLOTS:
    void reloadTransactions();

Q_SIGNALS:
    void accountChanged(QAccount *account);
    void transactionsChanged();
    void amountsChanged(const QHash<QAccount::AmountType, double> &amounts);

private:
    void filterTransactions();
    void recalculateAmounts();

    QAccount *m_account;
    QDate m_fromDate;
    QDate m_toDate;
    QStringList m_tags;
    QHash<QAccount::AmountType, double> m_amounts;

    QList<QTransaction*> m_filteredTransactions;
};


#endif //KASA_ACCOUNTMANAGER_H
