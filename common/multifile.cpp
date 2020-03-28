#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LIB_CACHE_DEBUG
//#include "qtcompat.h"

//#include "str.h"
//#include "lc_array.h"
#endif

#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

int hash(const unsigned char *key, int len);
#ifdef __cplusplus
}
#endif

#include "multifile.h"


MultiFile::MultiFile(const char *filename)
{
    m_file = ::open(filename, O_RDONLY);
    if ( m_file == -1 ) {
        return;
    }
    if ( (m_size = (size_t)lseek(m_file, 0, SEEK_END)) == -1 ) {
        return;
    }
    m_map = mmap(0, m_size, PROT_READ, MAP_PRIVATE, m_file, 0);
    if ( m_map == ((void *)-1) ) {
    }
#ifdef LIB_CACHE_DEBUG
    printf("map=%p size=%lu\n", m_map, m_size);
#endif
    parseLib();
}

bool MultiFile::parseLib()
{
    uint8_t *p = (uint8_t *)m_map;
    
    if ( *((uint32_t *)p) != 0xbad0beef ) {
        printf("%s:%d bad magic", __FILE__, __LINE__);
        return false;
    }
    p += sizeof(uint32_t);
    
    m_num_of_sects =*((uint32_t *)p);
    p += sizeof(uint32_t);
   
    m_sections = (SECTION *)p;
    p = (uint8_t *)(m_sections + m_num_of_sects);
    m_file_entries = (SINGLE_FILE *)p;

#ifdef LIB_CACHE_DEBUG
    printf("Magic OK, have %u sections\n", m_num_of_sects);
    for (int i = 0; i < m_num_of_sects; i++) {
        printf("Section %d: size=%u offset=%u\n", i, m_sections[i].size, m_sections[i].files_ptr);
    }
    for (int i = 0; i < m_num_of_sects; i++) {
        SINGLE_FILE *sec_files = (SINGLE_FILE *)(((uint8_t *)m_map) + m_sections[i].files_ptr);
        for (int j = 0; j < m_sections[i].size; j++) {
            printf("Section %d file %d: name_offset=%u data_offset=%u\n",
                   i, j, sec_files[j].name_ptr, sec_files[j].data_ptr);
            char *curr_name = (char *)m_map + sec_files[j].name_ptr;
            printf("\t file=%s\n", curr_name);
            char *curr_data = (char *)m_map + sec_files[j].data_ptr;
            printf("\t data=%lu\n", strlen(curr_data));
        }
    }

    SINGLE_FILE *fp = m_file_entries;
    for (int i = 0; i < m_num_of_sects; i++) {
        for (int j = 0; j < m_sections[i].size; j++) {
            printf("Section %d file %d: name_offset=%u data_offset=%u\n",
                   i, j, fp->name_ptr, fp->data_ptr);
            fp++;
        }
    }
#endif
    
    return true;
}

char *MultiFile::lookupByName(const char *filename)
{
    int hash_idx = hash((const unsigned char *)filename, (int)strlen(filename));
    if ( hash_idx == -1 ) {
#if DEBUG
        printf("Hash failed for name = '%s'\n", filename);
#endif
        return 0;
    }
    SINGLE_FILE *file_entry = m_file_entries + hash_idx;
    char *file_name_from_entry = (char *)m_map + file_entry->name_ptr;
    //printf("%s: hash=%d name from entry=%s\n", __func__, hash_idx, file_name_from_entry);
    if ( strcmp(filename, file_name_from_entry) == 0 ) {
        return (char *)m_map + file_entry->data_ptr;
    }
    printf("ERROR: lu name [%s] doesn't name in entry %d  [%s]\n", filename, hash_idx, file_name_from_entry);
    return 0;
}

//void MultiFile::GetFileList(int section, lcArray<String>& FileList)
//{
//    if ( section >= m_num_of_sects ) {
//        printf("ERROR: only have %d sections\n", m_num_of_sects);
//        return;
//    }
//    SECTION *curr_section = m_sections + section;
//    SINGLE_FILE *sec_files = (SINGLE_FILE *)ptrToMmap(curr_section->files_ptr);
//    for (int i = 0; i < curr_section->size; i++) {
//        char *name = ptrToMmap(sec_files[i].name_ptr);
//        FileList.Add(String(name, true));
//    }
//}

bool MultiFile::validateSection(int section)
{
    if ( section >= m_num_of_sects ) {
        printf("ERROR: only have %d sections\n", m_num_of_sects);
        return false;
    }
    SECTION *curr_section = m_sections + section;
    //SINGLE_FILE *sec_files = (SINGLE_FILE *)(((uint8_t *)m_map) + curr_section->files_ptr);
    SINGLE_FILE *sec_files = (SINGLE_FILE *)ptrToMmap(curr_section->files_ptr);
    printf("Section %d have %d entries\n", section, curr_section->size);
    for (int i = 0; i < curr_section->size; i++) {
        void *p = lookupByName(ptrToMmap(sec_files[i].name_ptr));
        if ( !p ) {
            printf("ERROR: lookup failed for index=%d offset=%u\n", i, sec_files[i].name_ptr);
        }
    }
    printf("Finished, looks good\n");
    return true;
}

#ifdef LIB_CACHE_DEBUG

int main()
{
    MultiFile mf("/Users/lfroen/prog/mylego/library.bin");
    
    if ( mf.lookupByName("badname") ) {
        printf("ERROR: not expected to find\n");
    }
    
    mf.validateSection(0);
    mf.validateSection(1);
    mf.validateSection(2);
    mf.validateSection(3);
    
    //printf(mf.lookupByName("2890.dat"));

    return 0;
}

#endif
