/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: GroupedRecyclerViewAdapter.java
 * File: grouped_recycler_adapter.cpp - Multi-group adapter implementation
 */

#include "grouped_recycler_adapter.h"

#include <stdexcept>
#include <algorithm>
#include <QDebug>

namespace PinkReader {

// --- Static Item unique ID generator ---
std::atomic<long> GroupedRecyclerViewAdapter::Item::sItemUniqueIdGenerator{100000};

// --- Constructor ---
GroupedRecyclerViewAdapter::GroupedRecyclerViewAdapter(int groups, QObject *parent)
    : QAbstractListModel(parent)
{
    // mItems = (ArrayList<Item<?>>[])new ArrayList[groups];
    m_items.resize(groups);
    for (int i = 0; i < groups; i++) {
        m_items[i] = QVector<Item *>();
    }
    // setHasStableIds(true) — in Qt, we support stable IDs via role
}

GroupedRecyclerViewAdapter::~GroupedRecyclerViewAdapter() = default;

// --- rowCount ---
int GroupedRecyclerViewAdapter::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    // Port of: getItemCount()
    int count = 0;
    for (int i = 0; i < m_items.size(); i++) {
        count += getGroupUnhiddenCount(i);
    }
    return count;
}

// --- data ---
QVariant GroupedRecyclerViewAdapter::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0) return QVariant();

    const int row = index.row();
    const int totalRows = rowCount();

    if (row >= totalRows) return QVariant();

    // Find the item at this visible position
    // (const_cast needed since getItemInternal is not const)
    Item *item = const_cast<GroupedRecyclerViewAdapter *>(this)->getItemInternal(row);
    if (!item) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case ItemRole:
        return QVariant::fromValue(reinterpret_cast<quintptr>(item));
    case UniqueIdRole:
        return QVariant::fromValue(item->uniqueId());
    case ViewTypeRole:
        return QVariant::fromValue(reinterpret_cast<quintptr>(&item->getViewType()));
    default:
        return QVariant();
    }
}

// --- roleNames ---
QHash<int, QByteArray> GroupedRecyclerViewAdapter::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ItemRole] = "item";
    roles[GroupIdRole] = "groupId";
    roles[ViewTypeRole] = "viewType";
    roles[UniqueIdRole] = "uniqueId";
    return roles;
}

// --- getItemAtPosition ---
GroupedRecyclerViewAdapter::Item *GroupedRecyclerViewAdapter::getItemAtPosition(int position) {
    return getItemInternal(position);
}

// --- getItemPositionInternal (by object) ---
// Port of: private int getItemPositionInternal(int groupId, Item<?> item)
int GroupedRecyclerViewAdapter::getItemPositionInternal(int groupId, Item *item) {
    const QVector<Item *> &group = m_items[groupId];

    for (int i = 0; i < group.size(); i++) {
        if (group[i] == item) {
            return getItemPositionInternal(groupId, i);
        }
    }

    throw std::runtime_error("Item not found");
}

// --- getItemPositionInternal (by index) ---
// Port of: private int getItemPositionInternal(int group, int positionInGroup)
// "positionInGroup" should include both hidden and visible items
int GroupedRecyclerViewAdapter::getItemPositionInternal(int group, int positionInGroup) {
    int result = 0;

    for (int i = 0; i < group; i++) {
        result += getGroupUnhiddenCount(i);
    }

    for (int i = 0; i < positionInGroup; i++) {
        if (!m_items[group][i]->currentlyHidden()) {
            result++;
        }
    }

    return result;
}

// --- getItemInternal ---
// Port of: private Item<?> getItemInternal(int desiredPosition)
GroupedRecyclerViewAdapter::Item *GroupedRecyclerViewAdapter::getItemInternal(int desiredPosition) {
    if (desiredPosition < 0) {
        throw std::runtime_error("Item desiredPosition " + std::to_string(desiredPosition) + " is too low");
    }

    int currentPosition = 0;

    for (int groupId = 0; groupId < m_items.size(); groupId++) {
        const QVector<Item *> &group = m_items[groupId];

        for (int positionInGroup = 0; positionInGroup < group.size(); positionInGroup++) {
            Item *item = group[positionInGroup];

            if (!item->currentlyHidden()) {
                if (currentPosition == desiredPosition) {
                    return item;
                }
                currentPosition++;
            }
        }
    }

    throw std::runtime_error("Item desiredPosition " + std::to_string(desiredPosition) + " is too high");
}

// --- getGroupUnhiddenCount ---
// Port of: private int getGroupUnhiddenCount(int groupId)
int GroupedRecyclerViewAdapter::getGroupUnhiddenCount(int groupId) const {
    const QVector<Item *> &group = m_items[groupId];
    int result = 0;

    for (int i = 0; i < group.size(); i++) {
        if (!group[i]->currentlyHidden()) {
            result++;
        }
    }

    return result;
}

// --- appendToGroup (single item) ---
// Port of: public void appendToGroup(int group, Item<?> item)
void GroupedRecyclerViewAdapter::appendToGroup(int group, Item *item) {
    // final int position = getItemPositionInternal(group + 1, 0);
    const int position = getItemPositionInternal(group + 1, 0);

    // mItems[group].add(item);
    m_items[group].append(item);

    // if(!item.mCurrentlyHidden) notifyItemInserted(position);
    if (!item->currentlyHidden()) {
        beginInsertRows(QModelIndex(), position, position);
        endInsertRows();
    }
}

// --- appendToGroup (multiple items) ---
// Port of: public void appendToGroup(int group, Collection<Item<?>> items)
void GroupedRecyclerViewAdapter::appendToGroup(int group, const QVector<Item *> &items) {
    // final int position = getItemPositionInternal(group + 1, 0);
    const int position = getItemPositionInternal(group + 1, 0);

    // mItems[group].addAll(items);
    m_items[group].append(items);

    // for(Item<?> item : items) item.mCurrentlyHidden = false;
    for (auto *item : items) {
        item->setCurrentlyHidden(false);
    }

    // notifyItemRangeInserted(position, items.size());
    beginInsertRows(QModelIndex(), position, position + items.size() - 1);
    endInsertRows();
}

// --- removeAllFromGroup ---
// Port of: public void removeAllFromGroup(int groupId)
void GroupedRecyclerViewAdapter::removeAllFromGroup(int groupId) {
    QVector<Item *> &group = m_items[groupId];

    for (int i = group.size() - 1; i >= 0; i--) {
        Item *item = group[i];
        const int position = getItemPositionInternal(groupId, i);

        group.removeAt(i);

        if (!item->currentlyHidden()) {
            beginRemoveRows(QModelIndex(), position, position);
            endRemoveRows();
        }
    }
}

// --- removeFromGroup ---
// Port of: public void removeFromGroup(int groupId, Item<?> item)
void GroupedRecyclerViewAdapter::removeFromGroup(int groupId, Item *item) {
    QVector<Item *> &group = m_items[groupId];

    for (int i = 0; i < group.size(); i++) {
        if (group[i] == item) {
            const int position = getItemPositionInternal(groupId, i);

            group.removeAt(i);

            if (!item->currentlyHidden()) {
                beginRemoveRows(QModelIndex(), position, position);
                endRemoveRows();
            }

            return;
        }
    }

    throw std::runtime_error("Item not found");
}

// --- updateHiddenStatus ---
// Port of: public void updateHiddenStatus()
void GroupedRecyclerViewAdapter::updateHiddenStatus() {
    int position = 0;

    for (int groupId = 0; groupId < m_items.size(); groupId++) {
        QVector<Item *> &group = m_items[groupId];

        for (int positionInGroup = 0; positionInGroup < group.size(); positionInGroup++) {
            Item *item = group[positionInGroup];

            const bool wasHidden = item->currentlyHidden();
            const bool isHidden = item->isHidden();
            item->setCurrentlyHidden(isHidden);

            if (isHidden && !wasHidden) {
                // notifyItemRemoved(position);
                beginRemoveRows(QModelIndex(), position, position);
                endRemoveRows();

            } else if (!isHidden && wasHidden) {
                // notifyItemInserted(position);
                beginInsertRows(QModelIndex(), position, position);
                endInsertRows();
            }

            if (!isHidden) {
                position++;
            }
        }
    }
}

// --- notifyItemChanged ---
// Port of: public void notifyItemChanged(int groupId, Item<?> item)
void GroupedRecyclerViewAdapter::notifyItemChanged(int groupId, Item *item) {
    const int position = getItemPositionInternal(groupId, item);
    const QModelIndex idx = index(position);
    emit dataChanged(idx, idx);
}

} // namespace PinkReader
