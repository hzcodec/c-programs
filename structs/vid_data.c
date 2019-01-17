/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-17
    File         : vid_data.c
    Reference    : -
    Description  : How to handle a vendor id struct.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct vendor_name {
	unsigned short vid;
	const char* name;
};

static struct vendor_name usb_vendor[] = {
	{ 0x0001, "Fry's Electronics" },
	{ 0x0002, "Ingram" },
	{ 0x0003, "Helix" },
};

 
const char* get_vendor_name(unsigned short vid)
{
	for(int i=0; i<sizeof(usb_vendor)/sizeof(usb_vendor[0]); i++) {
		printf("usb_vendor[%d] = %s\n", i, usb_vendor[i].name);

		if (usb_vendor[i].vid == vid) {
			return usb_vendor[i].name;
		}
	}
	printf("\n");

	return NULL;
}

int main(int argc, char *argv[])
{

	struct vendor_name vn;
	struct vendor_name *pvn = &vn;

	pvn->name = get_vendor_name(0);
	printf("name: %s\n", pvn->name);

	pvn->name = get_vendor_name(1);
	printf("name: %s\n", pvn->name);

	pvn->name = get_vendor_name(2);
	printf("name: %s\n", pvn->name);

	pvn->name = get_vendor_name(3);
	printf("name: %s\n", pvn->name);

	pvn->name = get_vendor_name(4);
	printf("name: %s\n", pvn->name);

        return 0;
}
