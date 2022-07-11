//
//  MachineProfileEditor.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

struct MachineProfileEditor: View {
    @Binding var machineProfile: MachineProfile
    
    var body: some View {
        List {
            HStack {
                Text("Machine ID").bold()
                Divider()
                TextField("ID", text: $machineProfile.id)
            }
        }
    }
}

struct MachineProfileEditor_Previews: PreviewProvider {
    static var previews: some View {
        MachineProfileEditor(machineProfile: .constant(.default))
    }
}
