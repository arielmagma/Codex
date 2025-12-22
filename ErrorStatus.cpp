#include <iostream>
#include "ErrorStatus.h"

const char* errorMessages[] = 
{
    "No error",
    "File not found or couldn't be opened at path: ",
    "Error reading file length: "
};

ErrorStatus::ErrorStatus(const int& errorCode, const std::string& cause)
{
    this->errorCode = errorCode;
    this->errorMessage = errorMessages[errorCode] + cause;
}

const int& ErrorStatus::getErrorCode() const
{
    return this->errorCode;
}

const std::string& ErrorStatus::getErrorMessage() const
{
    return this->errorMessage;
}

std::ostream& operator<<(std::ostream& os, const ErrorStatus& error)
{
    return os << "[" << error.getErrorCode() << "] " << error.getErrorMessage();
}