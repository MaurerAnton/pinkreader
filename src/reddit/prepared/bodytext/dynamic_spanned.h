// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/DynamicSpanned.java
// Ported to C++ for PinkReader

#pragma once

namespace PinkReader {

class DynamicSpanned {
public:
    virtual ~DynamicSpanned() = default;

    virtual void addSpanDynamic(
            void* what,
            int start,
            int end,
            int flags) = 0;
};

} // namespace PinkReader
