/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: GroupedRecyclerViewAdapter.java
 * File: grouped_recycler_adapter.h - Multi-group adapter with hidden item support
 */

#pragma once

#include <QObject>
#include <QVector>
#include <QHash>
#include <QAbstractListModel>
#include <atomic>
#include <memory>
#include <functional>
#include <typeindex>
#include <stdexcept>

namespace PinkReader {

/**
 * @brief Abstract item class for GroupedRecyclerViewAdapter.
 *
 * Mirrors: GroupedRecyclerViewAdapter.Item<VH>
 * In the C++/Qt port, ViewHolder is managed by the user.
 * Each item type maps to a delegate/prototype pattern.
 *
 * @tparam ViewHolder The view holder type (Qt widget or QObject)
 */
template<typename ViewHolder>
class GroupedRecyclerViewItem {
public:
    GroupedRecyclerViewItem() : m_uniqueId(sUniqueIdGenerator.fetch_add(1)) {}
    virtual ~GroupedRecyclerViewItem() = default;

    /** @return the class type used for view type identification */
    virtual std::type_index getViewType() const = 0;

    /** Create a new ViewHolder for this item type */
    virtual ViewHolder *onCreateViewHolder(QObject *parent) = 0;

    /** Bind data to an existing ViewHolder */
    virtual void onBindViewHolder(ViewHolder *viewHolder) = 0;

    /** @return true if this item should be hidden (filtered out) */
    virtual bool isHidden() const = 0;

    /** Get the unique ID for RecyclerView stable IDs */
    long uniqueId() const { return m_uniqueId; }

    /** Check and set hidden status (called by adapter) */
    bool currentlyHidden() const { return m_currentlyHidden; }
    void setCurrentlyHidden(bool hidden) { m_currentlyHidden = hidden; }

private:
    static std::atomic<long> sUniqueIdGenerator;
    long m_uniqueId;
    bool m_currentlyHidden = false;
};

// Initialize static counter (see .cpp)
template<typename VH>
std::atomic<long> GroupedRecyclerViewItem<VH>::sUniqueIdGenerator{100000};

/**
 * @brief Port of GroupedRecyclerViewAdapter.java
 *
 * A multi-group adapter that supports hidden items, stable IDs,
 * and view type mapping. Uses a QAbstractListModel base for Qt integration.
 *
 * In the Java original, this extends RecyclerView.Adapter<RecyclerView.ViewHolder>.
 * In Qt, we use QAbstractListModel and provide ViewHolder management.
 */
class GroupedRecyclerViewAdapter : public QAbstractListModel {
    Q_OBJECT

public:
    /**
     * @brief Type-erased item wrapper for the adapter.
     * Stores a GroupedRecyclerViewItem<QObject> pointer with type info.
     */
    class Item {
    public:
        Item() = default;
        virtual ~Item() = default;

        /** Get the class type for view type mapping */
        virtual std::type_index getViewType() const = 0;

        /** Create a ViewHolder (QWidget) */
        virtual QObject *onCreateViewHolder(QObject *parent) = 0;

        /** Bind data to ViewHolder */
        virtual void onBindViewHolder(QObject *viewHolder) = 0;

        /** Check if this item should be hidden */
        virtual bool isHidden() const = 0;

        /** Internal: get unique ID for RecyclerView stable IDs */
        long uniqueId() const { return m_uniqueId; }

        /** Internal: bind to any ViewHolder (type-erased wrapper) */
        void onBindViewHolderInner(QObject *viewHolder) {
            onBindViewHolder(viewHolder);
        }

        /** Internal: currently hidden flag */
        bool currentlyHidden() const { return m_currentlyHidden; }
        void setCurrentlyHidden(bool hidden) { m_currentlyHidden = hidden; }

    private:
        static std::atomic<long> sItemUniqueIdGenerator;
        long m_uniqueId = sItemUniqueIdGenerator.fetch_add(1);
        bool m_currentlyHidden = false;
    };

    // --- Model roles ---
    enum Roles {
        ItemRole = Qt::UserRole + 1,
        GroupIdRole,
        ViewTypeRole,
        UniqueIdRole
    };

    /**
     * @brief Construct with number of groups
     * @param groups Number of item groups
     * @param parent Parent QObject
     */
    explicit GroupedRecyclerViewAdapter(int groups, QObject *parent = nullptr);
    ~GroupedRecyclerViewAdapter() override;

    // --- QAbstractListModel interface ---
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // --- Public API (mirrors Java GroupedRecyclerViewAdapter methods) ---

    /** Get item at visible position */
    Item *getItemAtPosition(int position);

    /** Append item to a group */
    void appendToGroup(int group, Item *item);

    /** Append multiple items to a group */
    void appendToGroup(int group, const QVector<Item *> &items);

    /** Remove all items from a group */
    void removeAllFromGroup(int groupId);

    /** Remove specific item from a group */
    void removeFromGroup(int groupId, Item *item);

    /** Update hidden status of all items (mirrors updateHiddenStatus) */
    void updateHiddenStatus();

    /** Notify that an item's data changed */
    void notifyItemChanged(int groupId, Item *item);

protected:
    // --- Internal position helpers (mirrors Java private methods) ---

    /** Get absolute position of an item in a group */
    int getItemPositionInternal(int groupId, Item *item);

    /** Get absolute position by group index + position within group */
    int getItemPositionInternal(int group, int positionInGroup);

    /** Get item at absolute adapter position */
    Item *getItemInternal(int desiredPosition);

    /** Count unhidden items in a group */
    int getGroupUnhiddenCount(int groupId) const;

private:
    QVector<QVector<Item *>> m_items;
    QHash<std::type_index, int> m_itemViewTypeMap;
    QHash<int, Item *> m_viewTypeItemMap;
};

} // namespace PinkReader
