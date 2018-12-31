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

#include "qaccount.h"

#include <QString>
#include <QList>
#include <QDebug>

#include "qstatement.h"
#include "qtransaction.h"


class QAccountPrivate
{
public:
    QString accountId;
    QString accountNumber;
    QString bankId;
    QString branchId;
    QString name;
    QString currency;
    QString accountType;

    QList<QStatement> statements;
    QList<QTransaction*> transactions;
};

QAccount::QAccount(QObject *parent)
    : d(new QAccountPrivate())
{

}

QString QAccount::accountId() const
{
    return d->accountId;
}

QString QAccount::accountNumber() const
{
    return d->accountNumber;
}

QString QAccount::accountType() const
{
    return d->accountType;
}

QString QAccount::bankId() const
{
    return d->bankId;
}

QString QAccount::branchId() const
{
    return d->branchId;
}

QString QAccount::currency() const
{
    return d->currency;
}

QString QAccount::name() const
{
    return d->name;
}

QStatement QAccount::statement() const
{
    return d->statements.isEmpty() ? QStatement() : d->statements.first();
}

void QAccount::setAccountId(const QString &accountId)
{
    d->accountId = accountId;
}

void QAccount::setAccountNumber(const QString &accountNumber)
{
    d->accountNumber = accountNumber;
}

void QAccount::setAccountType(const QString &accountType)
{
    d->accountType = accountType;
}

void QAccount::setBankId(const QString &bankId)
{
    d->bankId = bankId;
}

void QAccount::setBranchId(const QString &branchId)
{
    d->branchId = branchId;
}

void QAccount::setCurrency(const QString &currency)
{
    d->currency = currency;
}

void QAccount::setName(const QString &name)
{
    d->name = name;
}

void QAccount::addStatement(const QStatement &statement)
{
    d->statements.append(statement);
}

void QAccount::setStatement(const QStatement &statement)
{
    
}

void QAccount::setTransactions(QList<QTransaction*> transactions)
{
    d->transactions = transactions;
}

void QAccount::addTransaction(QTransaction *transaction)
{
    d->transactions.append(transaction);
}

void QAccount::removeTransaction(QTransaction *transaction)
{
    d->transactions.removeOne(transaction);
}

QList<QTransaction*> QAccount::transactions() const
{
    return d->transactions;
}

QList<QTransaction*> QAccount::transactions(const QDate &from, const QStringList &tags) const
{
    QList<QTransaction*> transactions;
    Q_FOREACH (QTransaction *transaction, d->transactions) {
        if (transaction->datePosted() >= from) {
            transactions.append(transaction);
        }
    }
    
    return transactions;
}
