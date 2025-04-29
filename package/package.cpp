#include "text.h"
int main()
{
	uint16_t a = 0;
	client objiective(17804, "192.168.3.125");
	objiective.subscribe(a, 0xaf, 100, 60000);
	uint16_t b = 0;
	objiective.pause_action(b, 0x01, 0x08, 0x01);
	uint16_t c = 0;
	objiective.resume_action(c, 0x02, 0x09, 0x10, 0x11);
	uint16_t d = 0;
	objiective.cancel_action(d, 0x00, 0x10, 0x10, 0x01);
	uint16_t e = 0;
	objiective.pallet_action(e, 0x02, 0x11, 0x00);
	//service myService(17804, "192.168.3.125");

	/*while (1)
	{
		myService.receive();
	}*/

	return 0;
}