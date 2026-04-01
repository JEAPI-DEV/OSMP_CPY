/******************************************************************************
 * FILE: osmpRun.h
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * Header für den OSMP-Runner.
 ******************************************************************************/

#ifndef __OSMP_RUN_H__
#define __OSMP_RUN_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../osmpLibrary/osmpLib.h"

#define RUNNER_DEFAULT_LOG_FILE "osmprun.log"

typedef enum {
    RUNNER_SUCCESS = 0,
    RUNNER_FAILURE = 1,
    RUNNER_EXEC_FAILURE = 2,
    RUNNER_HELP = 3,
} runner_status_t;

typedef struct {
    int             process_count;
    OSMP_Verbosity  log_level;
    const char     *log_path;
    char          **exec_argv;
    int             exec_argc;
} runner_config_t;

void runner_print_help(const char *program_name);
runner_status_t runner_parse_args(int argc, char **argv, runner_config_t *config);
runner_status_t runner_execute(const runner_config_t *config);

#endif  // __OSMP_RUN_H__
