// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Consumer.java (lines 1-22)
// Ported from Java interface to C++ concept/template

#pragma once

#include <concepts>
#include <functional>

namespace PinkReader {

// Consumer.java line 20: public interface Consumer<E>
// Original: void consume(E value);
template<typename E>
using Consumer = std::function<void(E)>;

} // namespace PinkReader
