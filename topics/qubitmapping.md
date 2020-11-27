## 量子程序的量子比特映射

**量子比特映射**旨在解决将量子程序中使用的逻辑量子比特和逻辑门映射到量子计算设备的物理量子比特和物理门。其中，对于双量子门所作用的两个量子比特必须映射到量子计算设备上能实际执行对应物理门操作的两个物理量子比特（称这两个量子比特是**相邻的**）。

早期的量子比特映射假设各种量子门的执行时间是相等的，且没有考虑错误。现在的研究挑战主要包括在考虑量子门执行时长差异、量子操作的噪声、量子程序规模变大等时的量子比特初始映射方法以及量子程序执行时的重映射方法。

- [DAC 2020] Haowei Deng, Yu Zhang, Quanxi Li. [CODAR : A Contextual Duration-Aware Qubit Mapping for Various NISQ Devices](https://github.com/S4Plus/Q-CODAR)
- [DAC 2019] Robert Wille, Lukas Robert,  Alwin Zulehner. [Mapping Quantum Circuits to IBM QX Architectures Using the Minimal Number of SWAP and H Operations](https://github.com/iic-jku/minimal_ibm_qx_mapping)
- [DAC 2019] [QURE: Qubit Re-allocation in Noisy Intermediate-Scale
  Quantum Computers](http://ieeexplore.ieee.org/document/8806892)
- [ASPLOS 2019] Gushu Li, Yufei Ding, Yuan Xie. [Tackling the Qubit Mapping Problem for NISQ-Era Quantum Devices](https://dl.acm.org/doi/10.1145/3297858.3304023)
- [ASPLOS 2019] Prakash Murali, Jonathan M. Baker, Ali Javadi Abhari, Frederic T. Chong, Margaret Martonosi. [Noise-Adaptive Compiler Mappings for Noisy Intermediate-Scale Quantum Computers](https://dl.acm.org/doi/10.1145/3297858.3304075)
- [ASPLOS 2019] [Not All Qubits Are Created Equal:
  A Case for Variability-Aware Policies for NISQ-Era Quantum Computers](https://dl.acm.org/doi/10.1145/3297858.3304007)
- [[ASPDAC 2019] Alwin Zulehner, Robert Wille. (https://dl.acm.org/doi/proceedings/10.1145/3287624)] [Compiling SU(4) Quantum Circuits to IBM QX Architectures](https://dl.acm.org/doi/10.1145/3287624.3287704)
- ...