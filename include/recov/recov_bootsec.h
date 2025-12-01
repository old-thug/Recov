#pragma once

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t     jump[3];                // 0x00
    char        oem_id[8];              // 0x03  "NTFS    "
    uint16_t    bytes_per_sector;       // 0x0B
    uint8_t     sectors_per_cluster;    // 0x0D
    uint16_t    reserved;               // 0x0E
    uint8_t     unused1[5];             // 0x10
    uint8_t     media_descriptor;       // 0x15
    uint16_t    unused2;                // 0x16
    uint16_t    sectors_per_track;      // 0x18
    uint16_t    number_of_heads;        // 0x1A
    uint32_t    hidden_sectors;         // 0x1C
    uint32_t    unused3;                // 0x20
    uint32_t    unused4;                // 0x24
    uint64_t    total_sectors;          // 0x28
    uint64_t    mft_cluster;            // 0x30
    uint64_t    mft_mirror_cluster;     // 0x38
    int8_t      clusters_per_mft_record;// 0x40
    uint8_t     unused5[3];             // 0x41
    int8_t      clusters_per_index_buffer; // 0x44
    uint8_t     unused6[3];             // 0x45
    uint64_t    volume_serial;          // 0x48
    uint32_t    checksum;               // 0x50
    uint8_t     bootstrap[426];         // 0x54
    uint16_t    end_of_sector;          // 0x1FE (0xAA55)
} NTFS_BootSector;
#pragma pack(pop)
