//
//  multifile.h
//  legoview
//
//  Created by Leonid on 8/14/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#ifndef legoview_multifile_h
#define legoview_multifile_h

class MultiFile {
    int m_file;
    void *m_map;
    size_t m_size;
    
    uint32_t m_num_of_sects;
    
    typedef struct __attribute__((packed)) {
        uint32_t name_ptr;
        uint32_t data_ptr;
    } SINGLE_FILE;
    
    typedef struct __attribute__((packed)) {
        uint32_t size;
        uint32_t files_ptr;
    } SECTION;
    
    //
    // ptr to mmapped file
    //
    SECTION *m_sections;
    SINGLE_FILE *m_file_entries;
    
    bool parseLib();
    
    char *ptrToMmap(uint32_t offset)
    {
        return (char *)m_map + offset;
    }
public:
    MultiFile(const char *filename);
    
    char *lookupByName(const char *filename);
    
    uint32_t numOfSections()
    {
        return m_num_of_sects;
    }
    
    //void GetFileList(int section, lcArray<String>& FileList);
    
    bool validateSection(int section);
};


#endif
