This wiki page describe how to config the grader.  See HowToInstall for installation instructions.

## How to add users ##

Each user has **user\_id** and **password** (for logging in), **name** (can be full name).  A user can also belongs to a **group**.

There are 3 types of users: administrator (**A**), group supervisor (**S**), and contestant (**C**).  An admin can generate passwords, view score reports, add a list of new users, and view their passwords (so that the passwords can be distributed to users in competition).  A group supervisor can view the scores of all contestants in the group.  To specify user's type, we use `A`, `S`, or `C`.

### How to add individual users ###

To add an individual user, you have to do it directly on the database.  Just insert a row to table `user_info`.  The following shows the structure of `user_info` table:

```
TABLE: user_info 
COLUMNS:
  user_id  varchar(10)  -- username
  name     varchar(100) -- full name
  passwd   varchar(10)  -- password (not encrypted)
  grp      varchar(10)  -- group id
  type     char(1)      -- user type:
                          'C' for contestant
                          'S' for supervisor
                          'A' for admin
```

### How to add bulk of users ###

If you log in as an admin, you can see a menu **upload student info**.  That's the link for adding users.  Prepare a text file that contains users' information, one line per each user, in the following format:

> `username:name:password:type:group`

Note that we use `:` as a field separator.

## How to add problems ##

There are basically two steps: add problem info to the database, and set up evaluation information (i.e., test data).

To add problem to the database, add a row into table `prob_info`, whose fields are described below.

```
TABLE: prob_info 
COLUMNS:
  prob_id      varchar(10)  
  name         varchar(100)  -- the name of the problem
  avail        char(1)       -- shown on the web? either 'Y' or 'N' 
  prob_order   int(10)       -- problems are shown to user ordered by their prob_order
```

To configure problem evaluation information, create a directory named after the problem id (the `prob_id` field above) in `ev` directory.  Copy test data there.  The input files should be named `1.in`, `2.in`, etc; the solution files should be named `1.sol`, `2.sol`,.. etc.  Then create a configuration file named `conf` in that directory; this configuration file specifies the number of test cases, various limits, and evaluators.

Follow the example below:

```
cases: 2
evaluator: ../scomp.exe
timelimit: 1
memorylimit: 2097152
```

The `scomp.exe` is a simple string comparison evaluator.  It strips out all spaces and compare the output with the solution.   You can definitely write your own comparator.  See    scomp's [source code](http://code.google.com/p/fossil-grader/source/browse/grader/trunk/src/comparators/scomp.cc) for example.

## More information about the grader ##

### Contest mode? ###

Currently the grader does not support contest mode.  If you want to run a contest (where contestants' programs are evaluated with example test data and they'll later be graded with real test data), you can have two copies of evaluation (`ev`) directory, one containing problems with sample test data and another with real test data.

You can specify evaluation directory in `grader.conf`, using option `ev_dir`.

When the contest is over, you may want to grade all submissions (not only just ones on the queue).  See the options below.

### Various options for calling grader ###

By just calling `grader`, you run the grader in **queue mode**, i.e., it reads submitted program from the queue and grades each program.  When you run a contest, you usually runs the grader in this mode, but later you want to grade all submissions with the real test data.  You can call the grader with the following options:

  * `grade-all` --- Use this option to grade all submissions.
  * `grade-prob [prob_id1] [prob_id2] ...`  --- Use this option to grade submissions for specific problems.  You can list many problem id's.