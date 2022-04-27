#pragma once

#ifndef EXECUTE_H
#define EXECUTE_H
#endif

#include "../utils/defines.h"

SCYCLE_HANDLER* execute(SCYCLE_HANDLER *handler, u_int32_t *MEMORY);

SCYCLE_HANDLER* executeRType(SCYCLE_HANDLER *handler);

SCYCLE_HANDLER* executeIType(SCYCLE_HANDLER *handler, u_int32_t *MEMORY);

SCYCLE_HANDLER* executeJType(SCYCLE_HANDLER *handler);