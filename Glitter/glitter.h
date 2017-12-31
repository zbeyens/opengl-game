// Preprocessor Directives
#ifndef GLITTER
#define GLITTER

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#ifndef STBI
#define STBI
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#endif //~ Glitter Header
