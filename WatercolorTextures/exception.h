#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>
#include <iostream>
#include <sstream>

#define MAKE_EXCEPTION(className, shortTypedefName, errorMessage) \
	class className : public std::exception \
	{ \
	public: \
		className() {}; \
		className(const className &e) : file(e.file), line(e.line), assertion(e.assertion) {} \
		className(std::string file, int line, std::string assertion = "") : file(file), line(line), assertion(assertion) {} \
		virtual const char* what() const throw() \
		{ \
			std::stringstream oss; \
			oss << "Message: " << errorMessage << std::endl \
				<< "File   : " << std::string(file.length() > 60 ? "..." : "") << file.substr(file.length() > 60 ? file.length()-60 : 0) << std::endl \
				<< "Line   : " << line; \
			if (assertion.length() > 0) oss << std::endl << "Assert : " << assertion; \
			std::cout << oss.str() << std::endl; \
			return oss.str().c_str(); \
		} \
	protected: \
		std::string file; \
		int line; \
		std::string assertion; \
	}; \
	typedef className shortTypedefName;

MAKE_EXCEPTION(UnknownParamRequestException, E_UnkParReq, "Unknown parameter was requested")
MAKE_EXCEPTION(InvalidParamValueException, E_InvParVal, "Invalid parameter value was given")
MAKE_EXCEPTION(InvalidAssertionException, E_InvAss, "Invalid assertion detected")

#define THROW(exceptionClass) { exceptionClass e(__FILE__, __LINE__); e.what(); throw e; }
#define THROW_ASSERTION(exceptionClass, condition) { exceptionClass e(__FILE__, __LINE__, condition); e.what(); throw e; }

#define ASSERT(conditon) if (!(conditon)) THROW_ASSERTION(E_InvAss, #conditon)

#endif

#include <opencv2/core/core.hpp>
