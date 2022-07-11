//
//  RecipeRow.swift
//  TechChef
//
//  Created by Hairuo Sun on 12/6/21.
//

import SwiftUI

struct RecipeRow: View {
    var recipe: Recipe
    
    var body: some View {
        HStack {
            recipe.image
                .resizable()
                .frame(width: 50, height: 50)
            Text(recipe.name)

            Spacer()
        }
    }
}

struct RecipeRow_Previews: PreviewProvider {
    static var recipes = MachineModelData().recipes
    
    static var previews: some View {
        Group {
            RecipeRow(recipe: recipes[0])
            RecipeRow(recipe: recipes[1])
        }
        .previewLayout(.fixed(width: 300, height: 70))
    }
}
