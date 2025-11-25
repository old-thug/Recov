#pragma once

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    uint32_t type;               // Attribute type ID
    uint32_t length;             // Total length of this attribute
    uint8_t  non_resident;       // 0 = resident, 1 = non-resident
    uint8_t  name_length;        // Length of attribute name (UTF-16)
    uint16_t name_offset;        // Offset to name
    uint16_t flags;              // Rarely relevant
    uint16_t attr_id;            // Attribute ID
} NTFS_AttrHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint32_t value_length;
    uint16_t value_offset;
    uint8_t  indexed_flag;
    uint8_t  padding;
    // VALUE follows here
} NTFS_AttrResident;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct
{
    uint64_t start_vcn;
    uint64_t last_vcn;
    uint16_t run_list_offset;
    uint16_t compression_unit;
    uint32_t padding;
    uint64_t allocated_size;
    uint64_t data_size;
    uint64_t initialized_size;
    uint64_t compressed_size;
    // RUNLIST starts here
} NTFS_AttrNonResident;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct
{
    uint32_t signature;          // "FILE" (0x454C4946)
    uint16_t fixup_offset;
    uint16_t fixup_count;

    uint64_t lsn;                // Log file sequence number

    uint16_t sequence_number;
    uint16_t hard_link_count;

    uint16_t first_attr_offset;  // Offset to first attribute
    uint16_t flags;              // 0x01 = in use, 0x02 = directory

    uint32_t used_size;          // Real size of MFT entry
    uint32_t allocated_size;     // Allocated size (usually equal)

    uint64_t base_record;        // For extension records
    uint16_t next_attr_id;       // Next attribute ID

    uint16_t padding;
    uint32_t mft_record_number;  // Number of this entry
} NTFS_MFTHeader;
#pragma pack(pop)
