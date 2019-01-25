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

#ifndef KASA_DBDAO_H
#define KASA_DBDAO_H

#include <QObject>
#include <QList>
#include <QDate>

class QAccount;
class QTransaction;
class QStatement;
class QSqlQuery;
class QSqlRecord;

class DbDao : public QObject {
    Q_OBJECT
public:
    QList<QAccount*> accounts() const;
    QList<QTransaction*> transactions(const QString &accountId = QString(), const QDate &from = QDate(), const QDate &to = QDate()) const;
    QStringList tags() const;

    bool storeAccounts(QList<QAccount*> accounts);
    bool storeAccount(QAccount *account);

    bool storeTransactions(QList<QTransaction*> transactions);
    bool storeTransaction(QTransaction *transaction);

    bool updateTransaction(QTransaction *transaction);

    QPair<QDate, QDate> oldestAndNewestTransactionDate(QAccount *account);

    Q_INVOKABLE bool applyTags(QVariantList transactions, const QString &tags);
    Q_INVOKABLE bool removeTag(QTransaction *transaction, const QString &tag);
    bool storeTags(QTransaction *transaction);

    static DbDao* sharedInstance();

private:
//    DbDao();
    QAccount* recordToAccount(const QSqlRecord &record) const;
    QTransaction* recordToTransaction(const QSqlRecord &record) const;
    void bindTransactionToQuery(QTransaction *transaction, QSqlQuery &query);

};


#endif //KASA_DBDAO_H
