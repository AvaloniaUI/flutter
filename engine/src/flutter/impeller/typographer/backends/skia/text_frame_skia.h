// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_TYPOGRAPHER_BACKENDS_SKIA_TEXT_FRAME_SKIA_H_
#define FLUTTER_IMPELLER_TYPOGRAPHER_BACKENDS_SKIA_TEXT_FRAME_SKIA_H_

#include "impeller/typographer/font.h"
#include "impeller/typographer/text_frame.h"

#include "third_party/skia/include/core/SkTextBlob.h"

namespace impeller {

std::shared_ptr<impeller::TextFrame> MakeTextFrameFromTextBlobSkia(
    const sk_sp<SkTextBlob>& blob);

/// Like |MakeTextFrameFromTextBlobSkia|, but rasterizes the glyphs with the
/// given edging and hinting instead of anti-aliased edging and slight hinting.
std::shared_ptr<impeller::TextFrame> MakeTextFrameFromTextBlobSkia(
    const sk_sp<SkTextBlob>& blob,
    Font::Edging edging,
    Font::Hinting hinting);

}  // namespace impeller

#endif  // FLUTTER_IMPELLER_TYPOGRAPHER_BACKENDS_SKIA_TEXT_FRAME_SKIA_H_
