
solution "rast"
  configurations "app"
  objdir         "build/obj"
  
project "rast"
  location       "build"
  targetdir      "."
  targetname     "rast"
  kind           "ConsoleApp"
  language       "C"
  files        { "src/**.c" }
  includedirs  { "src" }
  links        { "GL" }
  linkoptions  { "`sdl2-config --static-libs`" }
  buildoptions { "-Wall", "`sdl2-config --cflags`" }
