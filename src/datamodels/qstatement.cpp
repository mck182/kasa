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

#include "qstatement.h"

#include <QDate>

#include "qtransaction.h"

class QStatementPrivate
{
public:
    QString accountId;
    QDate dateStart;
    QDate dateEnd;
    double availableBalance;
    double ledgerBalance;
    QString currency;
    QList<QTransaction> transactions;
};

QStatement::QStatement(QObject *parent)
    : d(new QStatementPrivate())
{

}

QStatement& QStatement::operator=(const QStatement &other)
{
    *d = *other.d;
    return *this;
}

QString QStatement::accountId() const
{
    return d->accountId;
}

double QStatement::availableBalance() const
{
    return d->availableBalance;
}

double QStatement::ledgerBalance() const
{
    return d->ledgerBalance;
}

QString QStatement::currency() const
{
    return d->currency;
}

QDate QStatement::dateEnd() const
{
    return d->dateEnd;
}

QDate QStatement::dateStart() const
{
    return d->dateStart;
}

QList<QTransaction> QStatement::transactions() const
{
    return d->transactions;    
}

void QStatement::setAccountId(const QString &accountId)
{
    d->accountId = accountId;
}

void QStatement::setAvailableBalance(double availableBalance)
{
    d->availableBalance = availableBalance;
}

void QStatement::setLedgerBalance(double ledgerBalance)
{
    d->ledgerBalance = ledgerBalance;
}

void QStatement::setCurrency(const QString &currency)
{
    d->currency = currency;
}

void QStatement::setDateEnd(const QDate &dateEnd)
{
    d->dateEnd = dateEnd;
}

void QStatement::setDateStart(const QDate &dateStart)
{
    d->dateStart = dateStart;
}

void QStatement::addTransaction(const QTransaction &transaction)
{
    d->transactions.append(transaction);
}

void QStatement::setTransactions(const QList<QTransaction> &transactions)
{
    d->transactions = transactions;
}
