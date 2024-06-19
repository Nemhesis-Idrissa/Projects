#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define LONGUEUR 50


void receiveMessage(const int sock_server, struct sockaddr_in* from , int* size ){
    char client_name[50];
    char buffer[50];

    int receive = 0;

    //Reçoit message du client
    receive = recvfrom(sock_server, buffer,sizeof (buffer),
                       0, (struct sockaddr*)from,size);

    if(receive == -1){
        perror("Error");
        exit(0);
    }

    //Reçoit nom du client
    receive = recvfrom(sock_server, client_name, sizeof (client_name),
                       0, (struct sockaddr*)from, size);

    if(receive == -1){
        perror("Error");
        exit(0);
    }else{
        puts("Message reçu...");
        printf("De %s : %s\n",client_name, buffer);
    }
}


void sendMessage(struct sockaddr_in from, int sock_server,
                 void* buffer,const int bufferSize, const void* server_name, const int size){

    int send = 0;
    //Envoie message serveur
    send = sendto(sock_server, buffer, bufferSize,
                  0, (const struct sockaddr*)&from,size);

    if(send == -1){
        perror("Error cannot send the message[:(]");
        exit(0);
    }else{
        puts("Message envoyé...\n");
    }

    //Envoie du nom du serveur
    send = sendto(sock_server, server_name, sizeof(server_name),
                  0, (const struct sockaddr *)&from,size);

    if(send == -1){
        perror("Error cannot send the message[:(]");
        exit(0);
    }
}


int main(void) {
    struct sockaddr_in addr;
    struct sockaddr_in from;
    int sock_server;
    int size = sizeof (from);
    char server_name[50];
    char buffer[50];
    const int buffeSize = sizeof (buffer);

    printf("Statue | serveur\n\n");
    if ( (sock_server = socket(AF_INET, SOCK_DGRAM, 0))  == -1 ) {
        perror("socket creation failed...");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket created...\n");
    }

    addr.sin_family    = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    //Binding
    if ( bind(sock_server, (const struct sockaddr *)&addr,
              sizeof(addr)) == -1 )
    {
        perror("bind failed[:(]...");
        exit(EXIT_FAILURE);
    }else{
        puts("Binding succeeded...\n");
    }

    int index = 0;
    printf("Entrez votre nom: ");
    while((server_name[index++]=getchar())!='\n');
    puts("\n");

    while(1){
        index = 0;
        //supprimer le retour à la ligne pour server_name
        server_name[strcspn(server_name, "\n")] = 0;
        bzero(buffer, sizeof(buffer));
        receiveMessage(sock_server,&from,&size);

        printf("Tapez un message [%s]: ",server_name);
        while((buffer[index++]=getchar())!='\n');
        sendMessage(from,sock_server,buffer,buffeSize,server_name,size);

        if ((!strncmp("bye", buffer, 3))){
            break;
        }
    }
      if(close(sock_server) == -1){
        perror("Impossible de fermer le seveur...");
        exit(EXIT_FAILURE);
    }else{
          puts("Serveur | Connexion terminée...\n");
    }
    return 0;
}

