#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8081
#define LONGUEUR 50

void sendMessage(const int accepter, const char buffer[restrict static 50],
unsigned int bufferSize, const char serverName[restrict static LONGUEUR],
const unsigned int serverNameSize){

    int retour = 0;
    //Ecrire envoie du message
    retour = write(accepter, buffer, bufferSize);
    if(retour == -1){
        perror("Nous ne pouvions pas envoyer le message...");
        exit(EXIT_FAILURE);
    }else{
        puts("Message envoyé...\n");
    }

    retour=write(accepter, serverName,serverNameSize);
    if(retour == -1){
        perror("Nous ne pouvons pas envoyé le nom");
        exit(EXIT_FAILURE);
    }
}

void receiveMessage(const int accepter, char buffer[restrict static LONGUEUR],
const unsigned int bufferSize){

    char clientName[50];
    int retour = 0;
    unsigned int clientNameSize = sizeof (clientName);

    retour = recv(accepter, buffer, bufferSize,0);
    if(retour == -1){
        perror("Lecture échouée...");
        exit(EXIT_FAILURE);
    }
    retour = recv(accepter, clientName, clientNameSize,0);
    if(retour == -1){
        perror("Envoie nom échouée...");
        exit(EXIT_FAILURE);
    }else{
        puts("Message reçu...");
        printf("De %s: %s\n",clientName, buffer);
    }
}


int main(void)
{
    struct sockaddr_in addr;
    struct sockaddr_in from;

    char buffer[LONGUEUR]=" ";
    char serverName[50]  =" ";

    int index                   = 0;
    int accepter                = 0;
    int sock_server             = 0;
    unsigned int size           = sizeof(from);
    unsigned int bufferSize     = sizeof(buffer);
    unsigned int serverNameSize = sizeof(serverName);

    puts("Statue | Serveur\n");
    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_server == -1) {
        printf("Socket failed...\n");
        exit(EXIT_FAILURE);
    }else{
        puts("Socket created...");
    }

    //Les ports et les adresses
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    //Etablir le lien
    if ((bind(sock_server, (struct sockaddr*)&addr, sizeof(addr))) != 0) {
        perror("Binding failed...");
        exit(h_errno);
    }else{
        puts("Bindind succeed...");
    }

    //Ecoute du serveur, vérification d'une nouvelle connexion
    if ((listen(sock_server, 10)) != 0) {
        printf("Listening failed...\n");
        exit(EXIT_FAILURE);
    }else{
        puts("New connexion established...");
    }

    //Connexion au client
    accepter = accept(sock_server, (struct sockaddr*)&from, &size);
    if (accepter == -1) {
        printf("Client rejected...\n");
        exit(EXIT_FAILURE);
    }else{
        puts("Client accepted...\n");
    }

    printf("Tapez votre nom: ");
    while((serverName[index++]=getchar())!='\n');
    puts("\n");

    while(1){
        index = 0;
        //Vider le buffer
        serverName[strcspn(serverName, "\n")] = 0;
        bzero(buffer, sizeof(&buffer));

        receiveMessage(accepter,buffer,bufferSize);

        bzero(buffer, sizeof(buffer));
        printf("Tapez un message [%s]: ", serverName);
        while((buffer[index++]=getchar())!='\n');
        sendMessage(accepter,buffer, bufferSize,serverName,serverNameSize);

        if ((!strncmp("bye", buffer, 3))) {
            break;
        }
    }

    //Fermeture du serveur
    if(close(sock_server) == -1){
        perror("Impossible de fermer le serveur...");
        exit(EXIT_FAILURE);
    }else{
        puts("Serveur | Connexion terminée...\n");
    }
}

