/*
 * BasicException.h
 *
 *  Created on: Oct 8, 2019
 *      Author: guogh
 */

#ifndef EDA_COMPETITION_LIBOUTPUTTER_BASICEXCEPTION_H_
#define EDA_COMPETITION_LIBOUTPUTTER_BASICEXCEPTION_H_

#include <string>

class BasicException
{
public:
    BasicException(const char* format, ...);

    const char* GetMessage() const
    {
        return msg_.c_str();
    }

private:
    void SetMessage(const std::string& msg)
    {
        msg_ = msg;
    }

private:
    std::string msg_;
};

#endif /* EDA_COMPETITION_LIBOUTPUTTER_BASICEXCEPTION_H_ */
