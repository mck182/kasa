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

#include "datamodels/qtransaction.h"
#include "transactionsfilterproxymodel.h"

TransactionsFilterProxyModel::TransactionsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_filterActive = false;
}

bool TransactionsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    QTransaction *transaction = qvariant_cast<QTransaction*>(index.data(Qt::UserRole));

    if (transaction == nullptr) {
        return false;
    }

    if (!m_filterActive) {
        return true;
    }

    bool matchesTags = false;
    if (!m_tags.isEmpty()) {
        Q_FOREACH (const QString &tag, transaction->tags()) {
            if (m_tags.contains(tag, Qt::CaseInsensitive) || tag.contains(m_tags, Qt::CaseInsensitive)) {
                matchesTags = true;
                break;
            }
        }
    } else {
        matchesTags = true;
    }

    bool matchesFromDate = false;
    if (m_fromDate.isValid()) {
        if (transaction->datePosted() >= m_fromDate) {
            matchesFromDate = true;
        }
    } else {
        matchesFromDate = true;
    }

    bool matchesToDate = false;
    if (m_toDate.isValid()) {
        if (transaction->datePosted() <= m_toDate) {
            matchesToDate = true;
        }
    } else {
        matchesToDate = true;
    }

    return matchesTags && matchesFromDate && matchesToDate;
}

void TransactionsFilterProxyModel::setTags(const QString &tags)
{
    m_tags = tags;
    activateFilter();
}

void TransactionsFilterProxyModel::setFromDate(const QDate &fromDate)
{
    m_fromDate = fromDate;
    activateFilter();
}

void TransactionsFilterProxyModel::setToDate(const QDate &toDate)
{
    m_toDate = toDate;
    activateFilter();
}

void TransactionsFilterProxyModel::activateFilter()
{
    m_filterActive = !m_tags.isEmpty() || m_fromDate.isValid() || m_toDate.isValid();
}
