#include "tcp_server.h"
#include "lwip/sockets.h"

 
char ReadBuff[BUFF_SIZE];
 

void vTCPServer_Task(void)
{ 
    int sfd, cfd;
    int read_len;                                
    struct sockaddr_in server_addr, client_addr; 
    socklen_t client_addr_len;
	
	err_t err;
 
    sfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(sfd < 0)
	{
		printf("创建失败\r\n");
	}
	else
	{
    printf("创建成功\r\n");
	}
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    err = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
    printf("绑定失败\r\n");
	}
	else
	{
    printf("绑定成功\r\n");
	}

    err = listen(sfd, 5); 
	if(err < 0)
	{
   printf("监听失败\r\n");
	}
	else
	{
    printf("监听成功\r\n");
	}
  
	 printf("等待客户端连接 \r\n");
 
	 while(1)
	 	{
		 client_addr_len = sizeof(client_addr);
		 cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len); 
			printf("IP:%s,端口号%d,客户端连接成功\r\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			
		 while (1)
		 {
			 read_len = read(cfd, ReadBuff, BUFF_SIZE);
			 if(read_len <= 0)
			 	{
			 	break;
			 	}
			 for (int i = 0; i < read_len; i++)
			 {
				 ReadBuff[i] = toupper(ReadBuff[i]);
			 }
			 write(cfd, ReadBuff, read_len);
		 
			 printf("IP:%s,端口号%d,客户端发送数据：%s\r\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),ReadBuff);
		 }
		 
		 
		 printf("IP:%s,端口号%d,客户端断开连接\r\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	    closesocket(cfd);
 
	 	}
}
