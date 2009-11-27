#!/bin/bash
#
# Copyright (c) Members of the EGEE Collaboration. 2008. 
# See http://www.eu-egee.org/partners/ for details on the copyright
# holders.  
# 
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
# 
#     http://www.apache.org/licenses/LICENSE-2.0 
# 
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and 
# limitations under the License.
# 
# ---------------------------------------------------------------------
# Developed by Etienne DUBLE - CNRS UREC (http://www.urec.fr)
# etienne __dot__ duble __at__ urec __dot__ cnrs __dot__ fr
# Oct 19, 2009.
# ---------------------------------------------------------------------
# 
# Last modifications: 
# Etienne DUBLE 	-3.0:	Creation

# ---------------------------------------------------------------------
# This is a special processing in order to get a command as 
# similar as the one which was really typed.
#
# The reason for this is that, for example, if you have a 
# script called 'execute.sh' with just the following code:
# -----------
# #!/bin/bash
# eval "$*"
# -----------
# then running the following command:
# $ execute.sh echo "a                      b"
# would incorrectly return:
# a b
# 
# With this code we want to avoid this kind of problems.
# ---------------------------------------------------------------------

# command itself
echo -n "$1"
shift

# arguments
while [ "$*" != "" ]
do	# put double-quotes in order to separate
	# each argument
	echo -n " \"$1\""
	shift
done

