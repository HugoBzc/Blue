#include "text.h"
int main()
{
    uint16_t a = 1;
    client objiective(17804, "10.93.68.189");
    //objiective.subscribe(a,0xaf,100,60000);
    client::point sss[] = { {2,1,1,1,0,{0,0,0,0,0,0}},{2,1,1,1,0,{0,0,0,0,0,0}} };//自己构造点结构体
    client::path  ddd[] = { {1,1,1,1,1,1,1,1,1,{1,1,1,1}},{1,1,1,1,1,1,1,1,1,{1,1,1,1}} };//自己构造边结构体
    objiective.mingle_navigation(sss,ddd,a,1,1,2,0);//调试一下
    return 0;
}
