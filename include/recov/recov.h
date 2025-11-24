#pragma once

#include <stdio.h>
#include <stdint.h>

#include "recov/recov_errno.h"

int readSector(FILE *disk, uint64_t sector, void *buffer);
int readCluster(FILE *disk, uint64_t cluster, void *buffer);

// Scan `drive_name` for deleted files.
// `drive_name` is passed as a drive name. e.g `E:`, `C:`
int scanDrive(const char *drive_name);

// Scan `drive_name` for deleted files.
// `drive_name` is passed as a path to a drive. e.g `\\.\E:`, `\\.\C:`
int scanDrivePath(const char *drive_name);
