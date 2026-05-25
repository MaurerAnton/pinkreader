// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElement.java
// Ported to C++ for PinkReader

#include "body_element.h"

namespace PinkReader {

BodyElement::BodyElement(BlockType type)
    : m_type(type)
{
}

BlockType BodyElement::getType() const
{
    return m_type;
}

} // namespace PinkReader
