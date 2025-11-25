#include "recov/recov.h"
#include "recov/recov_errno.h"
#include "recov/recov_bootsec.h"
#include "recov/recov_mftheader.h"

#include <string.h>
#include <stdlib.h>

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
     NTFS_BootSector sector = {0};
     fread(&sector, 1, 512, disk);

     // Ensure the disk is of NTFS format.
     if (memcmp(sector.oem_id, "NTFS    ", 8) != 0)
	  return RECOV_ERR_Not_NTFS;

     
     // 2. Extract NTFS info
     uint16_t bytes_per_sector = sector.bytes_per_sector;
     uint8_t sectors_per_cluster = sector.sectors_per_cluster;
     uint64_t mft_cluster = sector.mft_cluster;

     // 3. Seek to MFT
     int8_t mft_record_value = sector.clusters_per_mft_record;
     int mft_record_size;

     if (mft_record_value < 0)
	  mft_record_size = 1 << (-mft_record_value);  // typical case: -10 -> 1024
     else
	  mft_record_size = mft_record_value * sectors_per_cluster * bytes_per_sector;

     fseek(disk, mft_cluster * sectors_per_cluster * bytes_per_sector, SEEK_SET);

     // 4. Start reading MFT entries
     unsigned char *mft_entry = malloc(mft_record_size); // standard size
     while(fread(mft_entry, 1, 1024, disk) == 1024)
     {
	  NTFS_MFTHeader header = *((NTFS_MFTHeader*)mft_entry);
	  printf("FLAG = %d\n", header.flags);
     }

     
     fclose(disk);
     return RECOV_ERR_Ok;
}
