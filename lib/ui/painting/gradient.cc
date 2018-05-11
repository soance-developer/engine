// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "flutter/lib/ui/painting/gradient.h"

#include "lib/tonic/converter/dart_converter.h"
#include "lib/tonic/dart_args.h"
#include "lib/tonic/dart_binding_macros.h"
#include "lib/tonic/dart_library_natives.h"

namespace blink {

typedef CanvasGradient
    Gradient;  // Because the C++ name doesn't match the Dart name.

static void Gradient_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&CanvasGradient::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, Gradient);

#define FOR_EACH_BINDING(V) \
  V(Gradient, initLinear)   \
  V(Gradient, initRadial)   \
  V(Gradient, initSweep)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void CanvasGradient::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"Gradient_constructor", Gradient_constructor, 1, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}

fxl::RefPtr<CanvasGradient> CanvasGradient::Create() {
  return fxl::MakeRefCounted<CanvasGradient>();
}

void CanvasGradient::initLinear(const tonic::Float32List& end_points,
                                const tonic::Int32List& colors,
                                const tonic::Float32List& color_stops,
                                SkShader::TileMode tile_mode) {
  FXL_DCHECK(end_points.num_elements() == 4);
  FXL_DCHECK(colors.num_elements() == color_stops.num_elements() ||
             color_stops.data() == nullptr);

  static_assert(sizeof(SkPoint) == sizeof(float) * 2,
                "SkPoint doesn't use floats.");
  static_assert(sizeof(SkColor) == sizeof(int32_t),
                "SkColor doesn't use int32_t.");

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeLinear(
      reinterpret_cast<const SkPoint*>(end_points.data()),
      reinterpret_cast<const SkColor*>(colors.data()), color_stops.data(),
      colors.num_elements(), tile_mode)));
}

void CanvasGradient::initRadial(double center_x,
                                double center_y,
                                double radius,
                                const tonic::Int32List& colors,
                                const tonic::Float32List& color_stops,
                                SkShader::TileMode tile_mode,
                                const tonic::Float64List& matrix4) {
  FXL_DCHECK(colors.num_elements() == color_stops.num_elements() ||
             color_stops.data() == nullptr);

  static_assert(sizeof(SkColor) == sizeof(int32_t),
                "SkColor doesn't use int32_t.");

  SkMatrix sk_matrix;
  bool has_matrix = matrix4.data() != nullptr;
  if (has_matrix) {
    sk_matrix = ToSkMatrix(matrix4);
  }

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeRadial(
      SkPoint::Make(center_x, center_y), radius,
      reinterpret_cast<const SkColor*>(colors.data()), color_stops.data(),
      colors.num_elements(), tile_mode, 0, has_matrix ? &sk_matrix : nullptr)));
}

void CanvasGradient::initSweep(double center_x,
                               double center_y,
                               const tonic::Int32List& colors,
                               const tonic::Float32List& color_stops,
                               SkShader::TileMode tile_mode,
                               double start_angle,
                               double end_angle,
                               const tonic::Float64List& matrix4) {
  FXL_DCHECK(colors.num_elements() == color_stops.num_elements() ||
             color_stops.data() == nullptr);

  static_assert(sizeof(SkColor) == sizeof(int32_t),
                "SkColor doesn't use int32_t.");

  SkMatrix sk_matrix;
  bool has_matrix = matrix4.data() != nullptr;
  if (has_matrix) {
    sk_matrix = ToSkMatrix(matrix4);
  }

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeSweep(
      center_x, center_y, reinterpret_cast<const SkColor*>(colors.data()),
      color_stops.data(), colors.num_elements(), tile_mode,
      start_angle * 180.0 / M_PI, end_angle * 180.0 / M_PI, 0,
      has_matrix ? &sk_matrix : nullptr)));
}

CanvasGradient::CanvasGradient() = default;

CanvasGradient::~CanvasGradient() = default;

}  // namespace blink
