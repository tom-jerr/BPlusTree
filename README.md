# 全内存BPlusTree
实现了B+树的插入、删除和查找；实现了B+树到文件的序列化和反序列化

插入1千万结点时间接近7.9s，正在考虑优化
## 目录组织
- .vscode: 环境配置
- bin: 生成可执行文件的文件夹
- doc: 项目相关文档
- include: 存放项目头文件
  - CLBPlusTree.h: 存放B+树相关类的定义和实现
  - CLSerialization.h: 存放序列化相关类的定义和实现
- third_party: 存放第三方库
- src: 存放项目源文件
- test: 存放项目测试文件
- setup.sh: 项目启动脚本
- test.sh: 进行单元测试和1千万结点的性能测试

## 项目安装&测试
- 在项目目录下，直接运行脚本
~~~shell
chmod +x setup.sh
chmod +x test.sh
./setup.sh
./test.sh
~~~
- 自己进行cmake项目构建
~~~shell
cmake -S . -B ./build
cmake --build ./build
./build/bin/UNITTEST
./build/bin/PERFROMTEST
~~~

