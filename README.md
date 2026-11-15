<picture>
  <source media="(prefers-color-scheme: dark)" srcset="pnlo-logo.svg">
  <img src="pnlo-logo.svg" alt="pnlo-logo" height="128">
</picture>

# Pnlo 0.0.1 (Purple Neon Light Object)

Pnlo 配置文件代码语法简化比较好，再用这样行吧。支持压缩代码、更多嵌套。

## 目录结构

```text
├── src/
│   ├── Pnlo.h            # 对外接口（TokenType / Token / Variable / Lexer / parse）
│   └── Pnlo.cpp          # 实现 + 程序入口 main
├── examples/             # .pnlo 配置示例
│   ├── main.pnlo
│   └── text.pnlo
├── docs/syntax.plsf      # 语法规范（Lexer / Parse 形式化描述）
├── CMakeLists.txt        # CMake 构建，产物在 build/bin/pnlo
├── xmake.lua             # xmake 构建
├── .github/workflows/    # CI：CMake 多平台 + xmake
├── LICENSE               # Apache-2.0
└── pnlo-logo.svg
```

## 构建

需要 C++17 编译器（g++ / clang++ / MSVC），CMake 方式另需 CMake 3.19+。

### CMake

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/pnlo
```

### xmake

```sh
xmake build pnlo
xmake run pnlo
```

### 直接编译

```sh
g++ -std=c++17 -O2 -o build/pnlo src/Pnlo.cpp
```

## 使用

默认读取 [examples/text.pnlo](examples/text.pnlo)，也可以传入任意配置路径：

```sh
./build/bin/pnlo                     # 使用默认的 examples/text.pnlo
./build/bin/pnlo examples/main.pnlo  # 指定其他配置文件
```

输出为解析到的 `名称:值`（按文件中的出现顺序）：

```text
Pnlo:Purple Neon Light Object
version:1.0
Object1:obj-1
Object2:obj-2
Object3:obj-3
Object4:obj-4
main_obj_:Main Obj
Time:2026
OFF:false
ON:true
```

文件打不开时会打印 `Error: Cannot open config file: <路径>` 并以退出码 1 结束。

暂不支持的结构会被跳过而不是报错：数组值 `star_male = ["Hello", "World!"];` 与对象块 `obj > ... ~` 不会出现在输出中。

## 语法

下面是 [examples/text.pnlo](examples/text.pnlo) 的实际内容（当前解析器只输出其中的顶层赋值）：

```pnlo
// 
//   ______   __   __     __         ______  
//  /\  == \ /\ "-.\ \   /\ \       /\  __ \   
//  \ \  _-/ \ \ \-.  \  \ \ \____  \ \ \/\ \  
//   \ \_\    \ \_\\"\_\  \ \_____\  \ \_____\ 
//    \/_/     \/_/ \/_/   \/_____/   \/_____/ 
//                                
// Pnlo This is a comment, patorjk Sub-Zero

"Pnlo" = "Purple Neon Light Object";
"version" = "1.0";
Object1 = "obj-1";
Object2 = "obj-2";
"Object3" = "obj-3";
"Object4" = "obj-4";
main_obj_ = "Main Obj";

Time = 2026;
OFF = false;
ON = true;

// 数组
star_male = ["Hello", "World!"];

// 注释风格完全一致
/**/
//

// 对象定义， 一般形式:
obj >
   yes = true;
   num = 90;
   qwe = ["a", "b","c"];
~

```

形式化描述见 [docs/syntax.plsf](docs/syntax.plsf)。

## 未来支持的计划

- 下次支持整型、浮点型、正负数以及科学计数法表示的数字、布尔、字符串？
- 统一化文件读写多平台统一加载
  - [一个文件，一个启动类型，运行一个图形界面]
- 并发，异步
  - 加速启动速度，优化流畅度

## 许可

Apache-2.0，见 [LICENSE](LICENSE)。