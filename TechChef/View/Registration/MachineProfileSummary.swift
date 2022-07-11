//
//  MachineProfileSummary.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

struct MachineProfileSummary: View {
    var machineProfile: MachineProfile
    
    var body: some View {
        ScrollView{
            VStack(alignment: .leading, spacing: 10)  {
                Text("Machine ID: \(machineProfile.id)")
                    .bold()
                .font(.title)
                
                Divider()
                
                Text("Duration of Usage: \(machineProfile.age) year")
                
                // Text("Current machine temperature: \(machineProfile.temp) degree C")
                
            }
        }
    }
}

struct MachineProfileSummary_Previews: PreviewProvider {
    static var previews: some View {
        MachineProfileSummary(machineProfile: MachineProfile.default)
    }
}
