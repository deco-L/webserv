/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:49:30 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/10/18 12:25:30 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define FILE_NOT_FOUND 404
#define ACCESS_DENIED 403
#define REGULAR_FILE 1
#define DIRECTORY 2
#define ERROR -1

namespace mylib {
    int check_access(const char *path) {
        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            if (errno == ENOENT)
                return FILE_NOT_FOUND;
            return ERROR;
        }

        mode_t permissions = path_stat.st_mode;
        uid_t uid = geteuid();
        gid_t gid = getegid();

        // ユーザーの権限を確認
        bool can_read = false;
        bool can_write = false;
        bool can_execute = false;

        if (uid == path_stat.st_uid) {
            // オーナーの権限を確認
            can_read = permissions & S_IRUSR;
            can_write = permissions & S_IWUSR;
            can_execute = permissions & S_IXUSR;
        } else if (gid == path_stat.st_gid) {
            // グループの権限を確認
            can_read = permissions & S_IRGRP;
            can_write = permissions & S_IWGRP;
            can_execute = permissions & S_IXGRP;
        } else {
            // その他のユーザーの権限を確認
            can_read = permissions & S_IROTH;
            can_write = permissions & S_IWOTH;
            can_execute = permissions & S_IXOTH;
        }

        if (S_ISREG(permissions)) {
            if (can_read && can_write)
                return REGULAR_FILE;
            else
                return ACCESS_DENIED;
        }

        if (S_ISDIR(permissions)) {
            if (can_read && can_execute)
                return DIRECTORY;
            else
                return ACCESS_DENIED;
        }

        return ERROR;
    }
}
