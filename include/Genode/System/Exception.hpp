#ifndef GENODE_EXCEPTION_HPP
#define GENODE_EXCEPTION_HPP

#include <stdexcept>

namespace Gx
{
    /// Represents errors that occur during application execution.
    class Exception : public std::runtime_error
    {
    public:
        /// Initializes a new instance of Exception.
        /// \param message The exception message
        explicit Exception(const std::string& message) : std::runtime_error(message) {};
    };
}

#endif //GENODE_EXCEPTION_HPP
