//
//  RecipeList.swift
//  TechChef
//
//  Created by Hairuo Sun on 12/6/21.
//

import SwiftUI

struct RecipeList: View {
    @EnvironmentObject var machineModelData: MachineModelData
    
    var recipes: [Recipe] = MachineModelData().recipes
    
    var body: some View {
        NavigationView {
            List{
                ForEach(recipes) { recipe in
                    NavigationLink {
                        RecipeDetail(recipe: recipe)
                    } label: {
                        RecipeRow(recipe: recipe)
                    }
                }
            }
            .navigationTitle("Recipes")
        }
    }
}

struct RecipeList_Previews: PreviewProvider {
    static var previews: some View {
        RecipeList()
            .environmentObject(MachineModelData())
    }
}
