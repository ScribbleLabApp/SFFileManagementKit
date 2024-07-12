// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SFFileManagementKit",
    products: [
        // Products define the executables and libraries a package produces, making them visible to other packages.
        .library(
            name: "SFFileManagementKit",
            targets: ["SFFileManagementKit"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package, defining a module or a test suite.
        // Targets can depend on other targets in this package and products from dependencies.
        .target(
            name: "SFFileManagementKit"),
        .target(name: "SFFileCoreBridge",
            dependencies: ["SFFileCxxCore"],
            path: "Sources/SFFileCoreBridge",
            publicHeadersPath: "Sources/SFFileCoreBridge/include",
            cSettings: [
                .headerSearchPath("include")
            ]
               ),
        .target(
            name: "SFFileCxxCore",
            dependencies: ["SFFileManagementKit"],
            path: "Sources/SFFileCxxCore",
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
