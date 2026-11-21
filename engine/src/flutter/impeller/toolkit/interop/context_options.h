// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_TOOLKIT_INTEROP_CONTEXT_OPTIONS_H_
#define FLUTTER_IMPELLER_TOOLKIT_INTEROP_CONTEXT_OPTIONS_H_

#include "impeller/base/flags.h"
#include "impeller/toolkit/interop/impeller.h"
#include "impeller/toolkit/interop/object.h"

namespace impeller::interop {

/**
 * Flags read when a context is created.
 */
class ContextOptions final
    : public Object<ContextOptions,
                    IMPELLER_INTERNAL_HANDLE_NAME(ImpellerContextOptions)> {
 public:
  ContextOptions();

  ~ContextOptions() override;

  ContextOptions(const ContextOptions&) = delete;

  ContextOptions& operator=(const ContextOptions&) = delete;

  void SetUseSDFs(bool use_sdfs);

  void SetTopLeftDefaultFramebufferOrigin(bool top_left);

  const Flags& GetFlags() const;

 private:
  Flags flags_;
};

}  // namespace impeller::interop

#endif  // FLUTTER_IMPELLER_TOOLKIT_INTEROP_CONTEXT_OPTIONS_H_
