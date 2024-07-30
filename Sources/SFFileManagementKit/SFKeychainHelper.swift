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
//            setBundleIdentifier(cString)
//        }
//    }
//    
//    static func storeKeyInKeychain(key: Data, keySuffix: String) -> Bool {
//        return key.withUnsafeBytes { (keyBytes: UnsafeRawBufferPointer) -> Bool in
//            guard let keyBaseAddress = keyBytes.baseAddress else {
//                DEBUG_LOG("Failed to get key base address")
//                return false
//            }
//            let success = SFFileCore.storeKeyInKeychain(keyBaseAddress.assumingMemoryBound(to: UInt8.self), key.count, keySuffix) == 1
//            if !success {
//                DEBUG_LOG("Failed to store key in keychain for suffix: \(keySuffix)")
//            }
//            return success
//        }
//   }
//
//    static func retrieveKeyFromKeychain(keySuffix: String) -> Data? {
//        guard let cfData = retrieveKeyFromKeychain(keySuffix: keySuffix) else {
//            DEBUG_LOG("Failed to retrieve key from keychain for suffix: \(keySuffix)")
//            return nil
//        }
//        let length = CFDataGetLength(cfData as CFData)
//        let dataPointer = CFDataGetBytePtr(cfData as CFData)
//        if dataPointer == nil {
//            DEBUG_LOG("Failed to get data pointer for key suffix: \(keySuffix)")
//            return nil
//        }
//        return Data(bytes: dataPointer!, count: length)
//     }
//}
