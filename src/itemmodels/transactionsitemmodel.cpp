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

#include <QDebug>

#include "database/dbdao.h"
#include "transactionsitemmodel.h"
#include "datamodels/qtransaction.h"
#include "datamodels/qaccount.h"

TransactionsItemModel::TransactionsItemModel(QObject *parent, QAccount *account)
    : QAbstractTableModel(parent)
{
    if (account == nullptr) {
        m_transactions = DbDao::sharedInstance()->transactions();
    } else {
        m_transactions = account->transactions();
    }
}

int TransactionsItemModel::rowCount(const QModelIndex &parent) const
{
    return m_transactions.size();
}

int TransactionsItemModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant TransactionsItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int column = index.column();
    QTransaction *transaction = m_transactions.at(row);

    if (role == Qt::DisplayRole) {
        switch (column) {
            case 0:
                return transaction->datePosted();
            case 1:
                return transaction->name();
            case 2:
                return transaction->memo();
            case 3:
                return transaction->tags().join(", ");
            case 4:
                return transaction->amount();
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(transaction);
    }

    return QVariant();
}

bool TransactionsItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    int row = index.row();
    int column = index.column();

    QTransaction *transaction = m_transactions.at(row);

    switch (column) {
//        case 0:
//            transaction->setDatePosted(value);
        case 1:
            transaction->setName(value.toString());
        case 2:
            transaction->setMemo(value.toString());
        case 3:
            transaction->setTags(value.toString().split(',', QString::SkipEmptyParts));
        case 4:
            transaction->setAmount(value.toDouble());
    }

    Q_EMIT dataChanged(index, index);

    DbDao::sharedInstance()->updateTransaction(transaction);
}

QVariant TransactionsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Vertical) {
        return m_transactions.at(section)->id();
    } else if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return QStringLiteral("Date");
            case 1:
                return QStringLiteral("Name");
            case 2:
                return QStringLiteral("Memo");
            case 3:
                return QStringLiteral("Tags");
            case 4:
                return QStringLiteral("Amount");
        }
    }

    return QVariant();
}

Qt::ItemFlags TransactionsItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
