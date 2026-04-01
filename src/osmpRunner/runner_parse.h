/******************************************************************************
 * FILE: runner_parse.h
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * Parser-Funktionen fuer die Kommandozeilenargumente des Runners.
 ******************************************************************************/

#ifndef __RUNNER_PARSE_H__
#define __RUNNER_PARSE_H__

#include "osmpRun.h"

void runner_print_help(const char *program_name);
runner_status_t runner_parse_args(int argc, char **argv, runner_config_t *config);

#endif  // __RUNNER_PARSE_H__
