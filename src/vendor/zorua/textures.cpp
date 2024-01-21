#include "Textures.h"

std::unordered_map<zorua::textures::FilePath_t, zorua::textures::Texture> zorua::textures::loadedTexturesMap;

zorua::textures::Texture zorua::textures::load_texture(FilePath_t path) {
    Texture texture;
    if (loadedTexturesMap.count(path)) return loadedTexturesMap[path];

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        texture = {};

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    texture.texture = image_texture;
    texture.width = image_width;
    texture.height = image_height;

    loadedTexturesMap[path] = texture;
    return texture;
}

inline void zorua::textures::unload_texture(FilePath_t path) {
    glDeleteTextures(1, &loadedTexturesMap[path].texture);
}