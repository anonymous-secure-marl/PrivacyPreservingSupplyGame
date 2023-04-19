#include "GC/circuit_execution.h"
#include "GC/protocol_execution.h"

thread_local sci::ProtocolExecution *prot_exec = nullptr;
thread_local sci::CircuitExecution *circ_exec = nullptr;
/*
#ifndef THREADING
// sci::ProtocolExecution* sci::ProtocolExecution::prot_exec = nullptr;
// sci::CircuitExecution* sci::CircuitExecution::circ_exec = nullptr;
sci::ProtocolExecution* prot_exec = nullptr;
sci::CircuitExecution* circ_exec = nullptr;
#else
__thread sci::ProtocolExecution* sci::ProtocolExecution::prot_exec = nullptr;
__thread sci::CircuitExecution* sci::CircuitExecution::circ_exec = nullptr;
#endif
*/
