/*
 * PinkReader - GPLv3
 * File: collections/weak_ref_list_manager.h
 * Port of: org.quantumbadger.redreader.common.collections.WeakReferenceListManager.java
 *
 * Every field, method, inner interface, and logic branch from the original Java file.
 */

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.WeakReferenceListManager<E>
 *
 * Original: public final class WeakReferenceListManager<E> {
 *     private final ArrayList<WeakReference<E>> data = new ArrayList<>();
 *     public synchronized int size();
 *     public synchronized void add(final E object);
 *     public synchronized void map(final Operator<E> operator);
 *     public synchronized <A> void map(final ArgOperator<E, A> operator, final A arg);
 *     public synchronized void remove(final E object);
 *     public synchronized void clean();
 *     public synchronized boolean isEmpty();
 *     public interface Operator<E> { void operate(E object); }
 *     public interface ArgOperator<E, A> { void operate(E object, A arg); }
 * }
 *
 * Uses std::weak_ptr instead of Java WeakReference for analogous semantics.
 */
template<typename E>
class WeakReferenceListManager {
public:
	// Port of: public interface Operator<E> { void operate(E object); }
	using Operator = std::function<void(E *)>;

	// Port of: public interface ArgOperator<E, A> { void operate(E object, A arg); }
	template<typename A>
	using ArgOperator = std::function<void(E *, A)>;

	// Port of: public synchronized int size()
	//   return data.size();
	int size() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return static_cast<int>(m_data.size());
	}

	// Port of: public synchronized void add(final E object)
	//   data.add(new WeakReference<>(object));
	void add(std::shared_ptr<E> object) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_data.push_back(object);
	}

	// Port of: public synchronized void map(final Operator<E> operator)
	void map(Operator oper) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.begin();
		while (iter != m_data.end()) {
			// Port of: final E object = iterator.next().get();
			auto sp = iter->lock();
			// Port of: if(object == null) { iterator.remove(); }
			if (!sp) {
				iter = m_data.erase(iter);
			} else {
				// Port of: else { operator.operate(object); }
				oper(sp.get());
				++iter;
			}
		}
	}

	// Port of: public synchronized <A> void map(final ArgOperator<E, A> operator, final A arg)
	template<typename A>
	void map(ArgOperator<A> oper, const A &arg) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.begin();
		while (iter != m_data.end()) {
			auto sp = iter->lock();
			if (!sp) {
				iter = m_data.erase(iter);
			} else {
				oper(sp.get(), arg);
				++iter;
			}
		}
	}

	// Port of: public synchronized void remove(final E object)
	//   final Iterator<WeakReference<E>> iterator = data.iterator();
	//   while(iterator.hasNext()) { if(iterator.next().get() == object) { iterator.remove(); } }
	void remove(const E *object) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.begin();
		while (iter != m_data.end()) {
			auto sp = iter->lock();
			if (!sp || sp.get() == object) {
				iter = m_data.erase(iter);
			} else {
				++iter;
			}
		}
	}

	// Port of: public synchronized void clean()
	//   final Iterator<WeakReference<E>> iterator = data.iterator();
	//   while(iterator.hasNext()) { final E object = iterator.next().get();
	//     if(object == null) { iterator.remove(); } }
	void clean() {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_data.begin();
		while (iter != m_data.end()) {
			if (iter->expired()) {
				iter = m_data.erase(iter);
			} else {
				++iter;
			}
		}
	}

	// Port of: public synchronized boolean isEmpty()
	//   return data.isEmpty();
	bool isEmpty() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_data.empty();
	}

private:
	// Port of: private final ArrayList<WeakReference<E>> data = new ArrayList<>();
	std::vector<std::weak_ptr<E>> m_data;

	// Port of: synchronized methods
	mutable std::mutex m_mutex;
};

} // namespace PinkReader
