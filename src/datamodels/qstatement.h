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

#ifndef QSTATEMENT_H
#define QSTATEMENT_H

#include <QString>
#include <QDate>
#include <QList>
#include <QMetaType>

#include "qtransaction.h"

class QStatement
{
public:
    QStatement(QObject *parent = 0);
    
    QString accountId() const;
    QDate dateStart() const;
    QDate dateEnd() const;
    double availableBalance() const;
    double ledgerBalance() const;
    QString currency() const;
    QList<QTransaction> transactions() const;
    
    void setAccountId(const QString &accountId);
    void setDateStart(const QDate &dateStart);
    void setDateEnd(const QDate &dateEnd);
    void setAvailableBalance(double availableBalance);
    void setLedgerBalance(double ledgerBalance);
    void setCurrency(const QString &availableBalance);
    void addTransaction(const QTransaction &transaction);
    void setTransactions(const QList<QTransaction> &transactions);

    QStatement& operator=(const QStatement &other);

    bool operator==(const QStatement &other) const {
        return accountId() == other.accountId()
            && dateStart() == other.dateStart()
            && dateEnd() == other.dateEnd()
            && currency() == other.currency()
            && transactions().size() == other.transactions().size();
    };
private:
    class QStatementPrivate * const d;
};

Q_DECLARE_METATYPE(QStatement*)

#endif // QSTATEMENT_H
