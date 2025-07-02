add_rules("mode.debug", "mode.release")

set_rundir("$(projectdir)")

add_requires("nlohmann_json", "spdlog", "glm", "libsdl3", "libsdl3_image", "libsdl3_ttf")

set_languages("c++20")

target("platformer")
    set_kind("binary")
    add_packages("nlohmann_json", "spdlog", "glm", "libsdl3", "libsdl3_image", "libsdl3_ttf")
    add_files("src/**.cpp")
    