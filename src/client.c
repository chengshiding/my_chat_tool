#include "common.h"
#include "client.h"
#include "init.c"
int main(void)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	fd_set rfd_set, wfd_set, efd_set;
	struct timeval timeout;
	int ret;

	LOGIN login;
	TRANS recv_da;
	TRANS send_da;
	int ACK;
	int recvbytes;
	int send_size;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	bzero(&serv_addr, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVPORT);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr))\
			== -1)
	{
		perror("connect");
		exit(1);
	}
	fcntl(sockfd, F_SETFD, O_NONBLOCK);

/* login */
	printf("Enter the ID:\n");
	scanf("%s", login.ID);
	printf("Enter the name:\n");
	scanf("%s", login.name);
	printf("Enter the passwd:\n");
	scanf("%s", login.passwd);
	send_size = send(sockfd, (char*)&login, sizeof(LOGIN), 0);
	if(send_size < sizeof(LOGIN))
	{
		printf("send error : send_size = %d,and the size of LOGIN is %d\n", send_size, sizeof(LOGIN));
		close(sockfd);
		return 0;
	}
	recvbytes = recv(sockfd, (char*)&ACK, sizeof(int), 0);
	if(!ACK)
	{
		printf("login failed\n");
		close(sockfd);
		exit(-1);
	}
	else if(recvbytes!=sizeof(int))
	{
		if(recvbytes == 0 || recvbytes == -1)
		{
			if(recvbytes == 0)
				printf("recvbytes = 0,close the socket:%d\n", sockfd);
			else
				printf("recvbytes = -1 ,close the socket:%d\n", sockfd);
			close(sockfd);
			exit(0);
		}
	}
	else
		printf("login successed\n");
/* end */

	//printf("Lucy: ");
	fflush(stdout);
	while(1)
	{
		FD_ZERO(&rfd_set);
		FD_ZERO(&efd_set);
		FD_SET(STDIN, &rfd_set);
		FD_SET(sockfd, &rfd_set);
		FD_SET(sockfd, &efd_set);
		timeout.tv_sec = 1000;
		timeout.tv_usec = 600000;
		ret = select(sockfd +1,&rfd_set, NULL, &efd_set, &timeout);
		if(ret == 0)
		{
			//printf("timeout");
			//break;
			continue;
		}
		if(ret < 0)
		{
			perror("select error: ");
			exit(-1);
		}
		if(FD_ISSET(STDIN, &rfd_set))
		{
			init_trans(&send_da);
			printf("Enter the ID of side:");
			scanf("%s", send_da.ID);
			//printf("Enter your name:");
			//scanf("%s", send_da.name);
			strcpy(send_da.name, login.name);
			printf("Enter the your want to send the datas:\n");
			getchar();
			//fgets(send_da.data, MAXDATASIZE, stdin);
			//send_da.data[strlen(send_da.data) -1] = '\0';
			gets(send_da.data);
			if(strncmp("quit", send_da.data, 4) == 0)
			{
				close(sockfd);
				exit(0);
			}
			send_size = send(sockfd, (char*)&send_da, sizeof(TRANS), 0);
			if(send_size < sizeof(TRANS))
			{
				printf("send error : send_size = %d,and the size of TRANS is %d\n", send_size, sizeof(TRANS));
				close(sockfd);
				return 0;
			}
						
		}
		if(FD_ISSET(sockfd, &rfd_set))
		{
			init_trans(&recv_da);
			recvbytes = recv(sockfd, (char*)&recv_da, sizeof(TRANS), 0);
			if(recvbytes == 0 || recvbytes == -1)
			{
				if(recvbytes == 0)
					printf("recvbytes = 0,close the socket:%d\n", sockfd);
				else
					printf("recvbytes = -1 ,close the socket:%d\n", sockfd);
				close(sockfd);
				exit(0);
			}
			recv_da.data[strlen(recv_da.data)] = '\0';
			printf("*************%s said:******************\n\
					%s\n", recv_da.name, recv_da.data);
			fflush(stdout);
		}
		if(FD_ISSET(sockfd, &efd_set))
		{
			printf("efd_set of socket:%d found, close the socket\n", sockfd);
			close(sockfd);
			exit(0);
		}
	}
	return 0;
}
