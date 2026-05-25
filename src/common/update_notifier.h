/*
 * PinkReader - GPLv3
 * File: update_notifier.h
 * Port of: org.quantumbadger.redreader.common.UpdateNotifier.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.UpdateNotifier<E>
 *
 * Original: public abstract class UpdateNotifier<E> with:
 *   - LinkedList<WeakReference<E>> listeners
 *   - synchronized addListener/removeListener/updateAllListeners
 *   - protected abstract notifyListener(E listener)
 *
 * Uses std::weak_ptr instead of Java WeakReference for analogous semantics.
 */
template<typename E>
class UpdateNotifier {
public:
	virtual ~UpdateNotifier() = default;

	// Port of: public synchronized void addListener(final E updateListener)
	//   listeners.add(new WeakReference<>(updateListener));
	void addListener(std::shared_ptr<E> updateListener) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_listeners.push_back(updateListener);
	}

	// Port of: public synchronized void removeListener(final E updateListener)
	//   Iterator over listeners; removes if listener == null or listener == updateListener
	void removeListener(const E *updateListener) {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_listeners.begin();
		while (iter != m_listeners.end()) {
			auto sp = iter->lock();
			// Port of: if(listener == null || listener == updateListener)
			if (!sp || sp.get() == updateListener) {
				// Port of: iter.remove();
				iter = m_listeners.erase(iter);
			} else {
				++iter;
			}
		}
	}

	// Port of: public synchronized void updateAllListeners()
	//   Iterator over listeners; calls notifyListener if not null, else removes.
	void updateAllListeners() {
		std::lock_guard<std::mutex> lock(m_mutex);

		auto iter = m_listeners.begin();
		while (iter != m_listeners.end()) {
			auto sp = iter->lock();
			// Port of: if(listener == null) { iter.remove(); }
			if (!sp) {
				iter = m_listeners.erase(iter);
			} else {
				// Port of: else { notifyListener(listener); }
				notifyListener(sp.get());
				++iter;
			}
		}
	}

protected:
	// Port of: protected abstract void notifyListener(E listener);
	virtual void notifyListener(E *listener) = 0;

private:
	// Port of: private final LinkedList<WeakReference<E>> listeners = new LinkedList<>();
	std::vector<std::weak_ptr<E>> m_listeners;

	// Port of: synchronized methods
	std::mutex m_mutex;
};

} // namespace PinkReader
