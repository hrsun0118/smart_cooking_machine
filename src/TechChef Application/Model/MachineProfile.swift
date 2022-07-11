//
//  MachineProfile.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import Foundation

struct MachineProfile {
    var id: String
    // var users = [String]() // each machine can have a list of users
    var age: Int
    var temp: Float
    var oilButton: Bool
    var waterButton: Bool
    var motorButton: Bool
    
    static let `default` = MachineProfile(id: "1234", age: 1, temp: 25.00, oilButton: false, waterButton: false, motorButton: false)
}
