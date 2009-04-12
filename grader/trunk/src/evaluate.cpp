#include "evaluate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"
#include "db_interface.h"

#ifdef MSVC
// strange commands...

#include <direct.h>
#define chdir						_chdir

// all the system command for windoze
#define EVDIR						"ev\\"
#define COPYINCMD				"copy %s%s\\%d.in test > nul:"
#define COPYSOLCMD      "copy %s%s\\%d.sol test > nul:"
#define MOVEOLDTESTCMD  "copy/y test\\*.* old-test > nul:"
#define DELTESTDIR      "del/q test\\*.*"
#define COMPILECMD_C    "cmd.exe /c bcc -Dsystem=_SUS1_ -Dint86=_SUS2_ -Dintr=_SUS3_ -Ic:\\borlandc\\include -Lc:\\borlandc\\lib %s.c > compile.msg"
#define COMPILECMD_CPP  "cmd.exe /c bcc  -Dsystem=_SUS1_ -Dint86=_SUS2_ -Dintr=_SUS3_ -P -Ic:\\borlandc\\include -Lc:\\borlandc\\lib %s.c > compile.msg"
//#define COMPILECMD      "cmd.exe /c tcc %s.cpp"
//#define COMPILECMD_C    "cmd.exe /c tcc %s.c > compile.msg"
//#define COMPILECMD_CPP  "cmd.exe /c tcc -P %s.c > compile.msg"

#define TESTRES_CREATDIR	"mkdir test-res\\%s\\%s > nul:"
#define TESTRES_CPOUTPUT	"copy/y test\\*.out test-res\\%s\\%s > nul:"
#define TESTRES_COMPMSG		"copy/y test\\compile.msg test-res\\%s\\%s > nul:"

#else

#define EVDIR "ev/"

#endif

char copybuff[10000];

void copyfile(char* sfname, char* dfname)
{
  FILE* fin;
  FILE* fout;
  
  if(strcmp(sfname,dfname)==0)
    return;
  
  fin=fopen(sfname,"r");
  if(fin==NULL)
    return;
  
  fout=fopen(dfname,"w");
  if(fout==NULL) {
    fclose(fin);
    return;
  }

  while(fgets(copybuff,9999,fin)!=NULL)
    fprintf(fout,"%s",copybuff);
  fclose(fin);
  fclose(fout);
}

void copysmallfile(char* sfname, char* dfname)
{
  FILE* fin;
  FILE* fout;
  
  if(strcmp(sfname,dfname)==0)
    return;
  
  fin=fopen(sfname,"r");
  if(fin==NULL)
    return;
  
  fout=fopen(dfname,"w");
  if(fout==NULL) {
    fclose(fin);
    return;
  }
  
  int count=0;
  char line[100];
  while((count<100) && (fgets(line,99,fin)!=NULL)) {
    fprintf(fout,"%s",line);
    count++;
  }
  fclose(fin);
  fclose(fout);
}

bool iffileexist(char *fname)
{
  FILE *fp = fopen(fname,"r");
  
  if(fp!=NULL) {
    fclose(fp);
    return 1;
  } else
    return 0;
}

evaluator::evaluator(DB *mydb)
{
  prob_id = 0;
  evname = 0;
  infname = 0;
  outfname = 0;
  
  casecount = 0;
  fullscore = 0;
  db = mydb;
  language = EV_LANG_NONE;
}

evaluator::~evaluator()
{
  if(prob_id!=0)
    free(prob_id);
  if(evname!=0)
    free(evname);
  if(infname!=0)
    free(infname);
  if(outfname!=0)
    free(outfname);
}

void evaluator::readconf(char* pname)
{
  char confname[30];
  char line[100];
  
  if(prob_id!=0)
    free(prob_id);
  prob_id = strdup(pname);
  sprintf(confname,"%s%s/conf",EVDIR,pname);
  FILE *fp = fopen(confname,"r");
  if(fp!=NULL) {
    casecount = fullscore = 0;
    timelimit = 1.0;
    while(fgets(line,100,fp)!=NULL) {
      char topic[30];
      char value[20];
      sscanf(line,"%s %s",topic,value);
      if(strcmp(topic,"cases:")==0)
	sscanf(value,"%d",&casecount);
      else if(strcmp(topic,"score:")==0)
	sscanf(value,"%d",&fullscore);
      else if(strcmp(topic,"evaluator:")==0) {
	if(evname!=NULL)
	  free(evname);
	evname = strdup(value);
      } else if(strcmp(topic,"input:")==0) {
	if(infname!=NULL)
	  free(infname);
	infname = strdup(value);
      } else if(strcmp(topic,"output:")==0) {
	if(outfname!=NULL)
	  free(outfname);
	outfname = strdup(value);
      } else if(strcmp(topic,"timelimit:")==0)
	sscanf(value,"%lf",&timelimit);
    }
    if(fullscore==0)
      fullscore=casecount;
  } else {
    casecount = fullscore = 0;
  }
}

int evaluator::test(int c, char* msg)
{
  //This function is executed inside "./test/"
  char inname[10];
  char outname[10];
  char solname[10];
  char evfullname[20];
  int exres;
  
  sprintf(inname,"%d.in",c);
  sprintf(outname,"%d.out",c);
  sprintf(solname,"%d.sol",c);
  
  if(infname!=0)
    copyfile(inname,infname);
  
  if(outfname==0)
    exres = execute(prob_id, inname, outname, timelimit);
  else {
    exres = execute(prob_id, inname, 0, timelimit);
    copyfile(outfname,outname);
  }
  
  if(exres!=0) {
    //  fprintf(stderr,"verifying...\n");
    sprintf(evfullname,"../%s%s/%s",EVDIR,prob_id,evname);
    // printf("%s\n",evfullname);
    return verify(evfullname,c,msg);
  } else {
    strcpy(msg,"T");
    return 0;
  }
}

void evaluator::copytestcase(int c)
{
  char cmd[100];
  
  sprintf(cmd,COPYINCMD,EVDIR,prob_id,c);
  system(cmd);
  sprintf(cmd,COPYSOLCMD,EVDIR,prob_id,c);
  system(cmd);
}

void evaluator::savemessage(char* user_id)
{
  DB *myData;	
  char fname[100];
  char msg[1001];
  int msgsize;
  FILE *fp;
  
  if(db!=0)
    myData = db;
  else
    myData = connect_db();
  sprintf(fname,"test-res/%s/%s/compile.msg",user_id,prob_id);
  if((fp = fopen(fname,"r"))!=NULL) {
    msgsize = fread(msg,1,1000,fp);
    msg[msgsize]='\0';
    fclose(fp);
  } else {
    msgsize = 0;
    msg[0]='\0';
  }
  savecompilermsg(myData,user_id,prob_id,msg);
  if(db==0)
    close_db(myData);
}

void evaluator::copyoutput(char *user_id)
{
  char dir[100];
  char finname[100];
  char foutname[100];
  
  sprintf(dir,"test-res/%s",user_id);
  mkdir(dir);
  sprintf(dir,"test-res/%s/%s",user_id,prob_id);
  mkdir(dir);
  for(int i=0; i<casecount; i++) {
    sprintf(finname,"test/%d.out",i+1);
    sprintf(foutname,"test-res/%s/%s/%d.out",user_id,prob_id,i+1);
    copysmallfile(finname,foutname);
  }
  
  //copy compile message
  sprintf(foutname,"test-res/%s/%s/compile.msg",user_id,prob_id);
  copyfile("test/compile.msg",foutname);
  
  //copy source code
  sprintf(finname,"test/%s.c",prob_id);
  sprintf(foutname,"test-res/%s/%s/%s.c",user_id,prob_id,prob_id);
  copyfile(finname,foutname);
}

void evaluator::cleartestdir()
{
//  system(MOVEOLDTESTCMD);
  system(DELTESTDIR);
}

void evaluator::fetchsource(char* user_id, int sub_num, char *fname)
{
  DB *myData;
  
  if(db==0)
    myData = connect_db();
  else
    myData = db;
  
  saveprog_from_db(myData,user_id,prob_id,sub_num,fname);
  
  if(db==0)
    close_db(myData);
}

char *strupper(char *st)
{
  char *p = st;

  while(*p!=0) {
    if(((*p)>='a') && ((*p)<='z'))
      *p+=('A'-'a');
    p++;
  }
  return st;
}

int evaluator::getlanguage(char *fname)
{
  FILE *fp=fopen(fname, "r");
  char line[100];
  int lcount;
  
  if(fp==NULL)
    return -1;
  
  lcount=0;
  while((lcount<10) && (fgets(line,99,fp)!=NULL)) {
    lcount++;
    strupper(line);
    if(strstr(line,"LANG:")!=NULL) {
      if(strstr(line,"C++")!=NULL) {
	fclose(fp);
	return EV_LANG_CPP;
      } else if(strstr(line,"C")!=NULL) {
	fclose(fp);
	return EV_LANG_C;
      }
    }
  }
  fclose(fp);
  return EV_LANG_ERROR;
}

void writecompilemsg(char *st)
{
  FILE *fp=fopen("test/compile.msg","w");

  if(fp!=NULL) {
    fprintf(fp,"%s",st);
    fclose(fp);
  }
}

void appendcompilemsg(char *st)
{
  FILE *fp=fopen("test/compile.msg","a");
  
  if(fp!=NULL) {
    fprintf(fp,"%s",st);
    fclose(fp);
  }
}

bool evaluator::fetchandcompile(char *user_id, int sub_num)
{
  char sourcename[100];
  char cmd[100];
  char exname[100];
  int lang;
  
  sprintf(sourcename,"test/%s.c",prob_id);
  sprintf(exname,"%s.exe",prob_id);
  
  fetchsource(user_id,sub_num,sourcename);
  if(language==EV_LANG_NONE)
    lang = getlanguage(sourcename);
  else
    lang = language;
  if(lang==EV_LANG_ERROR) {
    writecompilemsg("Cannot determine language used. Check your program header.\n");
    return false;
  } else {
    chdir("test");
    if(lang==EV_LANG_C)
      sprintf(cmd,COMPILECMD_C,prob_id);
    else
      sprintf(cmd,COMPILECMD_CPP,prob_id);
    // printf("compiler (%d): %s\n",lang,cmd);
    system(cmd);
    chdir("..");
    sprintf(exname,"test\\%s.exe",prob_id);
    if(iffileexist(exname)) {
      appendcompilemsg("\n===========================\nCompiled successfully.\n");
      return true;
    } else {
      appendcompilemsg("\n===========================\nError compiling source program.\n");
      return false;
    }
  }
}

int evaluator::evaluate(char* user_id, int sub_num, char *mlog)
{
  int score = 0;
  char msg[100];
  char thismsg[100];
  
  msg[0]='\0';
  
  if(fetchandcompile(user_id,sub_num)) {
    for(int i=0; i<casecount; i++) {
      
      // to make life not so boring...
      printf("[%d]",i+1);
      fflush(stdout);
      
      copytestcase(i+1);
      
      chdir("test");
      score += test(i+1,thismsg);
      chdir("..");
      strcat(msg,thismsg);
    }
    printf("\n");
  } else
    strcpy(msg,"compile error");
  
  copyoutput(user_id);
  if(KEEP_COMPILE_MSG) {
    savemessage(user_id);
  }
  cleartestdir();
  
  if(mlog!=0)
    strcpy(mlog,msg);
  
  return score;
}

/*
//  printf("3\n");
if(s.estatus == submission_db::GRADING) {
//    printf("prob wait in queue\n");

int version = s.sub_count;

if((conf!=0) && (conf->chkdeadline)) {
version = logfile::findversion(id, u->getsect(), prob_id,
conf->hr, conf->min,
conf->d, conf->m, conf->y);
if(version!=s.sub_count)
				printf("(%d/%d)\n", version,s.sub_count);
      if(version>s.sub_count)
				version=s.sub_count;
    }

    if((version!=0) && (copyandcompile(id,u->getsect(),version,s.lang))) {
      char thismsg[100];
      
      for(int i=0; i<casecount; i++) {
				copytestcase(i+1);
	
				chdir("test");
				score += test(i+1,thismsg);
				chdir("..");
	
				strcat(evmsg,thismsg);
      }
    } else {
      score = 0;
      strcpy(evmsg,"-\ncompile error");
    }
    
    savemessage(id,u->getsect(),evmsg);
    
    submission_db* db = new submission_db(id,u->getsect());
    if(score==fullscore)
      s.estatus=submission_db::PASSED;
    else
      s.estatus=submission_db::FAILED;
    db->setsubstatus(probnum,s);
    delete db;
  } else {
    strcpy(evmsg,"not submitted");
    score = -1;
  }
  delete u;

  cleartestdir();

  if(mlog)
    strcpy(mlog,evmsg);
  return score;
*/
