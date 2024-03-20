<picture>
  <source media="(prefers-color-scheme: dark)" srcset="pnlo-logo.svg">
  <img src="pnlo-logo.svg" alt="pnlo-logo" height="128">
</picture>

# Pnlo 0.0.1 (Purple Neon Light Object)
Pnlo配置文件代码语法简化比较好，再用这样行吧。支持压缩代码、更多嵌套。

## 代码语法
```kt
// 
//   ______   __   __     __         ______    
//  /\  == \ /\ "-.\ \   /\ \       /\  __ \   
//  \ \  _-/ \ \ \-.  \  \ \ \____  \ \ \/\ \  
//   \ \_\    \ \_\\"\_\  \ \_____\  \ \_____\ 
//    \/_/     \/_/ \/_/   \/_____/   \/_____/ 
//                                  
// Pnlo This is a comment, patorjk Sub-Zero
"Pnlo" = "Pnlo";
"version" = "1.0";

Object1 = "obj-1";
Object2 = "obj-2";

"Object3" = "obj-3";
"Object4" = "obj-4";

main_obj_ = "Main Obj";
Time = 2207;
OFF = false;
ON = true;

obj >
  yes = true;
~

```


## 未来支持的计划
- 下次支持整型、浮点型、正负数以及科学计数法表示的数字、布尔、字符串？
- 统一化文件读写多平台统一加载 
  - [一个文件，一个启动类型，运行一个图形界面]
- 并发，异步
  - 加速启动速度，优化流畅度
