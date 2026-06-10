import json

# This is the file used to save tasks on your computer.
# JSON is a text format that can store Python-like data, such as list and dict.
FILE_NAME = "tasks.json"

# tasks is stored in memory while the program is running.
# If we do not save it to a file, all tasks will disappear after the program exits.
tasks = []


def ShowTasks():
    # enumerate gives us both the index and the task.
    # The index starts from 0, so we display index + 1.
    if len(tasks) == 0:
        print("No tasks yet.\n")
        return
    SortTasksByDdl()
    for index, task in enumerate(tasks):
        print(f'{index + 1}. {task["name"]} | DDL: {task["ddl"]}')
    print()


def AddTask():
    print("If you want to quit,please enter 'q'.")
    while True:
        name = input("Please enter the name: ").strip()

        if(name=="q"):
            print("Stop adding tasks.\n")
            break

        ddl = input("Please enter the ddl: ").strip()

        # A task only stores real information. The number is decided by its position in the list.
        task = {
            "name": name,
            "ddl": ddl
        }

        tasks.append(task)
        SaveTasks()
        print("Task added successfully!\n")


def DeleteTask():
    if len(tasks) == 0:
        print("No tasks to delete.\n")
        return
    print("If you want to quit,please enter 0.")
    while True:
        ShowTasks()

        # Users see numbers from 1, but list indexes start from 0.
        index = int(input("Please enter the number to delete: ")) - 1
        if(index==-1):
            break

        if index < 0 or index >= len(tasks):
            print("Invalid number.\n")
            return

        tasks.pop(index)
        SaveTasks()
        print("Task deleted successfully!\n")


def UpdateTask():
    if len(tasks) == 0:
        print("No tasks to update.\n")
        return

    ShowTasks()

    index = int(input("Please enter the number to update: ")) - 1

    if index < 0 or index >= len(tasks):
        print("Invalid number.\n")
        return

    new_name = input("Please enter the new name: ")
    new_ddl = input("Please enter the new ddl: ")

    tasks[index]["name"] = new_name
    tasks[index]["ddl"] = new_ddl

    SaveTasks()
    print("Task updated successfully!\n")


def Main():
    while True:
        print("===== DDL Admin =====")
        print("1. Show tasks")
        print("2. Add task")
        print("3. Delete task")
        print("4. Update task")
        print("0. Exit")
        print("DDL: YYYY-MM-DD")

        op = input("Please choose: ")

        if op == "1":
            ShowTasks()
        elif op == "2":
            AddTask()
        elif op == "3":
            DeleteTask()
        elif op == "4":
            UpdateTask()
        elif op == "0":
            print("Bye!\n")
            break
        else:
            print("Invalid option.\n")

def SortTasksByDdl():
    tasks.sort(key=lambda task: task["ddl"])
    print("Tasks sorted by DDL!\n")

def SaveTasks():
    # open(..., "w") means opening the file in write mode.
    # If the file does not exist, Python will create it.
    # If the file already exists, Python will overwrite it with the newest tasks.
    with open(FILE_NAME, "w", encoding="utf-8") as file:
        # json.dump() writes Python data into a JSON file.
        # Here, it saves the tasks list into tasks.json.
        #
        # ensure_ascii=False keeps Chinese characters readable in the file.
        # Without it, Chinese may become escaped text like "\u5fae\u79ef\u5206".
        #
        # indent=4 makes the JSON file easier for humans to read.
        json.dump(tasks, file, ensure_ascii=False, indent=4)

def LoadTasks():
    # We assign a new value to the global tasks list inside this function,
    # so we need to tell Python that "tasks" means the global variable above.
    global tasks

    try:
        # open(..., "r") means opening the file in read mode.
        # The program tries to read old tasks from tasks.json when it starts.
        with open(FILE_NAME, "r", encoding="utf-8") as file:
            # json.load() reads JSON data from the file and turns it back into Python data.
            # In this project, it turns tasks.json back into a list of dicts.
            tasks = json.load(file)
    except FileNotFoundError:
        # The first time you run this program, tasks.json may not exist yet.
        # That is normal, so we start with an empty task list instead of crashing.
        tasks = []

LoadTasks()
Main()
