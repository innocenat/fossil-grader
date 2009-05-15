#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

#include "db_interface.h"

#define MSVC

#define KEEP_COMPILE_MSG  1

#define EV_LANG_NONE      0
#define EV_LANG_C         1
#define EV_LANG_CPP       2
#define EV_LANG_ERROR    -1

struct compiler_config;

class evaluator {
private:
  DB *db;
  char* prob_id;
  char* ev_name;
  char* ev_dir;
  int casecount;
  int fullscore;
  double timelimit;
  int memorylimit;

  char* infname;
  char* outfname;
  
  int language;
  
  int verify(char *vname, int c, char *msg);
  int test(int c, char* msg);
  void copytestcase(int c);
  // int copyandcompile(char* id, char* sect, int subnum);
  void copyoutput(char* user_id, int compiler_index);
  void cleartestdir();
  char* read_compiler_message(char* user_id, int compiler_index);
  void savemessage(char* user_id);

  void fetchsource(char* user_id, int sub_num, char *fname);
  bool fetchandcompile(char *user_id, int sub_num, compiler_config config);
  
  int getlanguage(char *fname);
  
public:
  evaluator(DB *mydb = 0, char* evdir=0);
  ~evaluator();
  
  void readconf(char* probname);
  char* getevname();
  int getcasecount() { return casecount; }
  int getfullscore() { return fullscore; }
  
  void forcelanguage(int l) {language = l;}
  
  int evaluate(char* id, int sub_num, char* mlog=0);
};

#define MAX_COMPILERS  5

struct compiler_config {
  char* name;
  char* c_compilation_command;
  char* cpp_compilation_command;
};

void add_compiler(compiler_config config);

#endif

