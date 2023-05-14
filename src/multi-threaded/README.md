# Multi-Threaded Segmentation Fault Demo

This doc is work in progress.

The following output of the systemd logs shows that you do not necessarily get the call stack of the offending thread. The test code crashes thread 5 - but this shows thread 12. Strange given there should only be 10 threads....

```
May 14 03:49:06 crc-9ltqk-master-0 kernel: Thread 4
                                           [236820]: segfault at 0 ip 0000000000401113 sp 00007fb8aa765eb0 error 6
May 14 03:49:06 crc-9ltqk-master-0 kernel: Thread 5
                                           [236821]: segfault at 0 ip 0000000000401113 sp 00007fb8a9f64eb0 error 6
May 14 03:49:06 crc-9ltqk-master-0 kernel:  in mt-sigsegv[400000+2000]
May 14 03:49:06 crc-9ltqk-master-0 kernel:  in mt-sigsegv[400000+2000]
May 14 03:49:06 crc-9ltqk-master-0 kernel: 
May 14 03:49:06 crc-9ltqk-master-0 kernel: Code: 40 00 48 89 c7 b8 00 00 00 00 e8 38 fd ff ff bf 1e 00 00 00 e8 fe fd ff ff 48 c7 45 f0 00 00 00 00 8b 45 fc 89 c2 48 8b 45 f0 <88> 10 48 8b 05 04 20 20 00 8b 55 fc be 9f 18 40 00 48 89 c7 b8 00
May 14 03:49:06 crc-9ltqk-master-0 kernel: Code: 40 00 48 89 c7 b8 00 00 00 00 e8 38 fd ff ff bf 1e 00 00 00 e8 fe fd ff ff 48 c7 45 f0 00 00 00 00 8b 45 fc 89 c2 48 8b 45 f0 <88> 10 48 8b 05 04 20 20 00 8b 55 fc be 9f 18 40 00 48 89 c7 b8 00
May 14 03:49:06 crc-9ltqk-master-0 systemd[1]: Started Process Core Dump (PID 237789/UID 0).
May 14 03:49:07 crc-9ltqk-master-0 systemd-coredump[237790]: Process 236798 (mt-sigsegv) of user 1000660000 dumped core.
                                                             
                                                             Stack trace of thread 12:
                                                             #0  0x0000000000401113 n/a (/app/mt-sigsegv)
                                                             #1  0x000000000040168c n/a (/app/mt-sigsegv)
                                                             #2  0x00007fb8acb391ca n/a (/usr/lib64/libpthread-2.28.so)
May 14 03:49:07 crc-9ltqk-master-0 conmon[236785]: conmon 0121ed0763e987de3030 <ninfo>: container 236798 exited with status 139
May 14 03:49:07 crc-9ltqk-master-0 systemd[1]: systemd-coredump@5-237789-0.service: Succeeded.
May 14 03:49:07 crc-9ltqk-master-0 systemd[1]: systemd-coredump@5-237789-0.service: Consumed 819ms CPU time
```