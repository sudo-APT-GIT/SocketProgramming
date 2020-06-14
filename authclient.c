#include "headers.h"
int main()
{
	int sockfd,retval;
	int recedbytes,sentbytes,x,i;
	struct sockaddr_in serveraddr;
	char source[MAXSIZE], destination[MAXSIZE], msg[MAXSIZE];
	int num_to_be_booked[5], numbers[10];

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("[-]Socket Creation Error");
		exit(0);
	}
	printf("%s\n", "[+]Socket Created");

	memset(&serveraddr, '\0', sizeof(serveraddr));

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval<0)
	{
		printf("[-]Connection error");
		exit(0);
	}
	printf("%s\n", "[+]Connected to server");

	while(1)
	{
		
		printf("Please enter source: ");
		scanf("%s", source);
		printf("Please enter destination: ");
		scanf("%s", destination);
		
		// send source
		sentbytes=send(sockfd,source,sizeof(source),0);
		printf("%s\n", "sending source...");
		if(sentbytes==-1)
		{
			printf("[-]Error while sending!");
			close(sockfd);
			exit(0);
		}

		// send destination
		sentbytes=send(sockfd,destination,sizeof(destination),0);
		printf("%s\n", "sending destination...");
		if(sentbytes==-1)
		{
			printf("[-]Error while sending!");
			close(sockfd);
			exit(0);
		}
		printf("%s\n", "source and destination sent...");
		printf("%s\n", "Waiting for reply...");

		// num of seats available and num of booked
		recedbytes = recv(sockfd, numbers, sizeof(numbers), 0);
		if(recedbytes<0){
			printf("%s\n","[-]Error while recieving" );
			break;
		}
	
		if(numbers[0] != 109)
		{
			printf("Number of seats available: %d\n", numbers[0]);
			printf("Number of seats booked: %d\n", numbers[1]);

			// send number of seats to be booked
			printf("\nPlease enter number to be booked: ");
			scanf("%d", &num_to_be_booked[0]); 

			sentbytes=send(sockfd,num_to_be_booked,sizeof(num_to_be_booked),0);
			printf("sending %d...\n", num_to_be_booked[0]);
			if(sentbytes==-1)
			{
				printf("[-]Error while sending!");
				close(sockfd);
				exit(0);
			}
			
			// recv success or failure
			recedbytes = recv(sockfd, msg, sizeof(msg), 0);
			if(recedbytes<0){
				printf("%s\n","[-]Error while recieving" );
				break;
			}
			puts(msg);
			break;
		}
		else
		{
			printf("%s\n", "[-]No Trains available");
		}	

	}
	close(sockfd);
	return 0;
}