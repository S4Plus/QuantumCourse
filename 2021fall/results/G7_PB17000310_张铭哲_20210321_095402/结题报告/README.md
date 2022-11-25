## 小组分工

张铭哲：前期调研、选题报告、IQS编译与部署、编写代码、测试。

郝中凯：编写代码、测试、结题报告、ppt、小组汇报。

王祖卿：前期调研、选题报告、解释调研与代码中遇到的物理问题、提供测试思路与量子线路。



## Change Log

使用命令：

```shell
git apply patch
```

应用我们的修改。



项目的所有更改都将在此文件中说明。

### Added
- 有向无环图（DAG）的数据结构以及一些基本操作。见`include/dag.hpp`,`include/dag.cpp`。
- 调度优化算法的实现。见`include/gate_schedule.hpp`,`include/gate_schedule.cpp`。
- 对于1qubit和2qubit量子门的测试。见`tests/`。

### Changed
- QubitRegister类上新增参数化的ApplyGate方法。
- 一些CMake文件。

### Removed

### Fixed



## 编译运行

```shell
mkdir build && cd build
CXX=g++ cmake -DIqsMPI=ON ..
make
```

此时在`tests/bin`目录下生成编译好的可执行文件。

也可以手动进行编译：

```
g++ -DINTELQS_HAS_MPI -DUSE_MM_MALLOC -I/usr/include/mpich -I../src/../include -I/mnt/e/intel-qs -O3 -m64 -fopenmp -std=c++14 -o your_test_file.o -c your_test_file.cpp 

g++ your_test_file.o -o your_test_file  -Wl,-rpath,../build/lib /usr/lib/x86_64-linux-gnu/libmpichcxx.so /usr/lib/x86_64-linux-gnu/libmpich.so -lgomp -lpthread -lm -ldl -lstdc++ /usr/lib/gcc/x86_64-linux-gnu/7/libgomp.so -lpthread ../lib/libiqs.so /usr/lib/x86_64-linux-gnu/libmpichcxx.so /usr/lib/x86_64-linux-gnu/libmpich.so -lgomp -lpthread -lm -ldl -lstdc++ /usr/lib/gcc/x86_64-linux-gnu/7/libgomp.so -lpthread 
```

运行测试程序：

```
mpirun -np <num_procs> ./your_test_file <num_qubits>
```
