# 我眼中的架构师

## 软件架构师 vs 分布式系统架构师
- 互联网浪潮下催生了分布式系统架构，为了应对高并发，解决单台服务器的性能极限，用集群和高速缓存等提升系统处理能力（吞吐率）和响应速度。同时分布式集群也为高可用，消除单点故障，带来了可能。
- 无论互联网时代还是 PC 时代，都离不开代码编织的软件。

## 优秀的软件架构师将会带来什么
- 事半功倍的开发和维护效率，尤其是维护。结构、逻辑清晰的代码，BUG 少，开发过程很少需要分出精力排查问题；添加和改动又很方便，不会牵一发而动全身。

## 软件架构师的职责
### 1. 平衡效率和易维护性
- 易维护大多都会牺牲效率

#### 1.1 效率
- 无需多言，大家普遍会考虑
- 但是无需处处考虑效率优先，否则过度考虑效率会牺牲易维护性，顾此失彼。一般在复杂数据处理的地方，比如循环中，重点优化即可。

#### 1.2 易维护性
##### 1.2.1 复用
- 发现和提取共性
- 加速开发，也避免重写一遍类似的逻辑引入的 BUG，从而避免由此而带来的时间和精力浪费。

##### 1.2.2 框架和模块化
- 让优秀的框架帮我们节省人工，让电脑帮我们干活！
- 模块化：软件最粗的粒度划分
- 切割。庖丁解牛，化繁为简。明确每个模块的职责、功能，而每个模块的职责、功能应该是越简单越好（整体要让部分感觉到爽、要让干活的人感觉爽）。

###### 封装繁琐的代码
- 何为繁琐？比如：多层函数调用。
- 把繁琐放在，或者说优化到、移动到，不需要经常改动的地方。

###### 使用编程语言的自动化机制
###### 妙用回调函数
###### 设计模式





# C++ Utilities
## basic
used for command line program
## ui
cross-platform UI
## geometry
computation geometry
## Examples:
### UVC camera
```
make cam
./camera.out -h
```
console print:
```
usage: ./camera.out [--help] --dev_id [--print_info] [--format_index] [--width] [--height] {}

description:
	A program to test UVC camera

positional arguments:
{} commands

optional arguments:
--help	-h  : show help message
--dev_id	-d <digit_value> : /dev/video<dev_id>
--print_info	-p  : print supported format and image size
--format_index	-f <digit_value> : specify which format of image to snap
--width	-W <digit_value> : specify image width
--height	-H <digit_value> : specify image height

```
### TCP server based on multi-thread
```
make ser
make cli
```
- start server first
```
# ./ser.out
socket ok!
bind ok!
listen ok!
Server start!
Accept!
thread ID: 139757315221248: hello; tick count: 4904981
client close!
Accept!
thread ID: 139757306828544: hello; tick count: 2620932
client close!
Accept!
^C
```
- then start client(send "hello")
```
# ./cli.out
# ./cli.out
```

