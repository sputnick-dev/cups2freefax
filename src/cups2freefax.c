/*
   ------------------------------------------------------------------
      This program is free software; you can redistribute it and/or
      modify it under the terms of version 2 of the GNU General Public
      License published by the Free Software Foundation.
   ------------------------------------------------------------------

   cups2freefax is a fork of cups-pdf backend :

   cups-pdf.c -- CUPS Backend 
   08.02.2003, Volker C. Behr
   Exp. Physik V, Uni Wuerzburg 
   vrbehr@cip.physik.uni-wuerzburg.de
   http://cip.physik.uni-wuerzburg.de/~vrbehr/cups-pdf

   This code may be freely distributed as long as this header 
   is preserved. 

   This code is distributed under the GPL.
   (http://www.gnu.org/copyleft/gpl.html)

   ---------------------------------------------------------------------------

   Copyright (C) 2003-2008  Volker C. Behr

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   ---------------------------------------------------------------------------
  
   If you want to redistribute modified sources/binaries this header
   has to be preserved and all modifications should be clearly 
   indicated.
   In case you want to include this code into your own programs 
   I would appreciate your feedback via email.

  
   HISTORY: 
   2008-06-22 : 2.4.8      - fixed too small allocation of postprocessing string,
                             corrected typo in the config file documentation,
                             update for the SELinux .te-file in contrib/
   2008-03-24 : 2.4.7      - corrected the exit codes to match CUPS' specifications,
                             unmodified username passed to PostProcessing as 3rd arg,
                             made PPD file auto-selectable via IEEE-1284 device id
   2007-05-04 : 2.4.6      - optimized default options for GS to produce PDFs
   2007-02-27 : 2.4.5      - enclosing parentheses () are removed from title,
                             set a more comprehensive identification string
   2007-02-02 : 2.4.4      - DOS-style path separators '\' are now recognized
   2006-11-22 : 2.4.3      - replaced 'setenv' by 'putenv' to increase compatibility
   2006-09-30 : 2.4.2      - several code opimizations,
                             PostScript PPD now uncompressed and rev4 only
   2006-06-13 : 2.4.1      - added a PDF-to-mail postprocessing script to contrib/,
                             added a new revision of the PostscriptColor PPD file
   2006-05-28 : 2.4.0      - created the contrib/ directory with additional software
   2006-05-17 : 2.3.1	   - fixed possible read access after end of string,
                             fixed recognition of international characters 
   2006-05-14 : 2.3.0	   - experimental option to decode hex strings in title (NK)
   2006-04-14 : 2.2.0      - new option to remove prefixes from usernames
   2006-04-06 : 2.1.1      - minor bugfix to read umasks properly
   2006-03-25 : 2.1.0	   - new option to select where to look for title,
                             some code changes and optimizations
   2006-02-26 : 2.0.5      - several code optimizations
   2006-01-29 : 2.0.4      - beginning of (E)PS-code now determined by "%!",
                             optimized system call to GS in cups-pdf.conf,
              		     some minor code changes,
			     debug information shows version number,
      			     updated PostScript PPD file supporting ledger paper
   2006-01-07 : 2.0.3	   - improved checking for root privileges (incl. EUID)
   2006-01-04 : 2.0.2	   - fixed possible testing of an uninitialized variable
   2006-01-03 : 2.0.1	   - fixed hanging jobs when logging is disabled
   2005-12-23 : 2.0.0      - creation of non-existent parent directories,
                             improved configurability of output directories,
			     option CPHOMESUB is now obsolete,
                             PPD file now matches Adobe specifications
   2005-10-23 : 2.0beta2   - added support for post-processing script,
                             fixed a bug when parsing cups-pdf.conf
   2005-09-18 : 2.0beta1   - added support for runtime configuration,
                             introduced cups-pdf.conf,
                             made AnonDirName an abolute path
   2005-09-08 : 1.7.4      - added some options to cups-pdf.h for MacOSX
   2005-07-23 : 1.7.3      - fixed GhostScript failure with error 11,
                             fixed segfault when called without root privileges
   2005-07-07 : 1.7.2	   - adjustment of file mode moved to non-privileged child
   2005-06-16 : 1.7.1	   - PDFs are protected during creation by file mode 000
   2005-03-07 : 1.7.0a
   2005-03-01 : 1.7.0      - call of GhostScript from a non-privileged child process,
                             no more additional programs in the call of GhostScript,
                             several code optimizations
   2005-02-09 : 1.6.6      - updated PostScript PPD file supporting more DIN formats
   2005-01-04 : 1.6.5      - cups-pdf is running with gid of group defined in conf.grp
   2004-10-11 : 1.6.4      - improved configurability for different versions of 'su'
   2004-10-07 : 1.6.3      - corrected erroneous call of GhostScript
   2004-10-05 : 1.6.2      - added necessary quotes in the call of GhostScript
   2004-10-03 : 1.6.1      - allow users w/o valid shell in /etc/passwd,
                             several code optimizations
   2004-09-08 : 1.6.0      - new option to check against user names in lower case,
                             new option to label all created PDFs with a job-id
   2004-08-10 : 1.5.2      - fixed insecure creation of spoolfile,
     			     several smaller code optimizations
   2004-08-09 : 1.5.1	   - fixed possible access to element outside allocated area,
                             fixed possible conflict with overlapping memory areas
   2004-08-03 : 1.5.0      - call to GS without root privileges for enhanced security
   2004-08-01 : 1.4.3      - added -dSAFER to the call to GS to improve security
   2004-02-25 : 1.4.2a
   2004-02-24 : 1.4.2      - improved extraction of filenames,
                             handling of embedded (e)ps code (RVT)
   2004-02-09 : 1.4.1	   - logfile is created with mode 644 if it does not exist
   2004-01-27 : 1.4.0      - new option to set permissions for PDF output,
                             alternative options for improved image conversion
                             (moved conf.gscall to cups-pdf.h)
   2004-01-06 : 1.3.2	   - exit status of ghostscript gets logged in debug mode
   2004-01-02 : 1.3.1a
   2003-11-29 : 1.3.1	   - improved handling of domain prefix
   2003-11-02 : 1.3	   - new option to cut file name extensions
   2003-10-25 : 1.2        - untitled documents are labelled with job-id,
			     new option to add a domain prefix to user names,
                             removed a minor bug during character replacement
   2003-08-02 : 1.1	   - files not viewable until creation is finished,
   			     new qualifier "$HOME" for output directory,
			     "$HOME"-related new define in the header file
   2003-07-17 : 1.0.1	   - improved user name handling for multi-word names,
                             code now distributed under the GPL
   2003-06-29 : 1.0	   - enhanced debug output,
   			     replacement of special characters in title,
			     new option in the header file
   2003-05-26 : 1.0pre3    - several minor bugfixes and improvements 
   2003-05-21 : 1.0pre2    - optimized memory management
   2003-05-16 : 1.0pre1    - introduced cups-pdf.h, 
			     code now entirely written in C,
 			     cups-pdfgen is obsolete
   2003-02-14 : 0.2    - script re-located to /usr/lib/cups/filter/cups-pdfgen
   2003-02-12 : 0.1    - first beta release			                */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "cups2freefax.h"

extern int errno;

static FILE *logfp=NULL;

static void log_event(short type, char message[], char *detail) {
  time_t secs;
  char ctype[8], *timestring;

  if (strlen(conf.log) && (type & conf.logtype)) { 
    (void) time(&secs);
    timestring=ctime(&secs);
    timestring[strlen(timestring)-1]='\0';

    if (type == CPERROR) 
      sprintf(ctype, "ERROR");
    else if (type == CPSTATUS)
      sprintf(ctype, "STATUS");
    else
      sprintf(ctype, "DEBUG");
    if (detail != NULL)  {
      while (detail[strlen(detail)-1] == '\n') 
        detail[strlen(detail)-1]='\0';
      fprintf(logfp,"%s  [%s] %s (%s)\n", timestring, ctype, message, detail);
      if ((conf.logtype & CPDEBUG) && (type & CPERROR)) 
        fprintf(logfp,"%s  [DEBUG] ERRNO: %d\n", timestring, errno);
    }
    else
      fprintf(logfp,"%s  [%s] %s\n", timestring, ctype, message);
  }
  return;
}

static void read_config() {
  FILE *fp;
  cp_string buffer, key, value;

  fp=fopen(CPCONFIG,"r");
  if (fp==NULL)
    return;

  while (fgets(buffer, BUFSIZE, fp) != NULL) {
    key[0]='\0';
    value[0]='\0';
    if (sscanf(buffer,"%s %[^\n]",key,value)) {
      if (!strlen(key) || !strncmp(key,"#",1))
        continue;
      if (!_assign_value(key, value))
        (void) fputs("Error in configuration file!\n", stderr);
    }
  }

  (void) fclose(fp);
  return;
}

static int create_dir(char *dirname, int nolog) {
  struct stat fstatus;
  char buffer[BUFSIZE],*delim;
  int i;

  while ((i=strlen(dirname))>1 && dirname[i-1]=='/')
    dirname[i-1]='\0';
  if (stat(dirname, &fstatus) || !S_ISDIR(fstatus.st_mode)) {
    strncpy(buffer,dirname,BUFSIZE);
    delim=strrchr(buffer,'/');
    if (delim!=buffer)
      delim[0]='\0';
    else
      delim[1]='\0';
    if (create_dir(buffer,nolog)!=0)
      return 1;
    (void) stat(buffer, &fstatus);
    if (mkdir(dirname,fstatus.st_mode)!=0) {
      if (!nolog)
        log_event(CPERROR, "failed to create directory", dirname);
      return 1;
    }
    else
      if (!nolog)
        log_event(CPSTATUS, "directory created", dirname);
    if (chown(dirname,fstatus.st_uid,fstatus.st_gid)!=0)
      if (!nolog)
        log_event(CPDEBUG, "failed to set owner on directory (non fatal)", dirname);
  }
  return 0;
}
    
static int init() {
  struct stat fstatus;
  struct group *group;
  cp_string logfile;

  _set_defaults();
  if (strlen(CPCONFIG))
    if (!stat(CPCONFIG, &fstatus) && 
        (S_ISREG(fstatus.st_mode) || S_ISLNK(fstatus.st_mode)))
      read_config();

  (void) umask(0077);

  group=getgrnam(conf.grp);
  if (group)
    (void) setgid(group->gr_gid);

  if (strlen(conf.log)) {
    if (stat(conf.log, &fstatus) || !S_ISDIR(fstatus.st_mode)) {
      if (create_dir(conf.log, 1)) 
        return 1;
      if (chmod(conf.log, 0700))
        return 1;
      log_event(CPSTATUS, "log directory created", conf.log);
    }
    snprintf(logfile,BUFSIZE,"%s%s",conf.log,"/cups2freefax_log");
    logfp=fopen(logfile, "a");
  }

  if (!group) {
    log_event(CPERROR, "Grp not found", conf.grp);
    return 1;
  }
  else 
    log_event(CPDEBUG, "switching to new gid", conf.grp);

  (void) umask(0022);

  if (stat(conf.spool, &fstatus) || !S_ISDIR(fstatus.st_mode)) {
    if (create_dir(conf.spool, 0)) {
      log_event(CPERROR, "failed to create spool directory", conf.spool);
      return 1;
    }
    if (chmod(conf.spool, 0751)) {
      log_event(CPERROR, "failed to set mode on spool directory", conf.spool);
      return 1;
    }
    if (chown(conf.spool, -1, group->gr_gid)) 
      log_event(CPERROR, "failed to set group id on spool directory (non fatal)", conf.grp);
    log_event(CPSTATUS, "spool directory created", conf.spool);
  }

  (void) umask(0077);
  return 0;
}

static char *preparedirname(struct passwd *passwd, char *uname) {
  int size;
  char bufin[BUFSIZE], bufout[BUFSIZE], *needle, *cptr;

  needle=strstr(uname, conf.removeprefix);
  if ((int)strlen(uname)>(size=strlen(conf.removeprefix)))
    uname=uname+size;

  strncpy(bufin, conf.out, BUFSIZE);
  do {
    needle=strstr(bufin, "${HOME}");
    if (needle == NULL)
      break;
    needle[0]='\0';
    cptr=needle+7;
    snprintf(bufout, BUFSIZE, "%s%s%s", bufin, passwd->pw_dir, cptr);
    strncpy(bufin, bufout, BUFSIZE);
  } while (needle != NULL);
  do {
    needle=strstr(bufin, "${USER}");
    if (needle == NULL)
      break;
    needle[0]='\0';
    cptr=needle+7;
    if (!conf.dirprefix)
      snprintf(bufout, BUFSIZE, "%s%s%s", bufin, uname, cptr);
    else
      snprintf(bufout, BUFSIZE, "%s%s%s", bufin, passwd->pw_name, cptr);
    strncpy(bufin, bufout, BUFSIZE);
  } while (needle != NULL);
  size=strlen(bufin)+1;
  cptr=calloc(size, sizeof(char));
  if (cptr == NULL)
    return NULL;
  snprintf(cptr,size,"%s",bufin);
  return cptr;
}

static int prepareuser(struct passwd *passwd, char *dirname) {
  struct stat fstatus;

  (void) umask(0000);
  if (stat(dirname, &fstatus) || !S_ISDIR(fstatus.st_mode)) {
    if (!strcmp(passwd->pw_name, conf.anonuser)) {
      if (create_dir(dirname, 0)) {
        log_event(CPERROR, "failed to create anonymous output directory", dirname);
        return 1;
      }
      if (chmod(dirname, (mode_t)(0777&~conf.anonumask))) {
        log_event(CPERROR, "failed to set mode on anonymous output directory", dirname);
        return 1;
      }
      log_event(CPDEBUG, "anonymous output directory created", dirname);
    }
    else {
      if (create_dir(dirname, 0)) {
        log_event(CPERROR, "failed to create user output directory", dirname);
        return 1;
      }
      if (chmod(dirname, (mode_t)(0777&~conf.userumask))) {
        log_event(CPERROR, "failed to set mode on user output directory", dirname);
        return 1;
      }
      log_event(CPDEBUG, "user output directory created", dirname);
    }
    if (chown(dirname, passwd->pw_uid, passwd->pw_gid)) {
      log_event(CPERROR, "failed to set owner for output directory", passwd->pw_name);
      return 1;
    }
    log_event(CPDEBUG, "owner set for output directory", passwd->pw_name);
  }
  (void) umask(0077);
  return 0;
}

/* no validation is done here, please use is_ps_hex_string for that */
static void decode_ps_hex_string(char *string) {
  char *src_ptr, *dst_ptr;
  int is_lower_digit; 					/* 0 - higher digit, 1 - lower digit */
  char number, digit;

  dst_ptr=string; 					/* we should always be behind src_ptr,
                   			   		   so it's safe to write over original string */
  number=(char)0;
  is_lower_digit=0;
  for (src_ptr=string+1;*src_ptr != '>';src_ptr++) { 	/* begin after start marker */
    if (*src_ptr == ' ' || *src_ptr == '\t' ) {		/* skip whitespace */
      continue;
    }
    if (*src_ptr >= 'a') {				/* assuming 0 < A < a */
      digit=*src_ptr-'a'+(char)10;
    }
    else if (*src_ptr >= 'A') {
      digit=*src_ptr-'A'+(char)10;
    }
    else {
      digit=*src_ptr-'0';
    }
    if (is_lower_digit) {
      number|=digit;
      *dst_ptr=number;					/* write character */
      dst_ptr++;
      is_lower_digit=0;
    }
    else { 						/* higher digit */
      number=digit<<4;
      is_lower_digit=1;
    }
  }
  if (is_lower_digit) {					/* write character with lower digit = 0,
    							   as per PostScript Language Reference */
    *dst_ptr=number;
    dst_ptr++;
    /* is_lower_digit=0; */
  }
  *dst_ptr=0;						/* finish him! */
  return;
}

static int is_ps_hex_string(char *string) {
  int got_end_marker=0;
  char *ptr;
  
  if (string[0] != '<') { 	/* if has no start marker */
    log_event(CPDEBUG, "not a hex string, has no start marker", string);
    return 0;              		/* not hex string, obviously */
  }
  for (ptr=string+1;*ptr;ptr++) { 	/* begin after start marker */
    if (got_end_marker) { 		/* got end marker and still something left */
      log_event(CPDEBUG, "not a hex string, trailing characters after end marker", ptr);
      return 0;           		/* that's bad! */
    }
    else if (*ptr == '>') { 	/* here it is! */
      got_end_marker=1;
      log_event(CPDEBUG, "got an end marker in the hex string, expecting 0-termination", ptr);
    }
    else if ( !(
      isxdigit(*ptr) ||
      *ptr == ' ' ||
      *ptr == '\t'
    ) ) {
      log_event(CPDEBUG, "not a hex string, invalid character", ptr);
      return 0;    			/* that's bad, too */
    }
  }
  return got_end_marker;
}

static void alternate_replace_string(char *string) {
  unsigned int i;

  log_event(CPDEBUG, "removing alternate special characters from title", string);
  for (i=0;i<(unsigned int)strlen(string);i++)
   if ( isascii(string[i]) &&      	/* leaving non-ascii characters intact */
        (!isalnum(string[i])) && 
        string[i] != '-' && string[i] != '+' )
    string[i]='_';
  return;
}

static void replace_string(char *string) {
  unsigned int i;

  log_event(CPDEBUG, "removing special characters from title", string);
  for (i=0;i<(unsigned int)strlen(string);i++) 
    if ( ( string[i] < '0' || string[i] > '9' ) &&
         ( string[i] < 'A' || string[i] > 'Z' ) &&
         ( string[i] < 'a' || string[i] > 'z' ) &&
         string[i] != '-' && string[i] != '+' )
      string[i]='_';
  return;
}

static int preparetitle(char *title) {
  char *cut;
  
  if (title != NULL) {
    log_event(CPDEBUG, "removing trailing newlines from title", title);
    while (strlen(title) && ((title[strlen(title)-1] == '\n') || (title[strlen(title)-1] == '\r')))
      title[strlen(title)-1]='\0';
  }
  if (conf.decodehexstrings) {
    log_event(CPSTATUS, "***Experimental Option: DecodeHexStrings", NULL);
    log_event(CPDEBUG, "checking for hex strings", title);
    if (is_ps_hex_string(title))
      decode_ps_hex_string(title);
  }
  if (strlen(title) && title[0]=='(' && title[strlen(title)-1]==')') {
    log_event(CPDEBUG, "removing enclosing parentheses () from full title", title);
    title[strlen(title)-1]='\0';
    memmove(title, title+1, strlen(title));
  }
  cut=strrchr(title, '/');
  if (cut != NULL) {
    log_event(CPDEBUG, "removing slashes from full title", title);
    memmove(title, cut+1, strlen(cut+1)+1);
  }  
  cut=strrchr(title, '\\');
  if (cut != NULL) {
    log_event(CPDEBUG, "removing backslashes from full title", title);
    memmove(title, cut+1, strlen(cut+1)+1);
  }
  cut=strrchr(title, '.');
  if ((cut != NULL) && ((int)strlen(cut) <= conf.cut+1) && (cut != title)) {
    log_event(CPDEBUG, "removing file name extension", cut);
    cut[0]='\0';
  }
  if (conf.decodehexstrings) {
    log_event(CPDEBUG, "calling alternate_replace_string", NULL);
    alternate_replace_string(title);
  }
  else
    replace_string(title);
  return strcmp(title, "");
}

static int preparespoolfile(FILE *fpsrc, char *spoolfile, char *title, char *cmdtitle, 
                     int job, struct passwd *passwd) {
  cp_string buffer;
  int rec_depth,is_title=0;
  FILE *fpdest;

  if (fpsrc == NULL) {
    log_event(CPERROR, "failed to open source stream", NULL);
    return 1;
  }
  log_event(CPDEBUG, "source stream ready", NULL);
  fpdest=fopen(spoolfile, "w");
  if (fpdest == NULL) {
    log_event(CPERROR, "failed to open spoolfile", spoolfile);
    (void) fclose(fpsrc);
    return 1;
  }
  log_event(CPDEBUG, "destination stream ready", spoolfile);
  if (chown(spoolfile, passwd->pw_uid, -1)) {
    log_event(CPERROR, "failed to set owner for spoolfile", spoolfile);
    return 1;
  }
  log_event(CPDEBUG, "owner set for spoolfile", spoolfile);
  rec_depth=0;
  while (fgets(buffer, BUFSIZE, fpsrc) != NULL) {
    if (!strncmp(buffer, "%!", 2)) {
      log_event(CPDEBUG, "found beginning of postscript code", buffer);
      break;
    }
  }
  log_event(CPDEBUG, "now extracting postscript code", NULL);
  (void) fputs(buffer, fpdest);
  while (fgets(buffer, BUFSIZE, fpsrc) != NULL) {
    (void) fputs(buffer, fpdest);
    if (!is_title && !rec_depth)
      if (sscanf(buffer, "%%%%Title: %"TBUFSIZE"c", title)==1) {
        log_event(CPDEBUG, "found title in ps code", title);
        is_title=1;
      }
    if (!strncmp(buffer, "%!", 2)) {
      log_event(CPDEBUG, "found embedded (e)ps code", buffer);
      rec_depth++;
    }
    else if (!strncmp(buffer, "%%EOF", 5)) {
      if (!rec_depth) {
        log_event(CPDEBUG, "found end of postscript code", buffer);
        break;
      }
      else {
        log_event(CPDEBUG, "found end of embedded (e)ps code", buffer);
        rec_depth--;
      }
    }
  }
  (void) fclose(fpdest);
  (void) fclose(fpsrc);
  log_event(CPDEBUG, "all data written to spoolfile", spoolfile);

  if (!strcmp(cmdtitle, "(stdin)")) 
    cmdtitle="";
  if (conf.titlepref) {
    log_event(CPDEBUG, "trying to use commandline title", cmdtitle);
    if (!preparetitle(cmdtitle)) {
      log_event(CPDEBUG, "empty commandline title, using PS title", title);
      if (!preparetitle(title))
        log_event(CPDEBUG, "empty PS title", NULL); 
    }
    else
      snprintf(title, BUFSIZE, "%s", cmdtitle);
  }
  else {
    log_event(CPDEBUG, "trying to use PS title", title);
    if (!preparetitle(title)) {
      log_event(CPDEBUG, "empty PS title, using commandline title", cmdtitle);
      if (!preparetitle(cmdtitle))
        log_event(CPDEBUG, "empty commandline title", NULL);
      else
        snprintf(title, BUFSIZE, "%s", cmdtitle);
    }
  }          

  if (!strcmp(title, "")) { 
    snprintf(title, BUFSIZE, "job_%i-untitled_document", job);
    log_event(CPDEBUG, "no title found - using default value", title);
  }
  else {
    if (conf.label) {
      strcpy(buffer, title);
      snprintf(title, BUFSIZE, "job_%i-%s", job, buffer);
    }
    log_event(CPDEBUG, "title successfully retrieved", title);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  char *user, *dirname, *spoolfile, *outfile, *gscall, *ppcall;
  cp_string title="";
  int size;
  mode_t mode;
  struct passwd *passwd;
  pid_t pid;

  if (setuid(0)) {
    (void) fputs("cups2freefax cannot be called without root privileges!\n", stderr);
    return 0;
  }

  if (init()) 
    return 5;
  log_event(CPDEBUG, "initialization finished", CPVERSION);

  if (argc==1) {
    printf("file cups2freefax:/ \"CUPS2FREEFAX\" \"CUPS2FREEFAX\" \"MFG:Generic;MDL:CUPS2FREEFAX Printer;DES:Generic CUPS2FREEFAX Printer;CLS:PRINTER;CMD:POSTSCRIPT;\"\n"); 
    log_event(CPSTATUS, "identification string sent", NULL);
    return 0;
  }
  if (argc<6 || argc>7) {
    (void) fputs("Usage: cups2freefax job-id user title copies options [file]\n", stderr);
    log_event(CPERROR, "call contained illegal number of arguments", NULL);
    return 0;
  }

  size=strlen(conf.userprefix)+strlen(argv[2])+1;
  user=calloc(size, sizeof(char));
  if (user == NULL) {
    (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
    return 5;
  }  
  snprintf(user, size, "%s%s", conf.userprefix, argv[2]);
  passwd=getpwnam(user);
  if (passwd == NULL && conf.lowercase) {
    log_event(CPDEBUG, "unknown user", user);
    for (size=0;size<(int) strlen(argv[2]);size++) 
      argv[2][size]=tolower(argv[2][size]);
    log_event(CPDEBUG, "trying lower case user name", argv[2]);
    size=strlen(conf.userprefix)+strlen(argv[2])+1;
    snprintf(user, size, "%s%s", conf.userprefix, argv[2]);
    passwd=getpwnam(user);
  }  
  if (passwd == NULL) {
    if (strlen(conf.anonuser)) {
      passwd=getpwnam(conf.anonuser);
      if (passwd == NULL) {
        log_event(CPERROR, "username for anonymous access unknown", conf.anonuser);
        free(user);
        if (logfp!=NULL)
          (void) fclose(logfp);
        return 5;
      }
      log_event(CPDEBUG, "unknown user", user);
      size=strlen(conf.anondirname)+4;
      dirname=calloc(size, sizeof(char));
      if (dirname == NULL) {
        (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
        free(user);
        if (logfp!=NULL)
          (void) fclose(logfp);
        return 5;
      }  
      snprintf(dirname, size, "%s", conf.anondirname);
      while (strlen(dirname) && ((dirname[strlen(dirname)-1] == '\n') ||
             (dirname[strlen(dirname)-1] == '\r')))
        dirname[strlen(dirname)-1]='\0';
      log_event(CPDEBUG, "output directory name generated", dirname);
    }
    else {
      log_event(CPSTATUS, "anonymous access denied", user);
      free(user);
      if (logfp!=NULL)
        (void) fclose(logfp);
      return 0;
    }
    mode=(mode_t)(0666&~conf.anonumask);
  } 
  else {
    log_event(CPDEBUG, "user identified", passwd->pw_name);
    if ((dirname=preparedirname(passwd, argv[2])) == NULL) {
      (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
      free(user);
      if (logfp!=NULL)
        (void) fclose(logfp);
      return 5;
    }  
    while (strlen(dirname) && ((dirname[strlen(dirname)-1] == '\n') ||
           (dirname[strlen(dirname)-1] == '\r')))
      dirname[strlen(dirname)-1]='\0';
    log_event(CPDEBUG, "output directory name generated", dirname);
    mode=(mode_t)(0666&~conf.userumask);
  }
  free(user);
  if (prepareuser(passwd, dirname)) {
    free(dirname);
    if (logfp!=NULL)
      (void) fclose(logfp);
    return 5;
  }
  log_event(CPDEBUG, "user information prepared", NULL);

  size=strlen(conf.spool)+22;
  spoolfile=calloc(size, sizeof(char));
  if (spoolfile == NULL) {
    (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
    free(dirname);
    if (logfp!=NULL)
      (void) fclose(logfp);
    return 5;
  }
  snprintf(spoolfile, size, "%s/cups2pdf-%i", conf.spool, (int) getpid());
  log_event(CPDEBUG, "spoolfile name created", spoolfile);

  if (argc == 6) {
    if (preparespoolfile(stdin, spoolfile, title, argv[3], atoi(argv[1]), passwd)) {
      free(dirname);
      free(spoolfile);
      if (logfp!=NULL)
        (void) fclose(logfp);
      return 5;
    }
    log_event(CPDEBUG, "input data read from stdin", NULL);
  }
  else {
    if (preparespoolfile(fopen(argv[6], "r"), spoolfile, title, argv[3], atoi(argv[1]), passwd)) {
      free(dirname);
      free(spoolfile);
      if (logfp!=NULL)
        (void) fclose(logfp);
      return 5;
    }
    log_event(CPDEBUG, "input data read from file", argv[6]);
  }  

  size=strlen(dirname)+strlen(title)+6;
  outfile=calloc(size, sizeof(char));
  if (outfile == NULL) {
    (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
    free(dirname);
    free(spoolfile);
    if (logfp!=NULL)
      (void) fclose(logfp);
    return 5;
  }
  snprintf(outfile, size, "%s/%s.pdf", dirname, title);
  log_event(CPDEBUG, "output filename created", outfile);

  size=strlen(conf.gscall)+strlen(conf.ghostscript)+strlen(conf.pdfver)+strlen(outfile)+strlen(spoolfile)+6;
  gscall=calloc(size, sizeof(char));
  if (gscall == NULL) {
    (void) fputs("CUPS2FREEFAX: failed to allocate memory\n", stderr);
    free(dirname);
    free(spoolfile);
    free(outfile);
    if (logfp!=NULL)
      (void) fclose(logfp);
    return 5;
  }
  snprintf(gscall, size, conf.gscall, conf.ghostscript, conf.pdfver, outfile, spoolfile);
  log_event(CPDEBUG, "ghostscript commandline built", gscall);

  (void) unlink(outfile);
  log_event(CPDEBUG, "output file unlinked", outfile);

  if (putenv(conf.gstmp)) {
    log_event(CPERROR, "insufficient space in environment to set TMPDIR", conf.gstmp);
    free(dirname);
    free(spoolfile);
    free(outfile);
    free(gscall);
    if (logfp!=NULL)
      (void) fclose(logfp);
    return 5;
  }
  log_event(CPDEBUG, "TMPDIR set for GhostScript", getenv("TMPDIR"));

  if (logfp!=NULL)
    (void) fflush(logfp);
  pid=fork();

  if (!pid) {
    log_event(CPDEBUG, "entering child process", NULL);

    if (setgid(passwd->pw_gid))
      log_event(CPERROR, "failed to set GID for current user", NULL);
    else 
      log_event(CPDEBUG, "GID set for current user", NULL);
    if (setuid(passwd->pw_uid))
      log_event(CPERROR, "failed to set UID for current user", passwd->pw_name);
    else 
      log_event(CPDEBUG, "UID set for current user", passwd->pw_name);
     
    (void) umask(0077);
    size=system(gscall);
    snprintf(title,BUFSIZE,"%d",size);
    log_event(CPDEBUG, "ghostscript has finished", title);
    if (chmod(outfile, mode)) {
      log_event(CPERROR, "failed to set file mode for PDF file (non fatal)", outfile);
      return 5;
    }
    log_event(CPDEBUG, "file mode set for user output", outfile);
    
    if (strlen(conf.postprocessing)) {
      size=strlen(conf.postprocessing)+strlen(outfile)+strlen(passwd->pw_name)+strlen(argv[2])+4;
      ppcall=calloc(size, sizeof(char));
      if (ppcall == NULL) 
        log_event(CPERROR, "failed to allocate memory for postprocessing (non fatal)", NULL);
      else {
        snprintf(ppcall, size, "%s %s %s %s", conf.postprocessing, outfile, passwd->pw_name, argv[2]);
        log_event(CPDEBUG, "postprocessing commandline built", ppcall);
        size=system(ppcall);
        snprintf(title,BUFSIZE,"%d",size);
        log_event(CPDEBUG, "postprocessing has finished", title);
        free(ppcall);
      }
    }
    else
     log_event(CPDEBUG, "no postprocessing", NULL);

    return 0;
  }
  log_event(CPDEBUG, "waiting for child to exit", NULL);
  (void) waitpid(pid,NULL,0);

  if (unlink(spoolfile)) 
    log_event(CPERROR, "failed to unlink spoolfile (non fatal)", spoolfile);
  else 
    log_event(CPDEBUG, "spoolfile unlinked", spoolfile);

  free(dirname);
  free(spoolfile);
  free(outfile);
  free(gscall);
  
  log_event(CPDEBUG, "all memory has been freed", NULL);

  log_event(CPSTATUS, "PDF creation successfully finished", passwd->pw_name);

  if (logfp!=NULL)
    (void) fclose(logfp);
  return 0;
} 
