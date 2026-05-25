/*
 * PinkReader - GPLv3
 * File: account_store.cpp - Account store implementation
 */

#include "accounts/account_store.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QStandardPaths>

namespace PinkReader {

AccountStore::AccountStore(QObject *parent) : QObject(parent) {}
AccountStore::~AccountStore() = default;

bool AccountStore::save(const QJsonArray &accounts, const QString &path)
{
    QString p = path.isEmpty() ? defaultPath() : path;
    QDir().mkpath(QFileInfo(p).absolutePath());
    QFile file(p);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QJsonDocument doc(accounts);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QJsonArray AccountStore::load(const QString &path)
{
    QString p = path.isEmpty() ? defaultPath() : path;
    QFile file(p);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return QJsonArray();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    file.close();
    if (err.error != QJsonParseError::NoError) return QJsonArray();
    return doc.array();
}

bool AccountStore::remove(const QString &path)
{
    QString p = path.isEmpty() ? defaultPath() : path;
    return QFile::remove(p);
}

QString AccountStore::defaultPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/accounts_store.json";
}

} // namespace PinkReader
