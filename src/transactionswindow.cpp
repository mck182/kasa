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

#include "transactionswindow.h"

#include "ui_transactions.h"
#include "database/dbdao.h"
#include "itemmodels/transactionsfilterproxymodel.h"
#include "itemmodels/transactionsitemmodel.h"
#include "views/transactionitemdelegate.h"

TransactionsWindow::TransactionsWindow(QWidget *parent, QAccount *account)
    : QDialog(parent),
      m_ui(new Ui::TransactionsWindow)
{
    m_ui->setupUi(this);

    TransactionsFilterProxyModel *filterProxyModel = new TransactionsFilterProxyModel(this);
    filterProxyModel->setSourceModel(new TransactionsItemModel(this, account));
    filterProxyModel->setDynamicSortFilter(true);

    m_ui->listView->setItemDelegate(new TransactionItemDelegate(m_ui->listView));
    m_ui->listView->setModel(filterProxyModel);
    m_ui->listView->setMouseTracking(true);

    QPair<QDate, QDate> oldestAndNewest = DbDao::sharedInstance()->oldestAndNewestTransactionDate(account);
    m_ui->fromDateEdit->setDate(oldestAndNewest.first);
    m_ui->toDateEdit->setDate(oldestAndNewest.second);

    connect(m_ui->tagsEdit, &QLineEdit::textChanged, [=] {
        filterProxyModel->setTags(m_ui->tagsEdit->text());
        filterProxyModel->invalidate();
    });

    connect(m_ui->fromDateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
        filterProxyModel->setFromDate(date);
        filterProxyModel->invalidate();
    });

    connect(m_ui->toDateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
        filterProxyModel->setToDate(date);
        filterProxyModel->invalidate();
    });

}
