#ifndef TEXT_H
#define TEXT_H
#include <Winsock2.h>//这个必须在前
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
 class service
    {
    public:
        WSADATA wsaData;
        SOCKET sock;
        sockaddr_in serverAddr;
        struct Header  
        {
            uint8_t a1[16];
            uint8_t a2;
            uint8_t a3;
            uint16_t a4;
            uint8_t a5;
            uint8_t a6;
            uint8_t a7;
            uint8_t a8;
            uint16_t a9;
            uint8_t a10[2];
        };
        struct Data   
        {
            uint16_t  data1;
            uint16_t  data2;
            uint32_t  data3;
            uint8_t  data4[];
        };
    public:
        service(int com, const char adress[]);//构造函数创建UDP Socket 配置AGV地址（替换为实际IP和端口）
        ~service();
        void receive();
    };
    class client
    {
    public:
        WSADATA wsaData;
        SOCKET sock;
        sockaddr_in destAddr{};
        struct Header    //所有结构体的头
        {
            uint8_t a1[16];
            uint8_t a2;
            uint8_t a3;
            uint16_t a4;
            uint8_t a5;
            uint8_t a6;
            uint8_t a7;
            uint8_t a8;
            uint16_t a9;
            uint8_t  a10[2];
        };
        struct Data          //订阅的数据区
        {
            uint16_t  data1;
            uint16_t  data2;
            uint32_t  data3;
            uint8_t  data4[184];
        };
        
        struct point  // 导航定义子结构体类型
        {
            uint32_t a7;
            uint32_t a8;
            float a9;
            uint8_t a10;
            uint8_t a11;
            uint8_t a12[6]; 
        };
        struct path   // 导航定义子结构体类型
        {
            uint32_t a21;
            uint32_t a22;
            float  a23;
            uint8_t a24;
            uint8_t a25;
            uint8_t a26;
            uint8_t a27;
            float a28;
            float a29;
            uint8_t a30[4];
        };
        struct action //导航中的任务控制// 导航定义子结构体类型
        {
            uint16_t a1;
            uint8_t a2;
            uint8_t a3;//预留0
            uint32_t a4;
            uint8_t a5;//参数长度param_size 
            uint8_t a6[3];//预留0
            struct rise
            {
                uint8_t a1;
                uint8_t a2[3];
            };
            struct  stop
            {
                uint32_t a1;
                uint8_t a2;
                uint8_t a3[3];

            };
            struct restore
            {
                uint32_t id;
                uint32_t key;
            };    //恢复
        };
        struct mingle_navigation1 // 导航定义子结构体类型
        {
            uint32_t a1;
            uint32_t a2;
            uint8_t  a3;
            uint8_t  a4;
            uint8_t  a5;
            uint8_t  a6;
        };
    public:
        client(int com, const char adress[]);//构造函数创建UDP Socket 配置AGV地址（替换为实际IP和端口）
        ~client();
        void subscribe(uint16_t &aa4, uint16_t method, uint16_t time_interval, uint32_t time);//订阅函数；
        void mingle_navigation(point* point_structure,action*action_structure, path* path_structure, uint16_t& aa4, uint32_t point_id, uint32_t mission_key, uint8_t point_size, uint8_t path_size);//带动作的导航（高级版）
        void mingle_navigation(point* point_structure, path* path_structure, uint16_t& aa4, uint32_t point_id, uint32_t mission_key, uint8_t point_size, uint8_t path_size);//不带动作的导航（普通版）
        
    };
#endif 






