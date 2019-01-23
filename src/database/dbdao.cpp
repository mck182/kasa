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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMutableStringListIterator>
#include <QDebug>

#include "dbdao.h"
#include "db.h"
#include "datamodels/qaccount.h"
#include "datamodels/qtransaction.h"

Q_GLOBAL_STATIC(DbDao, dbDaoSharedInstance)
DbDao* DbDao::sharedInstance()
{
    // This is just for safety, to ensure that a Db connection
    // is established first
    Db::sharedInstance();
    return dbDaoSharedInstance;
}

QList<QAccount*> DbDao::accounts() const
{
    QList<QAccount*> list;

    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare("SELECT * FROM Accounts");
    if (q.exec()) {
        while (q.next()) {
            list.append(recordToAccount(q.record()));
        }
    }

    return list;
}

QList<QTransaction *> DbDao::transactions(const QString &accountId, const QDate &from, const QDate &to) const
{
    QList<QTransaction*> list;

    QString query = QStringLiteral(
                        "SELECT Transactions.*, GROUP_CONCAT(Tags.name) FROM Transactions "
                        "LEFT JOIN TransactionTags ON Transactions.id = TransactionTags.transactionId "
                        "LEFT JOIN Tags ON TransactionTags.tagId = Tags.id ");

    query.append(accountId.isEmpty() ? "" : QStringLiteral("WHERE accountId = :accountId "));

    if (from.isValid()) {
        query.append(accountId.isEmpty() ? "WHERE " : "AND ");
        query.append("datePosted >= :dateFrom ");
    }

    if (to.isValid()) {
        query.append(accountId.isEmpty() && !from.isValid() ? "WHERE " : "AND ");
        query.append("datePosted <= :dateTo ");
    }

    query.append(QStringLiteral("GROUP BY Transactions.id"));

    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(query);
    q.bindValue(":accountId", accountId);
    q.bindValue(":dateFrom", from.toString(Qt::ISODate));
    q.bindValue(":dateTo", to.toString(Qt::ISODate));

    if (q.exec()) {
        while (q.next()) {
            list.append(recordToTransaction(q.record()));
        }
    }

    return list;
}

QStringList DbDao::tags() const
{
    QStringList tags;
    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare("SELECT name FROM Tags ORDER BY name");

    if (q.exec()) {
        while (q.next()) {
            tags.append(q.record().value(0).toString());
        }
    }

    return tags;
}

bool DbDao::storeAccounts(QList<QAccount*> accounts)
{
    Db::sharedInstance()->startTransaction();

    Q_FOREACH (QAccount *account, accounts) {
        if (!storeAccount(account)) {
            Db::sharedInstance()->rollback();
            return false;
        }
    }

    Db::sharedInstance()->commit();
    return true;
}

bool DbDao::storeAccount(QAccount *account)
{
    if (account == nullptr) {
        return false;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO Accounts VALUES (NULL, :accountId, :accountNumber, :bankId, :branchId, :name, :currency, :accountType)");
    q.bindValue(":accountId", account->accountId());
    q.bindValue(":accountNumber", account->accountNumber());
    q.bindValue(":bankId", account->bankId());
    q.bindValue(":branchId", account->branchId());
    q.bindValue(":name", account->name());
    q.bindValue(":currency", account->currency());
    q.bindValue(":accountType", account->accountType());

    return q.exec();
}

bool DbDao::storeTransactions(QList<QTransaction*> transactions)
{
    Db::sharedInstance()->startTransaction();

    Q_FOREACH (QTransaction *transaction, transactions) {
        if (!storeTransaction(transaction)) {
            Db::sharedInstance()->rollback();
            return false;
        }
    }

    Db::sharedInstance()->commit();
    return true;
}

bool DbDao::storeTransaction(QTransaction *transaction)
{
    if (transaction == nullptr) {
        return false;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO Transactions VALUES (NULL, :accountId, :memo, :payeeId, :name, :bankId, :checkNumber, :datePosted, :amount)");
    bindTransactionToQuery(transaction, q);

    bool transactionSuccess = q.exec();
    int transactionId = q.lastInsertId().toInt();

    qDebug() << "Storing transaction" << (*transaction) << q.executedQuery();
    qWarning() << "Error?" << q.lastError().isValid();

    QList<int> tagIds;
    qDebug() << "Storing tags" << transaction->tags();
    Q_FOREACH (const QString &tag, transaction->tags()) {
        QSqlQuery q;
        q.prepare("INSERT OR IGNORE INTO Tags VALUES (NULL, :name)");
        q.bindValue(":name", tag.simplified());
        q.exec();

        if (q.lastInsertId().isValid()) {
            qDebug() << "Adding tag" << tag << q.lastInsertId() << "for transaction" << transaction->name();
            tagIds.append(q.lastInsertId().toInt());
        } else {
            qDebug() << q.lastError();
            qWarning() << "Last insert id for tag" << tag << "is invalid!";
        }
    }

    Q_FOREACH (int tagId, tagIds) {
        QSqlQuery q;
        q.prepare("INSERT INTO TransactionTags VALUES (:transactionId, :tagId)");
        q.bindValue(":transactionId", transactionId);
        q.bindValue(":tagId", tagId);
        if (!q.exec()) {
            qDebug() << q.lastError();
        } else {
            qDebug() << "Inserted tag id" << tagId << "for transaction" << transactionId << " " << transaction->name();
        }
    }

    return transactionSuccess;
}

bool DbDao::updateTransaction(QTransaction *transaction)
{
    if (!transaction->tags().isEmpty()) {
        Db::sharedInstance()->startTransaction();
        storeTags(transaction);
        Db::sharedInstance()->commit();
    }

    QSqlQuery q;
    q.prepare("UPDATE Transactions SET memo = :memo,"
              "payeeId = :payeeId,"
              "name = :name,"
              "bankId = :bankId,"
              "checkNumber = :checkNumber,"
              "datePosted = :datePosted,"
              "amount = :amount"
              "WHERE id = :id");

    bindTransactionToQuery(transaction, q);
    return q.exec();
}

QPair<QDate, QDate> DbDao::oldestAndNewestTransactionDate(QAccount *account)
{
    QSqlQuery q;
    //FIXME
    q.prepare("SELECT MIN(datePosted), MAX(datePosted) FROM Transactions");// WHERE accountId = :accountId");
//    q.bindValue(":accountId", account->accountId());

    if (!q.exec()) {
        qWarning() << "Oldest/newest transaction dates query failed!" << q.lastError().text();
        return qMakePair(QDate(), QDate());
    }

    while (q.next()) {
        return qMakePair(q.record().value(0).toDate(), q.record().value(1).toDate());
    }

}

bool DbDao::applyTags(QVariantList transactions, const QString &tags)
{
    QStringList tagsList = tags.split(",");
    QMutableStringListIterator i(tagsList);
    while (i.hasNext()) {
        i.setValue(i.next().trimmed());
    }

    qDebug() << "Tags to apply:" << tagsList;

    Db::sharedInstance()->startTransaction();
    bool storeTagsSuccess = true;

    Q_FOREACH (const QVariant &transactionVariant, transactions) {
        QTransaction *transaction = transactionVariant.value<QTransaction*>();
        transaction->setTags(tagsList);
        qDebug() << "Transaction has tags" << transaction->tags() << "from" << tagsList;
        if (!storeTags(transaction)) {
            storeTagsSuccess = false;
            break;
        }
    }

    if (storeTagsSuccess) {
        Db::sharedInstance()->commit();
    } else {
        Db::sharedInstance()->rollback();
    }
}

bool DbDao::storeTags(QTransaction *transaction)
{
    Q_FOREACH(const QString &tag, transaction->tags()) {
        qDebug() << "Processing tag" << tag;
        if (tag.isEmpty()) {
            qDebug() << "Empty, continuing..";
            continue;
        }

        //TODO: explore single-query multi-insert
        QSqlQuery q;
        q.prepare("INSERT OR IGNORE INTO Tags VALUES (NULL, :name)");
        q.bindValue(":name", tag);
        if (!q.exec()) {
            return false;
        }

        int tagId;

        //FIXME: This should work but it doesn't
//            qDebug() << "---> Last insert is valid:" << q.lastInsertId().isValid() << "is null:" << q.lastInsertId().isNull();
//            if (!q.lastInsertId().isValid()) {
            QSqlQuery tagQuery;
            tagQuery.prepare("SELECT id FROM Tags WHERE name = :name");
            tagQuery.bindValue(":name", tag);
            tagQuery.exec();

            if (tagQuery.isActive()) {
                tagQuery.next();
                tagId = tagQuery.value(0).toInt();
            }
//            } else {
//                qDebug() << "---> Got valid last insert id:" << q.lastInsertId().toInt();
//                tagId = q.lastInsertId().toInt();
//            }

            qDebug() << "Adding tag" << tag << "with id" << tagId << "to transaction" << transaction;

        QSqlQuery q2;
        q2.prepare("INSERT INTO TransactionTags VALUES (:transactionId, :tagId)");
        q2.bindValue(":transactionId", transaction->id());
        q2.bindValue(":tagId", tagId);
        if (!q2.exec()) {
            return false;
        }
    }
}

QAccount* DbDao::recordToAccount(const QSqlRecord &record) const
{
    auto *account = new QAccount();
    int pos = 1;
    account->setAccountId(record.value(pos++).toString());
    account->setAccountNumber(record.value(pos++).toString());
    account->setBankId(record.value(pos++).toString());
    account->setBranchId(record.value(pos++).toString());
    account->setName(record.value(pos++).toString());
    account->setCurrency(record.value(pos++).toString());
    account->setAccountType(record.value(pos++).toString());

    return account;
}

QTransaction* DbDao::recordToTransaction(const QSqlRecord &record) const
{
    auto *transaction = new QTransaction();
    int pos = 0;
    transaction->setId(record.value(pos++).toInt());
    transaction->setAccountId(record.value(pos++).toString());
    transaction->setMemo(record.value(pos++).toString());
    transaction->setPayeeId(record.value(pos++).toString());
    transaction->setName(record.value(pos++).toString());
    transaction->setBankId(record.value(pos++).toString());
    transaction->setCheckNumber(record.value(pos++).toString());
    transaction->setDatePosted(record.value(pos++).toDate());
    transaction->setAmount(record.value(pos++).toDouble());

    QString tags = record.value(pos++).toString();
    if (!tags.isEmpty()) {
        transaction->setTags(tags.split(','));
    }

    return transaction;
}

void DbDao::bindTransactionToQuery(QTransaction *transaction, QSqlQuery &query)
{
    query.bindValue(":accountId", transaction->accountId());
    query.bindValue(":memo", transaction->memo());
    query.bindValue(":payeeId", transaction->payeeId());
    query.bindValue(":name", transaction->name());
    query.bindValue(":bankId", transaction->bankId());
    query.bindValue(":checkNumber", transaction->checkNumber());
    query.bindValue(":datePosted", transaction->datePosted());
    query.bindValue(":amount", transaction->amount());
}
