/******************************************************************************
 * FILE: runner_log.h
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * Logging-Funktionen fuer den OSMP-Runner.
 ******************************************************************************/

#ifndef __RUNNER_LOG_H__
#define __RUNNER_LOG_H__

#include "osmpRun.h"

#include <stdarg.h>
#include <stdio.h>

typedef struct {
    FILE          *file;
    OSMP_Verbosity threshold;
} runner_logger_t;

bool runner_logger_open(const runner_config_t *config, runner_logger_t *logger);
void runner_logger_close(runner_logger_t *logger);
void runner_log(runner_logger_t *logger, OSMP_Verbosity verbosity,
                const char *format, ...);
void runner_vlog(runner_logger_t *logger, OSMP_Verbosity verbosity,
                 const char *format, va_list args);
void runner_log_parse_error_if_possible(const runner_config_t *config,
                                        const char *message);

#endif  // __RUNNER_LOG_H__
