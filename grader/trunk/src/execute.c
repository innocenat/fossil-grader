/*
This library is a modification from a program called trun, taken from
an unknown source.  (FIX THIS)

*/
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include "execute.h"

/* 
==How execute works==

===Start up===
Set up basic configurations: input file, output file
into STARTUPINFO struct to be passed to CreateProcess.

Create a child process with CreateProcess.

===Wait===
Use WaitForSingleObject to wait.

===Killing chile process===
This process is really involved, because (1) programs in
DOS mode actually runs inside NTVDM so killing them
requires to kill NTVDM, (2) something a program crashes
NTVDM and a dialog box pops up, and we need to close
that dialog box MANUALLY, and (3) for Win32 apps that crash,
some reporting service in Windows opens a dialog box,
and it has to be killed.

Those extra steps are what's exactly done here:
1. Kill the process if there's any
2. In case that there's no real process, find NTVDM 
and kill it (repeatedly until it's gone)
3. Check if NTVDM crashed and some warning dialog opens,
if there's any, signal the user and wait.
4. For real Win32 apps, find process "dwwin.exe" which
represents an agent for reporting service and also 
opens a dialog.  If finds it, kill it (repeatedly)
until it's gone.

Step 4. might be problematic --- dwwin.exe might not
be a universal process for error reporting services???
*/



/*
These are routines that check NTVDM crash dialog.
It works by enumerating all window titles, and
checks for "16 bit" or something with ".exe" somewhere 
and starts with "cmd.exe".
*/
bool NTVDMcrashed_found;

/* this is a callback for window title enumeration */
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
  char buffer[256];
  GetWindowText(hWnd, buffer, 256);
  
  if(strlen(buffer)!=0) {
    if(strstr(buffer,"16 bit")!=0) {
      NTVDMcrashed_found = true;
    }
    if((strstr(buffer,".exe")!=0) && 
       (strstr(buffer,"cmd.exe")==buffer)) {
      NTVDMcrashed_found = true;
      printf("Title: %s\n",buffer);
    }
  }
  return TRUE;
}

bool check_ntvdm_dialog()
{
  NTVDMcrashed_found = false;

  FARPROC EnumProcInstance = MakeProcInstance((FARPROC)EnumWindowsProc,
					      AfxGetInstanceHandle());
  EnumWindows((WNDENUMPROC)EnumProcInstance, (LPARAM)0);
  FreeProcInstance(EnumProcInstance);
  
  return NTVDMcrashed_found;
}

DWORD get_process_id(char *pname)
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;
  DWORD pid=0;

  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE ) {
    return 0;
  }
  
  pe32.dwSize = sizeof( PROCESSENTRY32 );
  if( !Process32First( hProcessSnap, &pe32 ) ) {
    CloseHandle( hProcessSnap ); 
    return 0;
  }

  do {
    if(strcasecmp(pe32.szExeFile ,pname)==0)
      pid = pe32.th32ProcessID;
  } while( Process32Next( hProcessSnap, &pe32 ) );
  
  CloseHandle( hProcessSnap );
  return pid;
}

DWORD get_ntvdm_pid()
{
  return get_process_id("ntvdm.exe");
}

void kill_error_report()
{
  DWORD pid;
  do {
    if((pid = get_process_id("dwwin.exe"))!=0) {
      fprintf(stderr," -- with error report (pid: %ld)\n",pid);
      HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid);
      if(hProcess!=NULL) {
	TerminateProcess(hProcess, 0);
	Sleep(500);
	while(get_process_id("dwwin.exe")==pid) {
	  fprintf(stderr,"wait...\n");
	  Sleep(500);
	}
      } else
	fprintf(stderr,"do not have permission (%d)\n",
		GetLastError());
    }
  } while(get_process_id("dwwin.exe")!=0);
}

void wait_dialog()
{
  kill_error_report();
  if(check_ntvdm_dialog()) {
    fprintf(stderr,"Some dialog opens; please MANUALLY kill it.");
    fflush(stderr);
    do {
      Sleep(1000);
    } while(check_ntvdm_dialog());
    fprintf(stderr,"... done\n");
  }
}

void setstartupinfo(STARTUPINFO *si, char *inname, char *outname)
{
  SECURITY_ATTRIBUTES sa;

  ZeroMemory(&sa, sizeof(sa));
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;
  
  si->dwFlags = STARTF_USESTDHANDLES;
  if((inname!=0) && (strcmp(inname,"-")!=0)) {
    si->hStdInput = CreateFile(inname,
			       FILE_READ_DATA, 
			       FILE_SHARE_READ,
			       &sa,
			       OPEN_EXISTING,
			       FILE_ATTRIBUTE_NORMAL,
			       NULL);
  } else
    si->hStdInput = NULL;
  
  if((outname!=0) && (strcmp(outname,"-")!=0)) {
    si->hStdOutput = CreateFile(outname,
				FILE_WRITE_DATA, 
				FILE_SHARE_READ,
				&sa,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
  } else
    si->hStdOutput = NULL;
  
  si->hStdError = NULL;
}


int execute(char *exname, char *inname, char *outname, double t)
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  int ifsuccess = 1;
  
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  
  setstartupinfo(&si, inname, outname);
  
  if(!CreateProcess( NULL,  // No module name (use command line). 
		     TEXT(exname), // Command line. 
		     NULL,  // Process handle not inheritable. 
		     NULL,  // Thread handle not inheritable. 
		     TRUE,  // Set handle inheritance to FALSE. 
		     0,     // No creation flags. 
		     NULL,  // Use parent's environment block. 
		     NULL,  // Use parent's starting directory. 
		     &si,   // Pointer to STARTUPINFO structure.
		     &pi))  // Pointer to PROCESS_INFORMATION structure. 
    {
      //printf( "CreateProcess failed (%d).\n", GetLastError() );
    }
  //fprintf(stderr,"Process ID: %ld\n",pi.dwProcessId);
  //fprintf(stderr,"time limit = %d\n",t);
  if(WaitForSingleObject( pi.hProcess, (int)(t*1000)+1)==WAIT_TIMEOUT) {
    // need to kill...
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
    
    if(hProcess != NULL) {
      fprintf(stderr,"killing pid: %ld\n",pi.dwProcessId);
      TerminateProcess(hProcess, 0);
      wait_dialog();
    } else {
      DWORD dwNtvdmId = get_ntvdm_pid();
      fprintf(stderr,"killing (ntvdm) pid: %ld\n",dwNtvdmId);
      if(dwNtvdmId!=0) {
	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwNtvdmId);
	TerminateProcess(hProcess, 0);
      } else {
        fprintf(stderr,"killing process error\n");
      }

      if(get_ntvdm_pid()!=0) {
	fprintf(stderr,"killing error, ntvdm.exe still remains;");
	fprintf(stderr,"please MANUALLY kill it.");
	fflush(stderr);
	do {
	  Sleep(1000);
	} while(get_ntvdm_pid()!=0);
	fprintf(stderr,"... done\n");
	wait_dialog();
      }
    }
    ifsuccess = 0;
  }
  wait_dialog();
  if(si.hStdInput!=NULL)
    CloseHandle(si.hStdInput);
  if(si.hStdOutput!=NULL)
    CloseHandle(si.hStdOutput);
  
  return ifsuccess;
}

int verify(char *vname, int c, char *msg)
{
  char inname[20];
  char outname[20];
  char solname[20];
  char scorename[20];
  char cmd[200];
  char line[100];
  FILE *sfp;
  int score;
  
  sprintf(inname,"%d.in",c);
  sprintf(outname,"%d.out",c);
  sprintf(solname,"%d.sol",c);
  sprintf(scorename,"%d.score",c);
  
  sprintf(cmd,"%s %s %s %s",vname,inname,outname,solname);
  execute(cmd,"-",scorename,100);
  
  sfp = fopen(scorename,"r");
  score=0;
  *msg = '\0';
  if(sfp!=NULL) {
    fgets(line,99,sfp);
    sscanf(line,"%d",&score);
    if(fgets(line,99,sfp)!=NULL)
      strcpy(msg,line);
    fclose(sfp);
  }
  return score;
}

