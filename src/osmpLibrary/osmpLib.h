/******************************************************************************
 * FILE: osmpLib.h
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * In dieser Datei sind zusätzliche (eigene) Hilfsfunktionen für die **interne**
 * Verwendung der OSMP-Bibliothek definiert.
 ******************************************************************************/

#ifndef __OSMP_LIB_H__
#define __OSMP_LIB_H__

#include <sys/types.h>

#include "OSMP.h"

/**
 * @brief Dieses Makro wird verwendet, um den Compiler davon zu überzeugen, dass
 * eine Variable verwendet wird.
 *
 * @param x Die zu verwendende Variable
 */
#define UNUSED(x)  \
    do {           \
        (void)(x); \
    } while (0);

/**
 * @brief  Hier wird ein Fehlercode definiert, der verwendet wird, wenn eine
 * Funktion noch nicht implementiert ist.
 */
#define OSMP_NOT_IMPLEMENTED -1

/**
 * @brief Status eines OSMP-Peer-Prozesses.
 *
 * Dieses Enum beschreibt den Lebenszyklus eines Peer-Prozesses innerhalb
 * einer OSMP-Ausfuehrung.
 *
 * Folgende Faelle sind als Status vorgesehen:
 *
 * - OSMP_PEER_UNUSED:
 *   Statuswert aller Peers zu Beginn (Slot noch nicht verwendet).
 * - OSMP_PEER_STARTING:
 *   Status nach einem erfolgreichen fork() im Runner, Peer befindet sich
 *   in der Start-/Initialisierungsphase.
 * - OSMP_PEER_READY:
 *   Peer hat OSMP_Init() erfolgreich abgeschlossen und ist bereit fuer die
 *   Kommunikation.
 * - OSMP_PEER_FAILED:
 *   Fehler waehrend der Ausfuehrung des Peers, z. B. Init-/Exec-Fehler oder
 *   interner Initialisierungsfehler.
 * - OSMP_PEER_TERMINATED:
 *   Peer wurde geordnet beendet, z. B. durch OSMP_Finalize().
 */
typedef enum {
    OSMP_PEER_UNUSED = 0,   /**< Slot noch nicht verwendet. */
    OSMP_PEER_STARTING,     /**< Peer wurde vom Runner gestartet und befindet sich
                                 in der Start-/Initialisierungsphase. */
    OSMP_PEER_READY,        /**< Peer hat OSMP_Init erfolgreich abgeschlossen und
                                 ist bereit fuer die Kommunikation. */
    OSMP_PEER_FAILED,       /**< Fehler waehrend der Ausfuehrung des Peers
                                 (z. B. Init-/Exec-Fehler). */
    OSMP_PEER_TERMINATED    /**< Peer wurde geordnet beendet und hat seine
                                 OSMP-Ressourcen freigegeben. */
} osmp_peer_state_t;

/**
 * @brief Peer-spezifische Verwaltungsinformationen.
 *
 * Diese Struktur dient als Vorlage fuer die Verwaltung einzelner OSMP-Peer-
 * Prozesse. Sie enthaelt mindestens die Prozess-ID des Peers sowie einen
 * Statuswert, der den aktuellen Zustand des Peers im OSMP-System beschreibt.
 *
 * Diese Struktur sollte in weiteren Projektverlauf (mit Shared Memory) in das 
 * gemeinsame Speicherlayout uebernommen und um weitere Felder ergaenzt werden
 */
typedef struct {
    pid_t             pid;        /**< Prozess-ID des Peers. */
    osmp_peer_state_t peer_state; /**< Status des Peers. */
    /* ...  */
} osmp_peer_t;

#endif  // __OSMP_LIB_H__
