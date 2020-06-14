#include "headers.h"
struct Tickets
{
	char src[MAXSIZE];
	char dest[MAXSIZE];
	int num_available;
	int num_booked;
};

void add_data(struct Tickets *data);

int main()
{
	int sockfd,newSocket,retval,x,recedbytes,sentbytes, num_to_be_booked[5], numbers[10], nbytes;
	socklen_t actuallen;
	struct sockaddr_in serveraddr,clientaddr;
	char src[MAXSIZE], dest[MAXSIZE], string_num_to_be_booked[MAXSIZE], readbuffer[MAXSIZE];
	pid_t childpid;
	char failureMsg[] = "[-]Seats Full sorry try again";
	char successMsg[] = "[+]Booking done!";
	struct Tickets data[2];
	add_data(&data);
	int fd[2], fd_t[2];
	

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("[-]Socket creation error");
		exit(0);
	}
	printf("%s\n","[+]Socket created");

	memset(&serveraddr, '\0', sizeof(serveraddr));

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);

	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("[-]Binding error");
		close(sockfd);
		exit(0);
	}
	printf("%s\n","[+]Socket Binded" );

	retval=listen(sockfd,10);
	if(retval==-1)
	{
		printf("%s\n","[-]Error listening");
		close(sockfd);
		exit(0);
	}

	printf("%s\n","[-]Listening...." );

	while(1)
	{
		newSocket = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
		if(newSocket < 0)
		{
			exit(1);
		}

		printf("Connection accepted from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		pipe(fd);
		pipe(fd_t);
		
		if((childpid = fork()) == 0){
			close(fd[0]);
			close(fd_t[0]);
			close(sockfd);

			while(1){
				// get src 
				recv(newSocket, src, sizeof(src), 0);
				puts(src);

				// get dest
				recv(newSocket, dest, sizeof(dest), 0);
				puts(dest);

				if(!strcmp(src,"kolkata"))
				{
					write(fd_t[1], src, (strlen(src)+1));

					numbers[0] = data[1].num_available;
					numbers[1] = data[1].num_booked;
					printf("%d\n", numbers[0]);
					printf("%d\n", numbers[1]);
					send(newSocket, numbers, sizeof(numbers), 0);

					recv(newSocket, num_to_be_booked, sizeof(num_to_be_booked), 0);
					printf("Number to be booked: %d\n", num_to_be_booked[0]);
					if((numbers[0]-num_to_be_booked[0]) < 0)
					{
						send(newSocket, failureMsg, sizeof(failureMsg), 0);
					}
					else {

						data[1].num_available-=num_to_be_booked[0];
						data[1].num_booked += num_to_be_booked[0];
						sprintf(string_num_to_be_booked, "%d", num_to_be_booked[0]);
                		write(fd[1], string_num_to_be_booked, (strlen(string_num_to_be_booked)+1));
						send(newSocket, successMsg, sizeof(successMsg), 0);
					}

					bzero(numbers, sizeof(numbers));

				}
				else if(!strcmp(src,"mumbai"))
				{
					write(fd_t[1], src, (strlen(src)+1));

					numbers[0] = data[0].num_available;
					numbers[1] = data[0].num_booked;
					printf("%d\n", numbers[0]);
					printf("%d\n", numbers[1]);
					send(newSocket, numbers, sizeof(numbers), 0);
					
					recv(newSocket, num_to_be_booked, sizeof(num_to_be_booked), 0);
					printf("Number to be booked: %d\n", num_to_be_booked[0]);
					if((numbers[0]-num_to_be_booked[0]) < 0)
					{
						send(newSocket, failureMsg, sizeof(failureMsg), 0);
					}
					else {

						data[0].num_available-=num_to_be_booked[0];
						data[0].num_booked += num_to_be_booked[0];
						sprintf(string_num_to_be_booked, "%d", num_to_be_booked[0]);
                		write(fd[1], string_num_to_be_booked, (strlen(string_num_to_be_booked)+1));
						send(newSocket, successMsg, sizeof(successMsg), 0);
					}

					bzero(numbers, sizeof(numbers));

				}
				else {
					numbers[0] = 109;
					printf("%d\n", numbers[0]);
					send(newSocket, numbers, sizeof(numbers), 0);
					bzero(numbers, sizeof(numbers));
				}
				// bzero(buff, sizeof(buff));


				// recv number of seats to be booked
				

			}

		}
		else{
			printf("IN PARENT MAKING CHANGES\n");
			close(fd[1]);
			close(fd_t[1]);
            /* Read in a string from the pipe */
            nbytes = read(fd_t[0], readbuffer, sizeof(readbuffer));
			if (!strcmp(readbuffer, "kolkata"))
			{
				nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
            	data[1].num_available-=atoi(readbuffer);
            	data[1].num_booked+=atoi(readbuffer);
			}
			else
			{
				nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
            	data[0].num_available-=atoi(readbuffer);
            	data[0].num_booked+=atoi(readbuffer);
			}
            
		}
	}

	close(newSocket);
	return 0;
}


void add_data(struct Tickets *data){

	// mumbai ---> banglore
	strcpy(data[0].src, "mumbai");
	strcpy(data[0].dest, "banglore");
	data[0].num_available = 100;
	data[0].num_booked = 0;

	// kolkata --> delhi
	strcpy(data[1].src, "kolkata");
	strcpy(data[1].dest, "delhi");
	data[1].num_available = 100;
	data[1].num_booked = 0;
}