#include "recov/recov.h"
#include "recov/recov_errno.h"
#include "recov/recov_bootsec.h"
#include "recov/recov_mftheader.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int
applyFixups (unsigned char *entry, NTFS_MFTHeader *hdr,
             uint64_t bytes_per_sector)
{
    uint16_t fixup_offset = hdr->fixup_offset;
    uint16_t fixup_count = hdr->fixup_count;

    uint16_t *usa = (uint16_t *)(entry + fixup_offset);
    uint16_t usa_value = usa[0];
    for (int i = 1; i < fixup_count; i++)
    {
        uint16_t *target = (uint16_t *)(entry + i * bytes_per_sector - 2);
        if (*target != usa_value)
            return 0;
        *target = usa[i];
    }

    return 1;
}

int displayAttributes(FILE *disk, unsigned char *entry, NTFS_MFTHeader *hdr)
{
    (void)disk;

    uint16_t first_attr_offset = hdr->first_attr_offset;
    uint8_t *aptr = entry + first_attr_offset;

    while (1)
    {
        NTFS_AttrHeaderResident *ah = (NTFS_AttrHeaderResident *)aptr;

        if (ah->type == 0xFFFFFFFF) // end marker
            break;

        if (ah->type == 0x30) // FILE_NAME attribute
        {
            uint8_t *value = aptr + ah->value_offset;
            NTFS_FileNameAttr *fnattr = (NTFS_FileNameAttr *)value;

            int filename_length = fnattr->filename_length;
            uint16_t *src = (uint16_t *)fnattr->filename;

            char name[512];
            int out = 0;

            for (int i = 0; i < filename_length; i++)
            {
                uint16_t ch = src[i];
                if (ch < 128)
                    name[out++] = (char)ch;
                else
                    name[out++] = '?';
            }
            name[out] = 0;

            printf("NAME = %s", name);
            if (hdr->flags & 0x01)
                printf(", In-Use");
            else
                printf(", Not In-Use");

            if (hdr->flags & 0x02)
                printf(", Directory");
            else
                printf(", File");

            printf("\n");
            fflush(stdout);
            return 1; // success
        }

        if (ah->length == 0) // sanity check to avoid infinite loop
            break;

        aptr += ah->length; // move to next attribute
    }

    return 0; // no FILE_NAME attribute found
}


char *mftGetFilename(uint8_t *record) {
    uint16_t off = *(uint16_t*)(record + 0x14);
    while (off < 1024) {
        uint32_t type = *(uint32_t*)(record + off);
        if (type == 0xFFFFFFFF) break;
        uint32_t len = *(uint32_t*)(record + off + 4);
        uint8_t nonres = *(uint8_t*)(record + off + 8);
        uint16_t name_len = *(uint16_t*)(record + off + 9);
        uint16_t content_off = *(uint16_t*)(record + off + 20);
        if (type == 0x30 && nonres == 0) {
            uint8_t *fn = record + off + content_off;
            uint8_t nlen = fn[64];
            uint16_t *w = (uint16_t*)(fn + 66);
            char *out = malloc(nlen * 3 + 1);
            int j = 0;
            for (int i = 0; i < nlen; i++) {
                uint16_t wc = w[i];
                if (wc < 0x80) out[j++] = wc;
                else if (wc < 0x800) {
                    out[j++] = 0xC0 | (wc >> 6);
                    out[j++] = 0x80 | (wc & 0x3F);
                } else {
                    out[j++] = 0xE0 | (wc >> 12);
                    out[j++] = 0x80 | ((wc >> 6) & 0x3F);
                    out[j++] = 0x80 | (wc & 0x3F);
                }
            }
            out[j] = 0;
            return out;
        }
        off += len;
    }
    return NULL;
}


void
dumpBlock(FILE *disk, uint64_t byte_offset, size_t len)    
{    
#ifdef _WIN32
    _fseeki64(disk, (long long)byte_offset, SEEK_SET);
#else
    fseeko(disk, (off_t)byte_offset, SEEK_SET);
#endif
    unsigned char *buf = malloc(len);
    if (!buf) return;
    size_t r = fread(buf, 1, len, disk);
    for (size_t i = 0; i < r; i += 16) {
        printf("%08" PRIx64 "  ", byte_offset + (uint64_t)i);
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < r) printf("%02x ", buf[i + j]); else printf("   ");
        }
        printf(" ");
        for (size_t j = 0; j < 16 && i + j < r; ++j) {
            unsigned char c = buf[i + j];
            putchar((c >= 32 && c <= 126) ? c : '.');
        }
        printf("\n");
    }
    for (size_t off = 0; off + 4 <= r; ++off) {
        if (memcmp(buf + off, "FILE", 4) == 0) {
            printf("FOUND 'FILE' at offset %" PRIu64 " (relative %zu)\n", byte_offset + off, off);
        }
    }
    free(buf);
}


int
scanDrive(const char *drive_name)    
{
    char raw_path[32];
    snprintf(raw_path, sizeof(raw_path), "\\\\.\\%s", drive_name);
    FILE *disk = fopen(raw_path, "rb");
    if (!disk)
        return RECOV_ERR_No_Such_Drive;

    NTFS_BootSector sector;
    memset(&sector, 0, sizeof(sector));
    if (fread(&sector, 1, 512, disk) != 512)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }

    if (memcmp(sector.oem_id, "NTFS    ", 8) != 0)
    {
        fclose(disk);
        return RECOV_ERR_Not_NTFS;
    }

    uint16_t bytes_per_sector = sector.bytes_per_sector;
    uint8_t sectors_per_cluster = sector.sectors_per_cluster;
    uint64_t mft_cluster = sector.mft_cluster;
    int8_t mft_record_value = sector.clusters_per_mft_record;
    uint64_t total_sectors = sector.total_sectors;

    printf("bytes_per_sector=%u\n", bytes_per_sector);
    printf("sectors_per_cluster=%u\n", sectors_per_cluster);
    printf("mft_cluster=%" PRIu64 "\n", mft_cluster);

    
    if (bytes_per_sector == 0 || sectors_per_cluster == 0)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }

    uint64_t total_clusters = (total_sectors == 0) ? 0 : (total_sectors / sectors_per_cluster);
    if (mft_cluster > total_clusters && total_clusters != 0)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }

    int mft_record_size = 0;
    if (mft_record_value < 0)
    {
        int power = -mft_record_value;
        if (power < 0 || power > 30)
        {
            fclose(disk);
            return RECOV_ERR_Bad_Format;
        }
        mft_record_size = 1 << power;
    }
    else if (mft_record_value > 0)
    {
        mft_record_size = (int)sectors_per_cluster * mft_record_value * bytes_per_sector;
    }
    else
    {
        mft_record_size = (int)sectors_per_cluster * bytes_per_sector;
    }

    if (mft_record_size <= 0)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }

    uint64_t mft_offset = mft_cluster * (uint64_t)sectors_per_cluster * (uint64_t)bytes_per_sector;

#ifdef _WIN32
    if (_fseeki64(disk, (long long)mft_offset, SEEK_SET) != 0)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }
#else
    if (fseeko(disk, (off_t)mft_offset, SEEK_SET) != 0)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }
#endif

    unsigned char *mft_entry = (unsigned char *)malloc((size_t)mft_record_size);
    if (!mft_entry)
    {
        fclose(disk);
        return RECOV_ERR_Bad_Format;
    }

    int count = 0;
    int empty_run = 0;
    int seen_valid = 0;

    printf("mft_offset=%" PRIu64 "\n", mft_offset);
    printf("mft_record_size=%d\n", mft_record_size);

    /* dumpBlock(disk, mft_offset, 4096); */
    /* dumpBlock(disk, mft_offset - 4096, 4096); */
    /* dumpBlock(disk, mft_offset + 4096, 4096); */
    
    for (;;/* int i = 0; i < mft_cluster/mft_record_size; i++ */)
    {
        size_t read = fread(mft_entry, 1, (size_t)mft_record_size, disk);
        if (read != (size_t)mft_record_size)
            break;

        NTFS_MFTHeader *header = (NTFS_MFTHeader *)mft_entry;

        if (memcmp(header->signature, "FILE", 4) != 0)
        {
            empty_run++;
            if (seen_valid && empty_run > 64)
                break;
            continue;
        }

        empty_run = 0;
        seen_valid = 1;

        char *name = mftGetFilename(mft_entry);
	// printf("Name = %s\n", name);
        
        if (!applyFixups(mft_entry, header, bytes_per_sector))
        {
            printf("Failed Fixup\n");
	    fflush(stdout);            
            count++;
            continue;
        }

        if (!displayAttributes(disk, mft_entry, header))
        {
	    printf("Failed to display attributes\n");
	}            
        fflush(stdout);
        count++;
    }

    free(mft_entry);
    fclose(disk);
    return RECOV_ERR_Ok;
}
