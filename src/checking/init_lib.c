/*
Copyright (c) Members of the EGEE Collaboration. 2008. 
See http://www.eu-egee.org/partners/ for details on the copyright
holders.  

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License. 
You may obtain a copy of the License at 

    http://www.apache.org/licenses/LICENSE-2.0 

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
See the License for the specific language governing permissions and 
limitations under the License.

---------------------------------------------------------------------
Developed by Etienne DUBLE - CNRS UREC (http://www.urec.fr)
etienne __dot__ duble __at__ urec __dot__ cnrs __dot__ fr
Nov 25, 2008.
---------------------------------------------------------------------

Last modifications: 
Etienne DUBLE 	-1.0:	Creation
Etienne DUBLE 	-2.0:	Test if interpreted language
Etienne DUBLE 	-2.2:	Added verbose_level
Etienne DUBLE 	-2.2:	Reworked init_lib mechanism and implementation

*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#include "common_other_tools.h"
#include "stack_tools.h"
#include "append_to_string.h"
#include "system_commands.h"
#include "function_state.h"

extern int log_all;
extern int max_function_depth_reported;
extern int interpreted_language;
extern int verbose_level;
extern char *program_command_line;
extern char *program_basename; 
extern char *interpreter_name; 

#define INTERPRETER_MAX_SIZE    32

enum init_lib_state
{
	not_done_state,
	being_done_state,
	done_state
};

void init_lib()
{
	FILE *fp;
	char c[2];
	char *path_of_program_alloc = NULL;
	char *substring, *char_pointer;

	// let's get the full command line of the program for future reference
	c[1] = '\0';
	fp = fopen("/proc/self/cmdline", "r");
	while (!feof(fp))
	{
		fread(c, 1, 1, fp);
		if (c[0] == '\0')
		{
			c[0] = ' ';
		}
		append_to_string(&program_command_line, c);
	}
	fclose(fp);
	program_command_line[strlen(program_command_line)-2] = '\0'; // it ends with two '\0' converted to spaces

	// and the program basename
	// Normally it is the first word of the process command line
	// but in the case of a script, the command line would be for example
	// /usr/bin/python my_script.py
	// so in this case we try to get 'my_script.py' instead of 'python' in order to be more
	// explicit about the program being run.
	substring = strstr(program_command_line, getenv("_"));
	if (substring == NULL)
	{	// we point to the first word
		asprintf(&path_of_program_alloc, "%s", program_command_line);
		
		// we seperate this first word by putting a '\0' after it
		char_pointer = path_of_program_alloc;
		while(*char_pointer != '\0')
		{
			if (isspace(*char_pointer))
			{
				*char_pointer = '\0';
				break;
			}
			char_pointer++;
		}
	}
	else
	{
		asprintf(&path_of_program_alloc, "%s", getenv("_"));
	}

	asprintf(&program_basename, "%s", basename(path_of_program_alloc));
	free(path_of_program_alloc);

	// let's save the interpreter name into the variable interpreter_name
	save_interpreter_name();
	
	// let's check if this program is written in an interpreted language
	// (= if the interpreter is known)
	if (	(strcmp(interpreter_name, "python") == 0) ||
		(strcmp(interpreter_name, "perl") == 0) ||
		(strcmp(interpreter_name, "java") == 0))
	{
		interpreted_language = 1;
	}
	else
	{
		interpreted_language = 0;
	}
	
	// Environment variables
	if (	(getenv("IPV6_LOG_ALL") != NULL) && 
		(strcmp(getenv("IPV6_LOG_ALL"), "1") == 0))
	{
		log_all = 1;
	}

	if (getenv("IPV6_MAX_FUNCTION_DEPTH") != NULL)
	{
		sscanf(getenv("IPV6_MAX_FUNCTION_DEPTH"), "%d", &max_function_depth_reported);
	}

	if (getenv("IPV6_VERBOSE_LEVEL") != NULL)
	{
		sscanf(getenv("IPV6_VERBOSE_LEVEL"), "%d", &verbose_level);
	}
}

void one_time_library_init(enum function_state *state)
{
	static enum init_lib_state __init_lib_state__ = not_done_state;

	switch(__init_lib_state__)
	{
		case not_done_state:
			__init_lib_state__ = being_done_state;
			init_lib();
			__init_lib_state__ = done_state;
			break;
		case being_done_state:
			// if a network related function call is done during the init_lib() procedure
			// this function will be called. 
			// In this case we don't want ipv6-care to analyse this function:
			*state = call_original_function_state;
			break;
		case done_state:
			break;
	}
}
