#pragma once

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    uint8_t  jump[3];            // 0x00: Jump to bootstrap
    uint8_t  oem_id[8];          // 0x03: "NTFS    "

    uint16_t bytes_per_sector;   // 0x0B
    uint8_t  sectors_per_cluster;// 0x0D

    uint16_t reserved1;          // 0x0E (unused)
    uint8_t  reserved2[5];       // 0x10 (unused)

    uint8_t  media_descriptor;   // 0x15
    uint16_t reserved3;          // 0x16 (unused)
    uint16_t sectors_per_track;  // 0x18
    uint16_t num_heads;          // 0x1A
    uint32_t hidden_sectors;     // 0x1C
    uint32_t reserved4;          // 0x20

    uint64_t total_sectors;      // 0x28

    uint64_t mft_cluster;        // 0x30: LCN of $MFT
    uint64_t mft_mirr_cluster;   // 0x38

    int8_t   clusters_per_mft_record; // 0x40: Usually -10 meaning 2^10 bytes
    uint8_t  reserved5[3];

    int8_t   clusters_per_index_record; // 0x44
    uint8_t  reserved6[3];

    uint64_t volume_serial;      // 0x48
    uint32_t checksum;           // 0x50

    uint8_t  bootstrap[426];     // 0x54
    uint16_t end_marker;         // 0x1FE: 0xAA55
} NTFS_BootSector;
#pragma pack(pop)
