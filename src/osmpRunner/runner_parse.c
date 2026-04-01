/******************************************************************************
 * FILE: runner_parse.c
 * AUTHOR: JEAPI
 * DESCRIPTION:
 * Parser-Implementierung fuer die Kommandozeilenargumente des Runners.
 ******************************************************************************/

#include "runner_parse.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialisiert die Konfiguration mit neutralen Default-Werten.
 */
void runner_config_init(runner_config_t *config) {
    config->process_count = -1;
    config->log_level = OSMP_LOG_NONE;
    config->log_path = NULL;
    config->exec_argv = NULL;
    config->exec_argc = 0;
}

/**
 * @brief Prueft, ob eine Option in der aktuellen Position erlaubt ist.
 */
bool validate_option_usage(bool already_seen, bool seen_e,
                                  const char *option) {
    if (already_seen || seen_e) {
        fprintf(stderr, "Ungueltige Reihenfolge oder doppelte Option: %s\n",
                option);
        return false;
    }
    return true;
}

/**
 * @brief Stellt sicher, dass auf eine Option ein Wert folgt.
 */
bool require_option_value(int argc, int index, const char *option) {
    if ((index + 1) >= argc) {
        fprintf(stderr, "Fehlender Wert fuer %s.\n", option);
        return false;
    }
    return true;
}

/**
 * @brief Parst einen positiven Integer aus einem Argumentstring.
 */
bool parse_positive_int(const char *text, int *value) {
    char *end = NULL;
    long  parsed;

    if (text == NULL || *text == '\0') {
        return false;
    }

    parsed = strtol(text, &end, 10);
    if (end == NULL || *end != '\0') {
        return false;
    }

    if (parsed <= 0 || parsed > INT_MAX) {
        return false;
    }

    *value = (int)parsed;
    return true;
}

/**
 * @brief Parst ein Loglevel aus dem Bereich 0..3.
 */
bool parse_verbosity(const char *text, OSMP_Verbosity *verbosity) {
    char *end = NULL;
    long  parsed;
    if (text == NULL || *text == '\0') return false;

    parsed = strtol(text, &end, 10);
    if (end == NULL || *end != '\0') return false;

    if (parsed < OSMP_LOG_NONE || parsed > OSMP_LOG_ALL) return false;

    *verbosity = (OSMP_Verbosity)parsed;
    return true;
}

void runner_print_help(const char *program_name) {
    fprintf(
        stdout,
        "Verwendung:\n"
        "  %s -p ProzessCount [-l Logfile] [-v Verbosity] -e Executable "
        "[Argumente...]\n"
        "  %s -h\n\n"
        "Optionen:\n"
        "  -h                Hilfe anzeigen und beenden.\n"
        "  -p ProzessCount   Anzahl zu startender Prozesse (> 0).\n"
        "  -l Logfile        Pfad zur Logdatei.\n"
        "  -v Verbosity      Loglevel 0..3.\n"
        "  -e Executable ... Executable mit Argumenten (muss letzter "
        "Runner-Parameter sein).\n\n"
        "Logging-Regeln:\n"
        "  Ohne -l und ohne -v: Logging aus.\n"
        "  Nur -l: Logging an mit Loglevel 1.\n"
        "  Nur -v: Logging an, Default-Logdatei '%s'.\n",
        program_name, program_name, RUNNER_DEFAULT_LOG_FILE);
}

runner_status_t runner_parse_args(int argc, char **argv, runner_config_t *config) {
    int index = 1;
    bool seen_p = false, seen_l = false, seen_v = false, seen_e = false;
    runner_config_init(config);

    if (argc < 2) {
        fprintf(stderr, "Fehlende Parameter. Verwenden Sie -h fuer Hilfe.\n");
        return RUNNER_FAILURE;
    }

    while (index < argc) {
        const char *arg = argv[index];

        if (strcmp(arg, "-h") == 0) return RUNNER_HELP;
        if (strcmp(arg, "-p") == 0) {
            if (!validate_option_usage(seen_p, seen_e, "-p") ||
                !require_option_value(argc, index, "-p")) {
                return RUNNER_FAILURE;
            }
            if (!parse_positive_int(argv[index + 1], &config->process_count)) {
                fprintf(stderr,
                        "Ungueltiger Wert fuer -p. Erwartet: positive Ganzzahl.\n");
                return RUNNER_FAILURE;
            }
            seen_p = true;
            index += 2;
            continue;
        }
        if (strcmp(arg, "-l") == 0) {
            if (!validate_option_usage(seen_l, seen_e, "-l") ||
                !require_option_value(argc, index, "-l")) {
                return RUNNER_FAILURE;
            }
            seen_l = true;
            config->log_path = argv[index + 1];
            index += 2;
            continue;
        }
        if (strcmp(arg, "-v") == 0) {
            if (!validate_option_usage(seen_v, seen_e, "-v") ||
                !require_option_value(argc, index, "-v")) {
                return RUNNER_FAILURE;
            }
            if (!parse_verbosity(argv[index + 1], &config->log_level)) {
                fprintf(stderr, "Ungueltiger Wert fuer -v. Erlaubt: 0,1,2,3.\n");
                return RUNNER_FAILURE;
            }
            seen_v = true;
            index += 2;
            continue;
        }
        if (strcmp(arg, "-e") == 0) {
            if (seen_e) {
                fprintf(stderr, "Option -e darf nur einmal gesetzt werden.\n");
                return RUNNER_FAILURE;
            }
            if (!require_option_value(argc, index, "-e")) {
                return RUNNER_FAILURE;
            }
            seen_e = true;
            config->exec_argv = &argv[index + 1];
            config->exec_argc = argc - (index + 1);
            break;
        }

        fprintf(stderr, "Unbekannter oder falsch platzierter Parameter: %s\n", arg);
        return RUNNER_FAILURE;
    }

    if (!seen_p) {
        fprintf(stderr, "Pflichtparameter -p fehlt.\n");
        return RUNNER_FAILURE;
    }
    if (!seen_e) {
        fprintf(stderr, "Pflichtparameter -e fehlt.\n");
        return RUNNER_FAILURE;
    }

    if (seen_l || seen_v)
    {
        if (!seen_l) config->log_path = RUNNER_DEFAULT_LOG_FILE;
        if (!seen_v) config->log_level = OSMP_LOG_BIB;
    }

    return RUNNER_SUCCESS;
}
