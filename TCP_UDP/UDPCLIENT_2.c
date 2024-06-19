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


void sendMessage(const struct sockaddr_in addr,const int sock_client,
                 const char buffer[restrict static LONGUEUR],const int bufferSize,
const char client_name[restrict static LONGUEUR]){

    int send = 0;

    //Envoie le message du client
    send = sendto(sock_client, buffer,bufferSize,
                  0, (const struct sockaddr *) &addr,sizeof(addr));
    if(send == -1){
        perror("Message ne peut pas être envoyé\n");
    }else{
        puts("Message envoyé...\n");
    }

    //Envoie le nom du client
    send = sendto(sock_client, client_name, LONGUEUR,
                  0, (const struct sockaddr*) &addr,sizeof(addr));
    if(send == -1){
        perror("Le nom ne peut pas être envoyé\n");
    }
}

void receiveMessage(const struct sockaddr_in addr, const int sock_client,
                    char buffer[restrict static LONGUEUR],const unsigned int bufferSize,
 unsigned int* size){

    int receive = 0;
    char server_name[50];

    //Reçoit le message du serveur
    receive = recvfrom(sock_client, buffer, bufferSize,
                       0,(struct sockaddr *) &addr, size);
    if(receive == -1){
        perror("Message du serveur non reçu\n");
        exit(0);
    }

    //Reçoit le nom du serveur
    receive = recvfrom(sock_client, server_name, sizeof(server_name),
                       0,(struct sockaddr *) &addr, size);
    if(receive == -1){
        perror("Nom du serveur non reçu\n");
        exit(0);
    }else{
        puts("Message reçu...");
        printf("De %s : %s\n",server_name, buffer);
    }
}

int main(void) {
    struct sockaddr_in addr;

    char buffer [50]     =" ";
    char client_name[50] =" ";

    int index = 0;
    int sock_client = 0;
    unsigned int size = sizeof (addr);
    const unsigned int bufferSize = sizeof(buffer);

    printf("Statue | Client\n\n");
    if ((sock_client = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }else{
        puts("Socket created...");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Entrez votre nom: ");
    while((client_name[index++]=getchar())!='\n');
    puts("\n");

    while(1){
        index = 0;
        bzero(buffer, sizeof(buffer));

        client_name[strcspn(client_name, "\n")] = 0;//supprimer les retours à la ligne
        printf("Tapez un message [%s]: ",client_name);
        while((buffer[index++]=getchar())!='\n');

        sendMessage(addr,sock_client,buffer,bufferSize,client_name);
        receiveMessage(addr,sock_client,buffer,bufferSize,&size);

        if ((!strncmp(buffer, "bye", 3))) {
            break;
        }
    }

    if(close(sock_client) == -1){
        perror("Impossible de fermer le seveur...");
        exit(EXIT_FAILURE);
    }else{
        puts("Client | Connexion terminée...\n");
    }
    return 0;
}

