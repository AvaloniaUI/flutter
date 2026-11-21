// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/toolkit/interop/context_options.h"

namespace impeller::interop {

ContextOptions::ContextOptions() = default;

ContextOptions::~ContextOptions() = default;

void ContextOptions::SetUseSDFs(bool use_sdfs) {
  flags_.use_sdfs = use_sdfs;
}

void ContextOptions::SetTopLeftDefaultFramebufferOrigin(bool top_left) {
  flags_.top_left_default_framebuffer_origin = top_left;
}

const Flags& ContextOptions::GetFlags() const {
  return flags_;
}

}  // namespace impeller::interop
