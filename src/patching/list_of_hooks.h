/*
Copyright (c)
See http://www.eu-egee.org/partners/ for details on the copyright
holders.  

Licensed under the Apache License, Version 2.0 (the "License")
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
June 29, 2009.
---------------------------------------------------------------------

Last modifications: 
Etienne DUBLE 	-3.0:	Creation

*/

#define PROTOTYPE(p) p
#define PARAMETERS(p) p

HOOK(	accept,
	PROTOTYPE(int accept(int socket, struct sockaddr *address,
		      socklen_t *address_len)),
	PARAMETERS((socket, address, address_len)))

HOOK(	bind,
	PROTOTYPE(int bind(int socket, const struct sockaddr *address,
		      socklen_t address_len)),
	PARAMETERS((socket, address, address_len)))

HOOK(	close,
	PROTOTYPE(int close(int fd)),
	PARAMETERS((fd)))

HOOK(	connect,
	PROTOTYPE(int connect(int s, const struct sockaddr *address,
		      socklen_t address_len)),
	PARAMETERS((s, address, address_len)))

HOOK(	getaddrinfo,
	PROTOTYPE(int getaddrinfo(const char *nodename,
			const char *servname,
			const struct addrinfo *hints,
			struct addrinfo **res)),
	PARAMETERS((nodename, servname, hints, res)))

HOOK(	gethostbyaddr,
	PROTOTYPE(struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type)),
	PARAMETERS((addr, len, type)))

HOOK(	gethostbyaddr_r,
	PROTOTYPE(int gethostbyaddr_r(	const void *addr, socklen_t len, int type,
				struct hostent *ret, char *buf, size_t buflen,
				struct hostent **result, int *h_errnop)),
	PARAMETERS((addr, len, type, ret, buf, buflen, result, h_errnop)))

HOOK(	gethostbyname,
	PROTOTYPE(struct hostent *gethostbyname(const char *name)),
	PARAMETERS((name)))

HOOK(	gethostbyname_r,
	PROTOTYPE(int gethostbyname_r(const char *name,
			struct hostent *ret, char *buf, size_t buflen,
			struct hostent **result, int *h_errnop)),
	PARAMETERS((name, ret, buf, buflen, result, h_errnop)))

HOOK(	getnameinfo,
	PROTOTYPE(GETNAMEINFO_PROTOTYPE),
	PARAMETERS((sa, salen, node, nodelen, service, servicelen, flags)))

HOOK(	getpeername,
	PROTOTYPE(int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen)),
	PARAMETERS((sockfd, addr, addrlen)))

HOOK(	getsockname,
	PROTOTYPE(int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)),
	PARAMETERS((sockfd, addr, addrlen)))

HOOK(	inet_addr,
	PROTOTYPE(in_addr_t inet_addr(const char *cp)),
	PARAMETERS((cp)))

HOOK(	inet_aton,
	PROTOTYPE(int inet_aton(const char *cp, struct in_addr *inp)),
	PARAMETERS((cp, inp)))

HOOK(	inet_ntoa,
	PROTOTYPE(char *inet_ntoa(struct in_addr in)),
	PARAMETERS((in)))

HOOK(	inet_ntop,
	PROTOTYPE(const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)),
	PARAMETERS((af, src, dst, size)))

HOOK(	inet_pton,
	PROTOTYPE(int inet_pton(int af, const char *src, void *dst)),
	PARAMETERS((af, src, dst)))

HOOK(	listen,
	PROTOTYPE(int listen(int sockfd, int backlog)),
	PARAMETERS((sockfd, backlog)))

HOOK(	poll,
	PROTOTYPE(int poll(struct pollfd *fds, nfds_t nfds, int timeout)),
	PARAMETERS((fds, nfds, timeout)))

HOOK(	ppoll,
	PROTOTYPE(int ppoll(struct pollfd *fds, nfds_t nfds,
		       const struct timespec *timeout, const sigset_t *sigmask)),
	PARAMETERS((fds, nfds, timeout, sigmask)))

HOOK(	pselect,
	PROTOTYPE(int pselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds,
	      const struct timespec *timeout, const sigset_t *sigmask)),
	PARAMETERS((nfds, readfds, writefds, errorfds, timeout, sigmask)))

HOOK(	select,
	PROTOTYPE(int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds,
	      struct timeval *timeout)),
	PARAMETERS((nfds, readfds, writefds, errorfds, timeout)))

HOOK(	setsockopt,
	PROTOTYPE(int setsockopt(int sockfd, int level, int optname,
			      const void *optval, socklen_t optlen)),
	PARAMETERS((sockfd, level, optname, optval, optlen)))

HOOK(	socket,
	PROTOTYPE(int socket(int domain, int type, int protocol)),
	PARAMETERS((domain, type, protocol)))
