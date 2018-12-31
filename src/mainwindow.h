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

#ifndef KASA_H
#define KASA_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QDate>

class QAccount;
class TagsItemModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
    void setAccount(QAccount *account);
    void calculateAmounts(const QStringList &tags);

private:
    Ui::MainWindow *m_ui;
    QAccount *m_account;

    QDate m_fromDate;
    QDate m_toDate;

    TagsItemModel *m_tagsModel;
};

#endif // KASA_H
