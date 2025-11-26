/*
 * Gestion de la caméra
 */

#ifndef CAMERA_SETUP_H
#define CAMERA_SETUP_H

#include <Arduino.h>

namespace Camera {
  void init();
  void applySettings();
  String captureBase64();
}

#endif
