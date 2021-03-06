// Copyright (c) 2014, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  LIBBFS_BFS_H_
#define  LIBBFS_BFS_H_

#include <stdint.h>

namespace bfs {

enum OpenType {
    O_RDONLY = 1,
    O_WRONLY = 2,
};

enum Status {
    OK = 0,
    NotFound = 1,
    NotFile = 2,
    NotDirectory = 3,
};

/// Bfs File interface
class File {
public:
    File() {}
    virtual ~File() {}
    virtual int64_t Pread(int64_t offset, char* buf, int64_t read_size) = 0;
    virtual int64_t Seek(int64_t offset, int32_t whence) = 0;
    virtual int64_t Read(char* buf, int64_t read_size) = 0;
    virtual int64_t Write(const char* buf, int64_t write_size) = 0;
    virtual bool Flush() = 0;
    virtual bool Sync() = 0;
private:
    // No copying allowed
    File(const File&);
    void operator=(const File&);
};

struct BfsFileInfo {
    char name[1024];
    uint32_t ctime;
    uint32_t mode;
};

// Bfs fileSystem interface
class FS {
public:
    FS() { }
    virtual ~FS() { }
    /// Open filesystem with nameserver address (host:port)
    static bool OpenFileSystem(const char* nameserver, FS** fs);
    /// Create directory
    virtual bool CreateDirectory(const char* path) = 0;
    /// List Directory
    virtual bool ListDirectory(const char* path, BfsFileInfo** filelist, int *num) = 0;
    /// Open file for read or write
    virtual bool OpenFile(const char* path, int32_t flags, File** file) = 0;
    virtual bool CloseFile(File* file) = 0;
    virtual bool DeleteFile(const char* path) = 0;
    virtual bool GetFileSize(const char* path, int64_t* file_size) = 0;
private:
    // No copying allowed
    FS(const FS&);
    void operator=(const FS&);
};

}
#endif  // LIBBFS_BFS_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
