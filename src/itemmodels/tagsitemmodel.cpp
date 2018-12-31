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

#include "database/dbdao.h"
#include "tagsitemmodel.h"

TagsItemModel::TagsItemModel(QObject *parent)
    : QAbstractListModel(parent)
{
    reloadData();
}

int TagsItemModel::rowCount(const QModelIndex &parent) const
{
    return m_tags.count();
}

QVariant TagsItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();

    if (role == Qt::DisplayRole) {
        if (row == 0) {
            return m_tags.at(0);
        } else {
            return "#" + m_tags.at(row);
        }
    } else if (role == Qt::UserRole) {
        return m_tags.at(row);
    }

    return QVariant();
}

void TagsItemModel::reloadData()
{
    beginResetModel();

    m_tags = DbDao::sharedInstance()->tags();
    m_tags.insert(0, "All");

    endResetModel();
}
