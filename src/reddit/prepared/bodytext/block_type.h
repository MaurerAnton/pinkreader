// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BlockType.java
// Ported to C++ for PinkReader

#pragma once

namespace PinkReader {

enum class BlockType {
    VERTICAL_SEQUENCE,
    NORMAL_TEXT,
    HEADER,
    LIST_ELEMENT,
    TABLE,
    TABLE_ROW,
    TABLE_CELL,
    CODE_BLOCK,
    HORIZONTAL_RULE,
    QUOTE,
    BUTTON,
    ERROR
};

} // namespace PinkReader
