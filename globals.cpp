/*
 * Définition des variables globales
 */

#include "config.h"

AppConfig cfg;
AppStats stats = {
  .captureCount = 0,
  .successCount = 0,
  .errorCount = 0,
  .lastResult = "N/A",
  .lastResponse = "",
  .lastCaptureTimestamp = 0,
  .captureInProgress = false
};
