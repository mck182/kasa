/*
 * Copyright 2005       Ace Jones acejones@users.sourceforge.net
 * Copyright 2010-2018  Thomas Baumgart tbaumgart@kde.org
 * Copyright 2018       Martin Klapetek mklapetek@kde.org
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

#include "ofximporter.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QRadioButton>
#include <QScopedPointer>
#include <QSpinBox>
#include <QByteArray>
#include <QDebug>
#include <QUrl>

// Project Includes

#include <libofx/libofx.h>
#include "datamodels/qtransaction.h"
#include "datamodels/qaccount.h"
#include "datamodels/qstatement.h"

class OFXImporter::Private
{
public:
    QAccount *account;
};


OFXImporter::OFXImporter(QObject *parent, const QVariantList &args)
    : d(new Private)
{
  Q_UNUSED(args)
}

OFXImporter::~OFXImporter()
{
  delete d;
}

bool OFXImporter::import(const QString& filename)
{
  QByteArray filename_deep = QFile::encodeName(filename);

  ofx_STATUS_msg = true;
  ofx_INFO_msg  = true;
  ofx_WARNING_msg = true;
  ofx_ERROR_msg = true;

#ifdef DEBUG_LIBOFX
  ofx_PARSER_msg = true;
  ofx_DEBUG_msg = true;
  ofx_DEBUG1_msg = true;
  ofx_DEBUG2_msg = true;
  ofx_DEBUG3_msg = true;
  ofx_DEBUG4_msg = true;
  ofx_DEBUG5_msg = true;
#endif

  LibofxContextPtr ctx = libofx_get_new_context();
  Q_CHECK_PTR(ctx);

  // Don't show the position that caused a message to be shown
  // This has no setter (see libofx.h)
  ofx_show_position = false;

  qDebug("setup callback routines");
  ofx_set_transaction_cb(ctx, ofxTransactionCallback, this);
  ofx_set_statement_cb(ctx, ofxStatementCallback, this);
  ofx_set_account_cb(ctx, ofxAccountCallback, this);
  ofx_set_security_cb(ctx, ofxSecurityCallback, this);
  ofx_set_status_cb(ctx, ofxStatusCallback, this);
  qDebug("process data");
  libofx_proc_file(ctx, filename_deep, AUTODETECT);
  qDebug("process data done");
  libofx_free_context(ctx);

  return true;
}

QAccount* OFXImporter::account() const
{
    return d->account;
}

void OFXImporter::setAccount(QAccount *account)
{
    d->account = account;
}


/* __________________________________________________________________________
 * AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *
 * Static callbacks for LibOFX
 *
 * YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */

int OFXImporter::ofxTransactionCallback(struct OfxTransactionData data, void * pv)
{
    QTransaction *t = new QTransaction();
    QDateTime transactionDate;
    
    if (data.date_posted_valid) {
        transactionDate.setTime_t(data.date_posted); // - pofx->d->m_timestampOffset * 60);
    } else if (data.date_initiated_valid) {
        transactionDate.setTime_t(data.date_initiated);// - pofx->d->m_timestampOffset * 60);
    }
    
    t->setDatePosted(transactionDate.date());
  
    if (data.amount_valid) {
        t->setAmount(data.amount);
    }
    
    if (data.check_number_valid) {
        t->setCheckNumber(QString::fromUtf8(data.check_number));
    }
    
    if (data.fi_id_valid) {
        t->setBankId(QStringLiteral("ID ") + QString::fromUtf8(data.fi_id));
    } else if (data.reference_number_valid) {
        t->setBankId(QStringLiteral("REF ") + QString::fromUtf8(data.reference_number));
    }
    
    if (data.payee_id_valid) {
        t->setPayeeId(QString::fromUtf8(data.payee_id));
    }
    
    if (data.name_valid) {
        t->setName(QString::fromUtf8(data.name));
    }
    
    if (data.memo_valid) {
        t->setMemo(QString::fromUtf8(data.memo));
    }
    
    OFXImporter *pofx = reinterpret_cast<OFXImporter*>(pv);
    t->setAccountId(pofx->account()->accountId());
    pofx->account()->addTransaction(t);
    
    qDebug() << *t;

  return 0;
}

int OFXImporter::ofxStatementCallback(struct OfxStatementData data, void *pv)
{
    QStatement statement;
    
    if (data.currency_valid) {
        statement.setCurrency(QString::fromUtf8(data.currency));
    }
    
    if (data.account_id_valid) {
        statement.setAccountId(QString::fromUtf8(data.account_id));
    }

    if (data.date_start_valid) {
        QDateTime dt;
        dt.setTime_t(data.date_start);// - pofx->d->m_timestampOffset * 60);
        statement.setDateStart(dt.date());
    }

    if (data.date_end_valid) {
        QDateTime dt;
        dt.setTime_t(data.date_end); // - pofx->d->m_timestampOffset * 60);
        statement.setDateEnd(dt.date());
    }
    
    if (data.ledger_balance_valid) {
        statement.setLedgerBalance(data.ledger_balance);
    }

    OFXImporter *pofx = reinterpret_cast<OFXImporter*>(pv);
    pofx->account()->addStatement(statement);
    
    qDebug() << statement.accountId() << statement.dateStart() << statement.dateEnd() << statement.availableBalance() << statement.ledgerBalance();
    
    return 0;
}

int OFXImporter::ofxAccountCallback(struct OfxAccountData data, void *pv)
{
    QAccount *account = new QAccount();
    
    if (data.account_id_valid) {
        account->setName(QString::fromUtf8(data.account_name));
        account->setAccountId(QString::fromUtf8(data.account_id));
    }
    
    if (data.bank_id_valid) {
        account->setBankId(QString::fromUtf8(data.bank_id));
    }
  
    if (data.currency_valid) {
        account->setCurrency(QString::fromUtf8(data.currency));
    }
    
    if (data.account_type_valid) {
        switch (data.account_type) {
            case OfxAccountData::OFX_CHECKING: account->setAccountType("checking"); break;
            case OfxAccountData::OFX_SAVINGS: account->setAccountType("savings"); break;
            case OfxAccountData::OFX_MONEYMRKT: account->setAccountType("investment"); break;
            case OfxAccountData::OFX_CREDITLINE: account->setAccountType("creditline"); break;
            case OfxAccountData::OFX_CMA: account->setAccountType("cma"); break;
            case OfxAccountData::OFX_CREDITCARD: account->setAccountType("creditcard"); break;
            case OfxAccountData::OFX_INVESTMENT: account->setAccountType("investment"); break;
        }
    }
    
    OFXImporter *pofx = reinterpret_cast<OFXImporter*>(pv);
    pofx->setAccount(account);

    return 0;
}

int OFXImporter::ofxSecurityCallback(struct OfxSecurityData data, void *pv)
{
    //TODO
    return 0;
}

int OFXImporter::ofxStatusCallback(struct OfxStatusData data, void *pv)
{
    //TODO
    return 0;
}

#include "ofximporter.moc"
