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
#include "ui_mainwindow.h"

#include <algorithm>

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QQuickView>
#include <QQmlContext>

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include "ofx/import/ofximporter.h"

#include "itemmodels/tagsitemmodel.h"
#include "itemmodels/transactionsfilterproxymodel.h"
#include "itemmodels/transactionsitemmodel.h"
#include "datamodels/qaccount.h"
#include "datamodels/qtransaction.h"
#include "transactionswindow.h"
#include "accountmanager.h"
#include "database/dbdao.h"

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_tagsModel(new TagsItemModel(this))
{
    m_ui->setupUi(this);
    m_ui->pieChart->setVisible(false);
    m_ui->transactionsView->setVisible(false);
    m_ui->pieChart->setRenderHint(QPainter::Antialiasing);

    connect(m_ui->actionEditTransactions, &QAction::triggered, [=] {
        TransactionsFilterProxyModel *filterProxyModel = new TransactionsFilterProxyModel(this);
        filterProxyModel->setSourceModel(new TransactionsItemModel(this, nullptr));
        filterProxyModel->setDynamicSortFilter(true);

        QQuickView *quickView = new QQuickView();
        quickView->rootContext()->setContextProperty(QStringLiteral("transactionsModel"), filterProxyModel);
        quickView->rootContext()->setContextProperty(QStringLiteral("dbDao"), DbDao::sharedInstance());
        quickView->setSource(QUrl::fromLocalFile("../kasa/src/qml/Transactions.qml"));
        quickView->setResizeMode(QQuickView::SizeRootObjectToView);
        quickView->setModality(Qt::ApplicationModal);
        quickView->show();


//        TransactionsWindow *transactionsWindow = new TransactionsWindow(this);
//        transactionsWindow->exec();

//        m_tagsModel->reloadData();
//        AccountManager::sharedInstance()->reloadTransactions();
    });

    connect(m_ui->actionImportOfxFile, &QAction::triggered, [=] {
        QString filename = QFileDialog::getOpenFileName(this, "Open OFX file", QDir::homePath(), "OFX Files (*.ofx)");

        OFXImporter *importer = new OFXImporter(this, QVariantList());
        bool import = importer->import(filename);

        if (!import) {
            QMessageBox::critical(this, "Couldn't load this file", "The file you have selected could not be loaded");
        } else {
            DbDao::sharedInstance()->storeAccount(importer->account());
            DbDao::sharedInstance()->storeTransactions(importer->account()->transactions());

            AccountManager::sharedInstance()->setAccount(importer->account());
            m_ui->listView->setCurrentIndex(m_tagsModel->index(0, 0));
        }
    });

    connect(AccountManager::sharedInstance(), &AccountManager::amountsChanged, [=](const QHash<QAccount::AmountType, double> &amounts) {
        m_ui->expensesAmountLabel->setText(QString::number(amounts.value(QAccount::AmountType::Expenses)));
        m_ui->creditAmountLabel->setText(QString::number(amounts.value(QAccount::AmountType::Credits)));
        m_ui->totalAmountLabel->setText(QString::number(amounts.value(QAccount::AmountType::Total)));
    });

    connect(AccountManager::sharedInstance(), &AccountManager::transactionsChanged, [=] {
        QHash<QString, double> slices;
        Q_FOREACH (QTransaction *transaction, AccountManager::sharedInstance()->allTransactions()) {
            if (transaction->amount() > 0) {
                continue;
            }

            if (transaction->tags().isEmpty()) {
                double amount = slices.value("Uncategorized", 0);
                amount += transaction->amount();
                slices.insert("Uncategorized", amount);
            } else {
                Q_FOREACH (const QString &tag, transaction->tags()) {
                    double amount = slices.value(tag, 0);
                    amount += transaction->amount();
                    slices.insert(tag, amount);
                }
            }
        }

        QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.35);

        QList<QPair<QString, double>> data;

        QHash<QString, double>::const_iterator i;
        for (i = slices.constBegin(); i != slices.constEnd(); ++i) {
            data.append(qMakePair(i.key(), i.value()));
        }

        std::sort(data.begin(), data.end(), [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
            return a.second < b.second;
        });

        Q_FOREACH (const auto &value, data) {
            series->append(value.first + " (" + QString::number(qAbs(value.second)) + ")", value.second);
        }

//        qreal totalOthersPercentage = 0;
//        QList<QPieSlice*> slicesToRemove;
//        Q_FOREACH (QPieSlice *slice, series->slices()) {
//            if (slice->percentage() < 2) {
//                totalOthersPercentage += slice->percentage();
//            }
//
//            slicesToRemove.append(slice);
//        }
//
//        Q_FOREACH (QPieSlice *slice, slicesToRemove) {
//            series->remove(slice);
//        }
//
//        qDebug() << (series->sum() * totalOthersPercentage) << totalOthersPercentage;
//
//        series->append("Others", series->sum() * totalOthersPercentage);

        series->setLabelsVisible(true);

        m_ui->pieChart->chart()->removeAllSeries();
        m_ui->pieChart->chart()->addSeries(series);
        m_ui->pieChart->chart()->setAnimationOptions(QChart::AllAnimations);
        m_ui->pieChart->chart()->setBackgroundVisible(false);
        m_ui->pieChart->chart()->setTheme(QChart::ChartThemeLight);
        m_ui->pieChart->chart()->legend()->setAlignment(Qt::AlignLeft);

//        QLineSeries *lineSeries = new QLineSeries();
//        QMap<QDate, double> lineAmounts;
//        Q_FOREACH (QTransaction *transaction, AccountManager::sharedInstance()->transactions()) {
//            if (transaction->amount() > 0) {
//                continue;
//            }
//
//            double amount = lineAmounts.value(transaction->datePosted(), 0);
//            amount += transaction->amount();
//            lineAmounts.insert(transaction->datePosted(), amount);
//        }
//
//        QMap<QDate, double>::iterator i;
//        for (i = lineAmounts.begin(); i != lineAmounts.end(); ++i) {
//            lineSeries->append(QDateTime(i.key()).toSecsSinceEpoch(), qAbs(i.value()));
//            qDebug() << QDateTime(i.key()).toSecsSinceEpoch() << "-" << qAbs(i.value());
//        }
//
//        m_ui->barChart->setRenderHint(QPainter::Antialiasing);
//        QChart *chart = new QChart();
//        chart->addSeries(lineSeries);
//
//        QDateTimeAxis *axisX = new QDateTimeAxis;
//        axisX->setTickCount(10);
//        axisX->setFormat("MMM yyyy");
//        axisX->setTitleText("Date");
//        chart->addAxis(axisX, Qt::AlignBottom);
//        lineSeries->attachAxis(axisX);
//
//        QValueAxis *axisY = new QValueAxis;
//        axisY->setLabelFormat("%i");
//        axisY->setTitleText("Sunspots count");
//        chart->addAxis(axisY, Qt::AlignLeft);
//        lineSeries->attachAxis(axisY);
//
//        m_ui->barChart->setChart(chart);


    });

    connect(m_ui->fromDateEdit, &QDateEdit::dateChanged, [=](const QDate &dateChanged) {
        AccountManager::sharedInstance()->setFromDate(dateChanged);
        m_ui->listView->setCurrentIndex(m_tagsModel->index(0, 0));
    });

    connect(m_ui->toDateEdit, &QDateEdit::dateChanged, [=](const QDate &dateChanged) {
        AccountManager::sharedInstance()->setToDate(dateChanged);
        m_ui->listView->setCurrentIndex(m_tagsModel->index(0, 0));
    });

    m_ui->listView->setModel(m_tagsModel);
    m_ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QItemSelectionModel *selectionModel = m_ui->listView->selectionModel();

    connect(selectionModel, &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {
        QStringList tags;
        Q_FOREACH (const QModelIndex &index, selectionModel->selectedIndexes()) {
            // If "All" is selected, just skip everything and pass empty list
            if (index.row() == 0) {
                tags.clear();
                break;
            }

            QString tag = m_tagsModel->data(index, Qt::UserRole).toString();
            tags.append(tag);
        }

        if (tags.isEmpty()) {
            m_ui->pieChart->setVisible(true);
            m_ui->transactionsView->setVisible(false);
        } else {
            m_ui->pieChart->setVisible(false);
            m_ui->transactionsView->setVisible(true);
        }

        AccountManager::sharedInstance()->setTags(tags);
    });
}

void MainWindow::setAccount(QAccount *account)
{
    AccountManager::sharedInstance()->setAccount(account);
    AccountManager::sharedInstance()->setDates(QDate(2018, 8, 1), QDate(2018, 12, 1));

    m_ui->listView->setCurrentIndex(m_tagsModel->index(0, 0));
}

void MainWindow::askForImport()
{
    auto answer = QMessageBox::question(this, "No accounts yet", "There are no transactions yet. Would you like to import a file now?");

    if (answer == QMessageBox::Yes) {
        m_ui->actionImportOfxFile->trigger();
    }
}
