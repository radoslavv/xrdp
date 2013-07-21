/**
 * xrdp: A Remote Desktop Protocol server.
 *
 * Copyright (C) Jay Sorg 2004-2013
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOG_H
#define LOG_H

//__rka__ presun sem
/* enable threading */
/*#define LOG_ENABLE_THREAD*/

#ifdef LOG_ENABLE_THREAD //__rka__
#include <pthread.h>
#endif //__rka__

#include "arch.h"

/* logging buffer size */
#define LOG_BUFFER_SIZE      1024

/* logging levels */
enum logLevels
{
  LOG_LEVEL_ALWAYS = 0,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG
};

/* startup return values */
enum logReturns
{
  LOG_STARTUP_OK = 0,
  LOG_ERROR_MALLOC,
  LOG_ERROR_NULL_FILE,
  LOG_ERROR_FILE_OPEN,
  LOG_ERROR_NO_CFG,
  LOG_ERROR_FILE_NOT_OPEN,
  LOG_GENERAL_ERROR
};

#define SESMAN_CFG_LOGGING           "Logging"
#define SESMAN_CFG_LOG_FILE          "LogFile"
#define SESMAN_CFG_LOG_LEVEL         "LogLevel"
#define SESMAN_CFG_LOG_ENABLE_SYSLOG "EnableSyslog"
#define SESMAN_CFG_LOG_SYSLOG_LEVEL  "SyslogLevel"

#ifdef DEBUG
//#define LOG_DBG(args...) log_message(LOG_LEVEL_DEBUG, args); //__rka__ 
#define LOG_DBG(args) log_message(LOG_LEVEL_DEBUG, args); //__rka__
#else
//#define LOG_DBG(args...) //__rka__
#define LOG_DBG(args) //__rka__
#endif

struct log_config
{
  char* program_name;
  char* log_file;
  int fd;
  unsigned int log_level;
  int enable_syslog;
  unsigned int syslog_level;
#ifdef LOG_ENABLE_THREAD //__RKA__
  pthread_mutex_t log_lock;
  pthread_mutexattr_t log_lock_attr;
#endif //__RKA__
};

/* internal functions, only used in log.c if this ifdef is defined.*/
#ifdef LOGINTERNALSTUFF

/**
 *
 * @brief Starts the logging subsystem
 * @param l_cfg loggging system configuration
 * @return
 *
 */
enum logReturns DEFAULT_CC
internal_log_start(struct log_config* l_cfg);

/**
 *
 * @brief Shuts down the logging subsystem
 * @param l_cfg pointer to the logging subsystem to stop
 *
 */
enum logReturns DEFAULT_CC
internal_log_end(struct log_config* l_cfg);

/**
 * Converts a log level to a string
 * @param lvl, the loglevel
 * @param str pointer where the string will be stored.
 */
void DEFAULT_CC
internal_log_lvl2str(const enum logLevels lvl, char* str);

/**
 *
 * @brief Converts a string to a log level
 * @param s The string to convert
 * @return The corresponding level or LOG_LEVEL_DEBUG if error
 *
 */
enum logLevels DEFAULT_CC
internal_log_text2level(char* s);

/**
 * A function that init our struct that holds all state and
 * also init its content.
 * @return  LOG_STARTUP_OK or LOG_ERROR_MALLOC
 */
enum logReturns DEFAULT_CC
internalInitAndAllocStruct(void);

/**
 * Read configuration from a file and store the values in lists.
 * @param file
 * @param lc
 * @param param_n
 * @param param_v
 * @param applicationName, the application name used in the log events.
 * @return
 */
enum logReturns DEFAULT_CC
internal_config_read_logging(int file, struct log_config* lc,
                             struct list* param_n,
                             struct list* param_v,
                             const char *applicationName);
/*End of internal functions*/
#endif
/**
 * This function initialize the log facilities according to the configuration
 * file, that is described by the in parameter.
 * @param iniFile
 * @param applicationName, the name that is used in the log for the running application
 * @return LOG_STARTUP_OK on success
 */
enum logReturns DEFAULT_CC
log_start(const char* iniFile, const char* applicationName);

/**
 * An alternative log_start where the caller gives the params directly.
 * @param iniParams
 * @return
 */
enum logReturns DEFAULT_CC
log_start_from_param(const struct log_config *iniParams);
/**
 * Function that terminates all logging
 * @return
 */
enum logReturns DEFAULT_CC
log_end(void);

/**
 * the log function that all files use to log an event.
 * @param lvl, the loglevel
 * @param msg, the logtext.
 * @param ...
 * @return
 */
enum logReturns DEFAULT_CC
log_message(const enum logLevels lvl, const char* msg, ...);

/**
 *
 * @brief Reads configuration
 * @param s translates the strings "1", "true" and "yes" in 1 (true) and
 * other strings in 0
 * @return 0 on success, 1 on failure
 *
 */
int APP_CC text2bool(char* s);

/**
 * This function returns the configured file name for the logfile
 * @param replybuf the buffer where the reply is stored
 * @param bufsize how big is the reply buffer.
 * @return
 */
char *getLogFile(char *replybuf, int bufsize);

#if defined(_WIN32) //__RKA__
  //windows build workaround for syslog.h

  void	closelog(void);
  void	openlog(const char *, int, int);
  void	syslog(int, const char *, ...);

  //if not given by linker
  #ifndef XRDP_LOG_PATH
    #define XRDP_LOG_PATH "C:\\TEMP\\xrdp\\"
  #endif
  //unix definitions not available in Windows
  #define O_SYNC 0
  
  //__RKA - from syslog.h
  /*
   * priorities/facilities are encoded into a single 32-bit quantity, where the
   * bottom 3 bits are the priority (0-7) and the top 28 bits are the facility
   * (0-big number).  Both the priorities and the facilities map roughly
   * one-to-one to strings in the syslogd(8) source code.  This mapping is
   * included in this file.
   *
   * priorities (these are ordered)
   */
  #define	LOG_CRIT	2	/* critical conditions */
  #define	LOG_ERR		3	/* error conditions */
  #define	LOG_WARNING	4	/* warning conditions */
  #define	LOG_INFO	6	/* informational */
  #define	LOG_DEBUG	7	/* debug-level messages */

  /*
   * Option flags for openlog.
   *
   * LOG_ODELAY no longer does anything.
   * LOG_NDELAY is the inverse of what it used to be.
   */
  #define	LOG_PID		0x01	/* log the pid with each message */
  #define	LOG_CONS	0x02	/* log on the console if errors in sending */

  /* facility codes */
  #define	LOG_DAEMON	(3<<3)	/* system daemons */
#endif

#endif
