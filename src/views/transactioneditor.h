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

#ifndef KASA_TRANSACTIONEDITOR_H
#define KASA_TRANSACTIONEDITOR_H

#include <QWidget>

#include "datamodels/qtransaction.h"

namespace Ui {
    class TransactionEditor;
}

class TransactionEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionEditor(QWidget *parent = nullptr);
    void setTransaction(QTransaction *transaction);

    QSize sizeHint() const override;

private:
    Ui::TransactionEditor *m_ui;
};

#endif //KASA_TRANSACTIONEDITOR_H
