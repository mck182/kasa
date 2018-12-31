/*
 * Copyright 2005       Ace Jones acejones@users.sourceforge.net
 * Copyright 2010-2018  Thomas Baumgart tbaumgart@kde.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OFXIMPORTER_H
#define OFXIMPORTER_H

#include <QObject>

#include "datamodels/qaccount.h"

/**
@author Ace Jones
*/

class OFXImporter : QObject
{
  Q_OBJECT

public:
  explicit OFXImporter(QObject *parent, const QVariantList &args);
  ~OFXImporter() override;

  /**
    * Import a file
    *
    * @param filename File to import
    *
    * @return bool Whether the import was successful.
    */
  bool import(const QString &filename);

  QAccount* account() const;
  void setAccount(QAccount *account);

protected:
  static int ofxTransactionCallback(struct OfxTransactionData, void*);
  static int ofxStatementCallback(struct OfxStatementData, void*);
  static int ofxAccountCallback(struct OfxAccountData, void*);
  static int ofxStatusCallback(struct OfxStatusData, void*);
  static int ofxSecurityCallback(struct OfxSecurityData, void*);

private:
  /// \internal d-pointer class.
  class Private;
  /// \internal d-pointer instance.
  Private* const d;
};

#endif
