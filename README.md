# SFFileManagementKit

## Overview

SFFileManagementKit is a comprehensive project for managing file operations and related utilities. It includes several modules written in C, C++, Swift, and assembly language. This project is part of the ScribbleFoundation open-source initiative, aimed at providing robust file management capabilities.

> [!WARNING]
> Please be aware that this project is still under active development. As such, it may contain bugs, incomplete features, and frequent changes. Use at your own risk and be prepared for potential issues.
>
> We appreciate your interest and welcome feedback and contributions. For any questions or issues, please refer to our [Issue Tracker](https://github.com/ScribbleLabApp/SFFileManagementKit/issues) or [Contact Us]().

## Requirements

Before building the SFFileManagementKit project, ensure you have the following tools and dependencies installed:

- **CMake**: Version 3.15.1 or higher
- **Xcode**: Version compatible with AppleClang and Swift 6.0 (for macOS users)
- **Swift**: Version 6.0 or higher
- **AppleClang**: Version 16.0 or higher
- **Command Line Tools**: Xcode 16 Command Line Tools installed
- **Ninja**: Version 1.12.1 or higher
- **LLVM (for clang-tidy)**: Homebrew LLVM version 18.1.8 Optimized build
- **OpenSSL**: Version 3.3.1 stable (bottled)

### Installation

#### Swift Package Manager (swift) (recommended)

You can integrate SFFileManagementKit into your project using Swift Package Manager (SPM). Here’s how:

1. In Xcode 16, open your project and navigate to File → Swift Packages → Add Package Dependency...
2. Paste the repository URL (https://github.com/ScribbleLabApp/SFFileManagementKit.git) and click Next.
3. For Version, verify it's Up to next major.
4. Click Next and select the `SFFileManagementKit` package.
5. Click Finish.

You can also add it to the dependencies of your `Package.swift` file:

```swift
dependencies: [
  .package(url: "https://github.com/ScribbleLabApp/SFFileManagementKit.git", .upToNextMajor(from: "0.1.0"))
]
```

### Build Instructions

To build the project, use the provided `build.sh` script. This script will create a build directory, configure the project with CMake using the ninja, and then build the project using ninja.

```sh
cd scripts
chmod u+x build.sh
./build.sh
```

This will:
- Create a `build` directory in the project root if it does not exist.
- Configure the project with CMake using the ninja.
- Build the project using `ninja`.
- Lint the entire project using `clang-tidy`

#### Clean Up

To clean up the build artifacts and CMake-generated files, use the `clean_build.sh` script:

```sh
chmod u+x clean_build.sh
./clean_build.sh
```

This script will remove the `build` directory and CMake cache files.

## Contributing

Be part of the next revolution in note-taking by contributing to the project. This is a community-led effort, so we welcome as many contributors who can help. Read the [Contribution Guide](https://github.com/ScribbleLabApp/ScribbleLab/blob/main/CONTRIBUTING.md) for more information.

This project spans [multiple repositories]() so instead of browsing issues in the issues tab, it may be helpful to find an issue to get started on in our [project board](https://github.com/orgs/ScribbleLabApp/projects/1/views/1).

For issues we want to focus on that are most relevant at any given time, please see the issues scoped to our current iteration [here]().

## Support Us

Your support is valuable to us and helps us dedicate more time to enhancing and maintaining this repository. Here's how you can contribute:

⭐️ Leave a Star: If you find this repository useful or interesting, please consider leaving a star on GitHub. Your stars help us gain visibility and encourage others in the community to discover and benefit from this work.

✨ Follow us on Social Media: If you find this repository useful or interesting, please consider leaving a sub on YouTube or Instagram. Your sub help us gain visibility and encourage others in the community to discover and benefit from this work.

📲 Share with Friends: If you like the idea behind this project, please share it with your friends, colleagues, or anyone who might find it valuable.
