#include "recov/recov.h"
#include "recov/recov_errno.h"
#include "recov/recov_bootsec.h"
#include "recov/recov_mftheader.h"

#include <string.h>
#include <stdlib.h>

int applyFixups(unsigned char *entry, NTFS_MFTHeader *hdr, uint64_t bytes_per_sector)
{
     uint16_t fixup_offset = hdr->fixup_offset;
     uint16_t fixup_count  = hdr->fixup_count;

     uint16_t *usa = (uint16_t*)(entry + fixup_offset);
     uint16_t usa_value = usa[0];
     for (int i = 1; i < fixup_count; i++)
     {
	  uint16_t *target = (uint16_t*)(entry + i * bytes_per_sector - 2);
	  if (*target != usa_value) return 0;
	  *target = usa[i];
     }

     return 1;
}

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

     printf("Raw clusters_per_mft_record byte = %d\n", ((unsigned char *)(&sector))[0x40]);
     // Ensure the disk is of NTFS format.
     if (memcmp(sector.oem_id, "NTFS    ", 8) != 0)
	  return RECOV_ERR_Not_NTFS;

     
     // 2. Extract NTFS info
     uint16_t bytes_per_sector = sector.bytes_per_sector;
     uint8_t sectors_per_cluster = sector.sectors_per_cluster;
     uint64_t mft_cluster = sector.mft_cluster;
     int8_t mft_record_value = sector.clusters_per_mft_record;
     
     printf("mft_cluster = %lld\n", mft_cluster);
     printf("bytes_per_sector = %d\n", bytes_per_sector);
     printf("sectors_per_cluster = %d\n", sectors_per_cluster);
     printf("mft_record_value = %d\n", mft_record_value);
     
     int mft_record_size;
     if (mft_record_value < 0)
	  mft_record_size = 10 << (-mft_record_value);  // typical case: -10 -> 1024
     else if (mft_record_value > 0)
	  mft_record_size = sectors_per_cluster * mft_record_value * bytes_per_sector;
     else
	  mft_record_size = sectors_per_cluster * bytes_per_sector;

     if (mft_record_size <= 0)
     {
	  fprintf(stderr, "Error: MFT Bad format\n");
	  return RECOV_ERR_Bad_Format;
     }
     
     // 3. Seek to MFT
     fseek(disk, mft_cluster * sectors_per_cluster * bytes_per_sector, SEEK_SET);
     
     printf("record size = %d\n", mft_record_size);
     fflush(stdout);
     // 4. Start reading MFT entries
     unsigned char *mft_entry = malloc(mft_record_size); // standard size
     while(fread(mft_entry, 1, mft_record_size, disk) == (size_t)mft_record_size)
     {
	  NTFS_MFTHeader *header = (NTFS_MFTHeader*)mft_entry;
	  if (header->signature != 0x454C4946) continue; /* Not an MFT Entry */

	  if (!applyFixups(mft_entry, header, bytes_per_sector)) continue; /* Corrupted entry */
	  
	  printf("FLAG = %d\n", header->flags);
	  fflush(stdout);
     }

     
     fclose(disk);
     return RECOV_ERR_Ok;
}
