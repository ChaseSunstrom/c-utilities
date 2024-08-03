workspace "c-utilities"
    configurations { "Debug", "Release" }
    architecture "x86_64"

project "c_utilities_lib"
    kind "StaticLib"
    language "C"
    cdialect "C17"
    targetdir "bin/%{cfg.buildcfg}"

    files {
        "src/util/**.c",
        "src/util/**.h"
    }

    includedirs {
        "src",
        "include"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "c_utilities_exe"
    kind "ConsoleApp"
    language "C"
    cdialect "C17"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "c-utilities"

    files { "src/main.c" }

    links { "c_utilities_lib" }

    includedirs {
        "src",
        "include"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "c_utilities_test"
    kind "ConsoleApp"
    language "C"
    cdialect "C17"
    targetdir "bin/%{cfg.buildcfg}"

    files { 
        "tests/test_main.c",
        "tests/test_vector.c",
        "tests/test_list.c",
        "tests/test_allocator.c"
    }

    links { "c_utilities_lib" }

    includedirs {
        "src",
        "include"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

-- Platform specific settings
filter "system:not windows"
    links { "m" }

filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }
    flags { "MultiProcessorCompile" }
    buildoptions { "/Zi" }
    linkoptions { "/DEBUG" }