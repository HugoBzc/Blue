#include "text.h"
client::client(int com, const char adress[])
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartupʧ��: " << WSAGetLastError() << std::endl;
    }
    // ����UDP Socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "����Socketʧ��: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    // ����AGV��ַ���滻Ϊʵ��IP�Ͷ˿ڣ�
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(com); // AGV�˿�
    inet_pton(AF_INET, adress, &destAddr.sin_addr);
}
client::~client()//���������ر�sock;
{
    closesocket(sock);
    WSACleanup();
}
void client::subscribe(uint16_t &aa4,uint16_t method,uint16_t time_interval,uint32_t time)
{
    // ���챨ͷ
    Header header{};
    memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
    //const char* str = "HEADER_A1_DATA";
    uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
    memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
    header.a2 = 0x01;
    header.a3 = 0x00;//������
    header.a4 = aa4;//���ڷֱ�ÿ��ָ��
    aa4++;
    header.a5 = 0x10;
    header.a6 = 0xb1;//������������
    header.a7 = 0x00;//��������0
    header.a8 = 0x00;//Ԥ����0
    header.a9 = 0xc0;//����������
    uint8_t Header_Array2[] = {0x00,0x00};
    memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����
      
    // ����������
    Data data{};
    memset(&data, 0, sizeof(Data)); // ��ձ����ֶ�
    data.data1 = method;
    data.data2 = time_interval;//�ϱ�ʱ����
    data.data3 = time;//�ϱ�ʱ��
    char buffer[sizeof(Header) + sizeof(Data)];
    memcpy(buffer, &header, sizeof(Header));
    memcpy(buffer + sizeof(Header), &data, sizeof(Data));

    // ��������
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
        std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
    }
}
//void client::mingle_navigation(point* point_structure, path* path_structure, uint16_t& aa4,uint32_t point_id, uint32_t mission_key, uint8_t point_size,
//    uint8_t path_size)//��ϵ�������
//{
//    // ���챨ͷ
//    Header header{};
//    memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
//    //const char* str = "HEADER_A1_DATA";
//    uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
//    memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
//    header.a2 = 0x01;
//    header.a3 = 0x00;//������
//    header.a4 = aa4;//���ڷֱ�ÿ��ָ��
//    aa4++;
//    header.a5 = 0x10;
//    header.a6 = 0xb1;//������������
//    header.a7 = 0x00;//��������0
//    header.a8 = 0x00;//Ԥ����0
//    header.a9 = 0xc0;//����������
//    uint8_t Header_Array2[] = {0x00,0x00};
//    memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����
//
//    mingle_navigation1 data;
//    data.a1 = point_id;//����id
//    data.a2 = mission_key;//����key
//    data.a4 = point_size;//����Ϣ�ṹ�������нṹ�����
//    data.a5 = path_size;//����Ϣ�ṹ�������нṹ�����
//    data.a5 = 0x0;//������ʽΪ·��ƴ��
//    data.a6 = 0x0;//Ԥ��
//
//    char buffer[sizeof(Header) + sizeof(Data)+sizeof(point)*3+ sizeof(path)*2];
//    memcpy(buffer, &header, sizeof(Header));
//    memcpy(buffer + sizeof(Header), &data, sizeof(Data));
//    memcpy(buffer + sizeof(Header)+ sizeof(data), point_structure, sizeof(point)*2);
//    memcpy(buffer + sizeof(Header) + sizeof(point)*2 + sizeof(data), path_structure, sizeof(path)*2);
//    // ��������
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
//        std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
//    }
//    else
//    {
//        std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
//    }
//}
void client::mingle_navigation(point* point_structure, path* path_structure, uint16_t& aa4,
    uint32_t point_id, uint32_t mission_key, uint8_t point_size,
    uint8_t path_size)
{
    // ���챨ͷ
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

    // ����������
    mingle_navigation1 data;
    data.a1 = point_id;////����id
    data.a2 = mission_key;//����key
    data.a3 = point_size;  // ������ʽΪ·��ƴ��
    data.a4 = path_size;//����Ϣ�ṹ�������нṹ�����
    data.a5 = 0x0;//����Ϣ�ṹ�������нṹ�����
    data.a6 = 0x0;  // Ԥ��

    // �����ܻ�������С
    size_t total_size = sizeof(Header) + sizeof(mingle_navigation1) +
        (sizeof(point) * point_size) +
        (sizeof(path) * path_size);
    header.a9 = (total_size)-sizeof(Header);//����a9��ֵ
    // ��̬���仺����
    char* buffer = new char[total_size];

    // ��������������
    char* ptr = buffer;
    memcpy(ptr, &header, sizeof(Header));
    ptr += sizeof(Header);

    memcpy(ptr, &data, sizeof(mingle_navigation1));
    ptr += sizeof(mingle_navigation1);

    memcpy(ptr, point_structure, sizeof(point) * point_size);
    ptr += sizeof(point) * point_size;
    memcpy(ptr, path_structure, sizeof(path) * path_size);
    // ��������
    int sentBytes = sendto(
        sock,
        buffer,
        total_size,
        0,
        (sockaddr*)&destAddr,
        sizeof(destAddr)
    );
    // �ͷŻ�����
    delete[] buffer;
    if (sentBytes == SOCKET_ERROR) {
        std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
    }
}
service::service(int com, const char adress[])//���캯��
{
    // ��ʼ��Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartupʧ��: " << WSAGetLastError() << std::endl;
    }

    // ����UDP Socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "����Socketʧ��: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    // �󶨱��ض˿�
    serverAddr.sin_family = AF_INET;
    unsigned long ip = 0;
    inet_pton(AF_INET, adress, &ip);
    serverAddr.sin_addr.s_addr = ip;
    serverAddr.sin_port = htons(com);
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "�󶨶˿�ʧ��: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
    }
    std::cout << "�ȴ���������..." << std::endl;
}
service::~service()//���������ر�sock;
{
    closesocket(sock);
    WSACleanup();
}
void service::receive()
{
    // ��������
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
        std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
    }
    else if (recvBytes != sizeof(buffer)) {
        std::cerr << "���ݲ�����������" << sizeof(buffer) << "�ֽڣ�ʵ���յ�" << recvBytes << "�ֽ�" << std::endl;
    }
    else {
        // ��������
        Header* header = reinterpret_cast<Header*>(buffer);
        Data* data = reinterpret_cast<Data*>(buffer + sizeof(Header));

        std::cout << "\n==== �յ����� ====" << std::endl;
        std::cout << "a1: " << std::hex << (int)header->a1[0] << std::endl;      // ����ַ���
        std::cout << "a1: " << std::hex << (int)header->a1[1] << std::endl;      // ����ַ���
        std::cout << "a1: " << std::hex << (int)header->a1[2] << std::endl;      // ����ַ���
        std::cout << "a1: " << std::hex << (int)header->a1[3] << std::endl;      // ����ַ���
        /*std::cout << "a2[0]: 0x" << std::hex << header->a2[0] << std::endl;
        std::cout << "a2[1]: 0x" << std::hex << header->a2[1] << std::endl;*/
        /*std::cout << "a3: " << std::dec << header->a3 << std::endl;*///
       /* std::cout << "a4: 0x" << std::hex << (int)header->a4 << std::endl;*/
        std::cout << "\n���������ݣ�" << std::endl;
    }
}