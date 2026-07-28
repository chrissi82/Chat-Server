// ============================================================
// client.cpp – Einfacher TCP-Chat-Client für Windows
// Fachprüfungsprojekt: C++ Netzwerkprogrammierung
// ============================================================

#include <iostream>       // Für cout und cin
#include <string>         // Für std::string
#include <winsock2.h>     // Windows-Socket-Bibliothek
#include <ws2tcpip.h>     // Für inet_pton (IP-Adresse konvertieren)

// Winsock2-Bibliothek beim Linker einbinden
#pragma comment(lib, "ws2_32.lib")

// Konstanten
const char* SERVER_IP = "127.0.0.1";  // Loopback (Server läuft lokal)
const int   PORT      = 9000;          // Port des Servers
const int   PUFFER    = 1024;          // Maximale Nachrichtenlänge in Bytes

int main() {

    // -------------------------------------------------------
    // SCHRITT 1: Winsock initialisieren (gleich wie beim Server)
    // -------------------------------------------------------
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Fehler: WSAStartup fehlgeschlagen.\n";
        return 1;
    }
    std::cout << "=== Chat-Client gestartet ===\n";

    // -------------------------------------------------------
    // SCHRITT 2: Client-Socket erstellen
    // Gleiche Parameter wie beim Server-Socket.
    // -------------------------------------------------------
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Fehler: Socket-Erstellung fehlgeschlagen.\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Socket erstellt. Verbinde mit Server...\n";

    // -------------------------------------------------------
    // SCHRITT 3: Server-Adresse konfigurieren
    // inet_pton() konvertiert die IP-Adresse (String) in Binärformat.
    // -------------------------------------------------------
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // -------------------------------------------------------
    // SCHRITT 4: Verbindung zum Server aufbauen (connect)
    // connect() sendet einen TCP-Verbindungsaufbau (SYN-Paket).
    // Blockiert bis die Verbindung steht oder ein Fehler auftritt.
    // -------------------------------------------------------
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Fehler: Verbindung zum Server fehlgeschlagen.\n";
        std::cerr << "Ist der Server gestartet und lauscht auf Port " << PORT << "?\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Verbunden mit Server " << SERVER_IP << ":" << PORT << "\n";
    std::cout << "Tippe Nachrichten oder 'exit' zum Beenden.\n\n";

    // -------------------------------------------------------
    // SCHRITT 5: Kommunikations-Loop
    // Abwechselnd: Nachricht eingeben & senden, dann Antwort empfangen.
    // -------------------------------------------------------
    char puffer[PUFFER];  // Empfangspuffer

    while (true) {

        // --- Nachricht vom Client-Nutzer eingeben ---
        std::cout << "[Du]: ";
        std::string nachricht;
        std::getline(std::cin, nachricht);

        // Prüfen ob Nutzer "exit" eingegeben hat
        if (nachricht == "exit") {
            send(clientSocket, "exit", 4, 0);
            std::cout << "Chat beendet.\n";
            break;
        }

        // --- Nachricht an Server senden (send) ---
        int gesendet = send(clientSocket, nachricht.c_str(),
                            (int)nachricht.size(), 0);
        if (gesendet == SOCKET_ERROR) {
            std::cerr << "Fehler beim Senden.\n";
            break;
        }

        // --- Antwort vom Server empfangen (recv) ---
        int empfangen = recv(clientSocket, puffer, PUFFER - 1, 0);
        if (empfangen <= 0) {
            std::cout << "Server hat die Verbindung getrennt.\n";
            break;
        }
        puffer[empfangen] = '\0';
        std::string antwort(puffer);

        // Prüfen ob Server "exit" gesendet hat
        if (antwort == "exit") {
            std::cout << "Server hat den Chat beendet.\n";
            break;
        }
        std::cout << "[Server]: " << antwort << "\n";
    }

    // -------------------------------------------------------
    // SCHRITT 6: Aufräumen
    // -------------------------------------------------------
    closesocket(clientSocket);
    WSACleanup();

    std::cout << "Client wurde sauber beendet.\n";
    return 0;
}
