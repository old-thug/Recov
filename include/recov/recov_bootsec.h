#pragma once

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
     uint8_t  jump[3];            // 0x00: Jump to bootstrap
     uint8_t  oem_id[8];          // 0x03: "NTFS    "
     
     uint16_t bytes_per_sector;   // 0x0B
     uint8_t  sectors_per_cluster;// 0x0D
     
     uint16_t reserved;
     uint8_t  zeros1[7];

     uint8_t  media_descriptor;   // 0x15
     uint8_t  zeros2[18];
     uint64_t total_sectors;      // 0x28
     
     uint64_t mft_cluster;        // 0x30: LCN of $MFT
     uint64_t mft_mirr_cluster;   // 0x38
     
     int8_t   clusters_per_mft_record; // 0x40: Usually -10 meaning 2^10 bytes

     uint8_t  clusters_per_index_record; // 0x44
     uint8_t  padding[0x200 - 0x42];
} NTFS_BootSector;
#pragma pack(pop)
