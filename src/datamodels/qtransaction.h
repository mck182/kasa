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

#ifndef QTRANSACTION_H
#define QTRANSACTION_H

#include <QString>
#include <QDate>
#include <QMetaType>


class QAccount;
class QStatement;

class QTransaction
{
public:
    QTransaction(QObject *parent = 0);

    int id() const;
    QString accountId() const;
    QString memo() const;
    QString payeeId() const;
    QString name() const;
    QString bankId() const;
    QString checkNumber() const;
    QDate datePosted() const;
    double amount() const;
    QStringList tags() const;

    void setId(int id);
    void setAccountId(const QString &accountId);
    void setMemo(const QString &memo);
    void setPayeeId(const QString &payeeId);
    void setName(const QString &name);
    void setBankId(const QString &bankId);
    void setCheckNumber(const QString &checkNumber);
    void setDatePosted(const QDate &datePosted);
    void setAmount(double amount);
    void setTags(const QStringList &tags);
    
    QTransaction& operator=(const QTransaction &other);
    
    bool operator==(const QTransaction &other) const {
        return memo() == other.memo()
            && accountId() == other.accountId()
            && payeeId() == other.payeeId()
            && name() == other.name()
            && bankId() == other.bankId()
            && checkNumber() == other.checkNumber()
            && datePosted() == other.datePosted()
            && amount() == other.amount();
    }
    
    operator QString() const {
        return "Name: " + name() + " | Memo: " + memo() + " | Date: " + datePosted().toString() + " | Amount: " + QString::number(amount());
    }

private:
    class QTransactionPrivate * const d;
    
};

Q_DECLARE_METATYPE(QTransaction*)

#endif // QTRANSACTION_H
