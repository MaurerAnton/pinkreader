// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GenericFactoryNoThrow.java (lines 1-24)
// Ported from Java interface to C++ concept/template

#pragma once

#include <concepts>
#include <functional>

namespace PinkReader {

// GenericFactoryNoThrow.java line 22: public interface GenericFactoryNoThrow<E>
// Original: @NonNull E create();
template<typename E>
using GenericFactoryNoThrow = std::function<E()>;

} // namespace PinkReader
