/*
** showip.c -- show IP addresses for a host given on the command line
*/

#define	FOR_WINDOWS
#undef	FOR_LINUX

#if defined(FOR_WINDOWS)
#define _WIN32_WINNT _WIN32_WINNT_WIN8 		// make sure that header ws2tcpip.h used is recent and that _WIN32_WINNT is defines as 0x6000 or later
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#elif defined(FOR_LINUX)
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
#if defined(FOR_WINDOWS)
        WSADATA wsaData;

	// Initialize use of Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
	if (argc != 2) {
	    fprintf(stderr,"usage: showip hostname\n");
//	    return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("office_2", NULL, &hints, &res)) != 0)       // hardcoded hostname, didn't use argv[1]
        {
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s:\n\n", "office_2");			// hardcoded hostname, didn't use argv[1]

	for(p = res;p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("  %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res); // free the linked list
#if defined(FOR_WINDOWS)
        Sleep(5000);
#elif defined(FOR_LINUX)
        sleep(5);
#endif
	return 0;
}
