/*peter.c  SERVER*/
#include "common.h"
#include "server.h"
#include "init.c"
int main()
{
	REGISTER *ppp, *p, reg;
	TRANS data_bag;
	LOGIN log_bag;
	int OK;
	int select_count;
	int send_fd;
	int recvbytes;
	int send_size;

	int sockfd, client_fd;
	int sin_size;
	struct sockaddr_in my_addr, remote_addr;
	
	fd_set rfd_set, wfd_set, efd_set, afd_set;
	struct timeval timeout;
	int ret;

	ppp = (REGISTER*)malloc(sizeof(REGISTER));
	ppp->next = NULL;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	bzero(&my_addr, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVPORT);
	inet_aton("127.0.0.1", &my_addr.sin_addr);
	if(bind(sockfd, (struct sockaddr *)& my_addr, sizeof(struct sockaddr))\
			== -1)
	{
		perror("bind");
		exit(1);
	}
	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}
	sin_size = sizeof(struct sockaddr_in);
	fcntl(sockfd, F_SETFD, O_NONBLOCK);
	FD_ZERO(&afd_set);
	FD_SET(sockfd, &afd_set);
	select_count = sockfd + 1;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	while(1)
	{
		FD_ZERO(&rfd_set);
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		rfd_set = afd_set;
		ret = select(select_count, &rfd_set, NULL, NULL, &timeout);
		if(ret == 0)
		{
			//printf("select continue\n"); ///////////
			continue;   // 超时继续运行
		}
		if(ret < 0)
		{
			perror("select error:");
			exit(-1);
		}
		if(FD_ISSET(sockfd, &rfd_set))
		{
			if((client_fd = accept(sockfd, (struct sockaddr*) & remote_addr, &sin_size)) == -1)
			{
				perror("accept");
				continue;
				//exit(1);
			}
			printf("client IP:%s port:%d try login\n", \
					inet_ntoa((struct in_addr)remote_addr.sin_addr),\
					remote_addr.sin_port);     ////////
			recvbytes = recv(client_fd, (char*)&log_bag, sizeof(LOGIN), 0);
			if(recvbytes < sizeof(LOGIN))
	{
		printf("recv error : recvbytes = %d,and the size of LOGIN is %d\n", recvbytes, sizeof(LOGIN));
		close(client_fd);
	}
			//printf("logdata size is %d\n", recvbytes);    //////
			//OK = check(ppp, &log_bag);
			//print_log_bag(&log_bag);  //////////
			OK = 1;
			send_size = send(client_fd, (char*)&OK, sizeof(int), 0);
			if(send_size < sizeof(int))
			{
				printf("send error : send_size = %d,and the size of ACK is %d\n", send_size, sizeof(int));
				printf("disconnect the socket %d\n", client_fd);
				close(client_fd);
			}
			printf("\nthe client sockfd:%d\n", client_fd);
			if(OK)
			{
				strcpy(reg.ID, log_bag.ID);
				strcpy(reg.name, log_bag.name);
				strcpy(reg.passwd, log_bag.passwd);
				reg.socket_ID = client_fd;

				insert(ppp, reg);
				FD_SET(client_fd, &afd_set);
				fcntl(client_fd, F_SETFD, O_NONBLOCK);
				if(client_fd > select_count)
					select_count = client_fd + 1;
				else
					select_count++;
			}
			else
			{
				printf("close the sockfd:%d\n", client_fd);
				close(client_fd);
			}
		}
		//printf("\ninsert into register\n");
		//print_register(ppp);  /////////

		p = ppp->next;
		while(p!=NULL)
		{
			if(FD_ISSET(p->socket_ID, &rfd_set))
			{
				//printf("sao miao socket:%d\n",p->socket_ID);
				recvbytes = recv(p->socket_ID,(char*)&data_bag, \
						sizeof(TRANS), 0);
				if(recvbytes < sizeof(TRANS))
				{
					printf("recv error : recvbytes = %d,and the size of LOGIN is %d\n", recvbytes, sizeof(TRANS));
					printf("disconnect the socket %d\n", p->socket_ID);
					FD_CLR(p->socket_ID, &afd_set);
					delet(ppp, p->socket_ID);
					close(p->socket_ID);
				}
				//print_trans_bag(&data_bag);
				send_fd = found_sockfd(ppp, data_bag.ID);
				send_size = send(send_fd, (char*)&data_bag, sizeof(TRANS), 0);
				if(send_size < sizeof(TRANS))
				{
					printf("send error : send_size = %d,and the size of ACK is %d\n", send_size, sizeof(TRANS));
					printf("disconnect the socket %d\n", p->socket_ID);
					FD_CLR(p->socket_ID, &afd_set);
					delet(ppp, p->socket_ID);
					close(p->socket_ID);
				}
			}
			p = p->next;
		}
/*
			else
				printf("lai guo\n");
			//printf("\ncurrent Node:\n");
			//print_Node(p);
		}
        get the IP and port of client 
	    printf("%s \t %d \n", inet_ntoa((struct in_addr)remote_addr.sin_addr.s),\
			remote_addr.sin_port);
*/
	}
	return 0;
}
