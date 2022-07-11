//
//  Recipe.swift
//  TechChef
//
//  Created by Hairuo Sun on 12/5/21.
//

import Foundation
import SwiftUI

struct Recipe: Hashable, Codable, Identifiable {
    var id: Int
    var name: String
    var servings: Int
    var ingredients: String
    var directions: String
    var video: String
    
    private var imageName: String
    var image: Image{
        Image(imageName)
    }
}
