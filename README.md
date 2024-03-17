<picture>
  <source media="(prefers-color-scheme: dark)" srcset="sNSON-logo.svg">
  <img src="sNSON-logo.svg" alt="NSON-logo" height="128">
</picture>

# NSON 0.1.0 (NeonScript Object Notation)

## 代码语法
`test.pnlo`
```c
// NSON This is a comment 
"NSO" = "NeonScript Object";
"version" = "1.0";
// Window UI
[window]
  text = "Hello World!";
  w = 800;
  h = 600;
  [button]
    text = "Run!";
    x = 500;
    y = 500;
  ~
~

text = "hello";
"text 2" = "hello";
text3 = "hello";
"text 4" = "512";
text5 = "12.5";

tew_ = 12.5;

```


## 未来支持的计划
- 下次支持整型、浮点型、正负数以及科学计数法表示的数字、布尔、字符串？
- 统一化文件读写多平台统一加载 
  - [一个文件，一个启动类型，运行一个图形界面]
- 并发，异步
  - 加速启动速度，优化流畅度
