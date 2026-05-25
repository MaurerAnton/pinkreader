/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_block.cpp
 * Port of: HtmlRawElementBlock.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_block.h"
#include "html_raw_element_img.h"
#include "html_raw_element_styled_text.h"
#include "html_raw_element_link_button.h"

// Stub forward declarations for BodyElement types
// These will be replaced when the bodytext module is ported
namespace PinkReader {

// Stub classes for BodyElement types used in generate()
class BodyElement {};
class BodyElementTextSpanned : public BodyElement {
public:
    int blockType;
    // SpannableStringBuilder is an Android type; using QString as proxy
    QString* spannableBuilder;
    BodyElementTextSpanned(int bt, QString* sb) : blockType(bt), spannableBuilder(sb) {}
};
class SpannableStringBuilder : public QString {
public:
    using QString::QString;
    int length() const { return size(); }
    void setSpan(void* span, int start, int end, int flags) {
        // Stub: Android span implementation
    }
};

} // namespace PinkReader

#include <QString>

namespace PinkReader {

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       for(final HtmlRawElement element : mChildren) {
//           element.getPlainText(stringBuilder);
//       }
//   }
void HtmlRawElementBlock::getPlainText(QString &stringBuilder) const {
    for(const HtmlRawElement* element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (non-virtual, returns new block) =====
// Port of:
//   public HtmlRawElementBlock reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity) {
//
//       final ArrayList<HtmlRawElement> reduced = new ArrayList<>();
//       final ArrayList<LinkButtonDetails> linkButtons = new ArrayList<>();
//
//       for(final HtmlRawElement child : mChildren) {
//           child.reduce(activeAttributes, activity, reduced, linkButtons);
//       }
//
//       for(final LinkButtonDetails details : linkButtons) {
//           reduced.add(new HtmlRawElementLinkButton(details));
//       }
//
//       return new HtmlRawElementBlock(mBlockType, reduced);
//   }
HtmlRawElementBlock HtmlRawElementBlock::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity) const {

    std::vector<HtmlRawElement*> reduced;
    std::vector<LinkButtonDetails> linkButtons;

    // for(final HtmlRawElement child : mChildren) {
    //     child.reduce(activeAttributes, activity, reduced, linkButtons);
    // }
    for(HtmlRawElement* child : mChildren) {
        child->reduce(activeAttributes, activity, reduced, linkButtons);
    }

    // for(final LinkButtonDetails details : linkButtons) {
    //     reduced.add(new HtmlRawElementLinkButton(details));
    // }
    for(const LinkButtonDetails &details : linkButtons) {
        reduced.push_back(new HtmlRawElementLinkButton(details));
    }

    return HtmlRawElementBlock(mBlockType, reduced);
}

// ===== reduce (virtual, adds to destination) =====
// Port of:
//   @Override
//   public void reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<HtmlRawElement> destination,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       destination.add(reduce(activeAttributes, activity));
//   }
void HtmlRawElementBlock::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(reduce(activeAttributes, activity));
    destination.push_back(
        new HtmlRawElementBlock(reduce(activeAttributes, activity)));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       boolean stringWrittenTo = false;
//       SpannableStringBuilder ssb = new SpannableStringBuilder();
//       BodyElementTextSpanned bodyElementTextSpanned =
//           new BodyElementTextSpanned(mBlockType, ssb);
//
//       for(final HtmlRawElement child : mChildren) {
//           if(child instanceof HtmlRawElementStyledText) {
//               ((HtmlRawElementStyledText)child).writeTo(ssb);
//               stringWrittenTo = true;
//           } else if (child instanceof HtmlRawElementImg) {
//               ((HtmlRawElementImg) child).writeTo(ssb,
//                   activity, bodyElementTextSpanned);
//               stringWrittenTo = true;
//           } else {
//               if (stringWrittenTo) {
//                   destination.add(bodyElementTextSpanned);
//                   ssb = new SpannableStringBuilder();
//                   bodyElementTextSpanned =
//                       new BodyElementTextSpanned(mBlockType, ssb);
//                   stringWrittenTo = false;
//               }
//               child.generate(activity, destination);
//           }
//       }
//
//       // If the last child in the array is a HtmlRawElementStyledText
//       // or HtmlRawElementImg object, it won't be added to the
//       // destination array in the loop
//       // Need this logic to make sure that it's added
//       if(stringWrittenTo) {
//           destination.add(bodyElementTextSpanned);
//       }
//   }
void HtmlRawElementBlock::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    bool stringWrittenTo = false;

    // SpannableStringBuilder ssb = new SpannableStringBuilder();
    SpannableStringBuilder* ssb = new SpannableStringBuilder();

    // BodyElementTextSpanned bodyElementTextSpanned =
    //     new BodyElementTextSpanned(mBlockType, ssb);
    BodyElementTextSpanned* bodyElementTextSpanned =
        new BodyElementTextSpanned(mBlockType, ssb);

    // for(final HtmlRawElement child : mChildren) {
    for(HtmlRawElement* child : mChildren) {

        // if(child instanceof HtmlRawElementStyledText) {
        HtmlRawElementStyledText* styledText =
            dynamic_cast<HtmlRawElementStyledText*>(child);
        if(styledText) {
            // ((HtmlRawElementStyledText)child).writeTo(ssb);
            styledText->writeTo(*ssb);
            stringWrittenTo = true;

        // } else if (child instanceof HtmlRawElementImg) {
        } else {
            HtmlRawElementImg* img = dynamic_cast<HtmlRawElementImg*>(child);
            if(img) {
                // ((HtmlRawElementImg) child).writeTo(ssb,
                //     activity, bodyElementTextSpanned);
                img->writeTo(*ssb, activity, *bodyElementTextSpanned);
                stringWrittenTo = true;

            // } else {
            } else {
                if(stringWrittenTo) {
                    // destination.add(bodyElementTextSpanned);
                    destination.push_back(bodyElementTextSpanned);

                    // ssb = new SpannableStringBuilder();
                    ssb = new SpannableStringBuilder();

                    // bodyElementTextSpanned =
                    //     new BodyElementTextSpanned(mBlockType, ssb);
                    bodyElementTextSpanned =
                        new BodyElementTextSpanned(mBlockType, ssb);

                    stringWrittenTo = false;
                }
                // child.generate(activity, destination);
                child->generate(activity, destination);
            }
        }
    }

    // if(stringWrittenTo) {
    //     destination.add(bodyElementTextSpanned);
    // }
    if(stringWrittenTo) {
        destination.push_back(bodyElementTextSpanned);
    }
}

} // namespace PinkReader
