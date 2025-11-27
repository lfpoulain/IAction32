/*
 * Implémentation du Logger - Optimisé pour ESP32
 */

#include "logger.h"

char Logger::logs[MAX_LOG_LINES][MAX_LOG_LENGTH];
uint8_t Logger::head = 0;
uint8_t Logger::count = 0;

void Logger::init() {
    memset(logs, 0, sizeof(logs));
    log("System initialized");
}

void Logger::log(const char* message) {
    Serial.println(message);
    
    unsigned long m = millis();
    uint8_t h = (m / 3600000) % 24;
    uint8_t mn = (m / 60000) % 60;
    uint8_t s = (m / 1000) % 60;
    
    snprintf(logs[head], MAX_LOG_LENGTH, "[%02u:%02u:%02u] %s", h, mn, s, message);
    
    head = (head + 1) % MAX_LOG_LINES;
    if (count < MAX_LOG_LINES) count++;
}

void Logger::log(const String& message) {
    log(message.c_str());
}

void Logger::printf(const char* format, ...) {
    char buf[MAX_LOG_LENGTH - 12]; // Reserve space for timestamp
    va_list arg;
    va_start(arg, format);
    vsnprintf(buf, sizeof(buf), format, arg);
    va_end(arg);
    log(buf);
}

String Logger::getLogsHTML() {
    String output;
    output.reserve(count * 80); // Pre-allocate
    // Renvoie les logs inversés (plus récents en haut)
    for (int i = 0; i < count; i++) {
        int idx = (head - 1 - i + MAX_LOG_LINES) % MAX_LOG_LINES;
        output += F("<div class='log-line'>");
        output += logs[idx];
        output += F("</div>");
    }
    return output;
}

void Logger::clear() {
    count = 0;
    head = 0;
    log("Logs cleared");
}
