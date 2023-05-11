# sigsegv-demo
Container that will segmentation fault after 30 seconds.

Use this project to generate SIGSEGV faults and see how to debug them in Kubernetes.

The image can be found at: quay.io/bryonbaker/sigsegv:latest

## How to use this with OpenShift if no core dumps are available

The following is the journal output from an OpenShift node. This is useful if you have not enabled the core dump:

```
May 11 09:32:33 crc-9ltqk-master-0 kernel: sigsegv[258213]: segfault at 0 ip 00007ff52d488c91 sp 00007ffe9e65e4d8 error 6 in libc-2.28.so[7ff52d3b9000+1bc000]
May 11 09:32:33 crc-9ltqk-master-0 kernel: Code: 0f 82 43 d7 06 00 0f 1f 00 f3 0f 1e fa 48 89 f8 48 83 fa 20 72 49 48 83 fa 40 0f 87 9f 00 00 00 c5 fe 6f 06 c5 fe 6f 4c 16 e0 <c5> fe 7f 07 c5 fe 7f 4c 17 e0 c5 f8 77 c3 48 3b 15 fa 48 2f 00 0f
May 11 09:32:33 crc-9ltqk-master-0 systemd[1]: Started Process Core Dump (PID 259186/UID 0).
May 11 09:32:33 crc-9ltqk-master-0 systemd-coredump[259187]: Process 258213 (sigsegv) of user 1000660000 dumped core.
                                                             
                                                             Stack trace of thread 1:
                                                             #0  0x00007ff52d488c91 __memmove_avx_unaligned_erms (libc.so.6)
                                                             #1  0x0000000000400811 n/a (/app/sigsegv)
                                                             #2  0x000000000040072e n/a (/app/sigsegv)
                                                             #3  0x00007ff52d3f3cf3 __libc_start_main (libc.so.6)
                                                             #4  0x000000000040065e n/a (/app/sigsegv)
May 11 09:32:33 crc-9ltqk-master-0 conmon[258200]: conmon f1ee30489db2a0242ef1 <ninfo>: container 258213 exited with status 139
May 11 09:32:33 crc-9ltqk-master-0 systemd[1]: systemd-coredump@26-259186-0.service: Succeeded.
May 11 09:32:33 crc-9ltqk-master-0 systemd[1]: systemd-coredump@26-259186-0.service: Consumed 577ms CPU time
```

What is interesting about this is that the memory reference that calls the calling ```__memmove_avx_unaligned_erms``` function is at 0x40072e. 

If you look in the map file you see this calling address is from inside the ```crasher()``` function that has a memory range of 0x400735 to 0x000816. 

```

                0x0000000000400630                _start
 .text          0x000000000040065f        0x0 /usr/lib/gcc/x86_64-redhat-linux/8/../../../../lib64/crt1.o
 *fill*         0x000000000040065f        0x1 
 .text          0x0000000000400660        0x5 /usr/lib/gcc/x86_64-redhat-linux/8/../../../../lib64/crt1.o
                0x0000000000400660                _dl_relocate_static_pie
 .text          0x0000000000400665        0x0 /usr/lib/gcc/x86_64-redhat-linux/8/../../../../lib64/crti.o
 *fill*         0x0000000000400665        0xb 
 .text          0x0000000000400670       0xa6 /usr/lib/gcc/x86_64-redhat-linux/8/crtbegin.o
 .text          0x0000000000400716      0x100 /tmp/cc2OUhZx.o
                0x0000000000400716                main
                0x0000000000400735                crasher
 *fill*         0x0000000000400816        0xa 
```

This would lead you to the ```memcpy()``` and a null pointer dereference. 

```
void
crasher() {
    char* bogusMsg = {"The quick brown fox jumps over the lazy dog"};
    char *nullPtr = 0;
    int crashDelay = 30;
    time_t  startTime, thisTime, tickTime;

    printf("Crashing in %d seconds...\n", crashDelay);

    startTime = tickTime = thisTime =  time(0);
    for (;;) {
        thisTime = time(0);

        if (difftime( thisTime, tickTime) >= 1) {
            tickTime = thisTime;
            printf(".\n");
        }
        else if (difftime( thisTime, startTime ) >= crashDelay) {
            memcpy( nullPtr, &bogusMsg, strlen(bogusMsg));
        }
    }
}
```


### How to extract the logs from the node

To perform the previous analysis you need to extract the journal from the node that was hosting the crashed pod.

Once you have found the node perform the following as a ```kubeadmin``` user and root:
```
$ oc get nodes
NAME                 STATUS   ROLES                         AGE   VERSION
crc-9ltqk-master-0   Ready    control-plane,master,worker   72d   v1.25.4+a34b9e9
```

```
$ oc debug node/crc-9ltqk-master-0

Starting pod/crc-9ltqk-master-0-debug ...
To use host binaries, run `chroot /host`
Pod IP: 192.168.126.11
If you don't see a command prompt, try pressing enter.
sh-4.4# 
```

```
$ chroot /host
sh-4.4# 
```

Search the journal for ```status 139```
```
$ journalctl
```

# Testing with podman
```podman run --rm --name sigsegv quay.io/bryonbaker/sigsegv:latest```

```journalctl -f``` generates:

```
May 11 17:21:34 rh-brbaker systemd[1]: Started systemd-coredump@16-59383-0.service - Process Core Dump (PID 59383/UID 0).
May 11 17:21:34 rh-brbaker audit[1]: SERVICE_START pid=1 uid=0 auid=4294967295 ses=4294967295 subj=system_u:system_r:init_t:s0 msg='unit=systemd-coredump@16-59383-0 comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
May 11 17:21:34 rh-brbaker systemd-coredump[59384]: [🡕] Process 59283 (sigsegv) of user 1000 dumped core.
                                                    
                                                    Module /app/sigsegv with build-id 76a4bb316446b6e42299bfc06e374498ef350b15
                                                    Module /usr/lib64/libc-2.28.so with build-id 64aa558dcdda2d8b0d7b04cef33ddbb2d9d8b8b4
                                                    Module /usr/lib64/ld-2.28.so with build-id 67aa0f1504abcfc4befb2b61a329a30a9984e0db
                                                    Module linux-vdso.so.1 with build-id e68f5c71076e43c6b64184a0639f2d84940ebc63
                                                    Stack trace of thread 1:
                                                    #0  0x00007f32028bfc91 n/a (/usr/lib64/libc-2.28.so + 0xcfc91)
                                                    #1  0x000000000040072e n/a (/app/sigsegv + 0x72e)
                                                    #2  0x00007f320282acf3 n/a (/usr/lib64/libc-2.28.so + 0x3acf3)
                                                    ELF object binary architecture: AMD x86-64
May 11 17:21:34 rh-brbaker systemd[1]: systemd-coredump@16-59383-0.service: Deactivated successfully.
May 11 17:21:34 rh-brbaker audit[1]: SERVICE_STOP pid=1 uid=0 auid=4294967295 ses=4294967295 subj=system_u:system_r:init_t:s0 msg='unit=systemd-coredump@16-59383-0 comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
May 11 17:21:34 rh-brbaker systemd[1981]: libpod-f3cf1a789063f2593fbcfd60df69208ae551c7c696e5caaa25bbf3cfe92be664.scope: Consumed 29.374s CPU time.

```