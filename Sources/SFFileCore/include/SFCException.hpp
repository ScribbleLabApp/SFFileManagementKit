//===-- include/SFCException.h - Exception Handler --------------*- C++ -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// Copyright (c) 2016 - 2024 Apple Inc. All Rights Reserved.                  //
// Some of the definitions were borrowed from Apple's CoreAudio Utility       //
// Classes.                                                                   //
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
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#ifndef __SFCException_h__
#define __SFCException_h__

#include <ConditionalMacros.h>
#include "SFCDebugMacros.h"
#include <ctype.h>
#include <string.h>
#include <CoreServices/CoreServices.h>

class SFC4CCString {
public:
    /**
     * @brief Constructs a string representation of an OSStatus error code.
     *
     * This constructor takes an OSStatus error code as input and generates a string representation of the error.
     * The string is formatted based on the following rules:
     * - If the error code represents a printable ASCII sequence, it is enclosed in single quotes.
     * - If the error code falls within the range of -200000 to 200000, it is represented as a decimal integer.
     * - Otherwise, the error code is represented in hexadecimal format.
     *
     * @param error The OSStatus error code to be represented as a string.
     */
    SFC4CCString(OSStatus error) {
        char *str = mStr;
        *(UInt32 *)(str + 1) = CFSwapInt32HostToBig(error);

        if (isprint(str[1]) && isprint(str[2]) && isprint(str[3]) && isprint(str[4])) {
			str[0] = str[5] = '\'';
			str[6] = '\0';
		} else if (error > -200000 && error < 200000)
			sprintf(str, "%d", (int)error);
		else
			sprintf(str, "0x%x", (int)error);
    }

    const char *get() const { return mStr; }
    operator const char *() const { return mStr; }

private:
    char mStr[16];
};

class SFCException {
public:
    /**
     * @brief Constructs an SFCException object with the given operation and error code.
     *
     * This constructor initializes the SFCException object with the provided operation and error code.
     * If the operation is NULL, an empty string is assigned to mOperation.
     * If the operation's length exceeds the size of mOperation, it is truncated to fit.
     * Otherwise, the operation is copied to mOperation.
     *
     * @param operation A pointer to a null-terminated C string representing the operation that caused the exception.
     * @param err An OSStatus error code representing the error that occurred during the operation.
     */
    SFCException(const char *operation, OSStatus err) : mError(err) {
        if (operation == NULL)
			mOperation[0] = '\0';
		else if (strlen(operation) >= sizeof(mOperation)) {
			memcpy(mOperation, operation, sizeof(mOperation) - 1);
			mOperation[sizeof(mOperation) - 1] = '\0';
		} else

			strlcpy(mOperation, operation, sizeof(mOperation));
    }
    
    /**
     * @brief Formats the error message based on the provided error code.
     *
     * This function takes a character pointer `str` as a parameter and formats the error message based on the
     * provided error code `mError`. The formatted error message is then returned.
     *
     * @param str A pointer to a character array where the formatted error message will be stored.
     *
     * @return A pointer to the character array `str` containing the formatted error message.
     *
     * @note This function internally calls the static member function `FormatError(char *str, OSStatus error)`.
     */
    char *FormatError(char *str) const {
		return FormatError(str, mError);
	}
	
	char				mOperation[256];
	const OSStatus		mError;

    typedef void (*WarningHandler)(const char *msg, OSStatus err);
	
    /**
     * @brief Formats the error message based on the provided error code.
     *
     * This function takes a character pointer `str` as a parameter and formats the error message based on the
     * provided error code `error`. The formatted error message is then returned.
     *
     * @param str A pointer to a character array where the formatted error message will be stored.
     * @param error An OSStatus error code representing the error that occurred during the operation.
     *
     * @return A pointer to the character array `str` containing the formatted error message.
     *
     * @note This function internally calls the static member function `SFC4CCString(OSStatus error)` to convert the error code to a string representation.
     */
    static char *FormatError(char *str, OSStatus error) {
		strcpy(str, SFC4CCString(error));
		return str;
	}
	
    /**
     * @brief A static member function that handles warnings by invoking a registered warning handler.
     *
     * This function checks if a warning handler has been registered using SetWarningHandler().
     * If a warning handler is registered, it is invoked with the provided warning message and error code.
     *
     * @param s A pointer to a null-terminated C string representing the warning message.
     * @param error An OSStatus error code representing the error that caused the warning.
     *
     * @note This function does not throw any exceptions.
     */
	static void Warning(const char *s, OSStatus error) {
		if (sWarningHandler)
			(*sWarningHandler)(s, error);
	}
	
    /**
     * @brief Sets the warning handler function for SFCException.
     *
     * This function allows the user to register a custom warning handler function.
     * The warning handler will be invoked whenever a warning is triggered within the SFCException class.
     *
     * @param f A pointer to the warning handler function. The function should take two parameters:
     *          - const char *msg: A pointer to a null-terminated C string representing the warning message.
     *          - OSStatus err: An OSStatus error code representing the error that caused the warning.
     *
     * @note The warning handler function should not throw any exceptions.
     *
     * @note If no warning handler is set, the default behavior is to do nothing.
     */	
    static void SetWarningHandler(WarningHandler f) { sWarningHandler = f; }
private:
    static WarningHandler sWarningHandler;
};

#if	DEBUG

#define XThrowIfError(error, operation)										                        \
		do {																	                    \
			OSStatus __err = error;												                    \
			if (__err) {														                    \
				DebugMessageN2("about to throw %s: %s", SFC4CCString(__err).get(), operation);      \
				__THROW_STOP;															            \
				throw SFCException(operation, __err);							                    \
			}																	                    \
		} while (0)

        #define XThrowIf(condition, error, operation)								                \
		do {																	                    \
			if (condition) {													                    \
				OSStatus __err = error;											                    \
				DebugMessageN2("about to throw %s: %s", SFC4CCString(__err).get(), operation);      \
				__THROW_STOP;															            \
				throw SFCException(operation, __err);							                    \
			}																	                    \
		} while (0)

	#define XRequireNoError(error, label)										                    \
		do {																	                    \
			OSStatus __err = error;												                    \
			if (__err) {														                    \
				DebugMessageN2("about to throw %s: %s", SFC4CCString(__err).get(), #error);\
				STOP;															                    \
				goto label;														                    \
			}																	                    \
		} while (0)
	
	#define XAssert(assertion)													                    \
		do {																	                    \
			if (!(assertion)) {													                    \
				DebugMessageN3("[%s, %d] error: failed assertion: %s", __FILE__, __LINE__, #assertion);		\
				__ASSERT_STOP;															            \
			}																	                    \
		} while (0)
	
	#define XAssertNoError(error)												                    \
		do {																	                    \
			OSStatus __err = error;												                    \
			if (__err) {														                    \
				DebugMessageN2("error %s: %s", SFC4CCString(__err).get(), #error);                  \
				STOP;															                    \
			}																	                    \
		} while (0)

	#define ca_require_noerr(errorCode, exceptionLabel)							                    \
        do	{																	                    \
			int evalOnceErrorCode = (errorCode);								                    \
            if ( __builtin_expect(0 != evalOnceErrorCode, 0) )	 {				                    \
				DebugMessageN5("ca_require_noerr: [%s, %d] (goto %s;) %s:%d",	                    \
					#errorCode,	evalOnceErrorCode,		 						                    \
					#exceptionLabel,											                    \
					__FILE__,													                    \
					__LINE__);													                    \
				goto exceptionLabel;											                    \
			}																	                    \
		} while ( 0 )

	#define ca_verify_noerr(errorCode)											                    \
		do {																	                    \
			int evalOnceErrorCode = (errorCode);								                    \
			if ( __builtin_expect(0 != evalOnceErrorCode, 0) )	 {									\
				DebugMessageN4("ca_verify_noerr: [%s, %d] %s:%d",				                    \
					#errorCode,	evalOnceErrorCode,								                    \
					__FILE__,													                    \
					__LINE__);													                    \
			}																	                    \
		} while ( 0 )

	#define ca_debug_string(message)											                    \
		do	{																	                    \
			DebugMessageN3("ca_debug_string: %s %s:%d",							                    \
				message,														                    \
				__FILE__,														                    \
				__LINE__);														                    \
		} while ( 0 )

	#define ca_verify(assertion)												                    \
		do	{																	                    \
			if ( __builtin_expect(!(assertion), 0) )	{			    		                    \
				DebugMessageN3("ca_verify: %s %s:%d",							                    \
					#assertion,													                    \
					__FILE__,													                    \
					__LINE__);													                    \
			}																	                    \
		} while ( 0 )

	#define ca_require(assertion, exceptionLabel)								                    \
		do	{																	                    \
			if ( __builtin_expect(!(assertion), 0) )	{						                    \
				DebugMessageN4("ca_require: %s %s %s:%d",						                    \
					#assertion,													                    \
					#exceptionLabel,											                    \
					__FILE__,													                    \
					__LINE__);													                    \
				goto exceptionLabel;											                    \
			}																	                    \
		} while ( 0 )

   #define ca_check(assertion)													                    \
      do {							    										                    \
          if ( __builtin_expect(!(assertion), 0) )	    {						                    \
              DebugMessageN3("ca_check: %s %s:%d",							                        \
                  #assertion,													                    \
                  __FILE__,														                    \
                  __LINE__);													                    \
          }																		                    \
      } while ( 0 )
#else
	#define XThrowIfError(error, operation)										                    \
		do {																	                    \
			OSStatus __err = error;												                    \
			if (__err) {														                    \
				throw SFCException(operation, __err);							                    \
			}																	                    \
		} while (0)

	#define XThrowIf(condition, error, operation)								                    \
		do {																	                    \
			if (condition) {													                    \
				OSStatus __err = error;											                    \
				throw SFCException(operation, __err);							                    \
			}																	                    \
		} while (0)

	#define XRequireNoError(error, label)										                    \
		do {																	                    \
			OSStatus __err = error;												                    \
			if (__err) {														                    \
				goto label;														                    \
			}																	                    \
		} while (0)

	#define XAssert(assertion)													                    \
		do {																	                    \
			if (!(assertion)) {													                    \
			}																	                    \
		} while (0)

	#define XAssertNoError(error)												                    \
		do {																	                    \
			/*OSStatus __err =*/ error;											                    \
		} while (0)

	#define ca_require_noerr(errorCode, exceptionLabel)							                    \
		do	{																	                    \
			if ( __builtin_expect(0 != (errorCode), 0) )    {						                \
				goto exceptionLabel;											                    \
			}																	                    \
		} while ( 0 )

	#define ca_verify_noerr(errorCode)											                    \
		do {																	                    \
			if ( 0 != (errorCode) )	{}				    						                    \
		} while ( 0 )

	#define ca_debug_string(message)

	#define ca_verify(assertion)												                    \
		do {																	                    \
			if ( !(assertion) )	{}												                    \
		} while ( 0 )

	#define ca_require(assertion, exceptionLabel)								                    \
		do {																	                    \
			if ( __builtin_expect(!(assertion), 0) )	{						                    \
				goto exceptionLabel;											                    \
			}																	                    \
		} while ( 0 )

   #define ca_check(assertion)													                    \
		do {												     				                    \
			if ( !(assertion) )	{}															        \
		} while ( 0 )
#endif

#define XThrow(error, operation) XThrowIf(true, error, operation)
#define XThrowIfErr(error) XThrowIfError(error, #error)

#endif
