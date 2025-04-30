#include "text.h"
client::client(int com, const char adress[])
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup失败: " << WSAGetLastError() << std::endl;
    }
    // 创建UDP Socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "创建Socket失败: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    // 配置AGV地址（替换为实际IP和端口）
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(com); // AGV端口
    inet_pton(AF_INET, adress, &destAddr.sin_addr);
}
client::~client()//析构函数关闭sock;
{
    closesocket(sock);
    WSACleanup();
}
void client::subscribe(uint16_t &aa4,uint16_t method,uint16_t time_interval,uint32_t time)
{
    // 构造报头
    Header header{};
    memset(&header, 0, sizeof(Header)); // 清空保留字段
    //const char* str = "HEADER_A1_DATA";
    uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
    memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
    header.a2 = 0x01;
    header.a3 = 0x00;//请求报文
    header.a4 = aa4;//用于分别每条指令
    aa4++;
    header.a5 = 0x10;
    header.a6 = 0xb1;//区分命令类型
    header.a7 = 0x00;//请求报文置0
    header.a8 = 0x00;//预留置0
    header.a9 = 0xc0;//数据区长度
    uint8_t Header_Array2[] = {0x00,0x00};
    memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组
      
    // 构造数据区
    Data data{};
    memset(&data, 0, sizeof(Data)); // 清空保留字段
    data.data1 = method;
    data.data2 = time_interval;//上报时间间隔
    data.data3 = time;//上报时间
    char buffer[sizeof(Header) + sizeof(Data)];
    memcpy(buffer, &header, sizeof(Header));
    memcpy(buffer + sizeof(Header), &data, sizeof(Data));

    // 发送数据
    int sentBytes = sendto(
        sock,
        buffer,
        sizeof(buffer),
        0,
        (sockaddr*)&destAddr,
        sizeof(destAddr)
    );
    if (sentBytes == SOCKET_ERROR)
    {
        std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
    }
}
//void client::mingle_navigation(point* point_structure, path* path_structure, uint16_t& aa4,uint32_t point_id, uint32_t mission_key, uint8_t point_size,
//    uint8_t path_size)//混合导航任务；
//{
//    // 构造报头
//    Header header{};
//    memset(&header, 0, sizeof(Header)); // 清空保留字段
//    //const char* str = "HEADER_A1_DATA";
//    uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
//    memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
//    header.a2 = 0x01;
//    header.a3 = 0x00;//请求报文
//    header.a4 = aa4;//用于分别每条指令
//    aa4++;
//    header.a5 = 0x10;
//    header.a6 = 0xb1;//区分命令类型
//    header.a7 = 0x00;//请求报文置0
//    header.a8 = 0x00;//预留置0
//    header.a9 = 0xc0;//数据区长度
//    uint8_t Header_Array2[] = {0x00,0x00};
//    memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组
//
//    mingle_navigation1 data;
//    data.a1 = point_id;//订单id
//    data.a2 = mission_key;//任务key
//    data.a4 = point_size;//点信息结构体数组中结构体个数
//    data.a5 = path_size;//边信息结构体数组中结构体个数
//    data.a5 = 0x0;//导航方式为路径拼接
//    data.a6 = 0x0;//预留
//
//    char buffer[sizeof(Header) + sizeof(Data)+sizeof(point)*3+ sizeof(path)*2];
//    memcpy(buffer, &header, sizeof(Header));
//    memcpy(buffer + sizeof(Header), &data, sizeof(Data));
//    memcpy(buffer + sizeof(Header)+ sizeof(data), point_structure, sizeof(point)*2);
//    memcpy(buffer + sizeof(Header) + sizeof(point)*2 + sizeof(data), path_structure, sizeof(path)*2);
//    // 发送数据
//    int sentBytes = sendto(
//        sock,
//        buffer,
//        sizeof(buffer),
//        0,
//        (sockaddr*)&destAddr,
//        sizeof(destAddr)
//    );
//    if (sentBytes == SOCKET_ERROR)
//    {
//        std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
//    }
//    else
//    {
//        std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
//    }
//}
void client::mingle_navigation(point* point_structure, path* path_structure, uint16_t& aa4,
    uint32_t point_id, uint32_t mission_key, uint8_t point_size,
    uint8_t path_size)
{
    // 构造报头
    Header header{};
    memset(&header, 0, sizeof(Header));
    uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
    memcpy(header.a1, Header_Array1, sizeof(header.a1));
    header.a2 = 0x01;
    header.a3 = 0x00;
    header.a4 = aa4++;
    header.a5 = 0x10;
    header.a6 = 0xAE;
    header.a7 = 0x00;
    header.a8 = 0x00;
   
    uint8_t Header_Array2[] = { 0x00,0x00 };
    memcpy(header.a10, Header_Array2, sizeof(header.a10));

    // 构造数据区
    mingle_navigation1 data;
    data.a1 = point_id;////订单id
    data.a2 = mission_key;//任务key
    data.a3 = point_size;  // 导航方式为路径拼接
    data.a4 = path_size;//点信息结构体数组中结构体个数
    data.a5 = 0x0;//边信息结构体数组中结构体个数
    data.a6 = 0x0;  // 预留

    // 计算总缓冲区大小
    size_t total_size = sizeof(Header) + sizeof(mingle_navigation1) +
        (sizeof(point) * point_size) +
        (sizeof(path) * path_size);
    header.a9 = (total_size)-sizeof(Header);//计算a9的值
    // 动态分配缓冲区
    char* buffer = new char[total_size];

    // 拷贝各部分数据
    char* ptr = buffer;
    memcpy(ptr, &header, sizeof(Header));
    ptr += sizeof(Header);

    memcpy(ptr, &data, sizeof(mingle_navigation1));
    ptr += sizeof(mingle_navigation1);

    memcpy(ptr, point_structure, sizeof(point) * point_size);
    ptr += sizeof(point) * point_size;
    memcpy(ptr, path_structure, sizeof(path) * path_size);
    // 发送数据
    int sentBytes = sendto(
        sock,
        buffer,
        total_size,
        0,
        (sockaddr*)&destAddr,
        sizeof(destAddr)
    );
    // 释放缓冲区
    delete[] buffer;
    if (sentBytes == SOCKET_ERROR) {
        std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
    }
}
service::service(int com, const char adress[])//构造函数
{
    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup失败: " << WSAGetLastError() << std::endl;
    }

    // 创建UDP Socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "创建Socket失败: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    // 绑定本地端口
    serverAddr.sin_family = AF_INET;
    unsigned long ip = 0;
    inet_pton(AF_INET, adress, &ip);
    serverAddr.sin_addr.s_addr = ip;
    serverAddr.sin_port = htons(com);
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "绑定端口失败: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
    }
    std::cout << "等待接收数据..." << std::endl;
}
service::~service()//析构函数关闭sock;
{
    closesocket(sock);
    WSACleanup();
}
void service::receive()
{
    // 接收数据
    char buffer[sizeof(Header) + sizeof(Data)];
    sockaddr_in clientAddr{};
    int clientAddrLen = sizeof(clientAddr);

    int recvBytes = recvfrom(
        sock,
        buffer,
        sizeof(buffer),
        0,
        (sockaddr*)&clientAddr,
        &clientAddrLen
    );

    if (recvBytes == SOCKET_ERROR) {
        std::cerr << "接收失败: " << WSAGetLastError() << std::endl;
    }
    else if (recvBytes != sizeof(buffer)) {
        std::cerr << "数据不完整，期望" << sizeof(buffer) << "字节，实际收到" << recvBytes << "字节" << std::endl;
    }
    else {
        // 解析数据
        Header* header = reinterpret_cast<Header*>(buffer);
        Data* data = reinterpret_cast<Data*>(buffer + sizeof(Header));

        std::cout << "\n==== 收到报文 ====" << std::endl;
        std::cout << "a1: " << std::hex << (int)header->a1[0] << std::endl;      // 输出字符串
        std::cout << "a1: " << std::hex << (int)header->a1[1] << std::endl;      // 输出字符串
        std::cout << "a1: " << std::hex << (int)header->a1[2] << std::endl;      // 输出字符串
        std::cout << "a1: " << std::hex << (int)header->a1[3] << std::endl;      // 输出字符串
        /*std::cout << "a2[0]: 0x" << std::hex << header->a2[0] << std::endl;
        std::cout << "a2[1]: 0x" << std::hex << header->a2[1] << std::endl;*/
        /*std::cout << "a3: " << std::dec << header->a3 << std::endl;*///
       /* std::cout << "a4: 0x" << std::hex << (int)header->a4 << std::endl;*/
        std::cout << "\n数据区内容：" << std::endl;
    }
}