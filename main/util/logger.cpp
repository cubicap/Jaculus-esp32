#include "logger.h"

std::unique_ptr<OutputStreamCommunicator> Logger::_logStream;
std::unique_ptr<OutputStreamCommunicator> Logger::_debugStream;
