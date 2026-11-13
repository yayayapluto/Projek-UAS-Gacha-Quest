#ifndef UTIL_H
#define UTIL_H

int readLine(char *buf, int max);
int readInt(const char *prompt, int min, int max);
void readString(const char *prompt, char *out, int max);
void clearScreen(void);
void printLine(void);
void shutdownGame(void);

#endif
