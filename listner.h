
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>

#include "message.h"

#define MAXEVENTS 64

static int make_socket_non_blocking (int sfd)
{
	int flags, s;

	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
		perror ("fcntl");
		return -1;
	}

	return 0;
}

static int create_and_bind (char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE; /* All interfaces */

	s = getaddrinfo (NULL, port, &hints, &result);
	if (s != 0)
	{
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
		return -1;
	}
	int counter = 0;
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		counter++;
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
			/* We managed to bind successfully! */
			break;
		}		
		printf("<<<<<<<<<<<<<<,,error=%d\n", s);
		close (sfd);
	}
	printf("num_of_addrinfo %d\n", counter);
	if (rp == NULL)
	{
		fprintf (stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo (result);

	return sfd;
}

void startlistning(char *port, int pipes[2])
{
	
	struct epoll_event event;
	struct epoll_event *events;
	int sfd, s;
	int efd;
	sfd = create_and_bind (port);
	if (sfd == -1)
		abort ();

	s = make_socket_non_blocking (sfd);
	if (s == -1)
		abort ();

	s = listen (sfd, SOMAXCONN);
	if (s == -1)
	{
		perror ("listen");
		abort ();
	}


	efd = epoll_create1 (0);
	if (efd == -1)
	{
		perror ("epoll_create");
		abort ();
	}

	event.data.fd = sfd;
	event.events = EPOLLIN | EPOLLET;
	s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
	if (s == -1)
	{
		perror ("epoll_ctl");
		abort ();
	}

	/* Buffer where events are returned */
	events = (epoll_event *)calloc (MAXEVENTS, sizeof( event));
	printf("start\n");
	/* The event loop */
	while (1)
	{
		int n, i;

		n = epoll_wait (efd, events, MAXEVENTS, -1);
		for (i = 0; i < n; i++)
		{
			if ((events[i].events & EPOLLERR) ||
			(events[i].events & EPOLLHUP) ||
			(!(events[i].events & EPOLLIN)))
			{
				fprintf (stderr, "epoll error\n");
				close (events[i].data.fd);
				continue;
			}

			else 
				if (sfd == events[i].data.fd)
				{
					/* We have a notification on the listening socket, which
					means one or more incoming connections. */
					while (1)
					{
						struct sockaddr in_addr;
						socklen_t in_len;
						int infd;
						char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

						in_len = sizeof in_addr;
						infd = accept (sfd, &in_addr, &in_len);
						if (infd == -1)
						{
							if ((errno == EAGAIN) ||
							(errno == EWOULDBLOCK))
							{
								/* We have processed all incoming
								connections. */
								break;
							}
							else
							{
								perror ("accept");
								break;
							}
						}

						s = getnameinfo (&in_addr, in_len,
						hbuf, sizeof hbuf,
						sbuf, sizeof sbuf,
						NI_NUMERICHOST | NI_NUMERICSERV);
						if (s == 0)
						{
							printf("Accepted connection on descriptor %d "
							"(host=%s, port=%s)\n", infd, hbuf, sbuf);
						}

						/* Make the incoming socket non-blocking and add it to the
						list of fds to monitor. */
						s = make_socket_non_blocking (infd);
						if (s == -1)
							abort ();

						event.data.fd = infd;
						event.events = EPOLLIN | EPOLLET;
						s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
						if (s == -1)
						{
							perror ("epoll_ctl");
							abort ();
						}
					}
					continue;
				}
			else
			{
				//working here
				message msg(events[i].data.fd, false);
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				write(pipes[1], &msg, sizeof msg);
			}
		}
	}

	free (events);

	close (sfd);

}

