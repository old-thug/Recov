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
    uint32_t type;
    uint32_t length;
    uint8_t non_resident;
    uint8_t name_length;
    uint16_t name_offset;
    uint16_t flags;
    uint16_t instance;
    uint32_t value_length;    // only for resident attributes
    uint16_t value_offset;    // only for resident attributes
    uint16_t reserved;
} NTFS_AttrHeaderResident;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint64_t parent_ref;
    uint64_t creation_time;
    uint64_t modify_time;
    uint64_t mft_mod_time;
    uint64_t access_time;
    uint64_t allocated_size;
    uint64_t data_size;
    uint32_t file_flags;
    uint32_t reparse;
    uint8_t  filename_length;     // number of Unicode characters
    uint8_t  namespace;           // 0 = POSIX, 1 = Win32, 2 = DOS, 3 = Win32+DOS
    uint16_t filename[1];         // UTF-16, variable length
} NTFS_FileNameAttr;
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
typedef struct {
    char        signature[4];          // "FILE"
    uint16_t    fixup_offset;          // Offset to Fixup Array
    uint16_t    fixup_count;           // Number of entries
    uint64_t    log_seq_number;
    uint16_t    sequence_number;
    uint16_t    hard_link_count;
    uint16_t    first_attr_offset;     // Offset to first attribute
    uint16_t    flags;                 // 0x01=InUse, 0x02=Directory
    uint32_t    used_size;             // Size of MFT entry in bytes
    uint32_t    allocated_size;        // Allocated entry size
    uint64_t    base_record_reference;
    uint16_t    next_attr_id;
    uint16_t    padding;
    uint32_t    mft_record_number;
} NTFS_MFTHeader;
#pragma pack(pop)
