/*
 * Gestion du stockage persistant (Preferences)
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>
#include "config.h"

namespace Storage {
  void init();
  void load();
  void save();
  void reset();
}

#endif
