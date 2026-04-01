# Termin 1

## 1. Build

```bash
./build.sh
```

### 2 echoall

Aufruf:

```bash
./build/echoall arg1 arg2 ...
```

Verhalten:

- Gibt alle uebergebenen Argumente (`argv[1]..argv[n]`) nummeriert aus.
- Gibt 3-mal `PID`, `PPID` und `TID` aus.
- Bricht mit Fehlermeldung ab, wenn keine Nutzargumente uebergeben wurden.

Beispiel:

```bash
./build/echoall hello world
```


### 2.1 echoall

```mermaid
flowchart TD
    A[Start echoall] --> B{argc groesser gleich zwei}
    B -->|nein| C[stderr: keine Nutzargumente + EXIT_FAILURE]
    B -->|ja| D[print_arguments: argv1..argvn]
    D --> E[loop 3x]
    E --> F[syscall SYS_gettid]
    F --> G{tid Fehler}
    G -->|ja| H[perror + EXIT_FAILURE]
    G -->|nein| I[printf Iteration + PID/PPID/TID]
    I --> J{weitere Iteration?}
    J -->|ja| E
    J -->|nein| K[EXIT_SUCCESS]
```

### 3 osmprun

Aufruf:

```bash
./build/osmprun -p ProzessCount [-l Logfile] [-v 0..3] -e Executable [Argumente...]
./build/osmprun -h
```

### 3.1 osmprun

```mermaid
flowchart TD
    A[main Start] --> B[runner_parse_args]
    B --> C{Parse Ergebnis}
    C -->|RUNNER_HELP| D[runner_print_help + EXIT_SUCCESS]
    C -->|RUNNER_FAILURE| E[runner_log_parse_error_if_possible + EXIT_FAILURE]
    C -->|RUNNER_SUCCESS| F[runner_execute]
    F -->|NO_LOGGER|J
    F --> G[runner_logger_open]
    G -->|ERROR| H[EXIT_FAILURE]
    G -->|OK | I[Start Logeintrag]
    I --> J[calloc Peer Array]
    J -->|ERROR| K[ERROR loggen und EXIT_FAILURE]
    J -->|OK| L[initialize_peers pid minus eins state UNUSED]
    L --> M[fork Schleife bis process_count]
    M --> N{fork Ergebnis}
    N -->|ERROR| O[ERROR loggen und Schleife abbrechen]
    N -->|Child| P[Child Start loggen]
    P --> Q[execvp Executable]
    Q -->|ERROR| R[perror + ERROR loggen + _exit 127]
    N -->|Parent| S[peer pid setzen state STARTING started_count plus eins]
    S --> T[Peer Start loggen]
    T --> M
    O --> U{started_count groesser null}
    M --> U
    U -->|ja| V[wait_for_children]
    U -->|nein| W[Skip wait]
    V --> X[Ergebnis mit status zusammenfuehren]
    W --> X
    X --> Y[Peer Array freigeben]
    Y --> Z[Terminierungs Logeintrag]
    Z --> AA[runner_logger_close]
    AA --> AB{status ist RUNNER_SUCCESS}
    AB -->|ja| AC[EXIT_SUCCESS]
    AB -->|nein| AD[EXIT_FAILURE]
```

# 4 Test

Auszug:

2026-04-01T23:47:43 1 204452 [RUNNER] Starte 50 Peers mit Executable './build/echoall'.
2026-04-01T23:47:43 2 204452 [RUNNER] Speicher fuer 50 Peer-Eintraege reserviert
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 0 gestartet mit PID 204453
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 1 gestartet mit PID 204454
2026-04-01T23:47:43 2 204453 [PEER 0] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 2 gestartet mit PID 204455
2026-04-01T23:47:43 2 204454 [PEER 1] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 3 gestartet mit PID 204456
2026-04-01T23:47:43 2 204455 [PEER 2] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 4 gestartet mit PID 204457
2026-04-01T23:47:43 2 204456 [PEER 3] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 5 gestartet mit PID 204458
...
2026-04-01T23:47:43 2 204538 [PEER 47] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204540 [PEER 48] Child startet exec './build/echoall'
2026-04-01T23:47:43 2 204452 [RUNNER] Peer 49 gestartet mit PID 204545
2026-04-01T23:47:43 2 204452 [RUNNER] Peer PID 204453 beendet mit Exit-Code 0
2026-04-01T23:47:43 2 204452 [RUNNER] Peer PID 204454 beendet mit Exit-Code 0
...
2026-04-01T23:47:43 2 204452 [RUNNER] Peer PID 204540 beendet mit Exit-Code 0
2026-04-01T23:47:43 2 204452 [RUNNER] Peer PID 204545 beendet mit Exit-Code 0
2026-04-01T23:47:43 2 204452 [RUNNER] Gebe Peer-Array frei
2026-04-01T23:47:43 1 204452 [RUNNER] Terminierung abgeschlossen
