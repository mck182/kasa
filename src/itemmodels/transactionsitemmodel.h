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

#ifndef KASA_TRANSACTIONSITEMMODEL_H
#define KASA_TRANSACTIONSITEMMODEL_H

#include <QAbstractTableModel>

class QTransaction;
class QAccount;

class TransactionsItemModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles {
        TransactionRole = Qt::UserRole,
        DatePostedRole,
        NameRole,
        MemoRole,
        TagsRole,
        AmountRole
    };

    explicit TransactionsItemModel(QObject *parent = nullptr, QAccount *account = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<QTransaction*> m_transactions;
};

#endif //KASA_TRANSACTIONSITEMMODEL_H
