// Copyright (c) 2011-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_QT_UFOADDRESSVALIDATOR_H
#define UFO_QT_UFOADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class UFOAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit UFOAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** UFO address widget validator, checks for a valid UFO address.
 */
class UFOAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit UFOAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // UFO_QT_UFOADDRESSVALIDATOR_H
