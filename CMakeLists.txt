cmake_minimum_required(VERSION 3.28)
project(archwin C)

set(CMAKE_C_STANDARD 23)

add_executable(archwin src/main.c
        include/menu.c
        include/menu.h
        include/banner.c
        include/banner.h
        include/select_disk.c
        include/select_disk.h
        include/exec.c
        include/exec.h
        include/select_edition.c
        include/select_edition.h
        include/cdrom.c
        include/cdrom.h
        lib/count_substr.c
        lib/count_substr.h
        include/install_windows.c
        include/install_windows.h
        include/create_user.c
        include/create_user.h
)
