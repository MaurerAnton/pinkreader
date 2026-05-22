#include "comment_tree_model.hpp"

namespace PinkReader {

CommentTreeModel::CommentTreeModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int CommentTreeModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_flattened.size();
}

QVariant CommentTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_flattened.size()) return {};
    
    const auto& c = m_flattened[index.row()];
    
    switch (role) {
    case IdRole:           return c.id;
    case AuthorRole:        return c.author;
    case BodyRole:          return c.body;
    case BodyHtmlRole:      return c.bodyHtml;
    case ScoreRole:         return c.score;
    case DepthRole:         return c.depth;
    case CreatedRole:       return c.createdUtc.toString(Qt::ISODate);
    case VoteStateRole:     return static_cast<int>(c.voteState);
    case SavedRole:         return c.saved;
    case StickiedRole:      return c.stickied;
    case IsSubmitterRole:   return c.isSubmitter;
    case DistinguishedRole: return c.distinguished;
    case ReplyCountRole:    return c.replies.size();
    case CollapsedRole:     return c.collapsed;
    case ParentIdRole:      return c.parentId;
    case GildedRole:        return c.gildedCount;
    default:                return {};
    }
}

QHash<int, QByteArray> CommentTreeModel::roleNames() const {
    return {
        {IdRole, "commentId"},
        {AuthorRole, "author"},
        {BodyRole, "body"},
        {BodyHtmlRole, "bodyHtml"},
        {ScoreRole, "score"},
        {DepthRole, "depth"},
        {CreatedRole, "created"},
        {VoteStateRole, "voteState"},
        {SavedRole, "saved"},
        {StickiedRole, "stickied"},
        {IsSubmitterRole, "isSubmitter"},
        {DistinguishedRole, "distinguished"},
        {ReplyCountRole, "replyCount"},
        {CollapsedRole, "collapsed"},
        {ParentIdRole, "parentId"},
        {GildedRole, "gilded"}
    };
}

void CommentTreeModel::setComments(const QVector<Comment>& comments) {
    beginResetModel();
    m_flattened.clear();
    flattenComments(comments);
    endResetModel();
    emit countChanged();
}

void CommentTreeModel::flattenComments(const QVector<Comment>& comments) {
    for (const auto& c : comments) {
        m_flattened.append(c);
        if (!c.replies.isEmpty() && !c.collapsed) {
            flattenComments(c.replies);
        }
    }
}

void CommentTreeModel::clear() {
    beginResetModel();
    m_flattened.clear();
    endResetModel();
    emit countChanged();
}

Comment* CommentTreeModel::commentAt(int index) {
    if (index >= 0 && index < m_flattened.size()) return &m_flattened[index];
    return nullptr;
}

} // namespace PinkReader
