"""
File:    p3.py
Author:  Olivia Patterson
Date:    12/1/22
Section: 45
E-mail:  opatter1@umbc.edu
Description: This program simulates command line input put in by the user.
"""


DIRECTORIES_KEY = 'directories'
FILES_KEY = 'files'
ROOT_DIRECTORY_NAME = '/'


def print_list(xlist):
    """
    Simply prints a list and should not be able to change any elements of the list given.
    :param xlist: a list given from another function
    :return: formatted list
    """
    for element in xlist:
        print(element)


def format_current(curr_path):
    """
    Formats the current file system path to end with a backslash.
    :param curr_path: the current path of the file system
    :return: formatted current path
    """
    curr_path = curr_path + '/'
    return curr_path


def find_current_dir(curr_path, file_system):
    """
    This function loops through the file system to find the current path.
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: updated current path
    """
    split_path = [ROOT_DIRECTORY_NAME]
    for item in curr_path[1:].split('/'):
        if item != '':
            split_path.append(item)

    current = file_system
    for dir_name in split_path:
        if dir_name == ROOT_DIRECTORY_NAME:
            current = current[dir_name]
        elif dir_name in current[DIRECTORIES_KEY].keys():
            current = current[DIRECTORIES_KEY][dir_name]

    return current


def check_dir_exists(path, current):
    """
    This function checks to make sure the directory exists.
    :param path: the directory to be checked
    :param current: the current path
    :return: True or False if the directory exists
    """
    path_split = path.split('/')
    for dir in path_split:
        if dir in current[DIRECTORIES_KEY].keys():
            current = current[DIRECTORIES_KEY][dir]
        else:
            print("Directory not found")
            return False
    return True


def touch(user_text, curr_path, file_system):
    """
    This function creates a 'file' in the system.
    :param user_text: the file name
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: updated file system
    """
    current = find_current_dir(curr_path, file_system)
    current[FILES_KEY] += [user_text]

    return file_system


def mkdir(user_text, curr_path, file_system):
    """
    This function makes a new directory as a dictionary key.
    :param user_text: directory name
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: updated file system
    """
    current = find_current_dir(curr_path, file_system)
    current[DIRECTORIES_KEY][user_text] = {FILES_KEY: [], DIRECTORIES_KEY: {}}

    return file_system


def ls(curr_path, file_system):
    """
    This function shows the contents for the current file system location.
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: printed contents
    """
    current = find_current_dir(curr_path, file_system)
    print(f"Contents for {curr_path}:")
    print_list(current[FILES_KEY])
    print_list(list(current[DIRECTORIES_KEY].keys()))


def cd(user_text, curr_path, file_system):
    """
    This function moves the user through the file system.
    :param user_text: location to move to
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: updated path
    """
    if user_text == '..':
        if (curr_path != ROOT_DIRECTORY_NAME) and (len(curr_path.split('/')) != 2):
            # change directory name to one above current
            return '/'.join(curr_path.split('/')[:-1])
        else:
            return ROOT_DIRECTORY_NAME
    elif user_text in ['', '.', None]:
        # leave current directory the same
        return curr_path
    elif user_text == ROOT_DIRECTORY_NAME:
        # should take back to root
        return ROOT_DIRECTORY_NAME
    else:
        if user_text[0] == '/':
            # Absolute path
            current = file_system
            if check_dir_exists(user_text, current):
                return user_text
            else:
                return curr_path
        else:
            # Relative path
            current = find_current_dir(curr_path, file_system)
            if check_dir_exists(user_text, current):
                if curr_path != ROOT_DIRECTORY_NAME:
                    return "/".join([curr_path, user_text])
                else:
                    return curr_path + user_text
            else:
                return curr_path


def rm(user_text, curr_path, file_system):
    """
    This function removes file from a directory.
    :param user_text: the file to be removed
    :param curr_path: the current path
    :param file_system: stores the entire dictionary file system
    :return: the updated file system
    """
    current = find_current_dir(curr_path, file_system)
    if user_text in current[FILES_KEY]:
        current[FILES_KEY].remove(user_text)
    else:
        print(f"{user_text} not found")


def locate(user_text, curr_dir_path, curr_dir, found_files):
    """
    This function locates the input file and outputs to path options to the file.
    :param user_text: the file to be found
    :param curr_dir_path: the current path
    :param curr_dir: the directory to search in
    :param found_files: paths of found files
    :return: paths to the file
    """
    for file in curr_dir[FILES_KEY]:
        if file == user_text:
            if curr_dir_path != ROOT_DIRECTORY_NAME:
                found_files += [f"{curr_dir_path}/{file}"]
            else:
                found_files += [f"/{file}"]

    for sub_dir in curr_dir[DIRECTORIES_KEY].keys():
        if curr_dir_path != ROOT_DIRECTORY_NAME:
            sub_dir_path = f"{curr_dir_path}/{sub_dir}"
        else:
            sub_dir_path = f"/{sub_dir}"
        found_files += locate(user_text, sub_dir_path, curr_dir[DIRECTORIES_KEY][sub_dir], [])

    if curr_dir_path == ROOT_DIRECTORY_NAME:
        if len(found_files) > 0:
            found_files_formatted = '\n\t'.join(found_files)
            print(f"A file with that name was found at the following paths:\n\t{found_files_formatted}")
        else:
            print("No file were found")

    return found_files

if __name__ == "__main__":
    curr_path = ROOT_DIRECTORY_NAME
    file_system = {ROOT_DIRECTORY_NAME: {FILES_KEY: [], DIRECTORIES_KEY: {}}}

    exit_command = False

    while not exit_command:
        user_input = input("$ ")
        split_input = user_input.split(" ")
        if len(split_input) == 1:
            command = split_input[0].lower()
            user_text = None
        else:
            command = split_input[0].lower()
            user_text = split_input[1]

        if command.lower() == "exit":
            exit_command = True
        elif command.lower() == "pwd":
            print(curr_path)
        elif command.lower() == "ls":
            ls(curr_path, file_system)
        elif command.lower() == "cd":
            curr_path = cd(user_text, curr_path, file_system)
        elif command.lower() == "mkdir":
            file_system = mkdir(user_text, curr_path, file_system)
        elif command.lower() == "touch":
            file_system = touch(user_text, curr_path, file_system)
        elif command.lower() == "rm":
            rm(user_text, curr_path, file_system)
        elif command.lower() == "locate":
            current_dir = find_current_dir(curr_path, file_system)
            locate(user_text, ROOT_DIRECTORY_NAME, current_dir, [])
