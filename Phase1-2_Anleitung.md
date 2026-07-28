# Detailanleitung Phase 1 & 2 (für VS-Code-Einsteiger)

Ziel am Ende: VS Code ist installiert, ein C++-Compiler ist eingerichtet, und `server.exe` + `client.exe` sind aus `server.cpp` / `client.cpp` gebaut.

---

## PHASE 1 – Umgebung einrichten

### 1.1 Visual Studio Code installieren

1. Gehe zu **https://code.visualstudio.com** und klicke auf den großen "Download for Windows"-Button.
2. Installer-Datei (`VSCodeUserSetup-x64-*.exe`) ausführen.
3. Lizenzbedingungen akzeptieren, "Weiter" klicken.
4. Bei "Zusätzliche Aufgaben auswählen": aktiviere zusätzlich
   - "Zum PATH hinzufügen" (meist schon vorausgewählt)
   - optional: "Kontextmenü-Eintrag hinzufügen" (praktisch: Rechtsklick auf Ordner → "Mit Code öffnen")
5. Installieren klicken, danach VS Code starten.

### 1.2 C++-Compiler installieren (MinGW-w64 über MSYS2)

Windows hat von Haus aus keinen C++-Compiler. Der einfachste Weg für Einsteiger ist **MSYS2**, das den Compiler `g++` mitbringt.

1. Gehe zu **https://www.msys2.org** und lade den Installer herunter.
2. Installer ausführen, Standard-Installationspfad übernehmen (`C:\msys64`).
3. Am Ende der Installation bleibt die Checkbox "Run MSYS2 now" aktiviert → Finish. Es öffnet sich ein blaues Terminal-Fenster.
4. In diesem Terminal folgenden Befehl eingeben und Enter drücken:
   ```
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```
5. Wenn gefragt wird, welche Pakete installiert werden sollen: einfach Enter drücken (alle installieren).
6. Bei "Proceed with installation? [Y/n]" → `Y` eingeben und Enter.
7. Installation abwarten (kann einige Minuten dauern, lädt mehrere hundert MB herunter).

### 1.3 Compiler zum PATH hinzufügen

Damit Windows (und VS Code) den Befehl `g++` überall erkennt:

1. Windows-Suche öffnen (Windows-Taste drücken), "Umgebungsvariablen" eingeben.
2. "Systemumgebungsvariablen bearbeiten" öffnen.
3. Button "Umgebungsvariablen..." klicken.
4. Im oberen Bereich ("Benutzervariablen") die Zeile `Path` markieren → "Bearbeiten".
5. "Neu" klicken und folgenden Pfad eintragen:
   ```
   C:\msys64\ucrt64\bin
   ```
6. Alle Fenster mit "OK" bestätigen.

### 1.4 Installation prüfen

1. Ein **neues** Terminal-Fenster öffnen (wichtig: neu, damit der PATH aktualisiert ist) – z. B. PowerShell über Windows-Suche.
2. Eingeben:
   ```
   g++ --version
   ```
3. Erwartete Ausgabe: Versionsinfo wie `g++.exe (Rev...) 13.x.x` o. ä. Falls "Befehl nicht gefunden" erscheint: Rechner neu starten und erneut testen (PATH-Änderungen brauchen manchmal einen Neustart).

### 1.5 C/C++-Extension in VS Code installieren

1. VS Code öffnen.
2. Links in der Seitenleiste auf das Extensions-Symbol klicken (viertes Icon von oben, sieht aus wie vier Quadrate).
3. In das Suchfeld oben "C/C++" eingeben.
4. Die Extension **"C/C++" von Microsoft** auswählen und "Install" klicken.

### 1.6 Projektordner in VS Code öffnen

1. In VS Code: Menü **Datei → Ordner öffnen...**
2. Zum Ordner `Chat-Server` navigieren (enthält bereits `server.cpp`, `client.cpp`, `CMakeLists.txt`) und "Ordner auswählen" klicken.
3. Links im Explorer-Bereich sollten jetzt die drei Dateien sichtbar sein.
4. Falls eine Meldung "Do you trust the authors of the files in this folder?" erscheint → "Yes, I trust the authors" klicken.

Phase 1 ist damit abgeschlossen: VS Code + Compiler sind einsatzbereit.

---

## PHASE 2 – Build (Kompilieren)

Der einfachste Weg für den Einstieg ist die direkte Kompilierung mit `g++` (ohne CMake). Das entspricht Variante B aus dem Ausführungsplan.

### 2.1 Terminal in VS Code öffnen

1. Menü **Terminal → Neues Terminal** (oder Tastenkombination `Strg + ö` / `Strg + \``, je nach Tastaturlayout).
2. Unten öffnet sich ein Terminal-Fenster. Der Pfad sollte automatisch schon im `Chat-Server`-Ordner stehen (prüfbar mit `dir` – dort sollten `server.cpp` etc. aufgelistet werden).

### 2.2 Server kompilieren

Im Terminal eingeben:
```
g++ -o server.exe server.cpp -lws2_32 -std=c++17
```
Enter drücken. Kein Output = kein Fehler = erfolgreich. Prüfen mit `dir` – es sollte jetzt eine `server.exe` im Ordner liegen.

### 2.3 Client kompilieren

```
g++ -o client.exe client.cpp -lws2_32 -std=c++17
```
Danach liegt zusätzlich `client.exe` im Ordner.

### 2.4 Ausführen und testen

1. Erstes Terminal (Server) ausführen:
   ```
   ./server.exe
   ```
   Ausgabe: `Warte auf Client-Verbindung auf Port 9000...`

2. Zweites Terminal öffnen (Symbol "+" oben rechts im Terminal-Bereich von VS Code, oder Terminal → Neues Terminal) und dort:
   ```
   ./client.exe
   ```

3. Nachrichten abwechselnd in beiden Terminals eintippen (siehe Abschnitt "Funktionstest" im Gesamtplan).

### 2.5 Häufige Fehler beim Kompilieren

| Fehlermeldung | Ursache | Lösung |
|---|---|---|
| `'g++' is not recognized` | PATH nicht korrekt gesetzt oder Terminal zu alt geöffnet | Schritt 1.3 prüfen, neues Terminal öffnen, ggf. Rechner neu starten |
| `undefined reference to WSAStartup` o. ä. (Linker-Fehler) | `-lws2_32` fehlt im Befehl | Kompilierbefehl exakt wie oben mit `-lws2_32` verwenden |
| `winsock2.h: No such file or directory` | Falscher/kein Windows-Compiler installiert | Sicherstellen, dass MSYS2 **ucrt64**-Toolchain installiert ist (Schritt 1.2) |
| `Bind fehlgeschlagen` beim Ausführen | Port 9000 bereits belegt (z. B. alter Server-Prozess läuft noch) | Terminal schließen/Prozess beenden, erneut versuchen |

### 2.6 Optional: Build mit CMake statt g++

Nur relevant, falls CMake explizit gefordert ist. Zusätzlich nötig: CMake installieren (**https://cmake.org/download**, Windows-Installer, bei Setup "Add CMake to PATH" auswählen). Dann im VS-Code-Terminal:
```
mkdir build
cd build
cmake ..
cmake --build .
```
Die `.exe`-Dateien liegen danach im Unterordner `build/Debug` oder direkt in `build/`, je nach verwendetem Compiler-Backend.

---

Damit ist Phase 1 (Setup) und Phase 2 (Build) vollständig abgedeckt. Weiter geht es mit Phase 3 (Funktionstest) aus dem Gesamtplan.
