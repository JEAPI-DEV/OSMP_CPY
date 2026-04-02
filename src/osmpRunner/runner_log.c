/******************************************************************************
 * FILE: runner_log.c
 * AUTHOR: JEAPI
 * DESCRIPTION:
 * Logging-Implementierung fuer den OSMP-Runner.
 ******************************************************************************/

#include "runner_log.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

bool runner_logger_open(const runner_config_t *config, runner_logger_t *logger)
{
    logger->file = NULL;
    logger->threshold = config->log_level;

    if (config->log_path == NULL) return true;

    logger->file = fopen(config->log_path, "a");
    if (logger->file == NULL)
    {
        perror("Logdatei konnte nicht geoeffnet werden");
        return false;
    }
    return true;
}

void runner_logger_close(runner_logger_t *logger)
{
    if (logger->file != NULL)
    {
        (void)fclose(logger->file);
        logger->file = NULL;
    }
}

void runner_vlog(runner_logger_t *logger, OSMP_Verbosity verbosity,
                 const char *format, va_list args)
{
    char timestamp[32];
    time_t now;
    struct tm local_tm;
    va_list args_copy;
    char *message = NULL;

    if (logger == NULL || logger->file == NULL) return;
    if (verbosity == OSMP_LOG_NONE || (int)verbosity > (int)logger->threshold) return;

    now = time(NULL);
    if (now == (time_t)-1 || localtime_r(&now, &local_tm) == NULL) return;
    if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", &local_tm) == 0) return;

    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    if (needed < 0)
    {
        return;
    }

    message = malloc((size_t)needed + 1U);
    if (message == NULL) return;
    vsnprintf(message, (size_t)needed + 1U, format, args);
    fprintf(logger->file, "%s %d %ld %s\n", timestamp, verbosity, (long) getpid(), message);
    fflush(logger->file);
    free(message);
}

void runner_log(runner_logger_t *logger, OSMP_Verbosity verbosity,
                const char *format, ...)
{
    va_list args;
    va_start(args, format);
    runner_vlog(logger, verbosity, format, args);
    va_end(args);
}

void runner_log_parse_error_if_possible(const runner_config_t *config,
                                        const char *message)
{
    runner_config_t temporary;
    runner_logger_t logger;

    if (config == NULL) return;
    if (config->log_path == NULL && config->log_level == OSMP_LOG_NONE) return;

    temporary = *config;
    if (temporary.log_path == NULL) temporary.log_path = RUNNER_DEFAULT_LOG_FILE;
    if (temporary.log_level == OSMP_LOG_NONE) temporary.log_level = OSMP_LOG_BIB;

    if (!runner_logger_open(&temporary, &logger))return;
    runner_log(&logger, OSMP_LOG_ALL, "[RUNNER][ERROR] %s", message);
    runner_logger_close(&logger);
}
