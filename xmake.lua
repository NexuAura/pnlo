add_rules("mode.debug", "mode.release")
target("NSON")
-- static   shared
set_kind("static")
-- add_cxflags("-mwindows")
-- add_cxflags("-fexec-charset=GBK")
-- add_cxflags("-finput-charset=utf-8")
-- add_cxflags("source-charset:utf-8")
add_includedirs("include")

add_headerfiles("include/*.h", {
    prefixdir = ""
})

add_files("source/**.cpp")
