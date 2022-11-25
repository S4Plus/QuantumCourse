# Change Log

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