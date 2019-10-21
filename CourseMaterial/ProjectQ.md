# ProjectQ and HiQ

[ProjectQ](http://projectq.ch/) is an open source effort for quantum computing.

## Resources

- [Tutorial](https://projectq.readthedocs.io/en/latest/tutorials.html)
- [Examples](https://projectq.readthedocs.io/en/latest/examples.html#examples)
- [Code Documentation](https://projectq.readthedocs.io/en/latest/projectq.html#code-doc)

- [HiQ在线编程界面](https://hiq.huaweicloud.com:9002/ide)
- [HiQ模拟器](https://github.com/Huawei-HiQ/HiQsimulator)

## 安装 ProjectQ
### Python安装
#### Python3
- sudo apt install python3 python3-pip
- 设置python3 为缺省的 python
  - sudo rm /usr/bin/python
  - sudo ln -s /usr/bin/python3.7 /usr/bin/python
- python -m pip install --upgrade pip
- 
### Build tools (for g++)
- sudo apt-get install build-essential 

### ProjectQ安装
- sudo pip3 install --user projectq
