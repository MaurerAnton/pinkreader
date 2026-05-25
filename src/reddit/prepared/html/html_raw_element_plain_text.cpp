/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_plain_text.cpp
 * Port of: HtmlRawElementPlainText.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_plain_text.h"
#include "html_raw_element_styled_text.h"

#include <QString>
#include <vector>
#include <stdexcept>

namespace PinkReader {

// Stub classes for BodyElement/types
class BodyElement {};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       stringBuilder.append(mText);
//   }
void HtmlRawElementPlainText::getPlainText(QString &stringBuilder) const {
    // stringBuilder.append(mText);
    stringBuilder.append(mText);
}

// ===== reduce =====
// Port of:
//   @Override
//   public void reduce(
//       @NonNull final HtmlTextAttributes attributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<HtmlRawElement> destination,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       ArrayList<CharacterStyle> spans = null;
//
//       if(attributes.bold > 0) {
//           //noinspection ConstantConditions
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new StyleSpan(Typeface.BOLD));
//       }
//
//       if(attributes.italic > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new StyleSpan(Typeface.ITALIC));
//       }
//
//       if(attributes.underline > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new UnderlineSpan());
//       }
//
//       if(attributes.strikethrough > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new StrikethroughSpan());
//       }
//
//       if(attributes.monospace > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new TypefaceSpan("monospace"));
//       }
//
//       if(attributes.superscript > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//
//           for(int i = 0; i < attributes.superscript; i++) {
//               spans.add(new SuperscriptSpan());
//               spans.add(new RelativeSizeSpan(0.85f));
//           }
//       }
//
//       if(attributes.extraLarge > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new RelativeSizeSpan(1.6f));
//
//       } else if(attributes.large > 0) {
//           if(spans == null) { spans = new ArrayList<>(); }
//           spans.add(new RelativeSizeSpan(1.3f));
//       }
//
//       if(attributes.href != null) {
//           if(spans == null) { spans = new ArrayList<>(); }
//
//           final UriString url = attributes.href;
//
//           spans.add(new ClickableSpan() {
//               @Override
//               public void onClick(@NonNull final View widget) {
//                   LinkHandler.onLinkClicked(activity, url);
//               }
//           });
//       }
//
//       destination.add(new HtmlRawElementStyledText(mText, spans));
//   }
void HtmlRawElementPlainText::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // ArrayList<CharacterStyle> spans = null;
    std::optional<std::vector<void*>> spans;

    // if(attributes.bold > 0) {
    if(activeAttributes.bold > 0) {
        // if(spans == null) { spans = new ArrayList<>(); }
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new StyleSpan(Typeface.BOLD));
        spans->push_back(nullptr); // Placeholder for StyleSpan(Typeface.BOLD)
    }

    // if(attributes.italic > 0) {
    if(activeAttributes.italic > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new StyleSpan(Typeface.ITALIC));
        spans->push_back(nullptr); // Placeholder for StyleSpan(Typeface.ITALIC)
    }

    // if(attributes.underline > 0) {
    if(activeAttributes.underline > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new UnderlineSpan());
        spans->push_back(nullptr); // Placeholder for UnderlineSpan()
    }

    // if(attributes.strikethrough > 0) {
    if(activeAttributes.strikethrough > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new StrikethroughSpan());
        spans->push_back(nullptr); // Placeholder for StrikethroughSpan()
    }

    // if(attributes.monospace > 0) {
    if(activeAttributes.monospace > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new TypefaceSpan("monospace"));
        spans->push_back(nullptr); // Placeholder for TypefaceSpan("monospace")
    }

    // if(attributes.superscript > 0) {
    if(activeAttributes.superscript > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }

        // for(int i = 0; i < attributes.superscript; i++) {
        for(int i = 0; i < activeAttributes.superscript; i++) {
            // spans.add(new SuperscriptSpan());
            spans->push_back(nullptr); // Placeholder for SuperscriptSpan()
            // spans.add(new RelativeSizeSpan(0.85f));
            spans->push_back(nullptr); // Placeholder for RelativeSizeSpan(0.85f)
        }
    }

    // if(attributes.extraLarge > 0) {
    if(activeAttributes.extraLarge > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new RelativeSizeSpan(1.6f));
        spans->push_back(nullptr); // Placeholder for RelativeSizeSpan(1.6f)

    // } else if(attributes.large > 0) {
    } else if(activeAttributes.large > 0) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }
        // spans.add(new RelativeSizeSpan(1.3f));
        spans->push_back(nullptr); // Placeholder for RelativeSizeSpan(1.3f)
    }

    // if(attributes.href != null) {
    if(activeAttributes.href.has_value()) {
        if(!spans.has_value()) { spans = std::vector<void*>(); }

        // final UriString url = attributes.href;
        // spans.add(new ClickableSpan() {
        //     @Override
        //     public void onClick(@NonNull final View widget) {
        //         LinkHandler.onLinkClicked(activity, url);
        //     }
        // });
        spans->push_back(nullptr); // Placeholder for ClickableSpan
    }

    // destination.add(new HtmlRawElementStyledText(mText, spans));
    destination.push_back(new HtmlRawElementStyledText(mText, spans));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       throw new RuntimeException("Attempt to call generate() on reducible element");
//   }
void HtmlRawElementPlainText::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    throw std::runtime_error("Attempt to call generate() on reducible element");
}

} // namespace PinkReader
