/**
 * xrdp: A Remote Desktop Protocol server.
 *
 * Copyright (C) Jay Sorg 2004-2012
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
 *
 * default file locations for log, config, etc
 */

#if !defined(FILE_LOC_H)
#define FILE_LOC_H

#if !defined(XRDP_CFG_PATH)
#define XRDP_CFG_PATH "C:\\temp\\xrdp"
#if defined(_WIN32) 
#else
#define XRDP_CFG_PATH "/etc/xrdp"
#endif
#endif

#if !defined(XRDP_PID_PATH)
#if defined(_WIN32) 
#define XRDP_PID_PATH XRDP_CFG_PATH
#else
#define XRDP_PID_PATH "/var/run"
#endif
#endif

#if !defined(XRDP_SBIN_PATH)
#if defined(_WIN32) 
#define XRDP_SBIN_PATH XRDP_PID_PATH
#else
#define XRDP_SBIN_PATH "/usr/local/sbin"
#endif
#endif

#if !defined(XRDP_SHARE_PATH)
#if defined(_WIN32) 
#define XRDP_SHARE_PATH XRDP_PID_PATH
#else
#define XRDP_SHARE_PATH "/usr/local/share/xrdp"
#endif
#endif

#if !defined(XRDP_LIB_PATH)
#if defined(_WIN32) 
#define XRDP_LIB_PATH XRDP_PID_PATH
#else
#define XRDP_LIB_PATH "/usr/local/lib/xrdp"
#endif
#endif

#endif
