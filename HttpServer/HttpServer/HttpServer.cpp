#include <iostream>
#include <winsock2.h>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")


int main() {

    int defautPort = 8080;

    // Etape 0 : initialise la bibliotheque Winsock (obligatoire sur Windows avant tout appel reseau)
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        std::cout << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    std::cout << "Winsock initialized successfully!" << std::endl;

    // Etape 1 : cree le socket "serveur" (juste un descripteur, pas encore utilisable pour communiquer)
    // AF_INET = IPv4, SOCK_STREAM = TCP, IPPROTO_TCP = protocole TCP
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        std::cout << "bind() failed with error: " << WSAGetLastError() << std::endl;
        std::cout << "socket() failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Etape 2 : decrit "ou" le serveur doit ecouter (quelle adresse, quel port)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(defautPort); // htons() convertit le port au format reseau (big-endian)
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY = ecoute sur toutes les interfaces reseau de la machine

    // Etape 2 (suite) : associe le socket a l'adresse/port decrits ci-dessus
    int bindResult = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (bindResult == SOCKET_ERROR) {
        std::cout << "bind() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Bind successful, listening port ready!" << std::endl;

    int listenResult =listen(serverSocket, SOMAXCONN);
    //
    if (listenResult == SOCKET_ERROR) {
        std::cout << "listen() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is now listening on port " << defautPort << "..." << std::endl;

    SOCKET clientSocket = accept(serverSocket,NULL,NULL);

    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accept() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected!" << std::endl;

    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        std::cout << "Received " << bytesReceived << " bytes from client" << std::endl;
    }

    const char* message = "Hello from C++ server!";
    int sendResult = send(clientSocket, message, strlen(message), 0);

    if (sendResult == SOCKET_ERROR) {
        std::cout << "send() failed with error: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Message sent to client!" << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();
    return 0;
}