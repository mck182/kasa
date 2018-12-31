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

#include "transactionitemdelegate.h"

#include <QPainter>
#include <QDebug>
#include <QStylePainter>
#include <QCommonStyle>
#include <QApplication>
#include <QLayout>

#include "database/dbdao.h"
#include "datamodels/qtransaction.h"
#include "views/transactioneditor.h"

TransactionItemDelegate::TransactionItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

void TransactionItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTransaction *transaction = qvariant_cast<QTransaction*>(index.data(Qt::UserRole));

    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect baseRect = option.rect.adjusted(8, 8, 8, 8);

    QRect dateRect = baseRect;
    dateRect.setRight(dateRect.left() + 110);

    QRect nameRect = baseRect;
    nameRect.setLeft(dateRect.right());

    QRect amountRect = baseRect;
    amountRect.setLeft(amountRect.right() - 72);

    QRect tagsRect = baseRect;
    tagsRect.setTop(baseRect.height() / 2 + 4);
    tagsRect.setLeft(nameRect.left());

    painter->save();
    painter->setBrush(option.palette.foreground());
    painter->setFont(option.font);
    painter->drawText(dateRect, transaction->datePosted().toString(Qt::DefaultLocaleShortDate));
    painter->drawText(nameRect, transaction->name());
    painter->drawText(amountRect, QString::number(transaction->amount()));
    painter->drawText(tagsRect, transaction->tags().join(", "));

    painter->restore();
}

QSize TransactionItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //FIXME this doesn't work
//    if (index == m_currentEditorIndex) {
//        return QSize(option.rect.width(), 70);
//    }

    return QSize(option.rect.width(), 50);
}

QWidget *TransactionItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    auto *transaction = qvariant_cast<QTransaction*>(index.data(Qt::UserRole));

    auto *editor = new TransactionEditor(parent);
    editor->setTransaction(transaction);

    const_cast<TransactionItemDelegate*>(this)->m_currentEditorIndex = index;
    connect(editor, &QObject::destroyed, [=] {
        const_cast<TransactionItemDelegate*>(this)->m_currentEditorIndex = QModelIndex();
    });

    return editor;
}

void TransactionItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    QRect r = option.rect;
    r.setHeight(editor->sizeHint().height());
    editor->setGeometry(r);
}

void TransactionItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//    QStyledItemDelegate::setEditorData(editor, index);
}

void TransactionItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    TransactionEditor *transactionEditor = qobject_cast<TransactionEditor*>(editor);
    QTransaction *transaction = qvariant_cast<QTransaction*>(index.data(Qt::UserRole));

    DbDao::sharedInstance()->updateTransaction(transaction);
    model->dataChanged(index, index, QVector<int>{Qt::UserRole, Qt::DisplayRole});
}
