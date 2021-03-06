/* 
 * File:   camera_ctrl.h
 * Author: fslib
 *
 * Created on 2013年6月15日, 上午11:19
 */

#ifndef CAMERA_CTRL_H
#define	CAMERA_CTRL_H
#include "../../PublicTools/publicDefine.h"


#ifdef	__cplusplus
extern "C" {
#endif

    /* 控制函数结构的定义,实质为数组,不能改变顺序 */
    struct Camera_ctl_function {
        /* 获取掩码,按位排布,0-任意转,1-左右转,2-上下转,3-45度斜角转,4-变倍,5-聚焦,6-预置位,7-3d定位 */
        unsigned long long (*mask_get)(void * const pCamera_item);
        /* 控制,任意转,成功返回1,失败返回-1 */
        signed char (*turn_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040,负数向左,正数向右 */const short speedx,
                /* 参考值精度为23040,负数向上,正数向下 */const short speedy, /* 转动时间 */const float time);
        /* 控制,左转,成功返回1,失败返回-1 */
        signed char (*turnLeft_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,右转,成功返回1,失败返回-1 */
        signed char (*turnRight_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,上转,成功返回1,失败返回-1 */
        signed char (*turnUp_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,下转,成功返回1,失败返回-1 */
        signed char (*turnDown_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,左上转,成功返回1,失败返回-1 */
        signed char (*turnLeftUp_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,左下转,成功返回1,失败返回-1 */
        signed char (*turnLeftDown_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,右上转,成功返回1,失败返回-1 */
        signed char (*turnRightUp_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,右下转,成功返回1,失败返回-1 */
        signed char (*turnRightDown_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
        /* 控制,变倍,成功返回1,失败返回-1 */
        signed char (*zoom_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040,负数缩小,正数放大 */const short speed, /* 变倍时间 */const float time);
        /* 控制,聚焦,成功返回1,失败返回-1 */
        signed char (*focus_pthreadSafety)(void * const pCamera_item, /* 参考值精度为23040,负数-,正数+ */const short speed, /* 聚焦时间 */const float time);
        /* 控制,设置预置位 */
        signed char (*preset_set_pthreadSafety)(void * const pCamera_item, /* 预置位号 */const unsigned short presetIndex);
        /* 控制,调用预置位 */
        signed char (*preset_goto_pthreadSafety)(void * const pCamera_item, /* 预置位号 */const unsigned short presetIndex);
        /* 控制,清除预置位 */
        signed char (*preset_clean_pthreadSafety)(void * const pCamera_item, /* 预置位号 */const unsigned short presetIndex);
        /* 控制,3D定位 */
        signed char (*zoom3D_pthreadSafety)(void * const pCamera_item, /* 相对图大小参考值23040 */const unsigned short x1,
                /* 相对图大小参考值23040 */const unsigned short y1, /* 相对图大小参考值23040 */const unsigned short x2, /* 相对图大小参考值23040 */const unsigned short y2);
    };
    struct Camera_ctrl_item;

    /* 相机控制 */
    struct Camera_ctrl {

        /* 
         * 结构约定:
         *     ro中为外部只读变量(仅限当前指针所指向的结构,内部指针所指向的内容是否可读由内部结构决定);
         *     rw中为外部可读写变量(仅限当前指针所指向的结构,内部指针所指向的内容是否可读写由内部结构决定);
         *     p中为私有变量,外部不可读也不可写;
         * 变量命名约定:
         *     凡需要(非0)初始化的变量,都要加上_的前缀;
         *     凡需要内存释放的变量,都要加上__的前缀. 
         */
        struct {
            /* 线程名 */
            char *_threadName;
            /* 监控的线程指针对象,开始线程后把创建的监控节点添加到此对象中 */
            void *_pMonitor;
            /* 父线程的线程号,由systemThreadTid获得,为0表示无父线程 */
            pthread_t _parentTid;
            /* 多线程共有的运行状态,如果此值为空,则引用默认的全局运行状态变量,本线程的运行状态只有与此值相同才能运行 */
            signed char *_externRunStatus;
            /* 连接池,为空表示内部使用独立的连接池 */
            void *_pSocketPool;
            /* 处理的相机链表 */
            FsObjectList *__cameralist;
            /* 添加或删除连接项(0:没有动作;1:添加;2:删除) */
            unsigned char changeItem : 2;
            /* 线程是否在运行(0:没有运行,1:在运行) */
            unsigned char inRun : 1;
            /* ThreadSocketIPv4线程的tid号 */
            pthread_t stid;
            /* 线程同步锁,仅用于添加或删除连接项 */
            pthread_mutex_t __mutex;
            /* 管道,用于添加和删除文件或发送数据 */
            int __pipe[2];
        } ro;

        struct {
            /* 运行状态:
             *     与_externRunStatus指向值相同-运行(一般此值为1);
             *     0-停止(手动退出后的状态值);
             *     2-重启(不一定实现);
             *     大于127-引用此值的所有线程都会退出;
             * 引用的指针类型必须是(char*);
             * 引用此值的其他线程应实现当自己的运行状态值[0-255]与此值[-128-127]不相等时退出.
             */
            unsigned char runStatus;
            /* 最长没有发送和接收数据的时间 */
        } rw;

        struct {
            /* 将要添加或删除的连接项 */
            struct Camera_ctrl_item *pCamera_ctrl_item_change;
        } p;
    };

    /* 相机控制项 */
    struct Camera_ctrl_item {

        /* 
         * 结构约定:
         *     ro中为外部只读变量(仅限当前指针所指向的结构,内部指针所指向的内容是否可读由内部结构决定);
         *     rw中为外部可读写变量(仅限当前指针所指向的结构,内部指针所指向的内容是否可读写由内部结构决定);
         *     p中为私有变量,外部不可读也不可写;
         * 变量命名约定:
         *     凡需要(非0)初始化的变量,都要加上_的前缀;
         *     凡需要内存释放的变量,都要加上__的前缀. 
         */
        struct {
            /* 用户名,可为空 */
            char* _username;
            /* 密码,可为空 */
            char* _password;
            /* uuid,通道标识,可为空 */
            char* _uuid;
            /* 数据连接对象 */
            struct SocketPool_item *__pSocketPool_item;
            /* 互斥锁 */
            pthread_mutex_t __mutex;
        } ro;

        struct {
        } rw;

        struct {
            /* 所属的海康相机控制对象 */
            struct Camera_ctrl *pCamera_ctrl;
            /* 掩码,按位排布,0-任意转,1-左右转,2-上下转,3-45度斜角转,4-变倍,5-聚焦,6-预置位,7-3d定位 */
            unsigned long long mask;
            /* 状态,0-未知,1-空闲,2-任意转,3-左转,4-右转,5-上转,6-下转,7-左上,8-左下,9-右上,10-右下,11-变倍,12-聚焦,13-设置预置位,14-调用预置位,15-清除预置位,16-3d定位 */
            unsigned char status;
            /* 设置的状态,2-任意转,3-左转,4-右转,5-上转,6-下转,7-左上,8-左下,9-右上,10-右下,11-变倍,12-聚焦,13-设置预置位,14-调用预置位,15-清除预置位,16-3d定位 */
            unsigned char status_set;
            /* 校验和 */
            int sum;
            /* 新的校验和 */
            int sum_set;
            /* 设置的参数1 */
            short parameter1_set;
            /* 设置的参数2 */
            short parameter2_set;
            /* 设置的参数3 */
            short parameter3_set;
            /* 设置的参数4 */
            short parameter4_set;
            /* 可以做下一个动作的时间 */
            double time_next;
            /* 动作最长可以执行的时间 */
            double time_end;
        } p;
    };
    /* 
     * 创建一个新的Camera_ctrl对象;
     * 返回Camera_ctrl指针.
     */
    struct Camera_ctrl *camera_ctrl_new__IO(/* 线程名 */const char threadName[],
            /* 监控的线程指针对象,开始线程后把创建的监控节点添加到此对象中 */ void *pMonitor,
            /* 父线程的线程号,由systemThreadTid获得,为0表示无父线程 */ const pthread_t parentTid,
            /* 多线程共有的运行状态,如果此值为空,则引用默认的全局运行状态变量,本线程的运行状态只有与此值相同才能运行 */signed char *externRunStatus,
            /* 连接池,为空表示内部使用独立的连接池 */ void * const pSocketPool);
    /* 删除pCamera_ctrl指向的实例对象 */
    void camera_ctrl_delete__OI(struct Camera_ctrl * const pCamera_ctrl);

    /* 开始线程,必须先设置好初始化时为可写全过程可读区的所有参数 */
    void camera_ctrl_startThread(struct Camera_ctrl * const pCamera_ctrl,
            /* 线程分时优先级,有效值为[-1-99],越大优先级越高,0表示不改变优先级,继承父线程的优先级,-1-表示强制线程的调度模式为分时调度策略,正数强制线程的调度模式为实时分时调度策略 */const signed char nice);
    /* 结束线程 */
    void camera_ctrl_stopThread(struct Camera_ctrl * const pCamera_ctrl);
    /* 
     * 创建一个新的Camera_ctrl_item对象;
     * 返回Camera_ctrl_item指针.
     */
    struct Camera_ctrl_item *camera_ctrl_item_new__IO(/* ipv4地址 */const unsigned int ipv4, /* ipv4地址端口号 */const unsigned short ipv4Port,
            /* 用户名,可为空 */const char username[], /* 密码,可为空 */const char password[], /* uuid,通道标识,可为空 */const char uuid[]);
    /* 删除pCamera_ctrl_item指向的实例对象 */
    void camera_ctrl_item_delete__OI(struct Camera_ctrl_item * const pCamera_ctrl_item);
    /* 向pCamera_ctrl中添加一个控制项 */
    void camera_ctrl_add_item__OI_2(struct Camera_ctrl * const pCamera_ctrl, struct Camera_ctrl_item * const pCamera_ctrl_item);
    /* 从pCamera_ctrl中移除一个控制项 */
    void camera_ctrl_remove_item__IO_2(struct Camera_ctrl * const pCamera_ctrl, struct Camera_ctrl_item * const pCamera_ctrl_item);
    /* 获取掩码,按位排布,0-任意转,1-左右转,2-上下转,3-45度斜角转,4-变倍,5-聚焦,6-预置位,7-3d定位 */
    unsigned long long camera_ctrl_item_mask_get(struct Camera_ctrl_item * const pCamera_ctrl_item);
    /* 控制,任意转 */
    void camera_ctrl_item_ctl_turn_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040,负数向左,正数向右 */const short speedx,
            /* 参考值精度为23040,负数向上,正数向下 */const short speedy, /* 转动时间 */const float time);
    /* 控制,左转 */
    void camera_ctrl_item_ctl_turnLeft_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,右转 */
    void camera_ctrl_item_ctl_turnRight_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,上转 */
    void camera_ctrl_item_ctl_turnUp_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,下转 */
    void camera_ctrl_item_ctl_turnDown_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,左上转 */
    void camera_ctrl_item_ctl_turnLeftUp_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,左下转 */
    void camera_ctrl_item_ctl_turnLeftDown_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,右上转 */
    void camera_ctrl_item_ctl_turnRightUp_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,右下转 */
    void camera_ctrl_item_ctl_turnRightDown_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040 */const unsigned short speed, /* 转动时间 */const float time);
    /* 控制,变倍 */
    void camera_ctrl_item_ctl_zoom_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040,负数缩小,正数放大 */const short speed, /* 变倍时间 */const float time);
    /* 控制,聚焦 */
    void camera_ctrl_item_ctl_focus_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 参考值精度为23040,负数-,正数+ */const short speed, /* 聚焦时间 */const float time);
    /* 控制,设置预置位 */
    void camera_ctrl_item_ctl_preset_set_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 预置位号 */const unsigned short presetIndex);
    /* 控制,调用预置位 */
    void camera_ctrl_item_ctl_preset_goto_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 预置位号 */const unsigned short presetIndex);
    /* 控制,清除预置位 */
    void camera_ctrl_item_ctl_preset_clean_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 预置位号 */const unsigned short presetIndex);
    /* 控制,3D定位 */
    void camera_ctrl_item_ctl_zoom3D_pthreadSafety(struct Camera_ctrl_item * const pCamera_ctrl_item, /* 相对图大小参考值23040 */const unsigned short x1,
            /* 相对图大小参考值23040 */const unsigned short y1, /* 相对图大小参考值23040 */const unsigned short x2, /* 相对图大小参考值23040 */const unsigned short y2);
#ifdef FsDebug
    void camera_ctrl_test();
#endif
#ifdef	__cplusplus
}
#endif

#endif	/* CAMERA_CTRL_HK_HTTP_H */

