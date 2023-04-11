#include "logger.h"


namespace jac {


std::unique_ptr<OutputStreamCommunicator> Logger::_errorStream;
std::unique_ptr<OutputStreamCommunicator> Logger::_logStream;
std::unique_ptr<OutputStreamCommunicator> Logger::_debugStream;


} // namespace jac
