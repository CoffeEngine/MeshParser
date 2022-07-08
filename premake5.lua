workspace "Caffeine" 
    configurations { "Debug","Release" }
    platforms { "Win64"}
    location "build"


project "MeshParserLib"
    kind "StaticLib"
    language "C"

    location "build/%{prj.name}"
    targetdir "build/%{prj.name}/bin/%{cfg.buildcfg}"
    objdir "build/%{prj.name}/obj/%{cfg.buildcfg}"

    includedirs {"includes"}

    files {"src/lib/*.c","src/lib/*.h"}

    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On" 

    filter "configurations:Release" 
        defines { "NDEBUG" }      
        optimize "On"
    
    newaction {
        trigger = "build_lib",
        description = "Builds the project with MSBuild with Debug Win64 config",
        onEnd = function()
            os.execute("MSBuild.exe build/MeshParserLib /p:configuration=\"Debug Win64\" /p:platform=x64")
        end
    }

project "MeshParserCLI"
    kind "ConsoleApp"
    language "C"
   
    location "build/%{prj.name}"
    targetdir "build/%{prj.name}/bin/%{cfg.buildcfg}"
    objdir "build/%{prj.name}/obj/%{cfg.buildcfg}"

    includedirs {"includes"}
    
    links {"MeshParserLib"}

    files {"src/cli/*.c","src/cli/*.h"}

    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On" 

    filter "configurations:Release" 
        defines { "NDEBUG" }      
        optimize "On"

    newaction {
        trigger = "build_cli",
        description = "Builds the project with MSBuild with Debug Win64 config",
        onEnd = function()
            os.execute("MSBuild.exe build/MeshParserCLI /p:configuration=\"Debug Win64\" /p:platform=x64")
        end
    }

    newaction {
        trigger = "cli",
        description = "Builds the project with MSBuild with Debug Win64 config",
        onEnd = function()
            os.execute("MSBuild.exe build/MeshParserCLI /p:configuration=\"Debug Win64\" /p:platform=x64")
            os.execute(".\\build\\MeshParserCLI\\bin\\Debug\\MeshParserCLI.exe")
        end
    }

