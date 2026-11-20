// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/toolkit/interop/glyph_run.h"

#include <algorithm>

#include "flutter/third_party/skia/include/core/SkFont.h"
#include "flutter/third_party/skia/include/core/SkMatrix.h"
#include "flutter/third_party/skia/modules/skparagraph/include/Paragraph.h"
#include "impeller/base/validation.h"
#include "impeller/display_list/dl_text_impeller.h"
#include "impeller/typographer/backends/skia/text_frame_skia.h"

namespace impeller::interop {

static Font::Edging ToFontEdging(ImpellerFontEdging edging) {
  switch (edging) {
    case kImpellerFontEdgingAlias:
      return Font::Edging::kAlias;
    case kImpellerFontEdgingAntiAlias:
    default:
      return Font::Edging::kAntiAlias;
  }
}

static Font::Hinting ToFontHinting(ImpellerFontHinting hinting) {
  switch (hinting) {
    case kImpellerFontHintingSlight:
      return Font::Hinting::kSlight;
    case kImpellerFontHintingNormal:
      return Font::Hinting::kNormal;
    case kImpellerFontHintingFull:
      return Font::Hinting::kFull;
    case kImpellerFontHintingNone:
    default:
      return Font::Hinting::kNone;
  }
}

static SkFontHinting ToSkiaHinting(Font::Hinting hinting) {
  switch (hinting) {
    case Font::Hinting::kSlight:
      return SkFontHinting::kSlight;
    case Font::Hinting::kNormal:
      return SkFontHinting::kNormal;
    case Font::Hinting::kFull:
      return SkFontHinting::kFull;
    case Font::Hinting::kNone:
      return SkFontHinting::kNone;
  }
  return SkFontHinting::kNone;
}

ScopedObject<GlyphRun> GlyphRun::Make(const Typeface& typeface,
                                      Scalar size,
                                      const ImpellerGlyphRunOptions& options,
                                      uint32_t glyph_count,
                                      const uint16_t* glyphs,
                                      const ImpellerPoint* positions) {
  if (glyph_count == 0u || glyphs == nullptr || positions == nullptr) {
    VALIDATION_LOG << "A glyph run needs at least one glyph and position.";
    return {};
  }
  if (!(size > 0.0f)) {
    VALIDATION_LOG << "Invalid font size " << size << ".";
    return {};
  }

  const auto edging = ToFontEdging(options.edging);
  const auto hinting = ToFontHinting(options.hinting);

  // The font settings match those the glyph atlas rasterizes with, so the
  // blob's bounds and outlines agree with the drawn glyphs.
  SkFont font(typeface.GetSkiaTypeface(), size, 1.0f, options.skew_x);
  font.setEmbolden(options.embolden);
  font.setEdging(edging == Font::Edging::kAlias ? SkFont::Edging::kAlias
                                                : SkFont::Edging::kAntiAlias);
  font.setHinting(ToSkiaHinting(hinting));
  font.setSubpixel(edging != Font::Edging::kAlias);

  SkTextBlobBuilder builder;
  const auto& run = builder.allocRunPos(font, static_cast<int>(glyph_count));
  std::copy_n(glyphs, glyph_count, run.glyphs);
  SkPoint* points = run.points();
  for (uint32_t i = 0; i < glyph_count; i++) {
    points[i] = SkPoint::Make(positions[i].x, positions[i].y);
  }
  auto blob = builder.make();
  if (!blob) {
    VALIDATION_LOG << "Could not build the glyph run.";
    return {};
  }

  auto frame = MakeTextFrameFromTextBlobSkia(blob, edging, hinting);
  if (!frame) {
    VALIDATION_LOG << "Could not build the text frame of the glyph run.";
    return {};
  }
  return Create<GlyphRun>(std::move(blob),
                          flutter::DlTextImpeller::Make(frame));
}

GlyphRun::GlyphRun(sk_sp<SkTextBlob> blob,
                   std::shared_ptr<flutter::DlText> text)
    : blob_(std::move(blob)), text_(std::move(text)) {}

GlyphRun::~GlyphRun() = default;

const std::shared_ptr<flutter::DlText>& GlyphRun::GetText() const {
  return text_;
}

flutter::DlPath GlyphRun::GetPath() const {
  // Paragraph::GetPath places the outlines relative to the blob's top left.
  SkPath path = skia::textlayout::Paragraph::GetPath(blob_.get());
  if (path.isEmpty()) {
    return flutter::DlPath(path);
  }
  return flutter::DlPath(path.makeTransform(
      SkMatrix::Translate(blob_->bounds().left(), blob_->bounds().top())));
}

ImpellerRect GlyphRun::GetBounds() const {
  const auto& bounds = blob_->bounds();
  return ImpellerRect{bounds.x(), bounds.y(), bounds.width(), bounds.height()};
}

}  // namespace impeller::interop
