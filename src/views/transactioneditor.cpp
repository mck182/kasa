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

#include "transactioneditor.h"
#include "ui_transactioneditor.h"


TransactionEditor::TransactionEditor(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::TransactionEditor)
{
    m_ui->setupUi(this);
    m_ui->dateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
}

QSize TransactionEditor::sizeHint() const
{
    return QSize(500, 70);
}

void TransactionEditor::setTransaction(QTransaction *transaction)
{
    m_ui->dateEdit->setDate(transaction->datePosted());
    m_ui->nameEdit->setText(transaction->name());
    m_ui->memoEdit->setText(transaction->memo());
    m_ui->amountEdit->setText(QString::number(transaction->amount()));
    m_ui->tagsEdit->setText(transaction->tags().join(", "));

    connect(m_ui->dateEdit, &QDateEdit::dateChanged, [=] (const QDate &date) {
        transaction->setDatePosted(m_ui->dateEdit->date());
    });

    connect(m_ui->nameEdit, &QLineEdit::editingFinished, [=] {
        transaction->setName(m_ui->nameEdit->text());
    });

    connect(m_ui->memoEdit, &QLineEdit::editingFinished, [=] {
        transaction->setMemo(m_ui->memoEdit->text());
    });

    connect(m_ui->amountEdit, &QLineEdit::editingFinished, [=] {
        transaction->setAmount(m_ui->amountEdit->text().toDouble());
    });

    connect(m_ui->tagsEdit, &QLineEdit::editingFinished, [=] {
        transaction->setTags(m_ui->tagsEdit->text().split(", ", QString::SplitBehavior::SkipEmptyParts));
    });
}
