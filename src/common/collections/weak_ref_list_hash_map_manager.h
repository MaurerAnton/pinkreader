/*
 * PinkReader - GPLv3
 * File: collections/weak_ref_list_hash_map_manager.h
 * Port of: org.quantumbadger.redreader.common.collections.WeakReferenceListHashMapManager.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include "weak_ref_list_manager.h"
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <utility>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.WeakReferenceListHashMapManager<K, V>
 *
 * Original: public class WeakReferenceListHashMapManager<K, V> {
 *     private final HashMap<K, WeakReferenceListManager<V>> mData = new HashMap<>();
 *     private byte mCleanupCounter = 0;
 *     public synchronized void add(final K key, final V value);
 *     public synchronized void remove(final K key, final V value);
 *     public synchronized void map(final K key, final WeakReferenceListManager.Operator<V> operator);
 *     public synchronized <A> void map(final K key, final WeakReferenceListManager.ArgOperator<V, A> operator, final A arg);
 *     public synchronized void clean();
 * }
 */
template<typename K, typename V>
class WeakReferenceListHashMapManager {
public:
	// Port of: public synchronized void add(final K key, final V value)
	void add(const K &key, std::shared_ptr<V> value) {
		std::lock_guard<std::mutex> lock(m_mutex);

		// Port of: WeakReferenceListManager<V> list = mData.get(key);
		auto iter = m_data.find(key);

		// Port of: if(list == null) { list = new WeakReferenceListManager<>(); mData.put(key, list); }
		if (iter == m_data.end()) {
			auto list = std::make_shared<WeakReferenceListManager<V>>();
			m_data[key] = list;
			list->add(std::move(value));
		} else {
			iter->second->add(std::move(value));
		}

		// Port of: // Perform cleanup once for each 256 values which are added
		//   if(++mCleanupCounter == 0) { clean(); }
		if (++m_cleanupCounter == 0) {
			clean();
		}
	}

	// Port of: public synchronized void remove(final K key, final V value)
	void remove(const K &key, const V *value) {
		std::lock_guard<std::mutex> lock(m_mutex);

		// Port of: final WeakReferenceListManager<V> list = mData.get(key);
		auto iter = m_data.find(key);

		// Port of: if(list != null) { list.remove(value); }
		if (iter != m_data.end()) {
			iter->second->remove(value);
		}
	}

	// Port of: public synchronized void map(
	//     final K key, final WeakReferenceListManager.Operator<V> operator)
	void map(const K &key, typename WeakReferenceListManager<V>::Operator oper) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.find(key);

		// Port of: if(list != null) { list.map(operator); }
		if (iter != m_data.end()) {
			iter->second->map(std::move(oper));
		}
	}

	// Port of: public synchronized <A> void map(
	//     final K key, final WeakReferenceListManager.ArgOperator<V, A> operator, final A arg)
	template<typename A>
	void map(const K &key,
	         typename WeakReferenceListManager<V>::template ArgOperator<A> oper,
	         const A &arg) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.find(key);

		// Port of: if(list != null) { list.map(operator, arg); }
		if (iter != m_data.end()) {
			iter->second->map(oper, arg);
		}
	}

	// Port of: public synchronized void clean()
	void clean() {
		std::lock_guard<std::mutex> lock(m_mutex);

		// Port of: final Iterator<Map.Entry<K, WeakReferenceListManager<V>>> iterator
		//     = mData.entrySet().iterator();
		auto iter = m_data.begin();

		while (iter != m_data.end()) {
			// Port of: final WeakReferenceListManager<V> list = entry.getValue();
			auto &list = iter->second;
			// Port of: list.clean();
			list->clean();

			// Port of: if(list.isEmpty()) { iterator.remove(); }
			if (list->isEmpty()) {
				iter = m_data.erase(iter);
			} else {
				++iter;
			}
		}
	}

private:
	// Port of: private final HashMap<K, WeakReferenceListManager<V>> mData = new HashMap<>();
	std::unordered_map<K, std::shared_ptr<WeakReferenceListManager<V>>> m_data;

	// Port of: private byte mCleanupCounter = 0
	uint8_t m_cleanupCounter = 0;

	// Port of: synchronized methods
	std::mutex m_mutex;
};

} // namespace PinkReader
