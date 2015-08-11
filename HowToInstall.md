## Introduction ##

There are two components in fossil-grader: a php web application and a grader, which is a console application.

## Requirements ##
  * Web server software with php support, e.g., Apache
  * MySQL

## How to install ##

### Download software ###

  * Download [web submission app](http://fossil-grader.googlecode.com/files/web-submission-0.5.zip), extract, and put it in somewhere accessible through web server.
  * Download [grader](http://fossil-grader.googlecode.com/files/grader-0.51.zip), extract, and put it somewhere.  It shouldn't be accessible from the web.

### Set up database ###

  * Create a database on MySQL.  Note the username, the password, and the database name.
  * Import initial data to the database.  Run sql commands in `init_db.sql` from web-submission dir.
  * The imported initial data comes with root account.  The default password is `serverroot`.  You **must** change this.

Note that the system provides no support for adding individual users or adding problems, you would do that directly to the database.  We usually use [phpmyadmin](http://www.phpmyadmin.net/) or some other interface to MySQL to do this.

### Web submission ###

  * Edit `config.php`.  Mainly edit these lines:
```
    //for MySQL
    define("MYSQL_USER","your_user_name_here");
    define("MYSQL_PASSWD","your_password_herer");
    define("MYSQL_DATABASE","your_database_name_here");
```

> (see example in config.php.SAMPLE.)

### Grader ###

  1. Get the web submission working first.
  1. Edit `grader.conf` (see example from grader.conf.SAMPLE)
    * Add database config
    * Check command line for compilation, especially on paths
  1. Edit test data (by default they are in ev/, see example in ev/add)
  1. Make sure you set paths correctly.
    * Make sure you have libmysql.dll on the path, or you can just copy it from mysql dir to grader's dir.
    * If you want to use Turbo C, make sure tlink.exe is on the path.
    * To change path settings, go to
> > > Settings -> Control Panel -> System -> Advanced -> Environment Variables

> > Then add the required paths in PATH variable.
> > For example, if you want to add Turbo C bin path, you can put "C:\TC\BIN;" in front of all other paths,
> > if your Turbo C executables, e.g., tcc.exe and tlink.exe are in C:\TC\BIN.
  1. Start grader by calling `grader.exe`.  The grader should start grading when you submit programs to it.

If you make any changes on the config, don't forget to restart the grader.  (Restart it using the usual way: Ctrl-C.)

By just calling `grader`, you run the grader in **queue mode**, i.e., it reads submitted program from the queue and grades each program.  See HowToConfig for other options to run the grader and how to use it in programming contest.

### What's next? ###

To add users and set up problems, please see HowToConfig