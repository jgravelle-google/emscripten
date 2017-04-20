#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) 0, __SYSCALL_LL_E((x))

#define __SC_socket      1
#define __SC_bind        2
#define __SC_connect     3
#define __SC_listen      4
#define __SC_accept      5
#define __SC_getsockname 6
#define __SC_getpeername 7
#define __SC_socketpair  8
#define __SC_send        9
#define __SC_recv        10
#define __SC_sendto      11
#define __SC_recvfrom    12
#define __SC_shutdown    13
#define __SC_setsockopt  14
#define __SC_getsockopt  15
#define __SC_sendmsg     16
#define __SC_recvmsg     17
#define __SC_accept4     18
#define __SC_recvmmsg    19
#define __SC_sendmmsg    20

// static syscalls. we must have one non-variadic argument before the rest due to ISO C.

#ifdef __cplusplus
extern "C" {
#endif

#define EM_SYSCALL_ARGS_0
#define EM_SYSCALL_ARGS_1 int arg1
#define EM_SYSCALL_ARGS_2 EM_SYSCALL_ARGS_1, int arg2
#define EM_SYSCALL_ARGS_3 EM_SYSCALL_ARGS_2, int arg3
#define EM_SYSCALL_ARGS_4 EM_SYSCALL_ARGS_3, int arg4
#define EM_SYSCALL_ARGS_5 EM_SYSCALL_ARGS_4, int arg5
#define EM_SYSCALL_ARGS_6 EM_SYSCALL_ARGS_5, int arg6
#define EM_SYSCALL_ARGS_7 EM_SYSCALL_ARGS_6, int arg7

long __syscall1(EM_SYSCALL_ARGS_1);
long __syscall3(EM_SYSCALL_ARGS_3);
long __syscall4(EM_SYSCALL_ARGS_3);
long __syscall5();
long __syscall6(EM_SYSCALL_ARGS_1);
long __syscall9(EM_SYSCALL_ARGS_2);
long __syscall10(EM_SYSCALL_ARGS_1);
long __syscall12(EM_SYSCALL_ARGS_1);
long __syscall14(EM_SYSCALL_ARGS_3);
long __syscall15(EM_SYSCALL_ARGS_2);
long __syscall20(EM_SYSCALL_ARGS_0);
long __syscall29(EM_SYSCALL_ARGS_0);
long __syscall33(EM_SYSCALL_ARGS_2);
long __syscall34(EM_SYSCALL_ARGS_1);
long __syscall36(EM_SYSCALL_ARGS_0);
long __syscall38(EM_SYSCALL_ARGS_2);
long __syscall39(EM_SYSCALL_ARGS_2);
long __syscall40(EM_SYSCALL_ARGS_1);
long __syscall41(EM_SYSCALL_ARGS_1);
long __syscall42(EM_SYSCALL_ARGS_0);
long __syscall51(EM_SYSCALL_ARGS_0);
long __syscall54(EM_SYSCALL_ARGS_3);
long __syscall57(EM_SYSCALL_ARGS_2);
long __syscall60(EM_SYSCALL_ARGS_1);
long __syscall63(EM_SYSCALL_ARGS_2);
long __syscall64(EM_SYSCALL_ARGS_0);
long __syscall65(EM_SYSCALL_ARGS_0);
long __syscall66(EM_SYSCALL_ARGS_0);
long __syscall75(EM_SYSCALL_ARGS_0);
long __syscall77(EM_SYSCALL_ARGS_2);
long __syscall83(EM_SYSCALL_ARGS_2);
long __syscall85(EM_SYSCALL_ARGS_3);
long __syscall91(EM_SYSCALL_ARGS_2);
long __syscall94(EM_SYSCALL_ARGS_2);
long __syscall96(EM_SYSCALL_ARGS_0);
long __syscall97(EM_SYSCALL_ARGS_0);
long __syscall102(EM_SYSCALL_ARGS_2);
long __syscall104(EM_SYSCALL_ARGS_0);
long __syscall114(EM_SYSCALL_ARGS_0);
long __syscall118(EM_SYSCALL_ARGS_1);
long __syscall121(EM_SYSCALL_ARGS_0);
long __syscall122(EM_SYSCALL_ARGS_1);
long __syscall125(EM_SYSCALL_ARGS_0);
long __syscall132(EM_SYSCALL_ARGS_1);
long __syscall133(EM_SYSCALL_ARGS_1);
long __syscall140(EM_SYSCALL_ARGS_5);
long __syscall142(EM_SYSCALL_ARGS_5);
long __syscall144(EM_SYSCALL_ARGS_3);
long __syscall145(EM_SYSCALL_ARGS_3);
long __syscall146(EM_SYSCALL_ARGS_3);
long __syscall147(EM_SYSCALL_ARGS_1);
long __syscall148(EM_SYSCALL_ARGS_1);
long __syscall150(EM_SYSCALL_ARGS_0);
long __syscall151(EM_SYSCALL_ARGS_0);
long __syscall152(EM_SYSCALL_ARGS_0);
long __syscall153(EM_SYSCALL_ARGS_0);
long __syscall163(EM_SYSCALL_ARGS_0);
long __syscall168(EM_SYSCALL_ARGS_3);
long __syscall178(EM_SYSCALL_ARGS_3);
long __syscall180(EM_SYSCALL_ARGS_6);
long __syscall181(EM_SYSCALL_ARGS_6);
long __syscall183(EM_SYSCALL_ARGS_2);
long __syscall191(EM_SYSCALL_ARGS_2);
long __syscall192(EM_SYSCALL_ARGS_6);
long __syscall193(EM_SYSCALL_ARGS_4);
long __syscall194(EM_SYSCALL_ARGS_4);
long __syscall195(EM_SYSCALL_ARGS_2);
long __syscall196(EM_SYSCALL_ARGS_2);
long __syscall197(EM_SYSCALL_ARGS_2);
long __syscall198(EM_SYSCALL_ARGS_3);
long __syscall199(EM_SYSCALL_ARGS_0);
long __syscall200(EM_SYSCALL_ARGS_0);
long __syscall201(EM_SYSCALL_ARGS_0);
long __syscall202(EM_SYSCALL_ARGS_0);
long __syscall203(EM_SYSCALL_ARGS_2);
long __syscall204(EM_SYSCALL_ARGS_2);
long __syscall205(EM_SYSCALL_ARGS_2);
long __syscall207(EM_SYSCALL_ARGS_3);
long __syscall208(EM_SYSCALL_ARGS_3);
long __syscall209(EM_SYSCALL_ARGS_3);
long __syscall211(EM_SYSCALL_ARGS_3);
long __syscall212(EM_SYSCALL_ARGS_3);
long __syscall218(EM_SYSCALL_ARGS_0);
long __syscall219(EM_SYSCALL_ARGS_0);
long __syscall220(EM_SYSCALL_ARGS_3);
long __syscall221();
// long __syscall221(EM_SYSCALL_ARGS_3);
long __syscall252(EM_SYSCALL_ARGS_1);
long __syscall265(EM_SYSCALL_ARGS_0);
long __syscall268(EM_SYSCALL_ARGS_3);
long __syscall269(EM_SYSCALL_ARGS_3);
long __syscall272(EM_SYSCALL_ARGS_0);
long __syscall295();
long __syscall296(EM_SYSCALL_ARGS_3);
long __syscall297(EM_SYSCALL_ARGS_4);
long __syscall298(EM_SYSCALL_ARGS_5);
long __syscall299(EM_SYSCALL_ARGS_0);
long __syscall300(EM_SYSCALL_ARGS_4);
long __syscall301(EM_SYSCALL_ARGS_3);
long __syscall302(EM_SYSCALL_ARGS_4);
long __syscall303(EM_SYSCALL_ARGS_0);
long __syscall304(EM_SYSCALL_ARGS_3);
long __syscall305(EM_SYSCALL_ARGS_4);
long __syscall306();
long __syscall307(EM_SYSCALL_ARGS_4);
long __syscall308(EM_SYSCALL_ARGS_0);
long __syscall320(EM_SYSCALL_ARGS_4);
long __syscall324(EM_SYSCALL_ARGS_6);
long __syscall330(EM_SYSCALL_ARGS_3);
long __syscall331(EM_SYSCALL_ARGS_0);
long __syscall333(EM_SYSCALL_ARGS_5);
long __syscall334(EM_SYSCALL_ARGS_5);
long __syscall337(EM_SYSCALL_ARGS_5);
long __syscall340(EM_SYSCALL_ARGS_4);
long __syscall345(EM_SYSCALL_ARGS_4);

#undef SYS_futimesat

#ifdef __cplusplus
}
#endif
