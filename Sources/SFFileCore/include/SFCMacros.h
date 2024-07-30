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

#ifndef __SFCMacros_h__
#define __SFCMacros_h__

/* Platform-specific macros */
#ifdef _WIN32
    #include <windows.h>
    #define EXPORT __declspec(dllexport)
#else
    #include <unistd.h>
    #define EXPORT
#endif

/* Compiler-specific macros */
#if defined(__GNUC__)
    #define LIKELY(x) __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#elif defined(_MSC_VER)
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
#else
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define NO_RETURN __attribute__((__noreturn__))
    #define PURE __attribute__((__pure__))
    #define CONST __attribute__((__const__))
#else
    #define NO_RETURN
    #define PURE
    #define CONST
#endif

#define NO_OPTIMIZE __attribute__((optimize("O0")))

#if __has_attribute(warn_unused_result)
    #define __result_use_check __attribute__((__warn_unused_result__))
#else
    #define __result_use_check
#endif

/* Deprecation and Unavailability Attributes */
#define __deprecated    __attribute__((__deprecated__))

#if __has_extension(attribute_deprecated_with_message) || \
        (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5))))
    #define __deprecated_msg(_msg) __attribute__((__deprecated__(_msg)))
#else
    #define __deprecated_msg(_msg) __attribute__((__deprecated__))
#endif

#if __has_extension(enumerator_attributes)
    #define __deprecated_enum_msg(_msg) __deprecated_msg(_msg)
#else
    #define __deprecated_enum_msg(_msg)
#endif

#define __kpi_deprecated(_msg) __deprecated_msg(_msg)

#if __has_attribute(unavailable)
    #define __unavailable __attribute__((__unavailable__))
#else
    #define __unavailable
#endif

#define __kpi_unavailable __unavailable
#define __kpi_deprecated_arm64_macos_unavailable __unavailable

#if __has_attribute(__availability__)
    #define __OSX_AVAILABLE_STARTING(_mac, _iphone) __attribute__((__availability__(macOS, introduced=_mac, ios, introduced=_iphone)))
    #define __API_AVAILABLE(_mac, _ios, _tvos, _watchos) __attribute__((__availability__(macOS, introduced=_mac), __availability__(ios, introduced=_ios), __availability__(tvos, introduced=_tvos), __availability__(watchOS, introduced=_watchos)))
#else
    #define __OSX_AVAILABLE_STARTING(_mac, _iphone)
    #define __API_AVAILABLE(_mac, _ios, _tvos, _watchos)
#endif

/* Swift-related attributes */
#if __has_feature(attribute_availability_swift)
    #define __swift_unavailable(_msg) __attribute__((__availability__(swift, unavailable, message=_msg)))
#else
    #define __swift_unavailable(_msg)
#endif

#if __has_attribute(__swift_attr__)
    #define __swift_unavailable_from_async(_msg) __attribute__((__swift_attr__("@_unavailableFromAsync(message: \"" _msg "\")")))
    #define __swift_nonisolated __attribute__((__swift_attr__("nonisolated")))
    #define __swift_nonisolated_unsafe __attribute__((__swift_attr__("nonisolated(unsafe)")))
#else
    #define __swift_unavailable_from_async(_msg)
    #define __swift_nonisolated
    #define __swift_nonisolated_unsafe
#endif

#if defined(__cplusplus)
    #define __BEGIN_DECLS extern "C" {
    #define __END_DECLS }
#else
    #define __BEGIN_DECLS
    #define __END_DECLS
#endif

/* Compile-time Assertions and Utility Macros */
#define STATIC_ASSERT(expr, msg) static_assert(expr, msg)

#define SAFE_DELETE(ptr) do { delete (ptr); (ptr) = nullptr; } while (0)
#define SAFE_DELETE_ARRAY(ptr) do { delete[] (ptr); (ptr) = nullptr; } while (0)

#define SANITY_CHECK(cond)                                                                                                  \
    do {                                                                                                                    \
        if (!(cond)) {                                                                                                      \
            std::cerr << "Sanity check failed: " << #cond << ", file " << __FILE__ << ", line " << __LINE__ << std::endl;   \
            std::abort();                                                                                                   \
        }                                                                                                                   \
    } while (0)

/* API Versioning */
#define API_VERSION_MAJOR 1
#define API_VERSION_MINOR 0
#define API_VERSION_PATCH 0

#define CHECK_API_VERSION(major, minor, patch) \
    ((API_VERSION_MAJOR > (major)) || \
     (API_VERSION_MAJOR == (major) && API_VERSION_MINOR > (minor)) || \
     (API_VERSION_MAJOR == (major) && API_VERSION_MINOR == (minor) && API_VERSION_PATCH >= (patch)))

/* Smart Pointers */
#define UNIQUE_PTR(type) std::unique_ptr<type>
#define SHARED_PTR(type) std::shared_ptr<type>

/* Stringification */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/* Memory Management */
#define NO_OPTIMIZE __attribute__((optimize("O0")))

/* Feature Detection */
#if __cplusplus >= 201703L
    #define HAS_CPP17 1
#else
    #define HAS_CPP17 0
#endif

#if defined(__cpp_constexpr)
    #define HAS_CONSTEXPR 1
#else
    #define HAS_CONSTEXPR 0
#endif

#if defined(_MSC_VER)
    #define INLINE __forceinline
#else
    #define INLINE inline
#endif

#ifndef __COPYRIGHT
#define __COPYRIGHT(s) __IDSTRING(copyright,s)
#endif

#ifndef __RCSID
#define __RCSID(s) __IDSTRING(rcsid,s)
#endif

#ifndef __SCCSID
#define __SCCSID(s) __IDSTRING(sccsid,s)
#endif

#ifndef __PROJECT_VERSION
#define __PROJECT_VERSION(s) __IDSTRING(project_version,s)
#endif

#endif /* __SFCMacros_h__ */
