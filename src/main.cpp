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

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "database/db.h"
#include "database/dbdao.h"
#include "datamodels/qaccount.h"
#include "datamodels/qtransaction.h"
#include "datamodels/qstatement.h"
#include "ofx/import/ofximporter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<QAccount*>();
    qRegisterMetaType<QStatement*>();
    qRegisterMetaType<QTransaction*>();

    MainWindow w;
    w.show();

    QList<QAccount*> accounts = DbDao::sharedInstance()->accounts();
    Q_FOREACH (QAccount *account, accounts) {
        QList<QTransaction*> transactions = DbDao::sharedInstance()->transactions(account->accountId());
        account->setTransactions(transactions);
    }

    if (accounts.isEmpty()) {
        w.askForImport();
    } else {
        w.setAccount(accounts.at(0));
    }

    return app.exec();
}

