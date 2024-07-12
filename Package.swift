// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SFFileManagementKit",
    products: [
        .library(
            name: "SFFileManagementKit",
            targets: ["SFFileManagementKit"]
        ),
    ],
    targets: [
        .target(
            name: "SFFileManagementKit",
            dependencies: []
        ),
        .target(
            name: "SFFileCoreBridge",
            dependencies: ["SFFileCxxCore"],
            path: "Sources/SFFileCoreBridge",
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include")
            ]
        ),
        .target(
            name: "SFFileCxxCore",
            dependencies: ["SFFileManagementKit", "_SFCxxUtils"],
            path: "Sources/SFFileCxxCore",
            publicHeadersPath: "include",
            cxxSettings: [
                .define("CXX_STANDARD", to: "17"),
                .unsafeFlags(["-std=c++17"], .when(platforms: [.macOS, .iOS]))
            ]
        ),
        .target(
            name: "_SFCxxUtils",
            dependencies: [],
            path: "Sources/_SFCxxUtils",
            publicHeadersPath: "include",
            cxxSettings: [
                .define("CXX_STANDARD", to: "17"),
                .unsafeFlags(["-std=c++17"], .when(platforms: [.macOS, .iOS]))
            ]
        ),
        .testTarget(
            name: "SFFileManagementKitTests",
            dependencies: ["SFFileManagementKit"]
        ),
    ]
)
