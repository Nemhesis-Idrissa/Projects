#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define LONGUEUR 50

void sendMessage(const int sock_client, const char buffer[restrict static LONGUEUR],
unsigned int bufferSize, const char client_name[restrict static LONGUEUR],
const unsigned int clientNameSize){

    int retour = 0;

    //Envoie le message du client au serveur
    retour = write(sock_client, buffer, bufferSize);
    if(retour == -1){
        perror("Nous ne pouvions pas envoyer le message...");
        exit(EXIT_FAILURE);
    }else{
        puts("Message envoyé...\n");
    }
    //Envoie le nom du client
    retour = write(sock_client, client_name, clientNameSize);
    if(retour == -1){
        perror("Nous ne pouvions pas envoyer le nom du client");
        exit(EXIT_FAILURE);
    }
}

void receiveMessage(const int sock_client,char buffer[restrict static LONGUEUR],
const int bufferSize){

    char serverName[50];
    unsigned int serverNameSize =sizeof (serverName);
    int retour = 0;

    //Reçevoir le message du serveur
    retour = recv(sock_client, buffer, bufferSize,0);
    if(retour == -1){
        perror("Lecture échouée...");
        exit(EXIT_FAILURE);
    }

    //Reçoit le nom du serveur
    retour = recv(sock_client, serverName, serverNameSize,0);
    if(retour == -1){
        perror("Nom non reçu...");
        exit(EXIT_FAILURE);
    }else{
        puts("Message reçu...");
        printf("De %s: %s\n", serverName, buffer);
    }
}


int main(void)
{
    struct sockaddr_in addr;

    char client_name[50] =" ";
    char buffer[LONGUEUR]=" ";

    int index                   = 0;
    int sock_client             = 0;
    unsigned int bufferSize     = sizeof(buffer);
    unsigned int clientNameSize = sizeof(client_name);

    //création de la socket
    puts("Statue | Client\n");
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_client == -1) {
        printf("Création de la socket échouée...\n");
        exit(EXIT_FAILURE);
    }else{
        puts("La socket a été créée...");
    }

    //Les ports et les adresses
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);

    //Connexion au serveur
    if (connect(sock_client, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("Connexion au serveur échouée...\n");
        exit(EXIT_FAILURE);
    }else{
        puts("Connecté au serveur...\n");
    }

    printf("Entrez votre nom: ");
    while((client_name[index++]=getchar())!='\n');
    puts("\n");

    while(1){
        index = 0;
        //réinitialisation du buffer
        bzero(buffer, sizeof(buffer));

        client_name[strcspn(client_name, "\n")] = 0;
        printf("Tapez un message [%s]: ", client_name);
        while((buffer[index++]=getchar())!='\n');

        sendMessage(sock_client,buffer,bufferSize,client_name, clientNameSize);
        receiveMessage(sock_client,buffer,bufferSize);

        if ((!strncmp(buffer, "bye", 3))) {
            break;
        }
    }

    //fermeture du client
    if(close(sock_client) == -1){
        perror("Impossible de fermer le client...");
        exit(EXIT_FAILURE);
    }else{
        puts("Client| Connexion terminée...\n");
    }
}
