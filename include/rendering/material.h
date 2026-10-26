#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct MaterialConfig {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

enum class MaterialType {
    Brass,
    Bronze,
    Chrome,
    Copper,
    Gold,
    Silver,
    Tin,
    Emerald,
    Jade,
    Obsidian,
    Pearl,
    Ruby,
    Turquoise,
    BlackPlastic,
    RedPlastic,
    CyanPlastic,
    BlackRubber
};

class MaterialManager {
    public:
    
    MaterialConfig getMaterial(MaterialType material);
};