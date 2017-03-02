# dds-example

Vortex Lite pub-sub examples (DCPS)

## // What are dds entities

Entitity is an abstract base class in DCPS, you can learn more from this [slides](http://www.slideshare.net/cmwang/entities-in-dcps-dds).

## // Procedures

### publish side

1. Create one (and only one) domain participant entity.
1. Create a publisher entity
1. Create multiple DDS topic entities
1. Create multiple typed writer entities
1. Publish data from specific typed writer

### subscriber side

1. Create one (and only one) domain participant entity.
1. Create a subscriber entity
1. Create multiple DDS topic entities
1. Create multiple typed reader entities
1. Receive data from specific typed reader

---

## // File Descriptions

- [mb.idl](mb.idl) : sample IDL with two DDS topics
- [pub.c](pub.c)  : publish voltage sample every 100 ms
- [pub2.c](pub2.c) : publish led sample every 100 ms
- [sub1.c](sub1.c) : subscribe voltage topic with listener (async)
- [sub2.c](sub2.c) : subscribe voltage topic with waitset (blocking)
- [sub3.c](sub3.c) : subscribe voltage topic with waitset (blocking) and [thread pool](https://github.com/mbrossard/threadpool)
- [sub4.c](sub4.c) : subscribe voltage topic with waitset (blocking) and [lock-free thread pool](https://github.com/xhjcehust/LFTPool)
- [sub5.c](sub5.c) : subscribe voltage and led topics with waitset (blocking) and [thread pool](https://github.com/mbrossard/threadpool)

## // Build

Download [Vortex Lite](http://www.prismtech.com/vortex/software-downloads) evaluation from Prismtech website, then install it.


```bash
mkdir build && cd build
cmake ..
make 
./sub2
./pub
```
