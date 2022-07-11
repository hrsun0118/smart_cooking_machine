//
//  TechChefApp.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

@main
struct TechChefApp: App {
    @StateObject private var machineModelData = MachineModelData()
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(machineModelData)
        }
    }
}
