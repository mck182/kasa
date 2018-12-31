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

#ifndef QACCOUNT_H
#define QACCOUNT_H

#include <QString>
#include <QMetaType>
#include <QDate>
#include <QHash>

class QStatement;
class QTransaction;

class QAccount
{
public:
    enum class AmountType { Expenses, Credits, Total };
    
    QAccount(QObject *parent = 0);
    QAccount(const QAccount &other);
    ~QAccount();
    
    QString accountId() const;
    QString accountNumber() const;
    QString bankId() const;
    QString branchId() const;
    QString name() const;
    QString currency() const;
    QString accountType() const;
    QList<QTransaction*> transactions() const;
    QList<QTransaction*> transactions(const QDate &from, const QStringList &tags = QStringList()) const;
    QStatement statement() const;
    QHash<AmountType, double> amounts(const QDate &from, const QStringList &tags = QStringList()) const;
    
    void setAccountId(const QString &accountId);
    void setAccountNumber(const QString &accountNumber);
    void setBankId(const QString &bankId);
    void setBranchId(const QString &branchId);
    void setName(const QString &name);
    void setCurrency(const QString &currency);
    void setAccountType(const QString &accountType);
    
    void setStatement(const QStatement &statement);
    void addStatement(const QStatement &statement);

    void setTransactions(QList<QTransaction*> transactions);
    void addTransaction(QTransaction *transaction);
    void removeTransaction(QTransaction *transaction);
    
    bool operator==(const QAccount &other) const {
        return accountId()     == other.accountId()
            && accountNumber() == other.accountNumber()
            && currency()      == other.currency()
            && bankId()        == other.bankId()
            && branchId()      == other.branchId()
            && accountType()   == other.accountType();
    }

    bool operator!=(const QAccount &other) const {
        return accountId()     != other.accountId()
            || accountNumber() != other.accountNumber()
            || currency()      != other.currency()
            || bankId()        != other.bankId()
            || branchId()      != other.branchId()
            || accountType()   != other.accountType();
    }
    
    QAccount& operator=(const QAccount &other);
    
    operator QString() const {
        QString s("Account; Name: " + name() + " | Id: " + accountId() + " | Currency: " + currency() + " | Account type: " + accountType());
        return s;
    }

private:
    class QAccountPrivate* const d;
};

inline uint qHash(QAccount::AmountType key, uint seed = 0)
{
    return ::qHash(static_cast<uint>(key), seed);
}

Q_DECLARE_METATYPE(QAccount*)

#endif // QACCOUNT_H
