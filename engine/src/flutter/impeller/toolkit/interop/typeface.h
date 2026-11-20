// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_TOOLKIT_INTEROP_TYPEFACE_H_
#define FLUTTER_IMPELLER_TOOLKIT_INTEROP_TYPEFACE_H_

#include <memory>

#include "flutter/fml/mapping.h"
#include "flutter/third_party/skia/include/core/SkTypeface.h"
#include "impeller/toolkit/interop/impeller.h"
#include "impeller/toolkit/interop/object.h"

namespace impeller::interop {

/**
 * A font face loaded from font data.
 */
class Typeface final
    : public Object<Typeface, IMPELLER_INTERNAL_HANDLE_NAME(ImpellerTypeface)> {
 public:
  //----------------------------------------------------------------------------
  /// @brief      Reads a face from font data.
  ///
  /// @return     The typeface, or null if the data holds no such face.
  ///
  static ScopedObject<Typeface> Make(std::shared_ptr<fml::Mapping> font_data,
                                     uint32_t face_index);

  explicit Typeface(sk_sp<SkTypeface> typeface);

  ~Typeface() override;

  Typeface(const Typeface&) = delete;

  Typeface& operator=(const Typeface&) = delete;

  const sk_sp<SkTypeface>& GetSkiaTypeface() const;

 private:
  sk_sp<SkTypeface> typeface_;
};

}  // namespace impeller::interop

#endif  // FLUTTER_IMPELLER_TOOLKIT_INTEROP_TYPEFACE_H_
