// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GenericFactory.java (lines 1-24)
// Ported from Java interface to C++ concept/template

#pragma once

#include <concepts>
#include <functional>

namespace PinkReader {

// GenericFactory.java line 22: public interface GenericFactory<E, T extends Throwable>
// Original: @NonNull E create() throws T;
template<typename E>
using GenericFactory = std::function<E()>;

} // namespace PinkReader
