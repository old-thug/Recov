#include "recov/recov.h"
#include "recov/recov_errno.h"

int scanDrive(const char *drive_name)
{
     char raw_path[32];
     snprintf(raw_path, sizeof(raw_path), "\\\\.\\%s", drive_name);
     printf("Scanning: `%s` Drive\n", raw_path);
     FILE *disk = fopen(raw_path, "rb");

     if (!disk)
     {	  
	  return RECOV_ERR_No_Such_Drive;
     }

     // 1. Read boot sector
     unsigned char sector[512];
     fread(sector, 1, 512, disk);

     // 2. Extract NTFS info
     uint16_t bytes_per_sector = *(uint16_t*)(sector + 11);
     uint8_t sectors_per_cluster = *(uint8_t*)(sector + 13);
     uint64_t mft_cluster = *(uint64_t*)(sector + 48); // usually offset 0x30

     // 3. Seek to MFT
     fseek(disk, mft_cluster * sectors_per_cluster * bytes_per_sector, SEEK_SET);

     // 4. Start reading MFT entries
     unsigned char mft_entry[1024]; // standard size
     while(fread(mft_entry, 1, 1024, disk) == 1024)
     {
	  
     }

     
     fclose(disk);
     return RECOV_ERR_Ok;
}
