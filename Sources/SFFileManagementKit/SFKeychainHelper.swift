//
//  File.swift
//  SFFileManagementKit
//
//  Created by Nevio Hirani on 25.07.24.
//

//import Foundation
//import SFFileCore
//
//class SFKeychainHelper {
//    static func setBundleIdentifier(_ bundleId: String) {
//        bundleId.withCString { cString in
////            setBundleIdentifier(cString)
//        }
//    }
//    
//    static func storeKeyInKeychain(key: Data, keySuffix: String) -> Bool {
//        return key.withUnsafeBytes { (keyBytes: UnsafeRawBufferPointer) -> Bool in
//            guard let keyBaseAddress = keyBytes.baseAddress else { return false }
//            // Return true if storeKeyInKeychain returns success (1), otherwise false
//            return SFFileCore.storeKeyInKeychain(keyBaseAddress.assumingMemoryBound(to: UInt8.self), key.count, keySuffix) == 1
//        }
//    }
//    
//    static func retrieveKeyFromKeychain(keySuffix: String) -> Data? {
//        guard let cfData = retrieveKeyFromKeychain(keySuffix: keySuffix) else { return nil }
//        let length = CFDataGetLength(cfData as CFData)
//        let dataPointer = CFDataGetBytePtr(cfData as CFData)
//        return dataPointer != nil ? Data(bytes: dataPointer!, count: length) : nil
//    }
//}
