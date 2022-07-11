//
//  ContentView.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        VStack {
            Text("TechChef")
                .font(.largeTitle)
                .bold()
                .foregroundColor(.green)
            HomeList()
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(MachineModelData())
    }
}
