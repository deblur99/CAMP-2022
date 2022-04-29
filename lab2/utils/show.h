#pragma once

#ifndef SHOW_H
#define SHOW_H
#endif

#include "defines.h"

int isEmptyInst(INSTRUCT *inst);

void showInstructorAfterFetch(SCYCLE_HANDLER *handler);

void showInstructorAfterDecode(INSTRUCT *inst);

void showStatusAfterExecInst(SCYCLE_HANDLER *handler);

void showCounterAfterExecProgram(SCYCLE_HANDLER *handler);