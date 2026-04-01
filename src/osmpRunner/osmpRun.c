/******************************************************************************
 * FILE: osmpRun.c
 * AUTHOR: Darius Malysiak | JEAPi
 * DESCRIPTION:
 * Diese Datei enthält die main Funktion für den OSMP-Runner.
 ******************************************************************************/

#include "osmpRun.h"

#include "runner_exec.h"
#include "runner_log.h"
#include "runner_parse.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    runner_config_t config;
    runner_status_t parse_status;
    runner_status_t run_status;

    parse_status = runner_parse_args(argc, argv, &config);
    if (parse_status == RUNNER_HELP)
    {
        runner_print_help(argv[0]);
        return EXIT_SUCCESS;
    }
    if (parse_status != RUNNER_SUCCESS)
    {
        runner_log_parse_error_if_possible(&config,
                                           "Abbruch wegen ungueltiger Parameter.");
        return EXIT_FAILURE;
    }

    run_status = runner_execute(&config);
    if (run_status == RUNNER_SUCCESS)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
