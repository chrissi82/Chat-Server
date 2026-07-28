# Ausführungsplan: C++ Chat-Server Projekt

Projektdateien (`server.cpp`, `client.cpp`, `CMakeLists.txt`) liegen bereits im Ordner.

## Phase 1 – Umgebung einrichten

1. Visual Studio Code installieren (falls noch nicht vorhanden).
2. C++-Compiler installieren: entweder **MSVC** (über "Desktop development with C++"-Workload in Visual Studio Build Tools) oder **MinGW-w64** (g++).
3. VS Code Extension "C/C++" (Microsoft) installieren.
4. CMake installieren (optional, falls Build über CMake statt g++ direkt erfolgen soll).

## Phase 2 – Build

**Variante A: CMake**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**Variante B: direkt mit g++**
```bash
g++ -o server.exe server.cpp -lws2_32 -std=c++17
g++ -o client.exe client.cpp -lws2_32 -std=c++17
```

Erwartetes Ergebnis: `server.exe` und `client.exe` ohne Compiler-/Linker-Fehler.

## Phase 3 – Funktionstest

1. Terminal 1: `./server.exe` starten → wartet auf Port 9000.
2. Terminal 2: `./client.exe` starten → verbindet sich.
3. Nachrichten abwechselnd in beiden Fenstern eintippen und prüfen, ob sie ankommen.
4. Beenden testen: `exit` in Client- oder Server-Fenster eingeben, beide Seiten sollten sauber terminieren.
5. Fehlerfälle testen:
   - Client starten, ohne dass der Server läuft → erwartete Fehlermeldung "Verbindung fehlgeschlagen".
   - Server zweimal starten → erwartete Fehlermeldung "Bind fehlgeschlagen" (Port belegt).

## Phase 4 – Code verstehen (für die Prüfung)

Anhand der Tabellen in Abschnitt 9 der Projektvorlage den Socket-Lebenszyklus auswendig erklären können:

- Server: `WSAStartup → socket → bind → listen → accept → recv/send → closesocket/WSACleanup`
- Client: `WSAStartup → socket → connect → send/recv → closesocket/WSACleanup`

Zusätzlich folgende Konzepte in eigenen Worten erklären können: TCP vs. UDP, blockierende Aufrufe (`accept`, `recv`), Zweck des Puffers, Umwandlung `char*` → `std::string`.

## Phase 5 – Optionale Erweiterungen (falls Zeit/Bonus gewünscht)

- Mehrere Clients gleichzeitig unterstützen (Threads pro Client-Verbindung).
- Nachrichten mit Zeitstempel und Absendername versehen.
- Einfaches Broadcast-Prinzip statt 1:1-Chat.
- Fehlerbehandlung robuster machen (z. B. Timeout bei `recv`).

## Phase 6 – Prüfungsvorbereitung

1. Kurze Einleitung (siehe Abschnitt 9 der Vorlage) auswendig vorbereiten.
2. Live-Demo einüben: Server + Client parallel starten, Chat vorführen, sauber beenden.
3. Häufige Fehlerfragen durchgehen (Tabelle Abschnitt 10): Bind-Fehler, fehlender Linker-Flag, blockierendes `recv`.
4. Auf Rückfragen vorbereiten: Warum Windows-Sockets anders als Linux? Warum `-lws2_32` nötig?

## Reihenfolge

Phase 1 → 2 → 3 zuerst abschließen (lauffähiges Projekt), danach Phase 4–6 parallel zur Prüfungsvorbereitung.
