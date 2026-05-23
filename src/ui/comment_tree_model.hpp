#pragma once

#include "../core/models.hpp"

#include <QAbstractListModel>
#include <QVector>

namespace PinkReader {

class CommentTreeModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        AuthorRole,
        BodyRole,
        BodyHtmlRole,
        ScoreRole,
        DepthRole,
        CreatedRole,
        VoteStateRole,
        SavedRole,
        StickiedRole,
        IsSubmitterRole,
        DistinguishedRole,
        ReplyCountRole,
        CollapsedRole,
        ParentIdRole,
        GildedRole
    };

    explicit CommentTreeModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const { return m_flattened.size(); }

    void setComments(const QVector<Comment>& comments);
    void clear();
    Comment* commentAt(int index);

signals:
    void countChanged();

private:
    void flattenComments(const QVector<Comment>& comments);
    QVector<Comment> m_flattened;
};

}  // namespace PinkReader
