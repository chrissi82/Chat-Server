// ============================================================
// server.cpp – Einfacher TCP-Chat-Server für Windows
// Fachprüfungsprojekt: C++ Netzwerkprogrammierung
// ============================================================

#include <iostream>       // Für cout und cin
#include <string>         // Für std::string
#include <winsock2.h>     // Windows-Socket-Bibliothek

// Winsock2-Bibliothek beim Linker einbinden
#pragma comment(lib, "ws2_32.lib")

// Konstanten
const int PORT    = 9000;       // Port, auf dem der Server lauscht
const int PUFFER  = 1024;       // Maximale Nachrichtenlänge in Bytes

int main() {

    // -------------------------------------------------------
    // SCHRITT 1: Winsock initialisieren
    // WSAStartup teilt Windows mit, dass wir Sockets verwenden.
    // MAKEWORD(2,2) gibt an, dass wir Version 2.2 nutzen wollen.
    // -------------------------------------------------------
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Fehler: WSAStartup fehlgeschlagen.\n";
        return 1;
    }
    std::cout << "=== Chat-Server gestartet ===\n";

    // -------------------------------------------------------
    // SCHRITT 2: Server-Socket erstellen
    // AF_INET    = IPv4-Adressfamilie
    // SOCK_STREAM = TCP (verbindungsorientiert, zuverlässig)
    // IPPROTO_TCP = TCP-Protokoll
    // -------------------------------------------------------
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Fehler: Socket-Erstellung fehlgeschlagen.\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Socket erfolgreich erstellt.\n";

    // -------------------------------------------------------
    // SCHRITT 3: Socket-Adresse konfigurieren
    // sockaddr_in enthält IP-Adresse und Port.
    // INADDR_ANY bedeutet: auf allen verfügbaren Netzwerkkarten lauschen.
    // htons() konvertiert den Port in Netzwerk-Byte-Reihenfolge.
    // -------------------------------------------------------
    sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port        = htons(PORT);

    // -------------------------------------------------------
    // SCHRITT 4: Socket an Port binden (bind)
    // Verknüpft den Socket mit der Adresse und dem Port.
    // -------------------------------------------------------
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Fehler: Bind fehlgeschlagen.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Socket an Port " << PORT << " gebunden.\n";

    // -------------------------------------------------------
    // SCHRITT 5: Auf Verbindungen lauschen (listen)
    // Der zweite Parameter (1) ist die Warteschlangenlänge
    // (wie viele Verbindungsanfragen zwischengespeichert werden).
    // -------------------------------------------------------
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Fehler: Listen fehlgeschlagen.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Warte auf Client-Verbindung auf Port " << PORT << "...\n";

    // -------------------------------------------------------
    // SCHRITT 6: Client-Verbindung annehmen (accept)
    // accept() blockiert das Programm, bis ein Client sich verbindet.
    // Gibt einen neuen Socket zurück, der für die Kommunikation genutzt wird.
    // -------------------------------------------------------
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Fehler: Accept fehlgeschlagen.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Client verbunden! Tippe Nachrichten oder 'exit' zum Beenden.\n\n";

    // -------------------------------------------------------
    // SCHRITT 7: Kommunikations-Loop
    // Abwechselnd: Nachricht vom Client empfangen, dann senden.
    // -------------------------------------------------------
    char puffer[PUFFER];  // Empfangspuffer

    while (true) {

        // --- Nachricht vom Client empfangen (recv) ---
        // recv() blockiert, bis Daten ankommen.
        // Gibt die Anzahl empfangener Bytes zurück (0 = Verbindung getrennt).
        int empfangen = recv(clientSocket, puffer, PUFFER - 1, 0);
        if (empfangen <= 0) {
            std::cout << "\nClient hat die Verbindung getrennt.\n";
            break;
        }
        puffer[empfangen] = '\0';  // Null-Terminierung für std::string
        std::string nachrichtVomClient(puffer);

        // Prüfen ob Client "exit" gesendet hat
        if (nachrichtVomClient == "exit") {
            std::cout << "Client hat den Chat beendet.\n";
            break;
        }
        std::cout << "[Client]: " << nachrichtVomClient << "\n";

        // --- Nachricht vom Server-Nutzer eingeben und senden (send) ---
        std::cout << "[Server]: ";
        std::string nachrichtAnClient;
        std::getline(std::cin, nachrichtAnClient);

        // Prüfen ob Server "exit" eingegeben hat
        if (nachrichtAnClient == "exit") {
            send(clientSocket, "exit", 4, 0);
            std::cout << "Chat vom Server beendet.\n";
            break;
        }

        // Nachricht an den Client senden
        int gesendet = send(clientSocket, nachrichtAnClient.c_str(),
                            (int)nachrichtAnClient.size(), 0);
        if (gesendet == SOCKET_ERROR) {
            std::cerr << "Fehler beim Senden der Nachricht.\n";
            break;
        }
    }

    // -------------------------------------------------------
    // SCHRITT 8: Aufräumen – Sockets schließen & Winsock beenden
    // Immer in umgekehrter Reihenfolge: zuerst Client-, dann Server-Socket.
    // -------------------------------------------------------
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    std::cout << "Server wurde sauber beendet.\n";
    return 0;
}
