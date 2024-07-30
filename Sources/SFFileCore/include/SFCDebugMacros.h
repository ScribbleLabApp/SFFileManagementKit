//===-- include/SFCDebugMacros.h - Debug Macros -----------------*- C++ -*-===//
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

#ifndef __SFCDebugMacros_h__
#define __SFCDebugMacros_h__

///	This is a macro that does a sizeof and casts the result to a UInt32. This is 
/// useful for all the places where -wshorten64-32 catches assigning a sizeof expression 
/// to a UInt32. For want of a better place to park this, we'll park it here.
#define	SizeOf32(X)	((UInt32)sizeof(X))

///	This is a macro that does a offsetof and casts the result to a UInt32. This is useful for all the
///	places where -wshorten64-32 catches assigning an offsetof expression to a UInt32.
///	For want of a better place to park this, we'll park it here.
#define	OffsetOf32(X, Y)	((UInt32)offsetof(X, Y))

///	This macro casts the expression to a UInt32. It is called out specially to allow us to track casts
///	that have been added purely to avert -wshorten64-32 warnings on 64 bit platforms.
///	For want of a better place to park this, we'll park it here.
#define	ToUInt32(X)	((UInt32)(X))

#define	DebugMsg(inFormat, ...)
#ifndef DEBUGPRINT
    #define DEBUGPRINT(msg)
#endif
#define vprint(msg)
#define	STOP
#define __ASSERT_STOP
#define __THROW_STOP

#define	DebugMessage(msg)										DebugMsg(msg)
#define DebugMessageN1(msg, N1)									DebugMsg(msg, N1)
#define DebugMessageN2(msg, N1, N2)								DebugMsg(msg, N1, N2)
#define DebugMessageN3(msg, N1, N2, N3)							DebugMsg(msg, N1, N2, N3)
#define DebugMessageN4(msg, N1, N2, N3, N4)						DebugMsg(msg, N1, N2, N3, N4)
#define DebugMessageN5(msg, N1, N2, N3, N4, N5)					DebugMsg(msg, N1, N2, N3, N4, N5)
#define DebugMessageN6(msg, N1, N2, N3, N4, N5, N6)				DebugMsg(msg, N1, N2, N3, N4, N5, N6)
#define DebugMessageN7(msg, N1, N2, N3, N4, N5, N6, N7)			DebugMsg(msg, N1, N2, N3, N4, N5, N6, N7)
#define DebugMessageN8(msg, N1, N2, N3, N4, N5, N6, N7, N8)		DebugMsg(msg, N1, N2, N3, N4, N5, N6, N7, N8)
#define DebugMessageN9(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)	DebugMsg(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)

/**
 * Logs an error message using the printf-style format string and variable arguments.
 *
 * @param fmt A printf-style format string specifying the error message.
 * @param ... Variable arguments to be formatted according to the format string.
 *
 * @return void
 *
 * @note This function does not return a value. It is used to log error messages.
 * @note The error message is formatted using the printf-style format string and variable arguments.
 * @note The error message is logged to the appropriate logging mechanism, such as the console or a file.
 */
void    LogError(const char *fmt, ...);

/**
 * Logs a warning message using the printf-style format string and variable arguments.
 *
 * @brief Logs a warning message.
 *
 * @param fmt A printf-style format string specifying the warning message.
 * @param ... Variable arguments to be formatted according to the format string.
 *
 * @return void
 *
 * @note This function does not return a value. It is used to log warning messages.
 * @note The warning message is formatted using the printf-style format string and variable arguments.
 */
void	LogWarning(const char *fmt, ...);


#define	NO_ACTION	(void)0

#if DEBUG

#pragma mark	Debug Macros

#define	Assert(inCondition, inMessage)													    \
			if(!(inCondition))															    \
			{																			    \
				DebugMessage(inMessage);												    \
				__ASSERT_STOP;															    \
			}

#define	AssertFileLine(inCondition, inMessage)											    \
			if(!(inCondition))															    \
			{																			    \
				DebugMessageN3("%s, line %d: %s", __FILE__, __LINE__, inMessage);		    \
				__ASSERT_STOP;															    \
			}

#define	AssertNoError(inError, inMessage)												    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					char __4CC[5] = CA4CCToCString(__Err);								    \
					DebugMessageN2(inMessage ", Error: %d (%s)", (int)__Err, __4CC);	    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	AssertNoKernelError(inError, inMessage)											    \
			{																			    \
				unsigned int __Err = (unsigned int)(inError);							    \
				if(__Err != 0)															    \
				{																		    \
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	AssertNotNULL(inPtr, inMessage)													    \
			{																			    \
				if((inPtr) == NULL)														    \
				{																		    \
					DebugMessage(inMessage);											    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	FailIf(inCondition, inHandler, inMessage)										    \
			if(inCondition)																    \
			{																			    \
				DebugMessage(inMessage);												    \
				STOP;																	    \
				goto inHandler;															    \
			}

#define	FailWithAction(inCondition, inAction, inHandler, inMessage)						    \
			if(inCondition)																    \
			{																			    \
				DebugMessage(inMessage);												    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfNULL(inPointer, inAction, inHandler, inMessage)							    \
			if((inPointer) == NULL)														    \
			{																			    \
				DebugMessage(inMessage);												    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfKernelError(inKernelError, inAction, inHandler, inMessage)				    \
			{																			    \
				unsigned int __Err = (inKernelError);									    \
				if(__Err != 0)															    \
				{																		    \
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					    \
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#define	FailIfError(inError, inAction, inHandler, inMessage)							    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					char __4CC[5] = CA4CCToCString(__Err);								    \
					DebugMessageN2(inMessage ", Error: %ld (%s)", (long int)__Err, __4CC);	\
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#define	FailIfNoMessage(inCondition, inHandler, inMessage)								    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				goto inHandler;															    \
			}

#define	FailWithActionNoMessage(inCondition, inAction, inHandler, inMessage)			    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfNULLNoMessage(inPointer, inAction, inHandler, inMessage)					    \
			if((inPointer) == NULL)														    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfKernelErrorNoMessage(inKernelError, inAction, inHandler, inMessage)		    \
			{																			    \
				unsigned int __Err = (inKernelError);									    \
				if(__Err != 0)															    \
				{																		    \
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#define	FailIfErrorNoMessage(inError, inAction, inHandler, inMessage)					    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#if defined(__cplusplus)

#define Throw(inException)  __THROW_STOP; throw (inException)

#define	ThrowIf(inCondition, inException, inMessage)									    \
			if(inCondition)																    \
			{																			    \
				DebugMessage(inMessage);												    \
				Throw(inException);														    \
			}

#define	ThrowIfNULL(inPointer, inException, inMessage)									    \
			if((inPointer) == NULL)														    \
			{																			    \
				DebugMessage(inMessage);												    \
				Throw(inException);														    \
			}

#define	ThrowIfKernelError(inKernelError, inException, inMessage)						    \
			{																			    \
				unsigned int __Err = (inKernelError);									    \
				if(__Err != 0)															    \
				{																		    \
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					    \
					Throw(inException);													    \
				}																		    \
			}

#define	ThrowIfError(inError, inException, inMessage)									    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					char __4CC[5] = CA4CCToCString(__Err);								    \
					DebugMessageN2(inMessage ", Error: %d (%s)", (int)__Err, __4CC);		\
					Throw(inException);													    \
				}																		    \
			}

#if TARGET_OS_WIN32
#define	ThrowIfWinError(inError, inException, inMessage)								    \
			{																			    \
				HRESULT __Err = (inError);												    \
				if(FAILED(__Err))														    \
				{																		    \
					DebugMessageN2(inMessage ", Code: %d, Facility: 0x%X", HRESULT_CODE(__Err), HRESULT_FACILITY(__Err));	\
					Throw(inException);													    \
				}																		    \
			}
#endif /* TARGET_OS_WIN32 */

#define	SubclassResponsibility(inMethodName, inException)								    \
			{																			    \
				DebugMessage(inMethodName": Subclasses must implement this method");	    \
				Throw(inException);														    \
			}


#endif /* defined(__cplusplus) */

#else 
#pragma mark	Release Macros

#define	Assert(inCondition, inMessage)													    \
			if(!(inCondition))															    \
			{																			    \
				__ASSERT_STOP;															    \
			}

#define AssertFileLine(inCondition, inMessage) Assert(inCondition, inMessage)

#define	AssertNoError(inError, inMessage)												    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	AssertNoKernelError(inError, inMessage)											    \
			{																			    \
				unsigned int __Err = (unsigned int)(inError);							    \
				if(__Err != 0)															    \
				{																		    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	AssertNotNULL(inPtr, inMessage)													    \
			{																			    \
				if((inPtr) == NULL)														    \
				{																		    \
					__ASSERT_STOP;														    \
				}																		    \
			}

#define	FailIf(inCondition, inHandler, inMessage)										    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				goto inHandler;															    \
			}

#define	FailWithAction(inCondition, inAction, inHandler, inMessage)						    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfNULL(inPointer, inAction, inHandler, inMessage)							    \
			if((inPointer) == NULL)														    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfKernelError(inKernelError, inAction, inHandler, inMessage)				    \
			if((inKernelError) != 0)													    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfError(inError, inAction, inHandler, inMessage)							    \
			if((inError) != 0)															    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfNoMessage(inCondition, inHandler, inMessage)								    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				goto inHandler;															    \
			}

#define	FailWithActionNoMessage(inCondition, inAction, inHandler, inMessage)			    \
			if(inCondition)																    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfNULLNoMessage(inPointer, inAction, inHandler, inMessage)					    \
			if((inPointer) == NULL)														    \
			{																			    \
				STOP;																	    \
				{ inAction; }															    \
				goto inHandler;															    \
			}

#define	FailIfKernelErrorNoMessage(inKernelError, inAction, inHandler, inMessage)		    \
			{																			    \
				unsigned int __Err = (inKernelError);									    \
				if(__Err != 0)															    \
				{																		    \
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#define	FailIfErrorNoMessage(inError, inAction, inHandler, inMessage)					    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					STOP;																    \
					{ inAction; }														    \
					goto inHandler;														    \
				}																		    \
			}

#if defined(__cplusplus)

#define Throw(inException)  __THROW_STOP; throw (inException)

#define	ThrowIf(inCondition, inException, inMessage)									    \
			if(inCondition)																    \
			{																			    \
				Throw(inException);														    \
			}

#define	ThrowIfNULL(inPointer, inException, inMessage)									    \
			if((inPointer) == NULL)														    \
			{																			    \
				Throw(inException);														    \
			}

#define	ThrowIfKernelError(inKernelError, inException, inMessage)						    \
			{																			    \
				unsigned int __Err = (inKernelError);									    \
				if(__Err != 0)															    \
				{																		    \
					Throw(inException);													    \
				}																		    \
			}

#define	ThrowIfError(inError, inException, inMessage)									    \
			{																			    \
				SInt32 __Err = (inError);												    \
				if(__Err != 0)															    \
				{																		    \
					Throw(inException);													    \
				}																		    \
			}

#if TARGET_OS_WIN32
#define	ThrowIfWinError(inError, inException, inMessage)								    \
			{																			    \
				HRESULT __Err = (inError);												    \
				if(FAILED(__Err))														    \
				{																		    \
					Throw(inException);													    \
				}																		    \
			}
#endif

#define	SubclassResponsibility(inMethodName, inException)								    \
			{																			    \
				Throw(inException);														    \
			}

#endif /* defined(__cplusplus) */
#endif /*        DEBUG         */

#endif /* __SFCDebugMacros_h__ */