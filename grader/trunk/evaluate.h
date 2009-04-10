#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

#include "db_interface.h"

#define MSVC

#define KEEP_COMPILE_MSG  1

#define EV_LANG_NONE      0
#define EV_LANG_C         1
#define EV_LANG_CPP       2
#define EV_LANG_ERROR    -1

class evaluator {
private:
	DB *db;
	char* prob_id;
	char* evname;
	int casecount;
	int fullscore;
	int timelimit;

	char* infname;
	char* outfname;

	int language;

	int test(int c, char* msg);
	void copytestcase(int c);
//	int copyandcompile(char* id, char* sect,
//										int subnum);
	void copyoutput(char* user_id);
	void cleartestdir();
	void savemessage(char* user_id);

	void fetchsource(char* user_id, int sub_num, char *fname);
	bool fetchandcompile(char *user_id, int sub_num);

	int getlanguage(char *fname);

public:
	evaluator(DB *mydb = 0);
	~evaluator();

	void readconf(char* probname);
	char* getevname();
	int getcasecount() { return casecount; }
	int getfullscore() { return fullscore; }

  void forcelanguage(int l) {language = l;}

	int evaluate(char* id, int sub_num, char* mlog=0);
};

#endif

