#include "execute.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

void extract_result(char* filename,
                    int& status,
                    int& mem_used,
                    double& time_used)
{
  char firstline[1000];
  char lastline[1000];
  char buf[1000];

  //printf(">> %s\n",filename);

  FILE* fp = fopen(filename,"r");
  if(fp==0) {
    status = EXE_RESULT_MISC_ERROR;
    return;
  }

  bool first = true;
  while(fgets(buf,1000,fp)!=NULL) {
    if(first) {
      strcpy(firstline,buf);
      first = false;
    }
    //printf(":: %s\n", buf);
    strcpy(lastline,buf);
  }
  fclose(fp);

  float tr,tu,ts;
  int c;

  tr = tu = ts = 0;
  mem_used = 0;

  c = sscanf(lastline,"%fr%fu%fs%dm",&tr,&tu,&ts,&mem_used);
  //printf("%s\n", lastline);
  //printf(">>> %d %.2f %.2f %.2f %d\n",c,tr,tu,ts,mem_used);

  time_used = ts + tu;

  if(strncasecmp(firstline,"OK",2)==0)
    status = EXE_RESULT_OK; 
  else if(strncasecmp(firstline,"Exited",6)==0)   // do not require exit(0);
    status = EXE_RESULT_OK;
  else if(strncasecmp(firstline,"Ti",2)==0)
    status = EXE_RESULT_TIMEOUT;
  else
    status = EXE_RESULT_MISC_ERROR;

  //printf("result: %d\n", status);
}


int execute(char* exname, char* inname, char* outname, double t, int max_mem)
{
  char cmd[1000];
  char res_filename[200];
  int max_mem_kb = max_mem/1024;

  if(max_mem_kb <= 0)
    max_mem_kb = 2000000000;  // 2TB should be more than enough.

  sprintf(res_filename,"%s-res",outname);

  sprintf(cmd,"../ev/box -a 2 -f -T -t %f -m %d -i %s -o %s %s 2> %s",
          t, max_mem_kb, inname, outname, exname, res_filename);

  //printf("%s\n",cmd);
  system(cmd);

  int res_status, mem_used;
  double time_used;
  extract_result(res_filename, res_status, mem_used, time_used);

  //printf("%d %d\n",mem_used, max_mem_kb);
  if(res_status==EXE_RESULT_OK) {
    if(time_used > t)
      return EXE_RESULT_TIMEOUT;
    if(mem_used > max_mem_kb)
      return EXE_RESULT_MEMORY;
    return EXE_RESULT_OK;
  } else
    return res_status;
}
