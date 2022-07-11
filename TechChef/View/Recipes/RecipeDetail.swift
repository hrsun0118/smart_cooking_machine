//
//  RecipeDetail.swift
//  TechChef
//
//  Created by Hairuo Sun on 12/4/21.
//

import SwiftUI

struct RecipeDetail: View {
    @EnvironmentObject var machineModelData: MachineModelData
    var recipe: Recipe
    
    var body: some View {
        ScrollView {
            CircleImage(image: recipe.image)
            Text(recipe.name)
                .font(.title)
                .bold()
                .multilineTextAlignment(.center)
            Text("Servings: \(recipe.servings) People")
                .font(.subheadline)
                .bold()
                .foregroundColor(.secondary)
            Divider()
            
            VStack(alignment: .leading) {
                Text("Recipe:")
                    .font(.title2)
                    .bold()
                Text("\(recipe.ingredients)")
                
                Text("Directions:")
                    .font(.title2)
                    .bold()
                Text("\(recipe.directions)")
            }
        }
        .padding()
    }
}

struct RecipeDetail_Previews: PreviewProvider {
    static let machineModelData = MachineModelData()
    
    static var previews: some View {
        RecipeDetail(recipe: machineModelData.recipes[0])
            .environmentObject(machineModelData)
    }
}
