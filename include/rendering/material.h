#ifndef MATERIAL_H
#define MATERIAL_H

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

inline MaterialConfig getMaterial(MaterialType material){
    switch(material){
        case MaterialType::Brass:
            return {
                glm::vec3(0.3294f, 0.2235f, 0.0275f),glm::vec3(0.7804f, 0.5686f, 0.1137f), glm::vec3(0.9922f, 0.9412f, 0.8078f), 27.8f 
            };
            break;
        case MaterialType::Bronze:
            return {
                glm::vec3(0.2125f, 0.1275f, 0.0540f), glm::vec3(0.7140f, 0.4284f, 0.1814f), glm::vec3(0.3935f, 0.2719f, 0.1668f), 25.6f
            };
            break;
        case MaterialType::Chrome:
            return {
                glm::vec3(0.2500f, 0.2500f, 0.2500f), glm::vec3(0.4000f, 0.4000f, 0.4000f), glm::vec3(0.7746f, 0.7746f, 0.7746f), 76.8f 
            };
            break;
        case MaterialType::Copper:
            return {
                glm::vec3(0.1913f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.2705f, 0.0828f), glm::vec3(0.2568f, 0.1371f, 0.0860f), 12.8f
            };
            break;
        case MaterialType::Gold:
            return {
                glm::vec3(0.2473f, 0.1995f, 0.0745f), glm::vec3(0.7516f, 0.6065f, 0.2265f), glm::vec3(0.6283f, 0.5559f, 0.3661f), 51.2f
            };
            break;
        case MaterialType::Silver:
            return {
                glm::vec3(0.1923f, 0.1923f, 0.1923f), glm::vec3(0.5075f, 0.5075f, 0.5075f), glm::vec3(0.5083f, 0.5083f, 0.5083f), 51.2f
            };
            break;
        case MaterialType::Tin:
            return {
                glm::vec3(0.1059f, 0.0588f, 0.1137f), glm::vec3(0.4275f, 0.4706f, 0.5412f), glm::vec3(0.3333f, 0.3333f, 0.5216f), 9.8f 
            };
            break;
        case MaterialType::Emerald:
            return {
                glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.0757f, 0.6142f, 0.0757f), glm::vec3(0.6330f, 0.7278f, 0.6330f),  76.8f
            };
            break;
        case MaterialType::Jade:
            return {
                glm::vec3(0.1350f, 0.2225f, 0.1575f), glm::vec3(0.5400f, 0.8900f, 0.6300f), glm::vec3(0.3162f, 0.3162f, 0.3162f), 12.8f
            };
            break;
        case MaterialType::Obsidian:
            return {
                glm::vec3(0.0538f, 0.0500f, 0.0663f), glm::vec3(0.1828f, 0.1700f, 0.2253f), glm::vec3(0.3327f, 0.3286f, 0.3464f), 38.4f
            };
            break;
        case MaterialType::Pearl:
            return {
                glm::vec3(0.2500f, 0.2073f, 0.2073f), glm::vec3(1.0000f, 0.8290f, 0.8290f), glm::vec3(0.2966f, 0.2966f, 0.2966f), 11.2f
            };
            break;
        case MaterialType::Ruby:
            return {
                glm::vec3(0.1745f, 0.0118f, 0.0118f), glm::vec3(0.6142f, 0.0414f, 0.0414f), glm::vec3(0.7278f, 0.6270f, 0.6270f), 76.8f
            };
            break;
        case MaterialType::Turquoise:
            return {
                glm::vec3(0.1000f, 0.1873f, 0.1745f), glm::vec3(0.3960f, 0.7415f, 0.6910f), glm::vec3(0.2973f, 0.3083f, 0.3067f), 12.8f
            };
            break;
        case MaterialType::BlackPlastic:
            return {
                glm::vec3(0.0000f, 0.0000f, 0.0000f), glm::vec3(0.0100f, 0.0100f, 0.0100f), glm::vec3(0.5000f, 0.5000f, 0.5000f), 32.0f
            };
            break;
        case MaterialType::RedPlastic:
            return {
                glm::vec3(0.0000f, 0.0000f, 0.0000f), glm::vec3(0.5000f, 0.0000f, 0.0000f), glm::vec3(0.7000f, 0.6000f, 0.6000f), 32.0f
            };
            break;
        case MaterialType::CyanPlastic:
            return {
                glm::vec3(0.0000f, 0.1000f, 0.0600f), glm::vec3(0.0000f, 0.5098f, 0.5098f), glm::vec3(0.5020f, 0.5020f, 0.5020f), 32.0f
            };
            break;
        case MaterialType::BlackRubber:
            return {
                glm::vec3(0.0200f, 0.0200f, 0.0200f), glm::vec3(0.0100f, 0.0100f, 0.0100f), glm::vec3(0.4000f, 0.4000f, 0.4000f), 10.0f
            };
            break;
        
    }
}

#endif