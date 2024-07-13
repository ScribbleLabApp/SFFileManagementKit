//===-- include/SFCxxLogger.h - Logging -------------------------*- C++ -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//  __    _____ _____                                                         //
// |  |  |     |  __|_   SFCxxLogger functions for SFFileCxxCore              //
// |  |__|  |  | |__| |  Version 1.0                                          //
// |_____|_____|______|  https://github.com/ScribbleLabApp/                   //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines logging functionality for SFFileCxxCore submodule.
///
/// This header file provides a Logger class that facilitates logging messages
/// with different severities to a specified file. It supports logging of messages
/// from different submodules and includes functions to convert severity levels
/// to strings and to retrieve the current date and time.
///
/// Example usage:
/// \code
///   sfcxx::Logger logger("app.log");
///   logger.log("_SFCxxUtils", sfcxx::Severity::INFO, "Application started.");
/// \endcode
///
/// \author ScribbleLabApp
/// \date July 2024
///
//===----------------------------------------------------------------------===//

#ifndef SFCxxLogger_h
#define SFCxxLogger_h

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>

namespace sfcxx {

/// \brief Enumerates severity levels for logging.
enum class Severity {
    DEBUG,       ///< Debugging information.
    INFO,        ///< Informational message.
    WARNING,     ///< Warning message.
    ERROR,       ///< Error message.
    CRITICAL     ///< Critical error message.
};

/// \brief Converts a Severity enum value to its string representation.
///
/// \param severity The Severity enum value.
/// \return String representation of the severity.
std::string severityToString(Severity severity) {
    switch (severity) {
        case Severity::DEBUG: return "DEBUG";
        case Severity::INFO: return "INFO";
        case Severity::WARNING: return "WARNING";
        case Severity::ERROR: return "ERROR";
        case Severity::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

/// \brief A logger class that writes log messages to a file.
///
/// Example usage:
/// \code
///   sfcxx::Logger logger("app.log");
///   logger.log("_SFCxxUtils", sfcxx::Severity::INFO, "Application started.");
/// \endcode
class Logger {
public:
    /// \brief Constructor that initializes the logger with a file.
    ///
    /// \param filename The name of the log file.
    Logger(const std::string& filename) : logFile(filename, std::ios::app) {
        if (!logFile.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
    }

    /// \brief Destructor that closes the log file if it is open.
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    /// \brief Logs a message with a specified submodule and severity level.
    ///
    /// \param submodule The submodule name associated with the log message.
    /// \param severity The severity level of the log message.
    /// \param message The log message to be written.
    void log(const std::string& submodule, Severity severity, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        logFile << currentDateTime() << " [" << severityToString(severity) << "] "
                << "[" << submodule << "] " << message << std::endl;
    }

private:
    std::ofstream logFile;              ///< Output file stream for logging.
    std::mutex logMutex;                ///< Mutex to ensure thread safety for logging operations.

    /// \brief Retrieves the current date and time as a formatted string.
    ///
    /// \return Current date and time as a string.
    std::string currentDateTime() {
        std::time_t now = std::time(nullptr);
        char buf[80];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %X", std::localtime(&now));
        return buf;
    }
};

}

#endif /* SFCxxLogger_h */
