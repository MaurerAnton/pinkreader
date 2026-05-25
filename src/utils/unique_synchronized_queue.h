/*
 * PinkReader - GPLv3
 * File: unique_synchronized_queue.h
 * Exact port of UniqueSynchronizedQueue.java (44 lines)
 *
 * Ports every field and method exactly.
 * This is a header-only template class.
 */

#pragma once

#include <QSet>
#include <QLinkedList>
#include <QMutex>
#include <QMutexLocker>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.UniqueSynchronizedQueue<E> (Java public class)
 *
 * A thread-safe queue that ensures each element appears at most once.
 * Elements are added only if they are not already present (uniqueness
 * is enforced via a HashSet). Dequeue operations remove elements
 * from both the queue and the set.
 *
 * Every field and method matches the Java original exactly.
 *
 * @tparam E The element type stored in the queue
 */
template<typename E>
class UniqueSynchronizedQueue {
public:
    UniqueSynchronizedQueue() = default;
    ~UniqueSynchronizedQueue() = default;

    // Disallow copy (synchronized state)
    UniqueSynchronizedQueue(const UniqueSynchronizedQueue &) = delete;
    UniqueSynchronizedQueue &operator=(const UniqueSynchronizedQueue &) = delete;

    // ========================================================================
    // Port of: public synchronized void enqueue(final E object) (Java lines 28-32)
    // ========================================================================
    /**
     * @brief Adds an element to the queue if it is not already present.
     *
     * Port of:
     *   if(set.add(object)) {
     *       queue.addLast(object);
     *   }
     *
     * @param object The element to enqueue
     */
    void enqueue(const E &object) {
        QMutexLocker locker(&m_mutex);

        // Port of: if(set.add(object))
        if (!m_set.contains(object)) {
            m_set.insert(object);

            // Port of: queue.addLast(object)
            m_queue.append(object);
        }
    }

    // ========================================================================
    // Port of: public synchronized E dequeue() (Java lines 34-43)
    // ========================================================================
    /**
     * @brief Removes and returns the first element from the queue.
     *
     * Port of:
     *   if(queue.isEmpty()) { return null; }
     *   final E result = queue.removeFirst();
     *   set.remove(result);
     *   return result;
     *
     * @return The dequeued element, or std::nullopt if the queue is empty
     */
    std::optional<E> dequeue() {
        QMutexLocker locker(&m_mutex);

        // Port of: if(queue.isEmpty()) { return null; }
        if (m_queue.isEmpty()) {
            return std::nullopt;
        }

        // Port of: final E result = queue.removeFirst();
        const E result = m_queue.takeFirst();

        // Port of: set.remove(result);
        m_set.remove(result);

        return result;
    }

    // ========================================================================
    // Additional convenience methods (not in original Java, but useful in C++)
    // ========================================================================

    /** @return true if the queue is empty */
    bool isEmpty() const {
        QMutexLocker locker(&m_mutex);
        return m_queue.isEmpty();
    }

    /** @return the number of elements in the queue */
    int size() const {
        QMutexLocker locker(&m_mutex);
        return m_queue.size();
    }

private:
    // ========================================================================
    // Port of: private final HashSet<E> set = new HashSet<>(); (Java line 25)
    // ========================================================================
    /** @brief Set for O(1) uniqueness checks */
    QSet<E> m_set;

    // ========================================================================
    // Port of: private final LinkedList<E> queue = new LinkedList<>(); (Java line 26)
    // ========================================================================
    /** @brief Queue for FIFO ordering */
    QLinkedList<E> m_queue;

    // ========================================================================
    // Synchronization — port of Java's `synchronized` keyword
    // ========================================================================
    /** @brief Mutex for thread safety (Java uses `synchronized` on methods) */
    mutable QMutex m_mutex;
};

} // namespace PinkReader
