#include "common.h"
int check(REGISTER* p_reg, LOGIN* log)
{
	REGISTER* r;
	r = p_reg->next;
	while(r!=NULL)
	{
		if((!strcmp(log->ID, r->ID)))
		{
			if(!(strcmp(log->passwd, r->passwd)))
				return 1;
			else
			{
				printf("passwd error\n");
				return 0;
			}
		}
		r = r->next;
	}
	printf("ID not found\n");
	return 0;
}
int found_sockfd(REGISTER* p_reg, char* it_ID)
{
	REGISTER* r;
	r = p_reg->next;
	while(r!=NULL)
	{
		if(!(strcmp(r->ID, it_ID)))
			return r->socket_ID;
		else
			r = r->next;
	}
	return 0;
}
int insert(REGISTER* p_reg, REGISTER Node)
{
	REGISTER *s;
	s = (REGISTER*)malloc(sizeof(REGISTER));
	strcpy(s->ID,Node.ID);
	strcpy(s->name,Node.name);
	strcpy(s->passwd,Node.passwd);
	s->socket_ID = Node.socket_ID;
	//strcpy(s->IP,Node.IP);
	//s->port = Node.port;

	s->next = p_reg->next;
	p_reg->next = s;
	return 0;
}
int delet(REGISTER* p_reg,int del_fd)
{
	REGISTER *p, *last;
	last = p_reg;
	p = p_reg->next;
	while(p!=NULL)
	{
		if(p->socket_ID == del_fd)
		{
			last->next = p->next;
			free(p);
			break;
		}
		else
		{
			p = p->next;
			last = last->next;
		}
	}
	return 0;
}
int init_trans(TRANS* trans)
{
	memset(trans->ID, '\0', strlen(trans->ID));
	memset(trans->name, '\0', strlen(trans->name));
	memset(trans->data, '\0', strlen(trans->data));
	return 0;
}
void print_log_bag(LOGIN* log)
{
	printf("client ID: %s\n", log->ID);
	printf("client name: %s\n", log->name);
	printf("client passwd: %s\n", log->passwd);
}
void print_register(REGISTER* reg)
{
	REGISTER* tmp = reg->next;
	while(tmp!=NULL)
	{
		printf("register:ID %s\n", tmp->ID);
		printf("register:name %s\n", tmp->name);
		printf("register:passwd %s\n", tmp->passwd);
		printf("register:socket_ID %d\n", tmp->socket_ID);
		tmp = tmp->next;															
	}
}
void print_trans_bag(TRANS* trans)
{
	printf("trans: ID %s\n", trans->ID);
	printf("trans: name %s\n", trans->name);
	printf("trans: data %s\n", trans->data);
}
void print_Node(REGISTER* tmp)
{
	printf("register:ID %s\n", tmp->ID);
	printf("register:name %s\n", tmp->name);
	printf("register:passwd %s\n", tmp->passwd);
	printf("register:socket_ID %d\n", tmp->socket_ID);
}
