//
//  CookingControls.swift
//  TechChef
//
//  Created by Hairuo Sun on 7/10/22.
//

import SwiftUI

struct CookingControls: View {
    @EnvironmentObject var machineModelData: MachineModelData
    
    var body: some View {
        ScrollView{
            VStack{
                Text("Cooking Controls").font(.largeTitle).bold().foregroundColor(.purple)
                Spacer()
                Spacer()
                Spacer()
                // Temperature Display
                HStack{
                    Text("Pot Temperature")
                    Spacer()
                    Text("\(machineModelData.machineProfile.temp, specifier: "%.2f") Degree C").foregroundColor(.init( red: 1.0, green: 0.0, blue: 0.5))
                }
                
                
                // Water Button
                Toggle("Water Button", isOn: $machineModelData.machineProfile.waterButton)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                
                // Oil Button
                Toggle("Oil Button", isOn: $machineModelData.machineProfile.oilButton)
                                .toggleStyle(SwitchToggleStyle(tint: .yellow))
                
                // Motor Input
                Toggle("Motor Button", isOn: $machineModelData.machineProfile.motorButton)
                                .toggleStyle(SwitchToggleStyle(tint: .brown))
                
            }
            .padding()
        }
        
    }
}

struct CookingControls_Previews: PreviewProvider {
    static var previews: some View {
        CookingControls().environmentObject(MachineModelData())
    }
}
