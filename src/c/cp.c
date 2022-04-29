#include "header/filesystem.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/fileio.h"
#include "header/program.h"
#include "header/utils.h"

int main() {
    struct message m;
    struct node_filesystem node_fs_buffer;
    struct file_metadata fileinfo;
    char buffer[8192];
    enum fs_retcode ret_code;
    int i;

    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    getMessage(&m, getCurrentSegment());

    fileinfo.parent_index = m.current_directory;
    fileinfo.buffer = buffer;
    strcpy(fileinfo.node_name, m.arg2);

    read(&fileinfo, &ret_code);

    if (ret_code != 0) {
        error_code(ret_code, m.arg1, m.arg2, m.arg3);

    } else if (m.arg3[0] == '\0') {
        ret_code = FS_R_NODE_NOT_FOUND;
        error_code(ret_code, m.arg1, m.arg2, m.arg3);

    } else {
        if (m.arg3[0] == '/') {
            i = read_relative_path(FS_NODE_P_IDX_ROOT, m.arg3 + 1, &ret_code);

        } else {
            i = read_relative_path(m.current_directory, m.arg3, &ret_code);
        }

        if (i == m.current_directory) {
            strcpy(fileinfo.node_name, m.arg3);
            write(&fileinfo, &ret_code);

            if (ret_code != 0) {
                error_code(ret_code, m.arg1, m.arg2, m.arg3);
            }

        } else {
            fileinfo.parent_index = i;
            write(&fileinfo, &ret_code);

            if (ret_code != 0) {
                error_code(ret_code, m.arg1, m.arg2, m.arg3);
            }
        }
    }

    sendMessage(&m, getCurrentSegment());
    exit();
}