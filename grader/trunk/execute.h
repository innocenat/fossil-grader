#ifndef EXECUTE_H_INCLUDED
#define EXECUTE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

int execute(char *exname, char *inname, char *outname, int t);
int verify(char *vname, int c, char *msg);

#ifdef __cplusplus
}
#endif

#endif
