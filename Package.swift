// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

//===-- ./Package.swift - Package Configuration ------------  -*- swift -*-===//
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
//===----------------------------------------------------------------------===//

import PackageDescription

let package = Package(
    name: "SFFileManagementKit",
    platforms: [.iOS(.v18), .macOS(.v15), .macCatalyst(.v18), .visionOS(.v2)],
    products: [
        .library(
            name: "SFFileManagementKit",
            targets: ["SFFileManagementKit"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/apple/swift-crypto.git", .upToNextMajor(from: "3.5.2")),
        .package(url: "https://github.com/ScribbleLabApp/CryptoSwiftWrapper.git", .upToNextMajor(from: "0.1.0-beta.2"))
    ],
    targets: [
        // MARK: - SFFileManagementKit (swift · main)
        .target(
            name: "SFFileManagementKit",
            dependencies: [
                "SFFileCoreBridge",                                 //< ObjC++ bridge
                "CryptoSwiftWrapper",                               //< SLA depend
                .product(name: "Crypto", package: "swift-crypto"),  //< APPL depend
            ],
            resources: [
                .copy("PrivacyInfo.xcprivacy"),
            ]
        ),
        
        // MARK: - SFFileCoreBridge (objc/objc++ · wrapper for libc/libcxx)
        .target(
            name: "SFFileCoreBridge",
            dependencies: [
                "SFFileCore",                                       //< libc/libcxx
                "CryptoSwiftWrapper",                               //< SLA depend
                .product(name: "Crypto", package: "swift-crypto"),  //< APPL depend
            ],
            path: "Sources/SFFileCoreBridge",
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include")
            ]
        ),
        
        // MARK: - SFFileCxxCore (c/c++ · libc/libcxx)
        .target(
            name: "SFFileCore",
            dependencies: [
                "_SFUtils",                                         //< libc/libcxx utils
                "SFFileCoreASM",                                    //< libasm
                "CryptoSwiftWrapper",                               //< SLA depend
                .product(name: "Crypto", package: "swift-crypto")   //< APPL depend
            ],
            path: "Sources/SFFileCore",
            resources: [
                .copy("PrivacyInfo.xcprivacy"),
            ],
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("../_SFUtils/include"),
                .headerSearchPath("../SFFileCoreASM/include"),
                .headerSearchPath("include/libc"),
                .headerSearchPath("include/libcxx")
            ],
            cxxSettings: [
                .define("CXX_STANDARD", to: "17"),
                .unsafeFlags(["-std=c++17"], .when(platforms: [.macOS, .iOS])),
            ],
            linkerSettings: [
                .linkedLibrary("ssl"),
                .linkedLibrary("crypto"),
                .unsafeFlags(["-L/opt/homebrew/Cellar/openssl@3/3.3.1/lib"])
            ]
        ),
        
        // MARK: - SFFileCoreASM (x86asm/arm64asm · libasm)
        .target(
            name: "SFFileCoreASM",
            dependencies: [],
            path: "Sources/SFFileCoreASM",
            resources: [
                .copy("PrivacyInfo.xcprivacy"),
            ],
            publicHeadersPath: "include"
        ),
        
        // MARK: - _SFUtils (c/c++ · utils for libc/libcxx)
        .target(
            name: "_SFUtils",
            dependencies: [],
            path: "Sources/_SFUtils",
            publicHeadersPath: "include",
            cxxSettings: [
                .define("CXX_STANDARD", to: "17"),
                .unsafeFlags(["-std=c++17"], .when(platforms: [.macOS, .iOS]))
            ]
        ),
        
        // MARK: - SFFileManagementKitTests (swift · unit tests)
        .testTarget(
            name: "SFFileManagementKitTests",
            dependencies: ["SFFileManagementKit"]
        ),
    ]
)
