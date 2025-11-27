/*
 * Gestionnaire de logs système
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

#define MAX_LOG_LINES 50
#define MAX_LOG_LENGTH 120

class Logger {
public:
    static void init();
    static void log(const char* message);
    static void log(const String& message);
    static void printf(const char* format, ...);
    static String getLogsHTML();
    static void clear();

private:
    static char logs[MAX_LOG_LINES][MAX_LOG_LENGTH];
    static uint8_t head;
    static uint8_t count;
};

#endif
