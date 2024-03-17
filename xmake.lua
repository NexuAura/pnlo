add_rules("mode.debug", "mode.release")

target("pnlo")

    set_languages("c++17")

    set_encodings("source:utf-8")

    -- 设置库
    -- set_kind("static")


    -- set_kind("binary")
    -- static   shared
    -- set_kind("static")
    -- add_cxflags("-mwindows")
    -- add_cxflags("-fexec-charset=GBK")
    -- add_cxflags("-finput-charset=utf-8")
    -- add_cxflags("source-charset:utf-8")
    -- add_includedirs("include")
    add_files("pnlo/cpp/*.cpp")
    -- add_files("pnlo/cpp/*.cpp")


-- add_headerfiles("include/*.h", {
--     prefixdir = ""
-- })

-- add_files("source/**.cpp")
