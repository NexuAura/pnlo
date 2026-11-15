add_rules("mode.debug", "mode.release")

target("pnlo")
    set_kind("binary")
    set_languages("c++17")
    set_encodings("source:utf-8")
    add_files("src/*.cpp")
    -- 运行目录设为项目根，便于读取默认的 examples/text.pnlo
    set_rundir("$(projectdir)")