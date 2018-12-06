struct ReservedRegion {
    unsigned int BS_jmpBoot;
    char[8] BS_OEMName;
    unsigned int BPB_BytsPerSec;
    unsigned int BPB_SecPerClus;
    unsigned int BPB_RsvdSecCnt;
    unsigned int BPB_NumFATs;
    unsigned int BPB_RootEntCnt;
    unsigned int BPB_TotSec16;
    unsigned int BPB_Media;
    unsigned int BPB_FATSz16;
    unsigned int BPB_SecPerTrk;
    unsigned int BPB_NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    unsigned int BPB_FATSz32;
    unsigned int BPB_ExtFlags;
    unsigned int BPB_RootClus;
    unsigned int BPB_FSInfo;
    unsigned int BPB_BkBootSec;
    unsigned int BPB_Reserved;
    unsigned int BS_DrvNum;
    unsigned int BS_BootSig;
    unsigned int BS_VolID;
    unsigned int BS_VolLab;
    char[8] BS_FilSysType;
};

typedef struct FAT32Image {
    struct ReservedRegion;
};
