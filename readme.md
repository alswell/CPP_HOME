C++ Utilities
### Examples:
#### TCP server based on multi-thread
```
# make ser
# make cli
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

