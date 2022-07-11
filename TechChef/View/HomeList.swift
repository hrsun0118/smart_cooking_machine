//
//  HomeList.swift
//  TechChef
//
//  Created by Hairuo Sun on 11/29/21.
//

import SwiftUI

struct HomeList: View {
    @EnvironmentObject var machineModelData: MachineModelData
    
    var body: some View {
        NavigationView {
            List{
                // register machine
                NavigationLink {
                    MachineProfileHost()
                        .environmentObject(machineModelData)
                } label: {
                    Text("Register")
                        .font(.title2)
                        .bold()
                        .foregroundColor(.yellow)
                }
                
                // select recipe to cook
                NavigationLink {
                    RecipeList()
                        .environmentObject(machineModelData)
                } label: {
                    Text("Select A Recipe")
                        .font(.title2)
                        .bold()
                        .foregroundColor(.red)
                }
                
                // cook directly without a recipe
                NavigationLink {
                    CookingControls().environmentObject(machineModelData)
                } label: {
                    Text("Cook Now")
                        .font(.title2)
                        .bold()
                        .foregroundColor(.purple)
                    
                }
                
                // Machine analytics for maintenance
                NavigationLink {
                    
                } label: {
                    Text("Maintenance")
                        .font(.title2)
                        .bold()
                        .foregroundColor(.blue)
                }
            }
            .navigationTitle("Home")
            
        }
    }
}

struct HomeList_Previews: PreviewProvider {
    static var previews: some View {
        HomeList()
            .environmentObject(MachineModelData())
    }
}
