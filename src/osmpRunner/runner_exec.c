/******************************************************************************
 * FILE: runner_exec.c
 * AUTHOR: JEAPI
 * DESCRIPTION:
 * Prozesssteuerungs-Implementierung fuer den OSMP-Runner.
 ******************************************************************************/

#include "runner_exec.h"

#include "runner_log.h"

#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Setzt eine Peer-Tabelle auf den initialen Zustand.
 */
void initialize_peers(osmp_peer_t *peers, int count)
{
    int index = 0;

    while (index < count)
    {
        peers[index].pid = -1;
        peers[index].peer_state = OSMP_PEER_UNUSED;
        index++;
    }
}

/**
 * @brief Markiert einen Peer als fehlgeschlagen und loggt die Fehlermeldung.
 */
runner_status_t fail_peer(runner_logger_t *logger, osmp_peer_t *peer,
                                 runner_status_t status, const char *format, ...)
{
    va_list args;
    peer->peer_state = OSMP_PEER_FAILED;

    va_start(args, format);
    runner_vlog(logger, OSMP_LOG_ALL, format, args);
    va_end(args);
    return status;
}

/**
 * @brief Wartet auf alle gestarteten Kindprozesse und aktualisiert deren Status.
 */
runner_status_t handle_child_status(runner_logger_t *logger, osmp_peer_t *peer, int status)
{
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        peer->peer_state = OSMP_PEER_TERMINATED;
        runner_log(logger, OSMP_LOG_SYS,
                   "[RUNNER] Peer PID %ld beendet mit Exit-Code 0",
                   (long)peer->pid);
        return RUNNER_SUCCESS;
    }

    if (WIFEXITED(status))
    {
        return fail_peer(logger, peer, RUNNER_EXEC_FAILURE,
                         "[RUNNER][ERROR] Peer PID %ld beendet mit Exit-Code %d",
                         (long)peer->pid, WEXITSTATUS(status));
    }

    if (WIFSIGNALED(status))
    {
        return fail_peer(logger, peer, RUNNER_EXEC_FAILURE,
                         "[RUNNER][ERROR] Peer PID %ld beendet durch Signal %d",
                         (long)peer->pid, WTERMSIG(status));
    }

    return fail_peer(logger, peer, RUNNER_FAILURE,
                     "[RUNNER][ERROR] Unbekannter Endstatus fuer PID %ld",
                     (long)peer->pid);
}

runner_status_t wait_for_children(osmp_peer_t *peers, int started_count, runner_logger_t *logger)
{
    int index = 0;
    runner_status_t result = RUNNER_SUCCESS;

    while (index < started_count)
    {
        osmp_peer_t *peer = &peers[index];
        int status = 0;
        runner_status_t child_result;

        const pid_t waited_pid = waitpid(peer->pid, &status, 0);
        if (waited_pid < 0)
        {
            perror("waitpid fehlgeschlagen");
            child_result = fail_peer(logger, peer, RUNNER_FAILURE,
                                     "[RUNNER][ERROR] waitpid fuer PID %ld fehlgeschlagen: %s",
                                     (long)peer->pid, strerror(errno));
        }
        else
        {
            child_result = handle_child_status(logger, peer, status);
        }

        if (result == RUNNER_SUCCESS && child_result != RUNNER_SUCCESS) result = child_result;
        index++;
    }

    return result;
}

runner_status_t runner_execute(const runner_config_t *config)
{
    osmp_peer_t *peers = NULL;
    runner_logger_t logger;
    runner_status_t status = RUNNER_SUCCESS;
    int started_count = 0;
    int index = 0;

    if (!runner_logger_open(config, &logger)) return RUNNER_FAILURE;

    runner_log(&logger, OSMP_LOG_BIB,
               "[RUNNER] Starte %d Peers mit Executable '%s'.",
               config->process_count, config->exec_argv[0]);

    peers = calloc((size_t)config->process_count, sizeof(*peers));
    if (peers == NULL)
    {
        perror("calloc fuer Peer-Array fehlgeschlagen");
        runner_log(&logger, OSMP_LOG_ALL,
                   "[RUNNER][ERROR] Speicherallokation fuer Peer-Array fehlgeschlagen");
        runner_logger_close(&logger);
        return RUNNER_FAILURE;
    }

    runner_log(&logger, OSMP_LOG_SYS,
               "[RUNNER] Speicher fuer %d Peer-Eintraege reserviert",
               config->process_count);
    initialize_peers(peers, config->process_count);

    while (index < config->process_count)
    {
        pid_t child_pid = fork();
        if (child_pid < 0)
        {
            perror("fork fehlgeschlagen");
            runner_log(&logger, OSMP_LOG_ALL,
                       "[RUNNER][ERROR] fork bei Peer %d fehlgeschlagen: %s",
                       index, strerror(errno));            
            runner_log(&logger, OSMP_LOG_ALL, "[RUNNER][ERROR] Breche ab und beende %d bereits gestartete Peers", started_count);
            for (int k = 0; k < started_count; k++) // attempt to fix asan error.
            {
                if (peers[k].pid > 0)
                {
                    kill(peers[k].pid, SIGKILL);
                }
            }
            status = RUNNER_FAILURE;
            break;
        }

        if (child_pid == 0)
        {
            runner_log(&logger, OSMP_LOG_SYS,
                       "[PEER %d] Child startet exec '%s'", index,
                       config->exec_argv[0]);
            execvp(config->exec_argv[0], config->exec_argv);
            perror("execvp fehlgeschlagen");
            runner_log(&logger, OSMP_LOG_ALL,
                       "[PEER %d][ERROR] execvp fuer '%s' fehlgeschlagen: %s",
                       index, config->exec_argv[0], strerror(errno));
            _exit(127); // Not found in path > Bash return the same error, apparently.
        }

        peers[index].pid = child_pid;
        peers[index].peer_state = OSMP_PEER_STARTING;
        started_count++;
        runner_log(&logger, OSMP_LOG_SYS,
                   "[RUNNER] Peer %d gestartet mit PID %ld", index,
                   (long)child_pid);
        index++;
    }

    if (started_count > 0)
    {
        runner_status_t wait_status = wait_for_children(peers, started_count, &logger);
        if (status == RUNNER_SUCCESS)
        {
            status = wait_status;
        }
    }

    runner_log(&logger, OSMP_LOG_SYS, "[RUNNER] Gebe Peer-Array frei");
    free(peers);
    runner_log(&logger, OSMP_LOG_BIB, "[RUNNER] Terminierung abgeschlossen");
    runner_logger_close(&logger);
    return status;
}
