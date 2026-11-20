// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/toolkit/interop/typeface.h"

#include "flutter/third_party/skia/include/core/SkData.h"
#include "flutter/third_party/skia/include/core/SkFontMgr.h"
#include "flutter/third_party/skia/include/core/SkStream.h"
#include "flutter/txt/src/txt/platform.h"
#include "impeller/base/validation.h"

namespace impeller::interop {

ScopedObject<Typeface> Typeface::Make(std::shared_ptr<fml::Mapping> font_data,
                                      uint32_t face_index) {
  if (!font_data || font_data->GetMapping() == nullptr ||
      font_data->GetSize() == 0u) {
    VALIDATION_LOG << "Invalid font data.";
    return {};
  }
  // The typeface reads the font data for as long as it lives.
  auto holder = new std::shared_ptr<fml::Mapping>(std::move(font_data));
  auto sk_data = SkData::MakeWithProc(
      (*holder)->GetMapping(),  // data ptr
      (*holder)->GetSize(),     // data size
      [](const void*, void* context) {
        delete reinterpret_cast<std::shared_ptr<fml::Mapping>*>(context);
      },      // release callback
      holder  // release callback context
  );
  auto sk_typeface = txt::GetDefaultFontManager()->makeFromStream(
      SkMemoryStream::Make(std::move(sk_data)), static_cast<int>(face_index));
  if (!sk_typeface) {
    VALIDATION_LOG << "Could not create a typeface from the font data.";
    return {};
  }
  return Create<Typeface>(std::move(sk_typeface));
}

Typeface::Typeface(sk_sp<SkTypeface> typeface)
    : typeface_(std::move(typeface)) {}

Typeface::~Typeface() = default;

const sk_sp<SkTypeface>& Typeface::GetSkiaTypeface() const {
  return typeface_;
}

}  // namespace impeller::interop
