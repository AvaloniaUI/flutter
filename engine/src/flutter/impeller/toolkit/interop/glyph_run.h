// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_TOOLKIT_INTEROP_GLYPH_RUN_H_
#define FLUTTER_IMPELLER_TOOLKIT_INTEROP_GLYPH_RUN_H_

#include <memory>

#include "flutter/display_list/dl_text.h"
#include "flutter/display_list/geometry/dl_path.h"
#include "flutter/third_party/skia/include/core/SkTextBlob.h"
#include "impeller/toolkit/interop/impeller.h"
#include "impeller/toolkit/interop/object.h"
#include "impeller/toolkit/interop/typeface.h"

namespace impeller::interop {

/**
 * An immutable run of positioned glyphs of one typeface at one size.
 */
class GlyphRun final
    : public Object<GlyphRun, IMPELLER_INTERNAL_HANDLE_NAME(ImpellerGlyphRun)> {
 public:
  static ScopedObject<GlyphRun> Make(const Typeface& typeface,
                                     Scalar size,
                                     const ImpellerGlyphRunOptions& options,
                                     uint32_t glyph_count,
                                     const uint16_t* glyphs,
                                     const ImpellerPoint* positions);

  GlyphRun(sk_sp<SkTextBlob> blob, std::shared_ptr<flutter::DlText> text);

  ~GlyphRun() override;

  GlyphRun(const GlyphRun&) = delete;

  GlyphRun& operator=(const GlyphRun&) = delete;

  //----------------------------------------------------------------------------
  /// @brief      The text drawn through the glyph atlas.
  ///
  const std::shared_ptr<flutter::DlText>& GetText() const;

  //----------------------------------------------------------------------------
  /// @brief      The outlines of the glyphs, relative to the run's origin, or
  ///             an empty path for runs of color glyphs.
  ///
  flutter::DlPath GetPath() const;

  ImpellerRect GetBounds() const;

 private:
  sk_sp<SkTextBlob> blob_;
  std::shared_ptr<flutter::DlText> text_;
};

}  // namespace impeller::interop

#endif  // FLUTTER_IMPELLER_TOOLKIT_INTEROP_GLYPH_RUN_H_
