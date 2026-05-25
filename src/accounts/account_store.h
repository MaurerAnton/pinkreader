/*
 * PinkReader - GPLv3
 * File: account_store.h - Account persistence store
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>

namespace PinkReader {

class AccountStore : public QObject
{
    Q_OBJECT
public:
    explicit AccountStore(QObject *parent = nullptr);
    ~AccountStore() override;

    bool save(const QJsonArray &accounts, const QString &path = QString());
    QJsonArray load(const QString &path = QString());
    bool remove(const QString &path = QString());
    QString defaultPath() const;
};

} // namespace PinkReader
