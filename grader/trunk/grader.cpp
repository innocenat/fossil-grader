#include <stdio.h>
#include "db_interface.h"
#include "evaluate.h"

#define CONFIG_FILE  "grader.cnf"

//#define CHOOSE_BEST_LANG 1

// configurations
char username[10];
char password[20];
char dbname[20];

void readconfig()
{
  FILE *fp;
  char line[100];
  char cmd[20];
  char val[20];
  
  username[0]='\0';
  password[0]='\0';
  dbname[0]='\0';
  
  if((fp=fopen(CONFIG_FILE,"r"))!=NULL) {
    while(fgets(line,99,fp)!=NULL) {
      if(sscanf(line,"%s %s",cmd,val)==1)
	val[0]='\0';
      if(strcmp(cmd,"username:")==0)
	strcpy(username,val);
      else if(strcmp(cmd,"password:")==0)
	strcpy(password,val);
      else if(strcmp(cmd,"database:")==0)
	strcpy(dbname,val);
    }
  }
  set_db_config(dbname,username,password);
}

void grade(DB *db, char *user_id, char *prob_id, int sub_num)
{
  evaluator ev(db);
  int score;
  char msg[100];
  
  setsubstatus(db,user_id,prob_id,SUBSTATUS_GRADING,0,"");
  ev.readconf(prob_id);
  
#ifndef CHOOSE_BEST_LANG
  score = ev.evaluate(user_id,sub_num,msg);
  
#else
  char msg1[100], msg2[100];
  int s1, s2;
  ev.forcelanguage(EV_LANG_C);
  s1 = ev.evaluate(user_id,sub_num,msg1);
  printf("In C: %d (%s)\n",s1,msg1);
  
  ev.forcelanguage(EV_LANG_CPP);
  s2 = ev.evaluate(user_id,sub_num,msg2);
  printf("In C++: %d (%s)\n",s2,msg2);
  
  if((s1==0) && (s2==0)) {
    // both zero... report one which compiled
    if(strstr(msg1,"error")==NULL)
      strcpy(msg,msg1);
    else
      strcpy(msg,msg2);
  } else if(s1>s2) {
    strcpy(msg,msg1);
    score = s1;
  } else {
    strcpy(msg,msg2);
    score = s2;
  }

#endif
  if(score == ev.getfullscore()) 
    setsubstatus(db,user_id,prob_id,SUBSTATUS_ACCEPTED,score*100/ev.getfullscore(),msg);
  else
    setsubstatus(db,user_id,prob_id,SUBSTATUS_REJECTED,score*100/ev.getfullscore(),msg);
}

void grade(DB *db, char *user_id, char *prob_id)
{
  int snum = findmaxsubnum(db,user_id,prob_id);
  if(snum!=0)
    grade(db, user_id, prob_id, snum);
}

void gradequeue(DB *db)
{
	char user_id[100];
	char prob_id[100];
	int sub_num;

	do {
		fetchqueue(db,user_id,prob_id,&sub_num);
		if(*user_id!='\0') {
			printf("grading: %s/%s/%d\n",user_id,prob_id,sub_num);
			grade(db,user_id,prob_id,sub_num);
		}
	} while(*user_id!='\0');
}

static bool iffileexist(char *fname)
{
	FILE *fp = fopen(fname,"r");

	if(fp!=NULL) {
		fclose(fp);
		return 1;
	} else
		return 0;
}

bool checkexit()
{
	return iffileexist("exit");
}

void stopgrader()
{
	FILE *fp = fopen("exit","w");
	fclose(fp);
	Sleep(2000);
	remove("exit");
}

void gradequeue()
{
	DB *db = connect_db();
//	grade(db,"jittat","test",1);
   char *moving_icon = "-\|/";
   int counter = 0;
	while(1) {
		gradequeue(db);
		Sleep(1000);
		if(checkexit())
			break;
		printf("%c\r",moving_icon[counter]);
		counter++;
		if(counter>=strlen(moving_icon))
		  	counter = 0;
	}
	close_db(db);
}

void gradeone(char *user_id, char *prob_id)
{
	DB *db = connect_db();
	grade(db,user_id,prob_id);
	close_db(db);
}

void gradeprob(char *prob_id)
{
	DB *db = connect_db();
	MYSQL_RES *res;
	MYSQL_ROW row;
	int usercount;

/*
  FILE *fp=fopen("test.tex","w");
	fprintf(fp,"\\documentclass[11pt]{article}\n");
	fprintf(fp,"\\usepackage[thai]{babel}\n");
	fprintf(fp,"\\usepackage{thswitch}\n");
	fprintf(fp,"\\begin{document}\n");
*/
	
	if(!mysql_query(db,"SELECT user_id,name FROM user_info WHERE type='C' OR type='A'")) {
		res = mysql_store_result(db);
		usercount = mysql_num_rows(res);
		for(int i=0; i<usercount; i++) {
			row = mysql_fetch_row(res);
			printf("grading[%s]: %s\n",prob_id,row[0]);
			grade(db,row[0],prob_id);
			//fprintf(fp,"%s,%s\\\\\n",row[0],row[1]);
		}
		mysql_free_result(res);
	}
	close_db(db);	
/*
	fprintf(fp,"\\end{document}\n");
	fclose(fp);
*/
}

void gradeuser(char *user_id)
{
	printf("sorry: incomplete feature\n");
}

void gradeall()
{
  DB *db = connect_db();
  MYSQL_RES *res;
  MYSQL_ROW row;
  int probcount;
  char **plist;
  
  // fetch problem lists
  if(!mysql_query(db,"SELECT prob_id FROM prob_info WHERE avail='Y'")) {
    res = mysql_store_result(db);
    probcount = mysql_num_rows(res);
    
    plist = (char **)malloc(sizeof(char *)*probcount);
    for(int i=0; i<probcount; i++) {
      row = mysql_fetch_row(res);
      plist[i] = strdup(row[0]);
    }
    mysql_free_result(res);
    close_db(db);
    
    for(int j=0; j<probcount; j++) {
      printf("[%s]\n",plist[j]);
      gradeprob(plist[j]);
      free(plist[j]);
    }
    
    free(plist);
  } else {
    close_db(db);
  }
}

/*
parameter:
first argument - command, one of (queue, stop, grade, grade-prob, grade-user, grade-all)
  queue: stands by and grades submission in queue
  stop: stop the current run of another grader in queue-mode
  grade [user_id] [prob_id] : grade recent submission of [prob_id] of [user_id]
  grade-prob [list of prob_id's] : grade submissions of prob of all users
  grade-user [list of user_id's] : grade all submissions of users
  grade-all : grade all submissions of all users

if no argument is given, will work in queue-mode --- 
call another grader with stop command to stop the current run.
*/
main(int argc, char *argv[])
{
  readconfig();
  if(argc!=1) {
    if(strcmp(argv[1],"stop")==0)
      stopgrader();
    else if((strcmp(argv[1],"grade")==0) && (argc==4))
      gradeone(argv[2],argv[3]);
    else if(strcmp(argv[1],"grade-prob")==0) {
      for(int i=0; i<argc-2; i++)
	gradeprob(argv[2+i]);
    } else if(strcmp(argv[1],"grade-user")==0) {
      for(int i=0; i<argc-2; i++)
	gradeuser(argv[2+i]);
    } else if(strcmp(argv[1],"grade-all")==0)
      gradeall();
    else if(strcmp(argv[1],"queue")==0)
      gradequeue();
    else {
      printf("using: grader [queue/stop/grade/grade-prob/grade-user/grade-all] (list...)\n");
    }
  } else
    gradequeue();	  
}
