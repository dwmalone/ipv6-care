# Copyright (c) Centre National de la Recherche Scientifique (CNRS,
# France). 2010. 
# Copyright (c) Members of the EGEE Collaboration. 2008-2010. See 
# http://www.eu-egee.org/partners/ for details on the copyright
# holders.  
#
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#
#    http://www.apache.org/licenses/LICENSE-2.0 
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and 
# limitations under the License.

# AX_CHECK_GETNAMEINFO_PROTOTYPE()
# -----------------------------------------------
# Check what is the form of the prototype of getnameinfo()
AC_DEFUN([AX_CHECK_GETNAMEINFO_PROTOTYPE],
[
AC_CACHE_CHECK([what kind of prototype getnameinfo() has],
                [ax_cv_getnameinfo_proto],
 [AC_TRY_COMPILE([
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
              char *node, socklen_t nodelen, char *service,
              socklen_t servicelen, int flags)
{
        return 0;
}
], [], [ax_cv_getnameinfo_proto=linux], [
AC_TRY_COMPILE([
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
              char *node, socklen_t nodelen, char *service,
              socklen_t servicelen, unsigned int flags)
{
        return 0;
}
], [], [ax_cv_getnameinfo_proto=old-linux], [
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *node, size_t nodelen, char *service, size_t servicelen, int flags)
{
	return 0;
}
], [], [ax_cv_getnameinfo_proto=freebsd], [ax_cv_getnameinfo_proto=unknown])]
)])])

if test "$ax_cv_getnameinfo_proto" = "linux"; then
  AC_DEFINE([GETNAMEINFO_PROTOTYPE], [int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, int flags)],
            [Record the prototype of getnameinfo().])
fi

if test "$ax_cv_getnameinfo_proto" = "old-linux"; then
  AC_DEFINE([GETNAMEINFO_PROTOTYPE], [int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, unsigned int flags)],
            [Record the prototype of getnameinfo().])
fi

if test "$ax_cv_getnameinfo_proto" = "freebsd"; then
  AC_DEFINE([GETNAMEINFO_PROTOTYPE], [int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *node, size_t nodelen, char *service, size_t servicelen, int flags)],
            [Record the prototype of getnameinfo().])
fi

if test "$ax_cv_getnameinfo_proto" = "unknown"; then
  AC_MSG_ERROR([could not recognise the prototype of getnameinfo()])
fi

])

