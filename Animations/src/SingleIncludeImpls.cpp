#include <iostream>
#include "core.h"
#define GABE_CPP_PRINT_IMPL
#include <cppUtilisLocal/cppPrint.hpp>
#undef GABE_CPP_PRINT_IMPL

#define GABE_CPP_UTILS_IMPL
#include <cppUtilisLocal/cppUtils.hpp>
#undef GABE_CPP_UTILS_IMPL

#define GABE_CPP_TESTS_IMPL
#include <cppUtilisLocal/cppTests.hpp>
#undef GABE_CPP_TESTS_IMPL

#define GABE_CPP_STRING_IMPL
#include <cppUtilisLocal/cppStrings.hpp>
#undef GABE_CPP_STRING_IMPL

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>
