#ifndef UTIL_H
#define UTIL_H

int bacaBaris(char *buf, int maks);
int bacaAngka(const char *prompt, int min, int max);
void bacaString(const char *prompt, char *out, int maks);
void bersihkanLayar(void);
void garis(void);
void shutdownGame(void);

#endif
