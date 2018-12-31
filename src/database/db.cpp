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

#include "db.h"

#include <QGlobalStatic>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "datamodels/qaccount.h"
#include "datamodels/qstatement.h"
#include "datamodels/qtransaction.h"

#define CREATE_TABLE_ACCOUNTS "CREATE TABLE Accounts (" \
                              "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, " \
                              "accountId TEXT, " \
                              "accountNumber TEXT, " \
                              "bankId TEXT, " \
                              "branchId TEXT, " \
                              "name TEXT, " \
                              "currency TEXT, " \
                              "accountType TEXT" \
                              ")"

#define CREATE_TABLE_STATEMENTS "CREATE TABLE Statements (" \
                                "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, " \
                                "accountId TEXT, " \
                                "dateStart TEXT, " \
                                "dateEnd TEXT, " \
                                "availableBalance REAL, " \
                                "ledgerBalance REAL, " \
                                "currency TEXT" \
                                ")"

#define CREATE_TABLE_TRANSACTIONS "CREATE TABLE Transactions (" \
                                  "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, " \
                                  "accountId TEXT, " \
                                  "memo TEXT, " \
                                  "payeeId TEXT, " \
                                  "name TEXT," \
                                  "bankId TEXT," \
                                  "checkNumber TEXT," \
                                  "datePosted TEXT," \
                                  "amount REAL" \
                                  ")"

#define CREATE_TABLE_TAGS "CREATE TABLE Tags (" \
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, " \
                          "name TEXT NOT NULL UNIQUE" \
                          ")"

#define CREATE_TABLE_TRANSACTIONS_TAGS "CREATE TABLE TransactionTags (" \
                                       "transactionId INT NOT NULL, " \
                                       "tagId INT NOT NULL, " \
                                       "PRIMARY KEY (transactionId, tagId)" \
                                       ")"

Q_GLOBAL_STATIC(Db, dbSharedInstance)
Db* Db::sharedInstance() {
    return dbSharedInstance;
}

Db::Db()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("kasa_db");
    if (!m_db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          "the Qt SQL driver documentation for information how "
                                          "to build it.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

        return;
    }

    QSqlQuery currentTablesQuery;
    bool querySuccess = currentTablesQuery.exec(QStringLiteral("SELECT * FROM sqlite_master WHERE name = 'Accounts' OR name = 'Statements' OR name = 'Transactions';"));
    if (m_db.tables(QSql::Tables).isEmpty()) {//querySuccess && !currentTablesQuery.isValid()) {
        qDebug() << "Creating new tables..";

        QSqlQuery createTablesQuery;
        bool accountsSuccess = createTablesQuery.exec(QStringLiteral(CREATE_TABLE_ACCOUNTS));
        if (!accountsSuccess) {
            qDebug() << createTablesQuery.lastError();
        }

        bool statementsSuccess = createTablesQuery.exec(QStringLiteral(CREATE_TABLE_STATEMENTS));
        if (!statementsSuccess) {
            qDebug() << createTablesQuery.lastError();
        }

        bool transactionsSuccess = createTablesQuery.exec(QStringLiteral(CREATE_TABLE_TRANSACTIONS));
        if (!transactionsSuccess) {
            qDebug() << createTablesQuery.lastError();
        }

        bool tagsSuccess = createTablesQuery.exec(QStringLiteral(CREATE_TABLE_TAGS));
        if (!tagsSuccess) {
            qDebug() << createTablesQuery.lastError();
        }

        bool transactionTagsSuccess = createTablesQuery.exec(QStringLiteral(CREATE_TABLE_TRANSACTIONS_TAGS));
        if (!transactionTagsSuccess) {
            qDebug() << createTablesQuery.lastError();
        }

        qDebug() << "Accounts table success: " << accountsSuccess;
        qDebug() << "Statements table success: " << statementsSuccess;
        qDebug() << "Transactions table success: " << transactionsSuccess;
        qDebug() << "Tags table success: " << tagsSuccess;
        qDebug() << "TransactionsTags: " << transactionTagsSuccess;
    }
}

bool Db::startTransaction() {
    return m_db.transaction();
}

bool Db::commit() {
    return m_db.commit();
}

bool Db::rollback() {
    return m_db.rollback();
}


