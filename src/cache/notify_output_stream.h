// Origin: org/quantumbadger/redreader/cache/NotifyOutputStream.java
#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

namespace PinkReader {

class NotifyOutputStream {
public:
	class Listener {
	public:
		virtual ~Listener() = default;
		virtual void onClose() = 0;
	};

private:
	std::ostream& m_out;
	Listener& m_listener;

public:
	NotifyOutputStream(std::ostream& out, Listener& listener)
		: m_out(out), m_listener(listener) {}

	void close() {
		m_out.flush();
		m_listener.onClose();
	}

	std::ostream& getOutputStream() {
		return m_out;
	}

	Listener& getListener() {
		return m_listener;
	}
};

} // namespace PinkReader
