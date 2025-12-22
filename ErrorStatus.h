#pragma once
#include <string>

class ErrorStatus
{
    private:
        int errorCode;
        std::string errorMessage;
    
    public:
        ErrorStatus(const int& errorCode, const std::string& cause);

        const int& getErrorCode() const;
        const std::string& getErrorMessage() const;
        
        friend std::ostream& operator<<(std::ostream& os, const ErrorStatus& error);
};