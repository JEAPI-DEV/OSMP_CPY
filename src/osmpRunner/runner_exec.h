/******************************************************************************
 * FILE: runner_exec.h
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * Prozesssteuerung fuer den OSMP-Runner.
 ******************************************************************************/

#ifndef __RUNNER_EXEC_H__
#define __RUNNER_EXEC_H__

#include "osmpRun.h"

runner_status_t runner_execute(const runner_config_t *config);

#endif  // __RUNNER_EXEC_H__
