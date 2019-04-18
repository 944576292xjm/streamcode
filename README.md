# streamcode
学习流媒体时的练习代码，代码没有整理
1.支持windows/linux，使用c++11,可配置的accept线程，windows使用select，linux使用epoll，可配置的io线程
2.动态工作线程池
3.socket接收发送使用ringbuffer，检测发送buffer水位，可以使用令牌桶实现固定速率发送
4.添加rtmp/rtmp/hls/rtmp转http-flv/切片hls，http-flv点播
