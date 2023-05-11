# sigsegv-demo
Container that will segmentation fault after 30 seconds.

Use this project to generate SIGSEGV faults and see how to debug them in Kubernetes.

The image can be found at: quay.io/bryonbaker/sigsegv:latest

## Testing with podman
```podman run --rm --name sigsegv quay.io/bryonbaker/sigsegv:latest```

```journalctl -f``` generates:

```
May 11 15:07:23 rh-brbaker systemd[1981]: Started libpod-739e563c8e83962a188ed75d260b21fdeade199021151612257eb609227f4f41.scope - libcrun container.
May 11 15:07:23 rh-brbaker podman[26309]: 2023-05-11 15:07:23.240521555 +1000 AEST m=+0.074878908 container init 739e563c8e83962a188ed75d260b21fdeade199021151612257eb609227f4f41 (image=localhost/sigsegv:latest, name=sigsegv, io.k8s.display-name=Red Hat Universal Base Image 8, build-date=2022-09-06T17:25:04.845275, distribution-scope=public, vcs-type=git, com.redhat.license_terms=https://www.redhat.com/en/about/red-hat-end-user-license-agreements#UBI, name=ubi8, description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., architecture=x86_64, version=8.6, vcs-ref=f1ee6e37554363ec55e0035aba1a693d3627fdeb, vendor=Red Hat, Inc., release=943, io.k8s.description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., io.buildah.version=1.30.0, maintainer=Red Hat, Inc., com.redhat.build-host=cpt-1002.osbs.prod.upshift.rdu2.redhat.com, com.redhat.component=ubi8-container, url=https://access.redhat.com/containers/#/registry.access.redhat.com/ubi8/images/8.6-943, io.openshift.expose-services=, summary=Provides the latest release of Red Hat Universal Base Image 8., io.openshift.tags=base rhel8)
May 11 15:07:23 rh-brbaker podman[26309]: 2023-05-11 15:07:23.247285653 +1000 AEST m=+0.081643008 container start 739e563c8e83962a188ed75d260b21fdeade199021151612257eb609227f4f41 (image=localhost/sigsegv:latest, name=sigsegv, version=8.6, io.openshift.tags=base rhel8, distribution-scope=public, com.redhat.license_terms=https://www.redhat.com/en/about/red-hat-end-user-license-agreements#UBI, vendor=Red Hat, Inc., vcs-ref=f1ee6e37554363ec55e0035aba1a693d3627fdeb, description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., io.k8s.display-name=Red Hat Universal Base Image 8, com.redhat.component=ubi8-container, release=943, architecture=x86_64, vcs-type=git, io.buildah.version=1.30.0, com.redhat.build-host=cpt-1002.osbs.prod.upshift.rdu2.redhat.com, name=ubi8, url=https://access.redhat.com/containers/#/registry.access.redhat.com/ubi8/images/8.6-943, io.openshift.expose-services=, summary=Provides the latest release of Red Hat Universal Base Image 8., io.k8s.description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., maintainer=Red Hat, Inc., build-date=2022-09-06T17:25:04.845275)
May 11 15:07:23 rh-brbaker podman[26309]: 2023-05-11 15:07:23.247381122 +1000 AEST m=+0.081738546 container attach 739e563c8e83962a188ed75d260b21fdeade199021151612257eb609227f4f41 (image=localhost/sigsegv:latest, name=sigsegv, summary=Provides the latest release of Red Hat Universal Base Image 8., io.openshift.tags=base rhel8, release=943, io.k8s.description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., io.openshift.expose-services=, vendor=Red Hat, Inc., build-date=2022-09-06T17:25:04.845275, version=8.6, com.redhat.build-host=cpt-1002.osbs.prod.upshift.rdu2.redhat.com, com.redhat.license_terms=https://www.redhat.com/en/about/red-hat-end-user-license-agreements#UBI, name=ubi8, url=https://access.redhat.com/containers/#/registry.access.redhat.com/ubi8/images/8.6-943, io.buildah.version=1.30.0, vcs-ref=f1ee6e37554363ec55e0035aba1a693d3627fdeb, maintainer=Red Hat, Inc., description=The Universal Base Image is designed and engineered to be the base layer for all of your containerized applications, middleware and utilities. This base image is freely redistributable, but Red Hat only supports Red Hat technologies through subscriptions for Red Hat products. This image is maintained by Red Hat and updated regularly., io.k8s.display-name=Red Hat Universal Base Image 8, vcs-type=git, architecture=x86_64, com.redhat.component=ubi8-container, distribution-scope=public)
May 11 15:07:23 rh-brbaker podman[26309]: 2023-05-11 15:07:23.190432456 +1000 AEST m=+0.024789811 image pull 1e07fdb8d6b7a1ee9f2d2284c8974f16491b4ced3c7a207ad5643f9ad223d5f9 sigsegv:latest
May 11 15:07:28 rh-brbaker audit[26338]: ANOM_ABEND auid=1000 uid=1000 gid=1000 ses=3 subj=system_u:system_r:container_t:s0:c965,c1009 pid=26338 comm="sigsegv" exe="/app/sigsegv" sig=11 res=1
May 11 15:07:28 rh-brbaker kernel: sigsegv[26338]: segfault at 0 ip 00007f29e8252c91 sp 00007ffed9c17508 error 6 in libc-2.28.so[7f29e8183000+1bc000] likely on CPU 15 (core 7, socket 0)
May 11 15:07:28 rh-brbaker kernel: Code: 0f 82 43 d7 06 00 0f 1f 00 f3 0f 1e fa 48 89 f8 48 83 fa 20 72 49 48 83 fa 40 0f 87 9f 00 00 00 c5 fe 6f 06 c5 fe 6f 4c 16 e0 <c5> fe 7f 07 c5 fe 7f 4c 17 e0 c5 f8 77 c3 48 3b 15 fa 48 2f 00 0f
May 11 15:07:28 rh-brbaker audit: BPF prog-id=117 op=LOAD
May 11 15:07:28 rh-brbaker audit: BPF prog-id=118 op=LOAD
May 11 15:07:28 rh-brbaker audit: BPF prog-id=119 op=LOAD
May 11 15:07:28 rh-brbaker systemd[1]: Started systemd-coredump@7-26373-0.service - Process Core Dump (PID 26373/UID 0).
May 11 15:07:28 rh-brbaker audit[1]: SERVICE_START pid=1 uid=0 auid=4294967295 ses=4294967295 subj=system_u:system_r:init_t:s0 msg='unit=systemd-coredump@7-26373-0 comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
May 11 15:07:28 rh-brbaker systemd-coredump[26374]: [🡕] Process 26338 (sigsegv) of user 1000 dumped core.
                                                    
                                                    Module /app/sigsegv with build-id 6778fd22a95b17bd3d49a93b167d08ea156e094f
                                                    Module /usr/lib64/libc-2.28.so with build-id 64aa558dcdda2d8b0d7b04cef33ddbb2d9d8b8b4
                                                    Module /usr/lib64/ld-2.28.so with build-id 67aa0f1504abcfc4befb2b61a329a30a9984e0db
                                                    Module linux-vdso.so.1 with build-id e68f5c71076e43c6b64184a0639f2d84940ebc63
                                                    Stack trace of thread 1:
                                                    #0  0x00007f29e8252c91 n/a (/usr/lib64/libc-2.28.so + 0xcfc91)
                                                    #1  0x00007f29e81bdcf3 n/a (/usr/lib64/libc-2.28.so + 0x3acf3)
                                                    ELF object binary architecture: AMD x86-64

```

The following is the journal output from an OpenSHift node. This is useful if you have not enabled the core dump:

```
May 11 06:48:22 crc-9ltqk-master-0 systemd-coredump[195034]: Process 194097 (sigsegv) of user 1000660000 dumped core.
                                                             
                                                             Stack trace of thread 1:
                                                             #0  0x00007fce02a2cc91 __memmove_avx_unaligned_erms (libc.so.6)
                                                             #1  0x00000000004007fc n/a (/app/sigsegv)
                                                             #2  0x00007fce02997cf3 __libc_start_main (libc.so.6)
                                                             #3  0x000000000040065e n/a (/app/sigsegv)
May 11 06:48:22 crc-9ltqk-master-0 systemd[1]: systemd-coredump@32-195033-0.service: Succeeded.
May 11 06:48:22 crc-9ltqk-master-0 conmon[194084]: conmon 1a029d43f0e76a2d520a <ninfo>: container 194097 exited with status 139
May 11 06:48:22 crc-9ltqk-master-0 systemd[1]: systemd-coredump@32-195033-0.service: Consumed 802ms CPU time
```