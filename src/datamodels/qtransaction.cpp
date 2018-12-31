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

#include "qtransaction.h"

class QTransactionPrivate
{
public:
    int id;
    QString accountId;
    QString memo;
    QString payeeId;
    QString name;
    QString bankId;
    QString checkNumber;
    QDate datePosted;
    double amount;
    QStringList tags;
};

QTransaction::QTransaction(QObject *parent)
    : d(new QTransactionPrivate())
{
    
}

int QTransaction::id() const
{
    return d->id;
}

void QTransaction::setId(int id)
{
    d->id = id;
}

QString QTransaction::accountId() const
{
    return d->accountId;
}

void QTransaction::setAccountId(const QString &accountId)
{
    d->accountId = accountId;
}

QString QTransaction::memo() const
{
    return d->memo;
}

void QTransaction::setMemo(const QString &memo)
{
    d->memo = memo;
}

QString QTransaction::bankId() const
{
    return d->bankId;
}

void QTransaction::setBankId(const QString &bankId)
{
    d->bankId = bankId;
}

QString QTransaction::checkNumber() const
{
    return d->checkNumber;
}

void QTransaction::setCheckNumber(const QString &checkNumber)
{
    d->checkNumber = checkNumber;
}

QDate QTransaction::datePosted() const
{
    return d->datePosted;
}

void QTransaction::setDatePosted(const QDate &datePosted)
{
    d->datePosted = datePosted;
}

QString QTransaction::name() const
{
    return d->name;
}

void QTransaction::setName(const QString &name)
{
    d->name = name;
}

QString QTransaction::payeeId() const
{
    return d->payeeId;
}

void QTransaction::setPayeeId(const QString &payeeId)
{
    d->payeeId = payeeId;
}

double QTransaction::amount() const
{
    return d->amount;
}

void QTransaction::setAmount(double amount)
{
    d->amount = amount;
}

QStringList QTransaction::tags() const
{
    return d->tags;
}

void QTransaction::setTags(const QStringList &tags)
{
    d->tags = tags;
}
