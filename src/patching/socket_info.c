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
July 29, 2009.
---------------------------------------------------------------------

Last modifications: 
Etienne DUBLE 	-3.0:	Creation

*/

#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "address.h"
#include "socket_info.h"
#include "common_original_functions.h"

struct socket_info_entry {
       struct socket_data data;
       LIST_ENTRY(socket_info_entry) entries;
};

LIST_HEAD(socket_info_list_head_type, socket_info_entry) socket_info_list_head;

int socket_info_list_initialised = 0;

void init_socket_info_list_if_needed()
{
	if (socket_info_list_initialised == 0)
	{
		LIST_INIT(&socket_info_list_head);
		socket_info_list_initialised = 1;
	}
}

struct socket_data *get_socket_info (int fd)
{
	struct socket_info_entry *entry;
	static __thread struct socket_data *result;
	static __thread int last_fd = -1;

	if (fd != last_fd) // optimization in the case of several operations on the same fd
	{
		init_socket_info_list_if_needed ();

		result = NULL;
		for (entry = socket_info_list_head.lh_first; entry != NULL; entry = entry->entries.le_next)
		{
			if (entry->data.fd == fd)
			{	// socket already known
				result = &entry->data;
				break;
			}
		}

		if (result == NULL)
		{	// socket not known yet, register it
			entry = calloc (1, sizeof (struct socket_info_entry));
			entry->data.fd = fd;
			LIST_INSERT_HEAD(&socket_info_list_head, entry, entries);
				
			result = &entry->data;
		}

		last_fd = fd;
	}
	return result;
}

void compute_socket_type (int fd, struct socket_data *data)
{
	unsigned int socktype_size;
	socktype_size = sizeof (data->type);
	getsockopt (fd, 1, 3, (char *) &data->type, &socktype_size);
}

void compute_listening_socket_v6only_option (int fd, struct socket_data *data)
{
	unsigned int sockoption_size;

	sockoption_size = sizeof (data->data_per_state.listening.v6only_option);
	getsockopt (fd, IPPROTO_IPV6, 26, (char *) &data->data_per_state.listening.v6only_option, &sockoption_size);
}

void compute_socket_protocol (int fd __attribute__ ((unused)), struct socket_data *data)
{	// I don't know any way to retrieve the protocol given the file descriptor
	// we will return 0 in this case
	data->protocol = 0;
}

void compute_listening_socket_backlog (int fd __attribute__ ((unused)), struct socket_data *data)
{	// I don't know any way to retrieve the backlog value given the file descriptor
	// we will return a big value in this case
	data->data_per_state.listening.backlog = 128;
}

#define FLAG_DATA_SET_LOCAL_ADDRESS 		0x01
#define FLAG_DATA_SET_REMOTE_ADDRESS 		0x02
#define FLAG_DATA_SET_LISTENING_ADDRESS		0x04
#define FLAG_DATA_SET_SOCKET_TYPE 		0x08
#define FLAG_DATA_SET_SOCKET_PROTOCOL 		0x10
#define FLAG_DATA_SET_SOCKET_BACKLOG 		0x20
#define FLAG_DATA_SET_SOCKET_STATE 		0x40
#define FLAG_DATA_SET_V6ONLY_OPTION		0x80

void compute_socket_state (int fd, struct socket_data *data)
{
	unsigned int option_size, sas_size;
	unsigned int option_listening;
	union u_sockaddr sa;

	option_size = sizeof(option_listening);
	original_getsockopt (fd, SOL_SOCKET, SO_ACCEPTCONN, (char *) &option_listening, &option_size);
	if (option_listening == 1)
	{
		data->state = socket_state_listening;
	}
	else
	{
		sas_size = sizeof (sa.sas);
		if ((original_getpeername(fd, &sa.sa, &sas_size) == -1)
				&& (errno == ENOTCONN))
		{
			data->state = socket_state_created;
		}
		else
		{
			memcpy (&data->data_per_state.communicating.remote_address, &sa,
					sizeof (union u_sockaddr));
			data->state = socket_state_communicating;
			data->flag_data_registered |= FLAG_DATA_SET_REMOTE_ADDRESS;
		}
	}
}

typedef int (*address_filling_function_t) (int, struct sockaddr *, socklen_t *);

void fill_address (int fd, struct polymorphic_sockaddr *psa,
	      address_filling_function_t function)
{
	psa->sa_len = sizeof (psa->sockaddr.sas);
	function(fd, &psa->sockaddr.sa, &psa->sa_len);
}

void compute_listening_socket_address (int fd, struct socket_data *data)
{
	fill_address(fd, &data->data_per_state.listening.address, getsockname);
}

void compute_local_socket_address (int fd, struct socket_data *data)
{
	fill_address(fd, &data->data_per_state.communicating.local_address, getsockname);
}

void compute_remote_socket_address (int fd, struct socket_data *data)
{
	fill_address(fd, &data->data_per_state.communicating.remote_address, getpeername);
}

#define indirection_in_get0	*
#define indirection_in_get1
#define indirection_in_get(_type_is_pointer)	indirection_in_get ## _type_is_pointer

#define __define_get_function(_suffix, _type, _type_is_pointer, _flag, _data_pointer) 	\
_type get_ ## _suffix (int fd)								\
{											\
  struct socket_data *data;								\
  data = get_socket_info (fd);								\
  if ((data->flag_data_registered & _flag) == 0)					\
    {											\
      compute_ ## _suffix (fd, data);							\
      data->flag_data_registered |= _flag;						\
    }											\
  return indirection_in_get(_type_is_pointer)(_data_pointer);				\
}
	

#define indirection_in_set0	&
#define indirection_in_set1
#define indirection_in_set(_type_is_pointer)	indirection_in_set ## _type_is_pointer

#define __define_set_function(_suffix, _type, _type_is_pointer, _flag, _data_pointer)	\
void register_ ## _suffix (int fd, _type value)						\
{											\
  struct socket_data *data;								\
  data = get_socket_info (fd);								\
  memcpy (	_data_pointer, 								\
  		indirection_in_set(_type_is_pointer)value, 				\
		sizeof (*(_data_pointer)));						\
  data->flag_data_registered |= _flag;							\
}


#define __define_get_and_set_functions(_suffix, _type, _type_is_pointer, _flag, _data_pointer)  \
__define_get_function(_suffix, _type, _type_is_pointer, _flag, _data_pointer)		\
__define_set_function(_suffix, _type, _type_is_pointer, _flag, _data_pointer)

__define_get_and_set_functions(socket_type, int, 0, FLAG_DATA_SET_SOCKET_TYPE, &data->type)
__define_get_and_set_functions(socket_state, enum socket_state, 0, FLAG_DATA_SET_SOCKET_STATE, &data->state)
__define_get_and_set_functions(socket_protocol, int, 0, FLAG_DATA_SET_SOCKET_PROTOCOL, &data->protocol)
__define_get_and_set_functions(listening_socket_backlog, int, 0, FLAG_DATA_SET_SOCKET_BACKLOG, 
						&data->data_per_state.listening.backlog)
__define_get_and_set_functions(listening_socket_v6only_option, int, 0, FLAG_DATA_SET_V6ONLY_OPTION, 
						&data->data_per_state.listening.v6only_option)
__define_get_and_set_functions(listening_socket_address, struct polymorphic_sockaddr *, 1, FLAG_DATA_SET_LISTENING_ADDRESS, 
						&data->data_per_state.listening.address)
__define_get_and_set_functions(local_socket_address, struct polymorphic_sockaddr *, 1, FLAG_DATA_SET_LOCAL_ADDRESS, 
						&data->data_per_state.communicating.local_address)
__define_get_and_set_functions(remote_socket_address, struct polymorphic_sockaddr *, 1, FLAG_DATA_SET_REMOTE_ADDRESS, 
						&data->data_per_state.communicating.remote_address)

