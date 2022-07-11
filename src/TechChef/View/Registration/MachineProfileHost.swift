//
//  MachineProfileHost.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

struct MachineProfileHost: View {
    @Environment(\.editMode) var editMode
    @EnvironmentObject var machineModelData: MachineModelData
    @State private var draftMachineProfile = MachineProfile.default
    
    var body: some View {
        VStack(alignment: .leading, spacing: 20) {
            HStack {
                if editMode?.wrappedValue == .active {
                    Button("Cancel", role: .cancel) {
                        draftMachineProfile = machineModelData.machineProfile
                        editMode?.animation().wrappedValue = .inactive
                    }
                }
                Spacer()
                EditButton()
            }
            
            if editMode?.wrappedValue == .inactive {
                MachineProfileSummary(machineProfile: machineModelData.machineProfile)
            } else {
                MachineProfileEditor(machineProfile: $draftMachineProfile)
                    .onAppear {
                        draftMachineProfile = machineModelData.machineProfile
                    }
                    .onDisappear {
                        machineModelData.machineProfile = draftMachineProfile
                    }
            }
        }
        .padding()
        .navigationTitle("Register")
        .navigationBarTitleDisplayMode(.inline)
    }
}

struct MachineProfileHost_Previews: PreviewProvider {
    static var previews: some View {
        MachineProfileHost()
            .environmentObject(MachineModelData())
    }
}
