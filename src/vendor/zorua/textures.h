#pragma once

#include "../stb_image.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <set>
#include <unordered_map>

namespace zorua {
	namespace textures {
		typedef std::string FilePath_t;
		struct Texture {
			int height;
			int width;
			GLuint texture;
		};

		extern std::unordered_map<FilePath_t, Texture> loadedTexturesMap;
		extern Texture load_texture(FilePath_t path);
		extern inline void unload_texture(FilePath_t path);
	}
}